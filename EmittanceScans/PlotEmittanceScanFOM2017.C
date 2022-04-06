////////////////////////////////////////////////////////////////////
//
// PlotEmittanceScanFOM2017.C -- a script to plot the FOM values from
// the 2017 emittance scans from the spreadsheet from David.
// Derived from PlotRAMSESCorrectionsPaper.C.
//
// Paul Lujan, April 6, 2022
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

// CSV file
std::string efficiencyFileName = "PLTFOM2017.csv";

// Fit functions -- beginning, end, constant term, slope term
const int nPeriods = 5;

// const float effFitParams[nPeriods][4] = {{46.42, 55.685, 1.0675, 0},
// 					 {55.809, 62.532, 2.0897, -0.0183}, 
// 					 {62.605, 71.025, 1.1628, -0.0014},
// 					 {71.040, 79.788, 1.6566, -0.0078},
// 					 {80.301, 96.362, 1.0579, -0.0002}};

const float effFitParams[nPeriods][4] = {{46.42, 55.685, 0.9968, -8.647e-5},
					 {55.809, 62.532, 1.6895, -0.01248},
					 {62.605, 71.025, 1.0837, -0.00133},
					 {71.040, 79.788, 1.4005, -0.00543},
					 {80.301, 96.362, 0.9785, -8.466e-6}};

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

    std::stringstream iLumiString(fields[3]);
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

void PlotEmittanceScanFOM2017(void) {
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

  std::vector<double> effX;
  std::vector<double> effXErr;
  std::vector<double> effY;
  std::vector<double> effYErr;
  readCSVFile(efficiencyFileName, effX, effXErr, effY, effYErr, 144, false);

  TGraphErrors *geff = new TGraphErrors(effX.size(), effX.data(), effY.data(), effXErr.data(), effYErr.data());

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  geff->Draw("AP");
  geff->SetTitle("2017 PLT efficiency corrections");
  geff->GetXaxis()->SetTitle("Integrated luminosity (fb^{-1})");
  geff->GetYaxis()->SetTitle("Relative efficiency from emittance scans");
  geff->SetMarkerStyle(kFullCircle);
  geff->SetMarkerColor(kBlue);
  geff->SetMarkerSize(1);
  geff->GetYaxis()->SetRangeUser(0.88, 1.05);
  geff->GetYaxis()->SetTitleOffset(1.35);

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

  TF1 *effFits[nPeriods];
  for (int i=0; i<nPeriods; i++) {
    effFits[i] = new TF1("r", "pol1", effFitParams[i][0], effFitParams[i][1]);
    effFits[i]->SetParameter(0, effFitParams[i][2]);
    effFits[i]->SetParameter(1, effFitParams[i][3]);
    effFits[i]->SetLineColor(kRed);
    effFits[i]->SetLineWidth(4);
    effFits[i]->Draw("same");
  }

  c1->Print("EfficiencyCorrections2017.png");
  c1->Print("EfficiencyCorrections2017.pdf");
}
