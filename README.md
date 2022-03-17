# PLTPaperPlots
A repository for the plotting scripts and data for the PLT paper.

Ideally, this directory should contain the scripts and all necessary data needed to make each plot. If this is not possible for whatever reason, then it should at least contain the plot saved as a ROOT .C macro so that it can be easily changed as necessary. In cases where I have only the figures but not the scripts, this is indicated by italics.

* Figs. 1 and 2 (PLT sketches): I created these in PowerPoint in PLTSketches.pptx and then saved them to create the two individual PDFs TripleCoincidenceSketch.pdf (Fig. 1) and AccidentalSketch.pdf (Fig. 2).

* Fig. 3 (schematic of PLT readout channels): DrawPLTReadoutChannels.C, a script I created for the paper to improve on the original version (which was just created in PowerPoint)

* *Fig. 5 (VdM scan curves)*: VdM/*, plots created by Lizardo using the VdM framework.

* Fig. 6 (accidental rates vs. SBIL): AccidentalRates/PlotAccidentalRatesPaper.C, originally derived from PLTOffline/AccidentalStudies/PlotAccidentalRatesAllScans.C, which uses the data in AccidentalRates/AccidentalData/. The 2015 data was originally also in PLTOfflineAccidentalStudies, while I had to pull the 2016 data from Joe's directory in /home/jheidema/PLTOffline/AccidentalStudies on pltoffline (this should really be committed at some point). I modified the script so it can run on either 2015 or 2016 depending on the argument; use .x PlotAccidentalRatesPaper.C(0) for 2015 and (1) for 2016.

* *Fig. 8 (track efficiency vs. time)*: TrackEfficiency/Efficiency_Ch*. Only plots from Francesco, still need to get the macros.

* *Fig. 10 (comparison of track efficiency and emittance scans)*: TrackEfficiency/EfficiencyComparison_2017_ch12.* and TrackEfficiency/EfficiencyRatios_2017.*. Plots only, no macros.

* Fig. 11 (track luminosity comparison for one fill): TrackLumi/PlotTrackLumiFillPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiFill.C, which uses the data in TrackLumi/TrackLumiData/ for fill 5019, also originally from that directory.

* Figs. 12 and 13 (track luminosity VdM results): TrackLumi/PlotTrackLumiVdMPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiVdM.C, which uses the data in TrackLumi/TrackLumiData/ for fill 6016, also originally from the same directory.

* Fig. 15 (sample alignment procedure): Alignment/PlotAlignment4444Paper.C, which takes the histograms produced by CalculateAlignment stored in the ROOT file alignment_4444_30M.root and formats them nicely. See the script for details on how the ROOT file is generated.

* Fig. 16 (2015 alignment over time): Alignment/PlotAlignmentVsTimePaper.C, originally derived from PLTOffline/plotScripts/PlotAlignmentVsTime.C, which uses the alignment data in Alignment2015/, originally from PLTOffline/ALIGNMENT.

* Figs. 18 (HV scan results) and 19 (depletion voltage over time): all code and data files in DepletionVoltage/ from Andres.

* *Fig. 20 (pulse height stability)*: PulseHeights/ph_stability.*. Only plots, still need to get the macros from Francesco.

* *Fig. 21 (pulse heights triple coincidence and timewalk)*: PulseHeights/ph_triple_coincidence_7117.* and PulseHeights/ph_timewalk.*. Only plots, still need to get the macros.

* *Fig. 22 (pulse heights vs. integrated luminosity)*: PulseHeights/ph_ch*_roc?.{pdf,gif}. Only plots, still need to get the macros.

* Fig. 23 (background measurement): PLTBackground/PlotBackgroundStudies.C. This is currently a straight copy of the script and data in PLTOffline/BackgroundStudies; see that directory for further documentation.

* *Fig. 24 (high-pileup ratios vs. SBIL)*: HighPileup/SBIL_7358_plt_13_hfocbx_*. Only plots from Javier.

* *Fig. 25 (high-pileup slope vs. BCID)*: HighPileup/slope_dependence_bxid_Q4_7358.*. Only plots from Javier.

* *Fig. 26 (beamspot vs. time)*: Beamspot/BeamspotEvolution*. Only plots from Himal.

* *Fig. 27 (beamspot 2D plot)*: Beamspot/Beamspot2D*. Only plots from Himal.

* *Fig. 28 (ML using occupancy maps)*: These plots are in MLOccupancy/. These consist of the original PDF and PNG from Peter; I don't have the macros for these.

* Fig. 29 (PLT/RAMSES corrections for 2016): PLTRamsesComparison/PlotRAMSESCorrectionsPaper.C, created for the paper to plot the data from David's spreadsheet (pltramseslin.xlsx) which I have extracted the two relevant csv files from (pltramses2016ratio.csv and pltramses2016slope.csv).

* *Figs. 31 and 32 (cross-luminometer ratio and slope plots)*: LuminometerComparisons/*. Only plots from Rafael.

* To calculate the combined uncertainty on the luminosity, use the file combined_PLT_luminosity.txt with the script Normtags/Scripts/combineYears.py. This contains the necessary uncertainties to compute the combined value.

* SystematicsTable.png is just a snippet from the rendered PDF that can be used for things like the public results page.