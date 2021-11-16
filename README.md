# PLTPaperPlots
A repository for the plotting scripts and data for the PLT paper.

Ideally, this directory should contain the scripts and all necessary data needed to make each plot. If this is not possible for whatever reason, then it should at least contain the plot saved as a ROOT .C macro so that it can be easily changed as necessary.

* Figs. 1 and 2 (PLT sketches): I created these in PowerPoint in PLTSketches.pptx and then saved them to create the two individual PDFs TripleCoincidenceSketch.pdf (Fig. 1) and AccidentalSketch.pdf (Fig. 2).

* Fig. 3 (schematic of PLT readout channels): DrawPLTReadoutChannels.C, a script I created for the paper to improve on the original version (which was just created in PowerPoint)

* Fig. 6 (accidental rates vs. SBIL): PlotAccidentalRatesPaper.C, originally derived from PLTOffline/AccidentalStudies/PlotAccidentalRatesAllScans.C, which uses the data in AccidentalData/. The 2015 data was originally also in PLTOfflineAccidentalStudies, while I had to pull the 2016 data from Joe's directory in /home/jheidema/PLTOffline/AccidentalStudies on pltoffline (this should really be committed at some point). I modified the script so it can run on either 2015 or 2016 depending on the argument; use .x PlotAccidentalRatesPaper.C(0) for 2015 and (1) for 2016.

* Fig. 11 (track luminosity comparison for one fill): PlotTrackLumiFillPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiFill.C, which uses the data in TrackLumiData/ for fill 5019, also originally from that directory.

* Figs. 12 and 13 (track luminosity VdM results): PlotTrackLumiVdMPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiVdM.C, which uses the data in TrackLumiData/ for fill 6016, also originally from the same directory.

* Fig. 15 (sample alignment procedure): PlotAlignment4444Paper.C, which takes the histograms produced by CalculateAlignment stored in the ROOT file alignment_4444_30M.root and formats them nicely. See the script for details on how the ROOT file is generated.

* Fig. 16 (2015 alignment over time): PlotAlignmentVsTimePaper.C, originally derived from PLTOffline/plotScripts/PlotAlignmentVsTime.C, which uses the alignment data in Alignment2015/, originally from PLTOffline/ALIGNMENT.

* Fig. 23 (background measurement): PlotBackgroundStudies.C. This is currently a straight copy of the script and data in PLTOffline/BackgroundStudies; see that directory for further documentation.

* Fig. 28 (PLT/RAMSES corrections for 2016): PlotRAMSESCorrectionsPaper.C, created for the paper to plot the data from David's spreadsheet (pltramseslin.xlsx) which I have extracted the two relevant csv files from (pltramses2016ratio.csv and pltramses2016slope.csv).

* Plots for ML using occupancy maps (not yet in paper but will be added soon): These are the MLOccupancy* files. These consist of the original PDF and PNG from Peter (although I don't actually have the macros).

* To calculate the combined uncertainty on the luminosity, use the file combined_PLT_luminosity.txt with the script Normtags/Scripts/combineYears.py. This contains the necessary uncertainties to compute the combined value.
