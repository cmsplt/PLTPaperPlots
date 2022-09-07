# PLTPaperPlots
A repository for the plotting scripts and data for the PLT paper.

Ideally, this directory should contain the scripts and all necessary data needed to make each plot. If this is not possible for whatever reason, then it should at least contain the plot saved as a ROOT .C macro so that it can be easily changed as necessary. In cases where I have only the figures but not the scripts, this is indicated by italics.

* Fig. 1 (PLT sketches): I created these in PowerPoint in PLTSketches.pptx and then saved them to create the two individual PDFs TripleCoincidenceSketch.pdf (Fig. 1 left) and AccidentalSketch.pdf (Fig. 1 right).

* Fig. 2 (schematic of PLT readout channels): DrawPLTReadoutChannels.C, a script I created for the paper to improve on the original version (which was just created in PowerPoint).

* Fig. 3 (picture of one cassette): AssembledCassette.jpg; no macro since this is just a picture (original from Andres). 

* *Fig. 4 (readout schematic)*: PLTReadoutSchematic.png; taken from original plot from Andreas, but no macro available.

* Fig. 5 (active area aligment): Alignment/MakeTrackOccupancyPaper.C, which uses the histograms produced by TrackOccupancy stored in the ROOT file histo_track_occupancy_4892.root. These are based on scripts I used to create the plots for the 2015 approval which were on april, but not committed to PLTOffline (this should be fixed soon).

* Fig. 6 (sample alignment procedure): Alignment/PlotAlignment4444Paper.C, which takes the histograms produced by CalculateAlignment stored in the ROOT file alignment_4444_30M.root and formats them nicely. See the script for details on how the ROOT file is generated.

* Fig. 7 (2015 alignment over time): Alignment/PlotAlignmentVsTimePaper.C, originally derived from PLTOffline/plotScripts/PlotAlignmentVsTime.C, which uses the alignment data in Alignment2015/, originally from PLTOffline/ALIGNMENT.

* Fig. 8 (accidental rates vs. SBIL): AccidentalRates/PlotAccidentalRatesPaper.C, originally derived from PLTOffline/AccidentalStudies/PlotAccidentalRatesAllScans.C, which uses the data in AccidentalRates/AccidentalData/. The 2015 data was originally also in PLTOfflineAccidentalStudies, while I had to pull the 2016 data from Joe's directory in /home/jheidema/PLTOffline/AccidentalStudies on pltoffline (this should really be committed at some point). I modified the script so it can run on either 2015 or 2016 depending on the argument; use .x PlotAccidentalRatesPaper.C(0) for 2015 and (1) for 2016. The right plot uses AccidentalRates/PlotAccidentalSlopesPaper.C, which also uses the 2016 data from Joe's directory.

* Fig. 9 (accidental rates using likelihood method): left plot in AccidentalLikelihood/AccidentalLikelihoodFit_4979.pdf. Created using the script fit_model_ggg_g.C from Nimmitha from the data file Fill_4979_v2.root (note that I've renamed the output of the script).

* Fig. 10 (alignment vs. mask size): MaskStudies/PlotAccidentalRatesMasks.C. The data in here is a copy from PLTOffline/AccidentalStudies/MaskStudies/; see that directory for further documentation.

* *Fig. 11 (track efficiency vs. time)*: TrackEfficiency/Efficiency_Ch*. Only plots from Francesco, still need to get the macros.

* *Fig. 12 (pulse height stability)*: PulseHeights/ph_stability.*. Only plots, still need to get the macros from Francesco.

* *Fig. 13 (pulse heights triple coincidence and timewalk)*: PulseHeights/ph_triple_coincidence_7117.* and PulseHeights/ph_timewalk.*. Only plots, still need to get the macros.

   Note that the pulse height vs. integrated luminosity plots have been removed from the paper, although they are still available here in PulseHeights/ph_ch*_roc?.{pdf,gif} (only plots, no macros).

* Figs. 14 (HV scan results) and 15 (depletion voltage over time): all code and data files in DepletionVoltage/ from Andres (including many more scans than shown here in the paper).

* *Fig. 16 (ML using occupancy maps)*: These plots are in MLOccupancy/. These consist of the original PDF and PNG from Peter; I don't have the macros for these.

* Fig. 17 (background measurement): PLTBackground/PlotBackgroundStudies.C. This is currently a straight copy of the script and data in PLTOffline/BackgroundStudies; see that directory for further documentation.

* *Fig. 18 (high-pileup ratios vs. SBIL)*: HighPileup/SBIL_7358_plt_13_hfocbx_*. Only plots from Javier.

* *Fig. 19 (high-pileup slope vs. BCID)*: HighPileup/slope_dependence_bxid_Q4_7358.*. Only plots from Javier.

* *Fig. 20 (beamspot vs. time)*: Beamspot/BeamspotEvolution*. Only plots from Himal.

* *Fig. 21 (beamspot 2D plot)*: Beamspot/Beamspot2D*. Only plots from Himal.

* *Fig. 22 (VdM scan curves)*: VdM/*, plots created by Lizardo using the VdM framework.

* Fig. 23 (emittance scan results): Efficiency plot for 2017 in EmittanceScans/EfficiencyCorrections2017.{pdf,png}, using PlotEmittanceScanFOM2017.C and the spreadsheet from David. Linearity plot for fill 6325 in EmittanceScans/SlopeFit_6325_orig.{pdf,png}, using PlotSlopeFit.C and the spreadsheets from David.

* *Fig. 24 (comparison of track efficiency and emittance scans)*: TrackEfficiency/EfficiencyComparison_2017_ch12.* and TrackEfficiency/EfficiencyRatios_2017.*. Plots only, no macros.

* Fig. 25 (PLT/RAMSES corrections for 2016): PLTRamsesComparison/PlotRAMSESCorrectionsPaper.C, created for the paper. This uses the data from David's two spreadsheets; the first (pltramseslin.xlsx) includes most of the information but doesn't include the final rescaling to make the efficiency equal to 1 in the VdM fill, so the final efficiency data comes from ForPaper.xlsx. I have extracted the two relevant csv files, pltramses2016ratio.csv and pltramses2016slope.csv, from these.

* *Fig. 26 (effect of reprocessing)*: Reprocessing/*. The plots beginning with Lumi are before the reprocessing, and LumiWeighted are after. These are the plots only from Francesco, no macros available.

* *Figs. 27 and 28 (cross-luminometer ratio and slope plots)*: LuminometerComparisons/*. Only plots from Rafael.

* Fig. 29 (track luminosity comparison for one fill): TrackLumi/PlotTrackLumiFillPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiFill.C, which uses the data in TrackLumi/TrackLumiData/ for fill 5019, also originally from that directory.

* Figs. 30 and 31 (track luminosity VdM results): TrackLumi/PlotTrackLumiVdMPaper.C, originally derived from PLTOffline/TrackLumi2020/PlotTrackLumiVdM.C, which uses the data in TrackLumi/TrackLumiData/ for fill 6016, also originally from the same directory.

* To calculate the combined uncertainty on the luminosity, use the file combined_PLT_luminosity.txt with the script Normtags/Scripts/combineYears.py. This contains the necessary uncertainties to compute the combined value.

* SystematicsTable.png is just a snippet from the rendered PDF that can be used for things like the public results page.