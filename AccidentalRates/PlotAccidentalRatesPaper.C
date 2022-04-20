////////////////////////////////////////////////////////////////////
//
//  PlotAccidentalRatesAllScans -- an enhanced version of
//  PlotAccidentalRates which plots a number of fills on the same
//  plot
//    Paul Lujan, December 6 2015
//
// Paper version created PL Feb. 24 2021
// This combines both the 2015 and 2016 plotters into a single script.
// Call this with the argument 0 for 2015 or 1 for 2016.
//
////////////////////////////////////////////////////////////////////

// To use: set nFiles to the total number of fills, then just specify
// the file names, labels, and whether you want the fit to be drawn for
// each fill in the arrays below.

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

// Magnet-on fills
const int nFiles[2] = {7, 6};
const int maxFiles = 7;
std::string fileNames[2][maxFiles] = {
  {"CombinedRates_4201_clean.txt",
   "CombinedRates_4266_AllScans_Central.txt",
   "CombinedRates_4381_clean.txt",
   "CombinedRates_MuScan_Central.txt",
   "CombinedRates_4444.txt",
   "CombinedRates_4467_clean.txt",
   "CombinedRates_4565_clean.txt"},
  {"CombinedRates_5151_MuScan.txt",
   "CombinedRates_5154.txt",
   "CombinedRates_5162.txt",
   "CombinedRates_5163.txt",
   "CombinedRates_5173.txt",
   "CombinedRates_5179.txt", ""}
};
const char *fillLabels[2][maxFiles] = {
  {"Fill 4201",
   "Fill 4266 (VdM scan)",
   "Fill 4381",
   "Fill 4435 (#mu scan)",
   "Fill 4444",
   "Fill 4467",
   "Fill 4565"},
  {"Fill 5151 (#mu scan)",
   "Fill 5154",
   "Fill 5162",
   "Fill 5163",
   "Fill 5173",
   "Fill 5179", ""}
};
const bool doAllFit = false;   // show fit to all points

// Use this to exclude a single fit from being drawn.
const bool doFit[2][maxFiles] = { {false, false, true, true, true, true, true},
				  {true, false, false, false, false, false, false} };

std::vector<double> fastOrLumiAll;
std::vector<double> fastOrLumiErrAll;
std::vector<double> accidentalRateAll;
std::vector<double> accidentalRateErrAll;

TGraph *readCombinedFile(std::string& fileName) {
  // Read input file.
  std::vector<double> fastOrLumi;
  std::vector<double> fastOrLumiErr;
  std::vector<double> accidentalRate;
  std::vector<double> accidentalRateErr;

  std::string infilename = "AccidentalData/"+fileName;
  FILE *rfile = fopen(infilename.c_str(), "r");
  if (rfile == NULL) {
    std::cerr << "Couldn't open combined rates file " << fileName << "!" << std::endl;
    return(NULL);
  }
  int nsteps, nBunches, tBegin, tEnd, nTrig, tracksAll, tracksGood, nMeas;
  double totLumi;

  fscanf(rfile, "%d %d", &nsteps, &nBunches);
  for (int i=0; i<nsteps; ++i) {
    fscanf(rfile, "%d %d %d %d %d %d %lf", &tBegin, &tEnd, &nTrig, &tracksAll, &tracksGood, &nMeas, &totLumi);
    // Process the data.
    fastOrLumi.push_back(totLumi/(nMeas*nBunches));
    fastOrLumiErr.push_back(0); // not implemented yet
    double accrate = (double)(tracksAll-tracksGood)/tracksAll;
    accidentalRate.push_back(100.0*accrate);
    accidentalRateErr.push_back(100.0*sqrt(accrate*(1-accrate)/tracksAll));

    fastOrLumiAll.push_back(fastOrLumi.back());
    fastOrLumiErrAll.push_back(fastOrLumiErr.back());
    accidentalRateAll.push_back(accidentalRate.back());
    accidentalRateErrAll.push_back(accidentalRateErr.back());
  }
  fclose(rfile);

  TGraph *g = new TGraphErrors(nsteps, &(fastOrLumi[0]), &(accidentalRate[0]),
			       &(fastOrLumiErr[0]), &(accidentalRateErr[0]));

  return g;
}

void PlotAccidentalRatesPaper(int which) {
  // style from PLTU
  gROOT->SetStyle("Plain");                  
  gStyle->SetPalette(1);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.18);
  //gStyle->SetTitleY(1.0);
  //gStyle->SetTitleH(0.06);
  //gStyle->SetTitleW(0.7);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLegendBorderSize(0);
  //gStyle->SetOptFit(1111);

  // Plot it all.

  TCanvas *c1 = new TCanvas("c1", "c1", 900, 600);
  TGraph* g[nFiles[which]];
  TF1* f[nFiles[which]];

  for (int i=0; i<nFiles[which]; ++i) {
    g[i] = readCombinedFile(fileNames[which][i]);
  }

  TGraph *gall = new TGraphErrors(fastOrLumiAll.size(), &(fastOrLumiAll[0]), &(accidentalRateAll[0]),
			       &(fastOrLumiErrAll[0]), &(accidentalRateErrAll[0]));
  gall->Draw("APX");
  //gall->SetTitle("Accidental rate vs. SBIL");
  gall->SetTitle("");
  gall->GetXaxis()->SetTitle("Uncorrected fast-or SBIL (Hz/#mub)");
  gall->GetYaxis()->SetTitle("Measured accidental rate (%)");
  gall->GetXaxis()->SetLabelSize(0.05);
  gall->GetXaxis()->SetTitleSize(0.05);
  gall->GetYaxis()->SetLabelSize(0.05);
  gall->GetYaxis()->SetTitleSize(0.05);
  gall->GetYaxis()->SetTitleOffset(0.8);
  if (which == 0)
    gall->GetYaxis()->SetRangeUser(4.5, 12.0);
  else
    gall->GetYaxis()->SetRangeUser(1.0, 10.0);

  TF1 *fall;
  if (doAllFit) {
    fall = new TF1("fall", "pol1");
    fall->SetLineColor(kBlack);
    fall->SetLineWidth(1);
    gall->Fit(fall);
  }
  
  for (int i=0; i<nFiles[which]; ++i) {
    g[i]->Draw("P same");
    g[i]->SetMarkerStyle(kFullCircle);
    int icolor = i+2;
    if (icolor >= 5) icolor++;
    if (icolor >= 10) icolor++;
    g[i]->SetMarkerColor(icolor);
    g[i]->SetLineColor(icolor);
    g[i]->SetMarkerSize(1);

    char buf[32];
    if (doFit[which][i]) {
      sprintf(buf, "f%d", i);
      f[i] = new TF1(buf, "pol1");
      f[i]->SetLineColor(icolor);
      f[i]->SetLineWidth(1);
      float maxLumi = 2.3;
      if (which == 1) maxLumi = 6.0;
      g[i]->Fit(f[i], "", "", 0, maxLumi);
    }
  }
  g[0]->Draw("P same");

  TLegend *l = new TLegend(0.60, 0.13, 0.94, 0.50);
  for (int i=0; i<nFiles[which]; ++i) {
    l->AddEntry(g[i], fillLabels[which][i], "LP");
  }
  if (doAllFit)
    l->AddEntry(fall, "Fit to all points", "L");
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->Draw();

  TText *t1 = new TText(0, 0, "CMS");
  t1->SetNDC();
  t1->SetX(0.15);
  t1->SetY(0.88);
  t1->SetTextFont(61);
  t1->SetTextSize(0.05);
  t1->Draw();
  TText *t2 = new TText(0, 0, "Preliminary");
  t2->SetNDC();
  t2->SetX(0.225);
  t2->SetY(0.88);
  t2->SetTextFont(52);
  t2->SetTextSize(0.05);
  t2->Draw();
  TText *t3;
  if (which == 0) t3 = new TText(0, 0, "2015");
  else t3 = new TText(0, 0, "2016");
  t3->SetNDC();
  t3->SetX(0.15);
  t3->SetY(0.82);
  t3->SetTextSize(0.05);
  t3->Draw();

  // if (doAllFit)
  //   std::cout << "Value of total fit at x=2 is " << fall->Eval(2.0) << std::endl;
  // for (int i=0; i<nFiles[which]; ++i) {
  //   if (doFit[which][i])
  //     std::cout << "Value of fit " << i << " at x=2 is " << f[i]->Eval(2.0) << std::endl;
  // }

  if (which == 0) {
    c1->Print("AccidentalRates2015.png");
    c1->Print("AccidentalRates2015.pdf");
  } else {
    c1->Print("AccidentalRates2016.png");
    c1->Print("AccidentalRates2016.pdf");
  }
}
