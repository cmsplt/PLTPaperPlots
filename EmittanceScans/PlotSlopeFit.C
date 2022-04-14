////////////////////////////////////////////////////////////////////
//
// PlotSlopeFit.C -- a script to plot the nonlinearity slope
// fit from the data for two emittance scans in a fill (here,
// fill 6325). This uses the spreadsheet Fill6325PLT.xlsx from David,
// from which I've extracted the two CSV files used in this script.
//
// Paul Lujan, April 14, 2022
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

// CSV files
std::string scan1 = "6325_orig_scan1.csv";
std::string scan2 = "6325_orig_scan2.csv";

// no fits for slope graph

void readCSVFile(std::string fileName,
		 std::vector<double>& sbilLeading, std::vector<double>& sbilErrLeading,
		 std::vector<double>& xsecLeading, std::vector<double>& xsecErrLeading,
		 std::vector<double>& sbilTrain, std::vector<double>& sbilErrTrain,
		 std::vector<double>& xsecTrain, std::vector<double>& xsecErrTrain) {
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
    if (line.at(0) == 'X') continue; // skip header line

    // Break into fields
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ','))
      fields.push_back(field);
    std::vector<double> data;
    for (int iField = 3; iField < 7; ++iField) {
      std::stringstream fieldString(fields[iField]);
      double val;
      fieldString >> val;
      data.push_back(val);
    }
    std::stringstream leadString(fields[7]);
    int isLeading;
    leadString >> isLeading;

    if (isLeading) {
      //std::cout << "BX " << fields[2] << " is leading" << std::endl;
      sbilLeading.push_back(data[2]);
      sbilErrLeading.push_back(data[3]);
      xsecLeading.push_back(data[0]);
      xsecErrLeading.push_back(data[1]);
    } else {
      //std::cout << "BX " << fields[2] << " is train" << std::endl;
      sbilTrain.push_back(data[2]);
      sbilErrTrain.push_back(data[3]);
      xsecTrain.push_back(data[0]);
      xsecErrTrain.push_back(data[1]);
    }
  }
}

void PlotSlopeFit(void) {
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

  std::vector<double> sbilLeading;
  std::vector<double> sbilErrLeading;
  std::vector<double> xsecLeading;
  std::vector<double> xsecErrLeading;
  std::vector<double> sbilTrain; 
  std::vector<double> sbilErrTrain;
  std::vector<double> xsecTrain; 
  std::vector<double> xsecErrTrain;
 
  readCSVFile(scan1, sbilLeading, sbilErrLeading, xsecLeading, xsecErrLeading, sbilTrain, sbilErrTrain, xsecTrain, xsecErrTrain);
  readCSVFile(scan2, sbilLeading, sbilErrLeading, xsecLeading, xsecErrLeading, sbilTrain, sbilErrTrain, xsecTrain, xsecErrTrain);

  TGraphErrors *glead = new TGraphErrors(sbilLeading.size(), sbilLeading.data(), xsecLeading.data(), sbilErrLeading.data(), xsecErrLeading.data());
  TGraphErrors *gtr = new TGraphErrors(sbilTrain.size(), sbilTrain.data(), xsecTrain.data(), sbilErrTrain.data(), xsecErrTrain.data());

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

  gtr->Draw("AP");
  gtr->SetTitle("Emittance scan results, fill 6325");
  gtr->GetXaxis()->SetTitle("SBIL (Hz/#mub)");
  gtr->GetYaxis()->SetTitle("Measured visible cross section (#mub)");
  gtr->SetMarkerStyle(kFullCircle);
  gtr->SetMarkerColor(kRed);
  gtr->SetLineColor(kRed);
  gtr->SetMarkerSize(1);
  gtr->GetYaxis()->SetTitleOffset(1.2);
  glead->Draw("same P");
  glead->SetMarkerStyle(kFullCircle);
  glead->SetMarkerColor(kBlue);
  glead->SetLineColor(kBlue);
  glead->SetMarkerSize(1);
  gtr->GetYaxis()->SetRangeUser(285, 310);

  glead->Fit("pol1", "", "", 1.5, 10.5);
  gtr->Fit("pol1", "", "", 1.5, 10.5);
  TF1* flead = glead->GetFunction("pol1");
  flead->SetLineColor(kBlue);
  TF1* ftr = gtr->GetFunction("pol1");
  ftr->SetLineColor(kRed);

  TLegend *l = new TLegend(0.45, 0.70, 0.89, 0.89);
  std::stringstream ls;
  ls << "#splitline{Leading bunches}{Slope = (" << std::fixed << std::setprecision(4) << flead->GetParameter(1) << " #pm " << flead->GetParError(1) << ") #mub/(Hz/#mub)}";
  l->AddEntry(glead, ls.str().c_str(), "PE");
  ls.str("");
  ls << "#splitline{Train bunches}{Slope = (" << std::fixed << std::setprecision(4) << ftr->GetParameter(1) << " #pm " << ftr->GetParError(1) << ") #mub/(Hz/#mub)}";
  l->AddEntry(gtr, ls.str().c_str(), "PE");
  l->SetBorderSize(0);
  l->Draw();

  TText *t1 = new TText(0, 0, "CMS");
  t1->SetNDC();
  t1->SetX(0.15);
  t1->SetY(0.85);
  t1->SetTextFont(61);
  t1->SetTextSize(0.05);
  t1->Draw();
  TText *t2 = new TText(0, 0, "Preliminary");
  t2->SetNDC();
  t2->SetX(0.235);
  t2->SetY(0.85);
  t2->SetTextFont(52);
  t2->SetTextSize(0.05);
  t2->Draw();
  TText *t3 = new TText(0, 0, "2017");
  t3->SetNDC();
  t3->SetX(0.15);
  t3->SetY(0.79);
  t3->SetTextSize(0.05);
  t3->Draw();

  std::cout << "For leading bunches, slope is " << flead->GetParameter(1) << " and intercept is " << flead->GetParameter(0) << std::endl;
  std::cout << "For train bunches, slope is " << ftr->GetParameter(1) << " and intercept is " << ftr->GetParameter(0) << std::endl;

  c1->Print("SlopeFit_6325_orig.png");
  c1->Print("SlopeFit_6325_orig.pdf");
}
