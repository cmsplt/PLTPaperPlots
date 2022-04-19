////////////////////////////////////////////////////////////////////
//
// PlotAlignment4444Paper -- plots the aligment residual plots
// from fill 4444 for the paper. Uses the ROOT file created by
// CalculateAlignment.
//
// To be precise, the ROOT file used by this script was created
// by modifying CalculateAlignment.cc to process 30M events (instead of
// the default 3M) and then running it as follows:
// ./CalculateAlignment /localdata/2015/SLINK/Slink_20150930.234122.dat GainCal/GainCalFits_20150923.225334.dat ALIGNMENT/Alignment_IdealInstall.dat
//
// Paul Lujan, February 25, 2021
//
////////////////////////////////////////////////////////////////////

#include <string>
#include "TROOT.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TText.h"

std::string fileName = "alignment_4444_30M.root";

void PlotAlignment4444Paper(void) {
  gROOT->SetStyle("Plain");
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(0.1);
  //gStyle->SetTitleY(1.0);
  //gStyle->SetTitleH(0.055);
  gStyle->SetTitleW(0.8);
  gStyle->SetTitleFontSize(0.055);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1111);

  TFile *f = new TFile(fileName.c_str());
  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 400);
  c1->Divide(3, 1);

  c1->cd(1);
  TProfile *h1 = (TProfile*)f->Get("XdY_NormalizedResidual_Ch11_ROC1_First");
  h1->SetTitle("Before alignment");
  h1->GetXaxis()->SetTitle("Track position in x [cm]");
  h1->GetYaxis()->SetTitle("Residual distance in y [cm]");
  h1->SetLabelSize(0.05, "xy");
  h1->SetTitleSize(0.05, "xy");
  h1->SetMaximum(-0.025);
  h1->SetMinimum(-0.045);
  h1->GetYaxis()->SetTitleOffset(2.2);
  h1->GetXaxis()->SetRangeUser(-0.4, 0.4);
  h1->SetStats(0);
  // get the fit line so we can modify it
  TF1 *f1 = h1->GetFunction("pol1");
  f1->SetLineWidth(1);
  f1->SetLineColor(kBlue);
  h1->Draw();

  TText *t1 = new TText(0, 0, "CMS");
  t1->SetNDC();
  t1->SetX(0.23);
  t1->SetY(0.84);
  t1->SetTextFont(61);
  t1->SetTextSize(0.05);
  t1->Draw();
  TText *t2 = new TText(0, 0, "Preliminary");
  t2->SetNDC();
  t2->SetX(0.345);
  t2->SetY(0.84);
  t2->SetTextFont(52);
  t2->SetTextSize(0.05);
  t2->Draw();
  TText *t3 = new TText(0, 0, "2015");
  t3->SetNDC();
  t3->SetX(0.23);
  t3->SetY(0.78);
  t3->SetTextSize(0.05);
  t3->Draw();

  c1->cd(2);
  TProfile *h2 = (TProfile*)f->Get("XdY_NormalizedResidual_Ch11_ROC1_Second");
  h2->SetTitle("After rotation");
  h2->GetXaxis()->SetTitle("Track position in x [cm]");
  h2->GetYaxis()->SetTitle("Residual distance in y [cm]");
  h2->SetLabelSize(0.05, "xy");
  h2->SetTitleSize(0.05, "xy");
  h2->SetMaximum(-0.025);
  h2->SetMinimum(-0.045);
  h2->GetYaxis()->SetTitleOffset(2.2);
  h2->GetXaxis()->SetRangeUser(-0.4, 0.4);
  h2->SetStats(0);
  TF1 *f2 = h2->GetFunction("pol1");
  f2->SetLineWidth(1);
  f2->SetLineColor(kBlue);
  h2->Draw();

  t1->Draw();
  t2->Draw();
  t3->Draw();

  c1->cd(3);
  TProfile *h3 = (TProfile*)f->Get("XdY_NormalizedResidual_Ch11_ROC1_Third");
  h3->SetTitle("After translation");
  h3->GetXaxis()->SetTitle("Track position in x [cm]");
  h3->GetYaxis()->SetTitle("Residual distance in y [cm]");
  h3->SetLabelSize(0.05, "xy");
  h3->SetTitleSize(0.05, "xy");
  h3->SetMaximum(0.01);
  h3->SetMinimum(-0.01);
  h3->GetYaxis()->SetTitleOffset(2.2);
  h3->GetXaxis()->SetRangeUser(-0.4, 0.4);
  h3->SetStats(0);
  h3->Draw();

  t1->Draw();
  t2->Draw();
  t3->Draw();

  c1->Print("Alignment_XdY_4444_Ch7_ROC1.pdf");
  c1->Print("Alignment_XdY_4444_Ch7_ROC1.png");
}
