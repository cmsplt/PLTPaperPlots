////////////////////////////////////////////////////////////////////
//
// PlotRAMSESCorrectionsPaper -- plots the 2016 corrections derived
// from the PLT/RAMSES ratios and slopes for the paper. Unlike most
// of the other scripts in this repo, it's not derived from a previous
// ROOT script but uses the data from David's Excel spreadsheet
// (here converted to two CSVs).
//
// Paul Lujan, February 24, 2021
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

std::string slopeFileName = "pltramses2016slope.csv";
std::string ratioFileName = "pltramses2016ratio.csv";

// Fit functions -- beginning, end, constant term, slope term
const int nPeriods = 5;
const float slopeFitParams[nPeriods][4] = {{4.387, 23.176, -0.0013287, -0.0002726},
					   {23.785, 30.295, -0.005, 0},
					   {30.516, 35.167, 0.02125, -0.0008979},
					   {35.736, 42.718, 0.0694434, -0.001938},
					   {42.863, 46.030, 0.04454943, -0.0012999}};

const float ratioFitParams[nPeriods][4] = {{4.387, 23.958, 0.99749, -0.0037736},
					   {24.040, 27.545, 1.03007726, -0.0036041},
					   {28.072, 35.167, 1.20194981, -0.0090217},
					   {35.736, 42.718, 1.09083291, -0.0041737},
					   {42.863, 46.030, 1.28129725, -0.0081923}};


// To account for the fact that the final corrections are scaled to be equal to 1 at the VdM
// fill, we have to apply a scale factor to the ratio fits in order for them to match properly.
// (Column K in the original spreadsheet)
const float ratioFitScaleFactor = 1/0.978594023;

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

    std::stringstream iLumiString(fields[3]);
    double il;
    iLumiString >> il;

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

void PlotRAMSESCorrectionsPaper(void) {
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
  //gStyle->SetOptFit(1111);

  // First, the slope.
  // Read CSV file.
  std::vector<double> slopeX;
  std::vector<double> slopeXErr;
  std::vector<double> slopeY;
  std::vector<double> slopeYErr;
  // scale by 100 to get percent
  readCSVFile(slopeFileName, slopeX, slopeXErr, slopeY, slopeYErr, 5, true, 100);

  // Plot it. Since most of the points have negligibly small errors, let's just ignore them.
  //TGraph *gslope = new TGraph(slopeX.size(), slopeX.data(), slopeY.data());
  TGraphErrors *gslope = new TGraphErrors(slopeX.size(), slopeX.data(), slopeY.data(), slopeXErr.data(), slopeYErr.data());

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
  gslope->Draw("AP");
  gslope->SetTitle("2016 PLT slope corrections");
  gslope->GetXaxis()->SetTitle("Integrated luminosity (fb^{-1})");
  gslope->GetYaxis()->SetTitle("PLT/RAMSES slope [%/(Hz/#mub)]");
  gslope->SetMarkerStyle(kFullCircle);
  gslope->SetMarkerColor(kBlue);
  gslope->SetMarkerSize(1);
  gslope->GetYaxis()->SetRangeUser(-0.02*100, 0.01*100);
  gslope->GetYaxis()->SetTitleOffset(1.35);

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
  TText *t3 = new TText(0, 0, "2016");
  t3->SetNDC();
  t3->SetX(0.84);
  t3->SetY(0.78);
  t3->SetTextSize(0.05);
  t3->Draw();

  TF1 *slopeFits[nPeriods];
  for (int i=0; i<nPeriods; i++) {
    slopeFits[i] = new TF1("r", "pol1", slopeFitParams[i][0], slopeFitParams[i][1]);
    slopeFits[i]->SetParameter(0, slopeFitParams[i][2]*100);
    slopeFits[i]->SetParameter(1, slopeFitParams[i][3]*100);
    slopeFits[i]->SetLineColor(kRed);
    slopeFits[i]->SetLineWidth(4);
    slopeFits[i]->Draw("same");
  }

  c1->Print("PLTRAMSESSlope2016.png");
  c1->Print("PLTRAMSESSlope2016.pdf");

  // Now, do basically the same thing again for the ratio plots.
  std::vector<double> ratioX;
  std::vector<double> ratioXErr;
  std::vector<double> ratioY;
  std::vector<double> ratioYErr;
  readCSVFile(ratioFileName, ratioX, ratioXErr, ratioY, ratioYErr, 8, false);

  // Plot it. No errors for the ratios.
  TGraph *gratio = new TGraph(ratioX.size(), ratioX.data(), ratioY.data());

  TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
  gratio->Draw("AP");
  gratio->SetTitle("2016 PLT efficiency corrections");
  gratio->GetXaxis()->SetTitle("Integrated luminosity (fb^{-1})");
  gratio->GetYaxis()->SetTitle("PLT/RAMSES ratio");
  gratio->SetMarkerStyle(kFullCircle);
  gratio->SetMarkerColor(kBlue);
  gratio->SetMarkerSize(1);
  gratio->GetYaxis()->SetRangeUser(0.90, 1.02);
  gratio->GetYaxis()->SetTitleOffset(1.35);

  t1->Draw();
  t2->Draw();
  t3->Draw();

  TF1 *ratioFits[nPeriods];
  for (int i=0; i<nPeriods; i++) {
    ratioFits[i] = new TF1("r", "pol1", ratioFitParams[i][0], ratioFitParams[i][1]);
    ratioFits[i]->SetParameter(0, ratioFitParams[i][2]*ratioFitScaleFactor);
    ratioFits[i]->SetParameter(1, ratioFitParams[i][3]*ratioFitScaleFactor);
    ratioFits[i]->SetLineColor(kRed);
    ratioFits[i]->SetLineWidth(4);
    ratioFits[i]->Draw("same");
  }

  c2->Print("PLTRAMSESRatio2016.png");
  c2->Print("PLTRAMSESRatio2016.pdf");
}
