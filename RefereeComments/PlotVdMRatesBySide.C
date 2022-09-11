///////////////////////////////////////////////////////////////////////////
//
// PlotVdMRatesBySide -- given the VdM rate data extracted from a histogram
// file by histfile_extractVdMRates, plot them. This script was developed
// to answer the referee comment about the robustness of the acceptance
// after losing some telescopes.
//
// Paul Lujan, September 11 2022
//
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include "TROOT.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include "TStyle.h"
#include "TLegend.h"

const int nChan = 16;

void plotInputFile(const char *fileName, std::vector<double>& separations, std::vector<double>& rates_minusSide,
		   std::vector<double>& rates_plusSide, const char *plotTitle) {
  std::ifstream inputFile(fileName);
  if (!inputFile.is_open()) {
    std::cerr << "Couldn't open input file " << fileName << "!" << std::endl;
    exit(1);
  }
  // Go through the lines of the file.
  std::string line;
  while (1) {
    std::getline(inputFile, line);
    if (inputFile.eof()) break;
    if (line.empty()) continue; // skip blank lines
    if (line.at(0) == '#' || line.at(0) == '=') continue; // skip comment lines and header lines for new scans

    // Break into fields
    std::stringstream ss(line);
    int nStep, timestampStart, timestampEnd;
    float separation;
    float nNibbles;
    int channelData[nChan];
    ss >> nStep >> timestampStart >> timestampEnd >> separation >> nNibbles;
    for (int i=0; i<nChan; ++i)
      ss >> channelData[i];

    //std::cout << "nstep=" << nStep << "

    if (nStep == 0 || nStep == 20) {
      if (fabs(separation) < 1e-4)
	continue; // skip first and last head-on steps
      std::cout << "warning: first or last step is not head-on as expected" << std::endl;
    }
    separations.push_back(separation);
    double total_minus = 0;
    double total_plus = 0;
    for (int i=0; i<8; ++i) total_minus += channelData[i];
    for (int i=8; i<nChan; ++i) total_plus += channelData[i];
    rates_minusSide.push_back(total_minus/(nNibbles*8));
    rates_plusSide.push_back(total_plus/(nNibbles*8));
  }
  inputFile.close();

  // Finally, normalize the rates.
  double maxRateMinus = *std::max_element(rates_minusSide.begin(), rates_minusSide.end());
  double maxRatePlus = *std::max_element(rates_plusSide.begin(), rates_plusSide.end());
  for (unsigned int i=0; i<rates_minusSide.size(); ++i) {
    rates_minusSide[i] /= maxRateMinus;
    rates_plusSide[i] /= maxRatePlus;
  }

  // Plot it all.
  TGraph *g_minus = new TGraph(separations.size(), separations.data(), rates_minusSide.data());
  TGraph *g_plus = new TGraph(separations.size(), separations.data(), rates_plusSide.data());

  g_minus->Draw("AP");
  g_minus->SetTitle(plotTitle);
  g_minus->GetXaxis()->SetTitle("Beamspot position (mm)");
  g_minus->GetYaxis()->SetTitle("Normalized rates");
  g_minus->GetYaxis()->SetTitleOffset(1.2);
  g_minus->SetMarkerStyle(kFullCircle);
  g_minus->SetMarkerColor(kRed);
  g_minus->SetMarkerSize(1);

  g_plus->Draw("P same");
  g_plus->SetMarkerStyle(kFullSquare);
  g_plus->SetMarkerColor(kBlue);
  g_plus->SetMarkerSize(1);

  TLegend *l = new TLegend(0.7, 0.7, 0.9, 0.8);
  l->AddEntry(g_minus, "-z side", "P");
  l->AddEntry(g_plus, "+z side", "P");
  l->SetBorderSize(0);
  l->SetFillColor(0);
  l->Draw();
}

void PlotVdMRatesBySide(void) {
  gROOT->SetStyle("Plain");
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.1);
  gStyle->SetTitleY(1.0);
  gStyle->SetTitleH(0.09);
  gStyle->SetTitleW(0.7);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLegendBorderSize(0);
  //gStyle->SetOptFit(1111);
  gStyle->SetTitleX(0.12); // to avoid the white space in the title hitting the numbers on the axis label

  // Read input file.
  std::vector<double> separations_X;
  std::vector<double> rates_minusSide_X;
  std::vector<double> rates_plusSide_X;
  std::vector<double> separations_Y;
  std::vector<double> rates_minusSide_Y;
  std::vector<double> rates_plusSide_Y;

  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 600);
  c1->Divide(2,1);
  c1->cd(1);
  plotInputFile("VdMRates_4266_BI2X.txt", separations_X, rates_minusSide_X, rates_plusSide_X, "Fill 4266, beam 2 imaging scan, X");
  c1->cd(2);
  plotInputFile("VdMRates_4266_BI2Y.txt", separations_Y, rates_minusSide_Y, rates_plusSide_Y, "Fill 4266, beam 2 imaging scan, Y");

  c1->Print("VdM4266ImagingScan_RatesBySide.png");
}
