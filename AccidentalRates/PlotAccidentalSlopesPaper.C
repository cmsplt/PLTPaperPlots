///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  PlotAccidentalSlopesPaper -- plots the evolution of the accidental slope over the course of 2016 for the
//   PLT paper (to replace the old plot from Joe). This is an adapted version of PlotAccidentalRatesPaper; the
//   basic part of reading, plotting, and fitting the accidental rates for each fill is the same, but it also
//   generates a new plot with the slope for each fill.
//      PL, May 23, 2022
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// To use: set nFiles to the total number of fills, then just specify
// the file names, labels, and whether you want the fit to be drawn for
// each fill in the arrays below.

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

// This contains all of the fills from 2016 in Joe's directory, with bad fills (too few points for slope to
// be well determined, obvious discontinuity in rate, high-pileup test fills, etc.) manually removed.
const int nFiles = 41;
std::string fileNames[nFiles] = {
  "CombinedRates_4925_clean.txt",
  "CombinedRates_4935.txt",
  "CombinedRates_4942.txt",
  "CombinedRates_4958.txt",
  "CombinedRates_4965.txt",
  "CombinedRates_4985.txt",
  "CombinedRates_5005_MuScan.txt",
  "CombinedRates_5013.txt",
  "CombinedRates_5024.txt",
  "CombinedRates_5072.txt",
  "CombinedRates_5083.txt",
  "CombinedRates5106.txt",
  "CombinedRates_5108.txt",
  "CombinedRates_5112.txt",
  "CombinedRates_5151_MuScan.txt",
  "CombinedRates_5154.txt",
  "CombinedRates_5162.txt",
  "CombinedRates_5163.txt",
  "CombinedRates_5173.txt",
  "CombinedRates_5179.txt",
  "CombinedRates_5181.txt",
  "CombinedRates_5206.txt",
  "CombinedRates_5211.txt",
  "CombinedRates_5251.txt",
  "CombinedRates_5254.txt",
  "CombinedRates_5257.txt",
  "CombinedRates_5264.txt",
  "CombinedRates_5266.txt",
  "CombinedRates_5267.txt",
  "CombinedRates_5282.txt",
  "CombinedRates_5288.txt",
  "CombinedRates_5339.txt",
  "CombinedRates_5345.txt",
  "CombinedRates_5352.txt",
  "CombinedRates_5355.txt",
  "CombinedRates_5401.txt",
  "CombinedRates_5433.txt",
  "CombinedRates_5437.txt",
  "CombinedRates_5441.txt",
  "CombinedRates_5446.txt",
  "CombinedRates_5448.txt",
};
// Fill luminosity, from the Run2FillData spreadsheet
std::map<int, float> fillIntLumi = {{4851, 0}, {4852, 0.000}, {4856, 0.000}, {4861, 0.001}, {4874, 0.002}, {4879, 0.002}, {4888, 0.006}, {4889, 0.006}, {4890, 0.007}, {4892, 0.010}, {4895, 0.017}, {4896, 0.040}, {4905, 0.042}, {4906, 0.051}, {4910, 0.060}, {4915, 0.088}, {4919, 0.147}, {4924, 0.202}, {4925, 0.236}, {4926, 0.267}, {4930, 0.370}, {4935, 0.401}, {4937, 0.474}, {4942, 0.474}, {4945, 0.513}, {4947, 0.513}, {4953, 0.782}, {4954, 0.792}, {4956, 0.792}, {4958, 0.799}, {4960, 0.902}, {4961, 0.909}, {4964, 1.046}, {4965, 1.251}, {4976, 1.409}, {4979, 1.467}, {4980, 1.716}, {4984, 2.069}, {4985, 2.077}, {4988, 2.386}, {4990, 2.740}, {5005, 2.976}, {5013, 3.024}, {5017, 3.460}, {5020, 3.948}, {5021, 4.354}, {5024, 4.556}, {5026, 4.755}, {5027, 5.158}, {5028, 5.361}, {5029, 5.468}, {5030, 5.612}, {5038, 6.135}, {5043, 6.221}, {5045, 6.768}, {5048, 7.479}, {5052, 7.733}, {5056, 8.097}, {5059, 8.116}, {5060, 8.216}, {5068, 8.337}, {5069, 8.459}, {5071, 9.016}, {5072, 9.373}, {5073, 9.544}, {5076, 10.041}, {5078, 10.580}, {5080, 11.144}, {5083, 11.650}, {5085, 12.031}, {5091, 12.566}, {5093, 12.878}, {5095, 13.430}, {5096, 14.001}, {5097, 14.518}, {5101, 15.068}, {5102, 15.250}, {5105, 15.790}, {5106, 16.442}, {5107, 16.957}, {5108, 17.154}, {5109, 17.304}, {5110, 17.714}, {5111, 17.748}, {5112, 18.234}, {5116, 18.349}, {5117, 18.513}, {5149, 18.579}, {5151, 18.579}, {5154, 18.634}, {5161, 19.000}, {5162, 19.340}, {5163, 19.377}, {5169, 19.730}, {5170, 19.760}, {5173, 19.781}, {5179, 20.217}, {5181, 20.368}, {5183, 20.738}, {5187, 21.051}, {5194, 21.347}, {5196, 21.347}, {5197, 21.347}, {5198, 21.529}, {5199, 21.977}, {5205, 22.497}, {5206, 22.720}, {5209, 22.824}, {5210, 22.863}, {5211, 22.987}, {5213, 23.418}, {5219, 23.452}, {5222, 23.560}, {5223, 23.588}, {5229, 23.603}, {5246, 23.791}, {5247, 23.791}, {5251, 23.798}, {5253, 24.039}, {5254, 24.486}, {5256, 24.703}, {5257, 24.799}, {5258, 25.231}, {5261, 25.643}, {5264, 26.189}, {5265, 26.421}, {5266, 26.506}, {5267, 26.901}, {5270, 27.236}, {5274, 27.313}, {5275, 27.799}, {5276, 27.896}, {5277, 28.148}, {5279, 28.746}, {5282, 29.086}, {5287, 29.551}, {5288, 30.093}, {5330, 30.494}, {5331, 30.494}, {5332, 30.506}, {5338, 30.514}, {5339, 30.602}, {5340, 31.097}, {5345, 31.591}, {5351, 32.004}, {5352, 32.420}, {5355, 32.803}, {5370, 33.209}, {5385, 33.234}, {5386, 33.234}, {5391, 33.245}, {5393, 33.284}, {5394, 33.783}, {5395, 34.250}, {5401, 34.296}, {5405, 34.509}, {5406, 34.676}, {5412, 34.939}, {5416, 34.946}, {5418, 35.561}, {5421, 35.936}, {5422, 36.394}, {5423, 36.394}, {5424, 36.929}, {5426, 37.061}, {5427, 37.063}, {5433, 37.297}, {5437, 37.502}, {5439, 37.731}, {5441, 38.149}, {5442, 38.447}, {5443, 38.890}, {5446, 39.307}, {5448, 39.646}, {5450, 40.026}, {5451, 40.137}, {5456, 40.522}};

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

void PlotAccidentalSlopesPaper(void) {
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

  TCanvas *c1 = new TCanvas("c1", "c1", 900, 750);
  TGraph* g[nFiles];
  TF1* f[nFiles];

  for (int i=0; i<nFiles; ++i) {
    g[i] = readCombinedFile(fileNames[i]);
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
  gall->GetYaxis()->SetRangeUser(1.0, 20.0);

  for (int i=0; i<nFiles; ++i) {
    g[i]->Draw("P same");
    g[i]->SetMarkerStyle(kFullCircle);
    int icolor = i+2;
    if (icolor >= 5) icolor++;
    if (icolor >= 10) icolor++;
    g[i]->SetMarkerColor(icolor);
    g[i]->SetLineColor(icolor);
    g[i]->SetMarkerSize(1);

    char buf[32];
    sprintf(buf, "f%d", i);
    f[i] = new TF1(buf, "pol1");
    f[i]->SetLineColor(icolor);
    f[i]->SetLineWidth(1);
    g[i]->Fit(f[i], "", "", 0, 9.0);
  }
  g[0]->Draw("P same");

  TLegend *l = new TLegend(0.60, 0.13, 0.94, 0.30);
  for (int i=0; i<nFiles; ++i) {
    l->AddEntry(g[i], fileNames[i].c_str(), "LP");
  }
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->Draw();

  // OK, now we have all the fits, now let's plot the slopes for all of them!
  float xval[nFiles];
  float xerr[nFiles];
  float yval[nFiles];
  float yerr[nFiles];
  for (int i=0; i<nFiles; ++i) {
    // extract fill number from file name string
    int fill = 0;
    size_t firstDigit = fileNames[i].find_first_of("0123456789");
    if (firstDigit != std::string::npos) {
      std::stringstream fillDigits(fileNames[i].substr(firstDigit));
      fillDigits >> fill;
    }
    if (fill == 0)
      std::cerr << "Failed to get fill number from file name " << fileNames[i] << "!!!" << std::endl;
    //std::cout << fileNames[i] << " " << fill << " " << fillIntLumi[fill] << " " << f[i]->GetParameter(1) << " +/- " << f[i]->GetParError(1) << std::endl;

    //xval[i] = fill;
    xval[i] = fillIntLumi[fill];
    xerr[i] = 0;
    yval[i] = f[i]->GetParameter(1);
    yerr[i] = f[i]->GetParError(1);
  }

  TGraphErrors *allSlopes = new TGraphErrors(nFiles, xval, yval, xerr, yerr);
  TCanvas *c2 = new TCanvas("c2", "c2", 800, 800);
  allSlopes->Draw("AP");
  allSlopes->SetTitle("");
  allSlopes->SetMarkerStyle(kFullCircle);
  allSlopes->SetMarkerSize(1);
  allSlopes->SetMarkerColor(kBlue);
  allSlopes->SetLineColor(kBlue);
  //allSlopes->GetXaxis()->SetTitle("Fill number");
  allSlopes->GetXaxis()->SetTitle("Integrated luminosity [fb^{-1}]");
  allSlopes->GetYaxis()->SetTitle("Slope of accidental rate [%/(Hz/#mub)]");
  allSlopes->GetYaxis()->SetTitleOffset(1.1);
  allSlopes->Fit("pol1");

  TText *t1 = new TText(0, 0, "CMS");
  t1->SetNDC();
  t1->SetX(0.81); // 0.585 for Preliminary
  t1->SetY(0.88);
  t1->SetTextFont(61);
  t1->SetTextSize(0.05);
  t1->Draw();
  TText *t2 = new TText(0, 0, "Preliminary");
  t2->SetNDC();
  t2->SetX(0.70);
  t2->SetY(0.88);
  t2->SetTextFont(52);
  t2->SetTextSize(0.05);
  //t2->Draw();
  TText *t3;
  t3 = new TText(0, 0, "2016");
  t3->SetNDC();
  t3->SetX(0.81);
  t3->SetY(0.82);
  t3->SetTextSize(0.05);
  t3->Draw();

  c2->Print("AccidentalSlopes2016.pdf");
  c2->Print("AccidentalSlopes2016.png");
}
