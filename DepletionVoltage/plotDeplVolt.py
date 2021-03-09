#!/usr/bin/env python3

import os, sys, pandas, json, matplotlib.pyplot

def loadDepletionVoltageJSON() -> pandas.DataFrame:
    with open(f'depletionVoltage.json', 'r') as deplVoltFile: deplVolt = json.load(deplVoltFile)
    deplVolt = pandas.DataFrame.from_dict(deplVolt).T
    deplVolt.index = [fname.split('/')[1].lstrip('Scan_').rstrip('.txt') for fname in deplVolt.index.to_list()]
    deplVolt.index = pandas.to_datetime(deplVolt.index, format = '%Y_%m_%d_%H_%M_%S')
    return deplVolt

def lumiByDay() -> pandas.DataFrame:
    # return dataframe Run2 cumulative integrated lumi (in 1/fb) with the date as index
    file='lumiByDay.csv' if os.path.exists('lumiByDay.csv') else 'https://cern.ch/cmslumi/publicplots/lumiByDay.csv'
    lumiByDay = pandas.read_csv(file)
    lumiByDayRun2 = lumiByDay[lumiByDay.Date >= '2015'] # LHC Run2
    date = pandas.to_datetime(lumiByDayRun2.Date, format = '%Y-%m-%d')
    cumulativeIntLumi = lumiByDayRun2['Delivered(/ub)'].cumsum().divide(10**9).rename('Delivered(/fb)')
    return pandas.concat([date, cumulativeIntLumi], axis=1).set_index('Date')['Delivered(/fb)']

def plotAxVline(intLumi:str, label:str, position:str):
    # add vertical line at the specified intLumi (given by lumiByDay)
    matplotlib.pyplot.axvline(x=intLumi, color='grey', linestyle='--', alpha=0.4, label=label)
    matplotlib.pyplot.text(x=intLumi+1, y=10, s=label, fontsize=12, rotation=90, verticalalignment=position)

def dateSecondaryAxis(intLumiAx, lumiPerScanDate, dataCh):
    # Add secondary date axis [https://stackoverflow.com/a/33447004/13019084]
    dateAx = intLumiAx.twiny()
    dateAx.set_xlim(intLumiAx.get_xlim()) # set same range as intLumi axis
    dateAx.set_xticks(lumiPerScanDate) # copy location of intLumi x-ticks
    dateAx.set_xticklabels(dataCh.index.to_series().dt.strftime("%Y-%m-%d").to_list(), horizontalalignment='left') # draw them as the date!
    dateAx.tick_params(axis='x', labelrotation=45, labelsize=10)
    _ = [label.set_visible(False) for label in dateAx.get_xaxis().get_ticklabels()[1::2]] # print every second xticklabel starting with the first [https://stackoverflow.com/a/50034357/13019084]

def plotDeplVolt(ch:int):
    # plot depletion voltage vs time for channel 'ch'
    deplVolt = loadDepletionVoltageJSON()
    dataCh = deplVolt[ch][deplVolt[ch] != 0] # depletion voltage dataframe (drop entries where depletion voltage == 0 )
    lumiByDaySeries = lumiByDay()
    lumiPerScanDate = [lumiByDaySeries[pandas.to_datetime(scan.date())] for scan in dataCh.index.to_list()] # Run2 cumulativeIntLumi for each scan date
    fig, intLumiAx = matplotlib.pyplot.subplots(figsize=(10, 6))
    intLumiAx.scatter(x=lumiPerScanDate, y=dataCh.to_list())
    intLumiAx.set_title(label=f'Ch{ch} Depletion Voltage vs Integrated Luminosity', fontsize=20)
    matplotlib.pyplot.xlabel(xlabel='Integrated Luminosity (1/fb)', fontsize=16)
    matplotlib.pyplot.ylabel(ylabel='Depletion Voltage (V)', fontsize=16)
    matplotlib.pyplot.xticks(fontsize=12)
    matplotlib.pyplot.yticks(fontsize=12)
    matplotlib.pyplot.text(x=0.01, y=0.99, transform=intLumiAx.transAxes, horizontalalignment='left', verticalalignment='top', s=r'$\bf{CMS}$ $\it{Preliminary}$', fontsize=16)
    intLumiAx.axes.set_ylim(0.0, 800.0)
    intLumiAx.axes.set_xlim(0.0, 165.0)
    # dateSecondaryAxis()
    hvSetPoints = {'2016-09-09 16:10':'200V', '2017-08-10 01:10':'300V', '2017-10-18 21:00':'400V', '2018-03-22 17:45':'500V', '2018-06-10 04:50':'800V', '2018-08-18 04:35':'VcThr'}
        # 200:[http://cmsonline.cern.ch/cms-elog/948105]  250:[http://cmsonline.cern.ch/cms-elog/1002826] 300:[http://cmsonline.cern.ch/cms-elog/1003149]
        # 350:[http://cmsonline.cern.ch/cms-elog/1015071] 400:[http://cmsonline.cern.ch/cms-elog/1016344] 800:[http://cmsonline.cern.ch/cms-elog/1047254]
        # VcThr:[http://cmsonline.cern.ch/cms-elog/1058918]
    _ = [plotAxVline(lumiByDaySeries[f'{pandas.to_datetime(date).date()}'], hv, 'bottom') for date,hv in hvSetPoints.items()]
    matplotlib.pyplot.tight_layout()
    matplotlib.pyplot.show()
    # matplotlib.pyplot.savefig(f'ch{ch}DepletionVoltage.png', dpi=300)
    # matplotlib.pyplot.close('all')

plotDeplVolt(10)
# _ = [plotDeplVolt(ch) for ch in range(16)]
