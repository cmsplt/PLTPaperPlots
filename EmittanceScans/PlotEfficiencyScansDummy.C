////////////////////////////////////////////////////////////////////
//
// PlotEfficiencyScansDummy.C -- a quick and dirty script to make
// some placeholder plots of the efficiency scan corrections
// from the spreadsheets we do have from David while we await
// the final numbers.
//
// Paul Lujan, March 27, 2022
//
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include "TROOT.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TText.h"

// csv file with dummy linearity data. note fills with no train data are discarded
std::string slopeFileName = "linearity2018.csv";

// Fit functions -- beginning, end, constant term, slope term
const int nPeriods = 5;

const float ratioFitParams[nPeriods][4] = {{46.42, 55.685, 1.0675, 0},
					   {55.809, 62.532, 2.0897, -0.0183}, 
					   {62.605, 71.025, 1.1628, -0.0014},
					   {71.040, 79.788, 1.6566, -0.0078},
					   {80.301, 96.362, 1.0579, -0.0002}};

// no fits for slope graph

void readCSVFile(std::string fileName, std::vector<double>& iLumi, std::vector<double>& iLumiErr,
		 std::vector<double>& y, std::vector<double>& yErr, int targetField,
		 bool hasError, float scaleFactor = 1.0) {
  std::ifstream csvFile(fileName.c_str());
  if (!csvFile.is_open()) {
    std::cerr << "ERROR: cannot open csv file: " << fileName << std::endl;
    return;
  }
    
  // Go through the lines of the file.
  int lsnum = 0;
  std::string line;
  while (1) {
    std::getline(csvFile, line);
    if (csvFile.eof()) break;
    if (line.empty()) continue; // skip blank lines
    if (line.at(0) == 'F') continue; // skip header line

    // Break into fields
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ','))
      fields.push_back(field);

    std::stringstream iLumiString(fields[1]);
    double il;
    iLumiString >> il;
    if (il == 0) continue; // couldn't read lumi, this line probably isn't actually a data line

    std::stringstream yString(fields[targetField]);
    double yval;
    yString >> yval;

    double yerrval;
    if (hasError) {
      std::stringstream yErrString(fields[targetField+1]);
      yErrString >> yerrval;
    } else {
      yerrval = 0;
    }

    //std::cout << line.at(0) << " iLumi " << il << " slope " << yval << std::endl;

    iLumi.push_back(il);
    iLumiErr.push_back(0);
    y.push_back(yval*scaleFactor);
    yErr.push_back(yerrval*scaleFactor);
  }
}

void PlotEfficiencyScansDummy(void) {
  gROOT->SetStyle("Plain");
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.2);
  //gStyle->SetTitleY(1.0);
  gStyle->SetTitleH(0.055);
  //gStyle->SetTitleW(0.7);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetOptStat(0);

  // First do the ratios. For this there's no actual data points so we just make a dummy histogram
  // to set the frame.

  TH1F *h1 = new TH1F("h1", "", 10, 45, 100);

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  h1->Draw("");
  h1->SetTitle("2017 PLT efficiency corrections");
  h1->GetXaxis()->SetTitle("Integrated luminosity (fb^{-1})");
  h1->GetYaxis()->SetTitle("Efficiency from emittance scans");
  h1->SetMarkerStyle(kFullCircle);
  h1->SetMarkerColor(kBlue);
  h1->SetMarkerSize(1);
  h1->GetYaxis()->SetRangeUser(0.9, 1.15);
  h1->GetYaxis()->SetTitleOffset(1.35);

  TText *t1 = new TText(0, 0, "CMS");
  t1->SetNDC();
  t1->SetX(0.67);
  t1->SetY(0.84);
  t1->SetTextFont(61);
  t1->SetTextSize(0.05);
  t1->Draw();
  TText *t2 = new TText(0, 0, "Preliminary");
  t2->SetNDC();
  t2->SetX(0.755);
  t2->SetY(0.84);
  t2->SetTextFont(52);
  t2->SetTextSize(0.05);
  t2->Draw();
  TText *t3 = new TText(0, 0, "2017");
  t3->SetNDC();
  t3->SetX(0.84);
  t3->SetY(0.78);
  t3->SetTextSize(0.05);
  t3->Draw();

  TF1 *ratioFits[nPeriods];
  for (int i=0; i<nPeriods; i++) {
    ratioFits[i] = new TF1("r", "pol1", ratioFitParams[i][0], ratioFitParams[i][1]);
    ratioFits[i]->SetParameter(0, ratioFitParams[i][2]);
    ratioFits[i]->SetParameter(1, ratioFitParams[i][3]);
    ratioFits[i]->SetLineColor(kRed);
    ratioFits[i]->SetLineWidth(4);
    ratioFits[i]->Draw("same");
  }

  //c1->Print("EfficiencyCorrections2017Dummy.png");
  c1->Print("EfficiencyCorrections2017Dummy.pdf");
 
  // Next, the slope.
  // Read CSV file.
  std::vector<double> slopeX;
  std::vector<double> slopeXErr;
  std::vector<double> slopeY;
  std::vector<double> slopeYErr;
  readCSVFile(slopeFileName, slopeX, slopeXErr, slopeY, slopeYErr, 10, true, 1);

  // Plot it. Since most of the points have negligibly small errors, let's just ignore them.
  //TGraph *gslope = new TGraph(slopeX.size(), slopeX.data(), slopeY.data());
  TGraphErrors *gslope = new TGraphErrors(slopeX.size(), slopeX.data(), slopeY.data(), slopeXErr.data(), slopeYErr.data());

  TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
  gslope->Draw("AP");
  gslope->SetTitle("2018 PLT slope corrections");
  gslope->GetXaxis()->SetTitle("Integrated luminosity (fb^{-1})");
  gslope->GetYaxis()->SetTitle("Linearity from emittance scan data [%]");
  gslope->SetMarkerStyle(kFullCircle);
  gslope->SetMarkerColor(kBlue);
  gslope->SetMarkerSize(1);
  gslope->GetYaxis()->SetRangeUser(0, 4);
  gslope->GetYaxis()->SetTitleOffset(1.35);

  t1->Draw();
  t2->Draw();
  TText *t3a = new TText(0, 0, "2018");
  t3a->SetNDC();
  t3a->SetX(0.84);
  t3a->SetY(0.78);
  t3a->SetTextSize(0.05);
  t3a->Draw();

  // TF1 *slopeFits[nPeriods];
  // for (int i=0; i<nPeriods; i++) {
  //   slopeFits[i] = new TF1("r", "pol1", slopeFitParams[i][0], slopeFitParams[i][1]);
  //   slopeFits[i]->SetParameter(0, slopeFitParams[i][2]*100);
  //   slopeFits[i]->SetParameter(1, slopeFitParams[i][3]*100);
  //   slopeFits[i]->SetLineColor(kRed);
  //   slopeFits[i]->SetLineWidth(4);
  //   slopeFits[i]->Draw("same");
  // }

  //c2->Print("SlopeCorrections2018Dummy.png");
  c2->Print("SlopeCorrections2018Dummy.pdf");
}
