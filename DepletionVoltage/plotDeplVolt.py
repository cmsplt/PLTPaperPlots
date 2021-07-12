#!/usr/bin/env python3

import os, sys, typing, mplhep, pandas, json, matplotlib.pyplot

def loadDeplVolt() -> pandas.DataFrame:
    # load json file with depletion voltage values
    with open('depletionVoltage.json', 'r') as deplVoltFile:
        deplVolt = pandas.DataFrame.from_dict(json.load(deplVoltFile)).T
    deplVolt.index = [fname.split('/')[1].lstrip('Scan_').rstrip('.txt') for fname in deplVolt.index.to_list()]
    deplVolt.index = pandas.to_datetime(deplVolt.index, format='%Y_%m_%d_%H_%M_%S')
    return deplVolt

def intLumiByDate() -> pandas.DataFrame:
    # return dataframe Run2 cumulative integrated lumi (in 1/fb) with the date as index
    file='lumiByDay.csv' if os.path.exists('lumiByDay.csv') else 'https://cern.ch/cmslumi/publicplots/lumiByDay.csv'
    lumiByDay = pandas.read_csv(file)
    lumiByDay = lumiByDay[lumiByDay.Date >= '2015'] # LHC Run2
    lumiByDay.Date = pandas.to_datetime(lumiByDay.Date, format='%Y-%m-%d')
    lumiByDay['IntLumi(/fb)'] = lumiByDay['Delivered(/ub)'].cumsum().divide(10**9)
    return lumiByDay[['Date','IntLumi(/fb)']].set_index('Date')

def plotAxVline(intLumi:str, intLumiAx:matplotlib.axes.Axes, label:str, fontsize:int=14, position:str='top'):
    # add vertical line at the specified intLumi (given by intLumiByDate())
    matplotlib.pyplot.axvline(x=intLumi, color='grey', linestyle='--', alpha=0.4, label=label)
    matplotlib.pyplot.text(x=(intLumi+1)/intLumiAx.get_xlim()[1], y=0.18, s=label, color='grey', rotation=90, fontsize=fontsize, verticalalignment=position, transform=intLumiAx.transAxes)

def dateSecondaryAxis(intLumiAx:matplotlib.axes.Axes, lumiAtScanDate:typing.List[float], dataCh:pandas.Series):
    # Add secondary date axis [https://stackoverflow.com/a/33447004/13019084]
    dateAx = intLumiAx.twiny()
    dateAx.set_xlim(intLumiAx.get_xlim()) # set same range as intLumi axis
    dateAx.set_xticks(lumiAtScanDate) # copy location of intLumi x-ticks
    dateAx.set_xticklabels(dataCh.index.to_series().dt.strftime("%Y-%m-%d").to_list(), horizontalalignment='left') # draw them as the date!
    dateAx.tick_params(axis='x', labelrotation=45, labelsize=10)
    _ = [label.set_visible(False) for label in dateAx.get_xaxis().get_ticklabels()[1::2]] # print every second xticklabel starting with the first [https://stackoverflow.com/a/50034357/13019084]

def plotDeplVolt(ch:int):
    # plot depletion voltage vs time for channel 'ch'
    deplVolt = loadDeplVolt()
    dataCh = deplVolt[ch][deplVolt[ch] != 0] # depletion voltage dataframe (drop entries where depletion voltage == 0 )
    intLumi = intLumiByDate()
    lumiAtScanDate = [intLumi.loc[f'{scan.date()}'].values[0] for scan in dataCh.index] # Run2 IntLumi for each scan date
    mplhep.style.use('CMS')
    (fig, intLumiAx) = matplotlib.pyplot.subplots(figsize=(10, 6))
    # (fontsize, markersize) = 12, 40
    intLumiAx.scatter(x=lumiAtScanDate, y=dataCh.to_list()) # s=markersize
    intLumiAx.set_title(label=f'Ch{ch} Depletion Voltage vs Integrated Luminosity') # fontsize=20
    matplotlib.pyplot.xlabel(xlabel='Integrated Luminosity (1/fb)') # fontsize=fontsize
    matplotlib.pyplot.ylabel(ylabel='Depletion Voltage (V)') # fontsize=fontsize
    # matplotlib.pyplot.xticks(fontsize=fontsize)
    # matplotlib.pyplot.yticks(fontsize=fontsize)
    # matplotlib.pyplot.text(x=0.01, y=0.99, transform=intLumiAx.transAxes, horizontalalignment='left', verticalalignment='top', s=r'$\bf{CMS}$ $\it{Preliminary}$', fontsize=fontsize)
    mplhep.cms.text('Preliminary', loc=1)
    intLumiAx.axes.set_ylim(0.0, 800.0)
    intLumiAx.axes.set_xlim(0.0, 165.0)
    # dateSecondaryAxis()
    hvSetPoints = {'2015-05-01':'150V','2016-09-09 16:10':'200V', '2017-08-10 01:10':'300V', '2017-10-18 21:00':'400V', '2018-03-22 17:45':'500V', '2018-06-10 04:50':'800V', '2018-08-18 04:35':'VcThr'}
        # 200:[http://cmsonline.cern.ch/cms-elog/948105]  250:[http://cmsonline.cern.ch/cms-elog/1002826] 300:[http://cmsonline.cern.ch/cms-elog/1003149]
        # 350:[http://cmsonline.cern.ch/cms-elog/1015071] 400:[http://cmsonline.cern.ch/cms-elog/1016344] 800:[http://cmsonline.cern.ch/cms-elog/1047254]
        # VcThr:[http://cmsonline.cern.ch/cms-elog/1058918]
    _ = [plotAxVline(intLumi.loc[f'{pandas.to_datetime(date).date()}'].values[0], intLumiAx, hv) for date,hv in hvSetPoints.items()]
    matplotlib.pyplot.tight_layout()
    # matplotlib.pyplot.show()
    matplotlib.pyplot.savefig(f'ch{ch}DepletionVoltage.png', dpi=300, bbox_inches='tight')
    matplotlib.pyplot.close('all')

def main():
    _ = [plotDeplVolt(ch) for ch in range(16)]

if __name__== "__main__":
    main()
