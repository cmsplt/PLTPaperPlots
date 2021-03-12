#!/usr/bin/env python3

import os, sys, typing, pandas, pathlib, matplotlib.pyplot

def parseLogFile(logFile:str) -> pandas.DataFrame:
    # Define column headers and load log file into a pandas dataframe (only keep lines matching "#M")
    def perChList(prefix:str) -> typing.List[str]:
        # channel order hard-coded to match auto-HV scan log files
        # $ grep 'HpFT0,HpFT1,HpFT2,HpFT3,HpNT0,HpNT1,HpNT2,HpNT3,HmFT0,HmFT1,HmFT2,HmFT3,HmNT0,HmNT1,HmNT2,HmNT3' AutoScanLogs/*.txt
        return [f'{prefix}{i}' for i in [12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3]]
    def toTimestamp(df:pandas.Series):
        # convert timestamps from local CERN time to UTC
        # pandas.tz_localize and pandas.tz_convert only operate on pandas.DatetimeIndex because of reasons [https://stackoverflow.com/a/26090113/13019084]
        return pandas.DatetimeIndex(pandas.to_datetime(df, format='%Y.%m.%d %H:%M:%S.%f')).tz_localize('Europe/Amsterdam').tz_convert('UTC')
    with open(logFile, 'r') as log:
        data = [line.rstrip('\n').split(',') for line in log.readlines() if '#M' in line]
    cols = ['timestamp', 'ignore', *perChList('vMon'), *perChList('iMon'), *perChList('rate'), 'avgRate'] # [ timestamp, #M, vMon[0:15], iMon[0:15], rate[0:15], avgRate ]
    logData = pandas.DataFrame(data, columns=cols)
    logData.timestamp = toTimestamp(logData.timestamp)
    logData[cols[2:]] = logData[cols[2:]].astype(float)
    return logData

def brilcalc(beginTS:str, endTS:str, det:str) -> pandas.DataFrame:
    # load brilcalc data between {beginTS} and {endTS} for {det} detector [https://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#brilcalc]
    # [https://janakiev.com/blog/python-shell-commands/]
    command = f'brilcalc lumi --begin "{beginTS}" --end "{endTS}" --byls --tssec --type {det} --output-style csv'
    brilcalcData = os.popen(f'brilcalc lumi --begin "{beginTS}" --end "{endTS}" --byls --tssec --type {det} --output-style csv').read().splitlines()
    cols = brilcalcData[1].split(',')
    brilcalcData = [line.split(',') for line in brilcalcData[2:-4]] # skip line 0 (norm tag version), line 1 (column headers), and the last 4 lines (summary)
    brilcalcDF = pandas.DataFrame(brilcalcData, columns=cols)
    brilcalcDF[['delivered(/ub)', 'recorded(/ub)', 'avgpu']] = brilcalcDF[['delivered(/ub)', 'recorded(/ub)', 'avgpu']].astype(float)
    brilcalcDF['dt'] = pandas.to_datetime(brilcalcDF.time, unit='s', utc=True)
    return brilcalcDF

def mergeDF(logData:pandas.DataFrame) -> pandas.DataFrame:
    # merge logData and brilcalcDF based on closest timestamps and add new PLT/HF rate column to logData
    beginTS = logData.timestamp.iloc[0].strftime('%m/%d/%y %H:%M:%S')
    endTS = logData.timestamp.iloc[-1].strftime('%m/%d/%y %H:%M:%S')
    brilcalcDF = brilcalc(beginTS, endTS, 'hfet')
    if len(brilcalcDF) == 0:
        brilcalcDF = brilcalc(beginTS, endTS, 'bcm1f')
    mergedDF = pandas.merge_asof(left=logData, right=brilcalcDF, left_on='timestamp', right_on='dt')
    for ch in range(16):
        logData[f'rateN{ch}'] = mergedDF[f'rate{ch}'] / mergedDF['avgpu']
    return logData.dropna()

def processChannel(logData:pandas.DataFrame, ch:int) -> pandas.DataFrame:
    # determine scanpoints for ch, filter logData for each ch & scanpoint based on uniformity of vMon and rateN values, and return median/std for rateCh & rateNCh
    vMonGroup = logData[f'vMon{ch}'].groupby(logData[f'vMon{ch}'].round(decimals=0)).count() > 4 # group all integer-rounded vMon values and return 'true' if vMon group members > 4
    scanSteps = vMonGroup[vMonGroup].index.astype('int').to_list() # return HV setpoint values as a list of integers
    def processStep(df:pandas.DataFrame, ch:int, step:int) -> typing.List[typing.Union[int,float]]:
        def filterStep(s:pandas.Series, hv:int) -> pandas.Series:
            # return true for rows within ±2 V of {hv} setpoint
            return (s <= hv+2) & (s >= hv-2)
        def filterRate(s:pandas.Series) -> pandas.Series:
            # return true for rows with normalized rate within ±5% of the final value in the setpoint
            return s.divide(s.iloc[-1]).subtract(1).abs() < 0.05
        df = df[filterStep(df[f'vMon{ch}'], step)]
        df = df[filterRate(df[f'rateN{ch}'])]
        return [step, df[f'rate{ch}'].median(), df[f'rate{ch}'].std(), df[f'rateN{ch}'].median(), df[f'rateN{ch}'].std()]
    cols = ['hv', f'medianRate{ch}', f'stdevRate{ch}', f'medianRateN{ch}', f'stdevRateN{ch}']
    return pandas.DataFrame((processStep(logData, ch, step) for step in scanSteps), columns=cols)

def calculateDeplVolt(logData:pandas.DataFrame, ch:int, thr1:float=0.01, thr2:float=0.02) -> int:
    # calculate depletion voltage based on percent change. select first value (highest to lowest HV setpoints) within 'thr1' of the previous value and within 'thr2' of next-to-previous value
    chData  = processChannel(logData, ch)[::-1] # reverse order of scan points (from highest to lowest)
    pctP1 = chData[f'medianRateN{ch}'].pct_change(periods=1).abs()
    pctP2 = chData[f'medianRateN{ch}'].pct_change(periods=2).abs()
    plateau = chData[(pctP1>thr1) & (pctP2>thr2)]['hv']
    deplVolt = plateau.iloc[0] if not plateau.empty else 0
    deplVoltStDev = chData[chData['hv'] > deplVolt][f'medianRateN{ch}'].std() # st dev of rates for hv above the calculated depl volt
    return deplVolt

def plotChannel(chData:pandas.DataFrame, ch:int, deplVolt:float, date:str):
    # plot rate (raw and normalized) vs HV
    scale = chData[f'medianRateN{ch}'].max() / chData[f'medianRate{ch}'].max()
    (fig, ax) = matplotlib.pyplot.subplots(figsize=(10, 6))
    (fontsize, markersize) = 12, 8
    ax.set_title(label=f'Ch{ch} rate vs HV ({date})', fontsize=20)
    ax.set_xlabel(xlabel='HV setpoint (V)', fontsize=fontsize)
    ax.set_ylabel(ylabel='Raw inst. lumi rate', fontsize=fontsize)
    matplotlib.pyplot.xticks(fontsize=fontsize)
    matplotlib.pyplot.yticks(fontsize=fontsize)
    matplotlib.pyplot.text(x=0.01, y=0.99, transform=ax.transAxes, horizontalalignment='left', verticalalignment='top', s=r'$\bf{CMS}$ $\it{Preliminary}$', fontsize=fontsize)
    ax.errorbar(x=chData['hv'], y=chData[f'medianRateN{ch}'], yerr=chData[f'stdevRateN{ch}'], ls='', marker='o', markersize=markersize, label='PLT/HF')
    ax.errorbar(x=chData['hv'], y=chData[f'medianRate{ch}']*scale, yerr=chData[f'stdevRate{ch}']*scale, ls='', marker='o', markersize=markersize, label='PLT')
    ax.legend(loc='lower right', borderpad=0.1, labelspacing=0.1, fancybox=True, framealpha=0.4, fontsize=fontsize)
    ax.axvline(deplVolt, color='red')
    fig.tight_layout()
    # matplotlib.pyplot.show()
    os.makedirs(f'{date}/', exist_ok=True)
    fig.savefig(f'{date}/{date}.ch{ch}.png', dpi=300)
    matplotlib.pyplot.close('all')

def main():
    for logFile in sorted(pathlib.Path.cwd().glob('scanLogs/*txt')):
        print(f'processing {logFile.name}...')
        logData = parseLogFile(logFile)
        logData = mergeDF(logData)
        date = logData.timestamp.iloc[0].strftime('%Y-%m-%d')
        for ch in range(16):
            chData = processChannel(logData, ch)
            deplVolt = calculateDeplVolt(logData, ch, 0.01, 0.02)
            plotChannel(chData, ch, deplVolt, date)

if __name__== "__main__":
    main()
