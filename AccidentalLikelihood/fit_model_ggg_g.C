
{
  using namespace RooFit;

  gROOT->SetStyle("Plain");                  
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetOptStat(0);

  uint32_t track_init = 0;
  uint32_t step = 50000;

  Int_t N;
  Double_t x, y;
  Int_t ntracks;
  
  TFile f1("Fill_4979_v2.root");
  TTree *t;
  f1.GetObject("T", t);

  // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
  RooRealVar SlopeY("SlopeY", "SlopeY", 0, -0.12, 0.12);
  RooRealVar track("track", "track", track_init, track_init + step - 1);
  RooRealVar event_time("event_time", "event_time", 0);

  // Read tree
  RooDataSet dataRead("dataRead", "dataset with SlopeY", t, RooArgSet(track, event_time, SlopeY));
  dataRead.Print("V");

  // Define Fixed Signal Gaussians
  RooRealVar m0("m0", "mean 0", 0.026902);
  RooRealVar m1("m1", "mean 1", 0.02607);
  RooRealVar m2("m2", "mean 2", 0.030);

  RooRealVar s0("s0", "sigma 0", 0.001160);
  RooRealVar s1("s1", "sigma 1", 0.0036);
  RooRealVar s2("s2", "sigma 2", 0.0150);

  RooGaussian g0("g0", "gaussian PDF 0", SlopeY, m0, s0);
  RooGaussian g1("g1", "gaussian PDF 1", SlopeY, m1, s1);
  RooGaussian g2("g2", "gaussian PDF 2", SlopeY, m2, s2);

  // Define Signal
  RooRealVar frac0("frac0", "fraction 0", 0.798);
  RooRealVar frac1("frac1", "fraction 1", 0.135);
  RooAddPdf sig("sig", "g0+g1+g2", RooArgList(g0, g1, g2), RooArgList(frac0, frac1));

  // Define background
  RooRealVar bkg_m0("bkg_m0", "bkg_m0", -0.1, 0.1);
  RooRealVar bkg_s0("bkg_s0", "bkg_s0", 0., 0.1);
  RooRealVar bkg_frac0("bkg_frac0", "bkg_frac0", 0., 1.);

  RooGaussian bkg("bkg", "bkg", SlopeY, bkg_m0, bkg_s0);

  //Define Model
  RooAddPdf model("model", "model (ggg + g)", RooArgList(bkg, sig), bkg_frac0);

  // Construct plot frame
  RooPlot *frame1 = SlopeY.frame(Range(-0.05, 0.1));
  RooPlot *frame2 = SlopeY.frame(Range(-0.05, 0.1), Title("Residual Distribution"));
  RooPlot *frame3 = SlopeY.frame(Range(-0.05, 0.1), Title("Pull Distribution"));

  // Plot data in frame
  dataRead.plotOn(frame1, Name("dataSet"));

  // Fit data
  model.fitTo(dataRead, Save());

  model.plotOn(frame1, Components(RooArgSet(sig)), LineColor(kGreen), LineStyle(kDotted), Name("signal"));
  model.plotOn(frame1, Components(RooArgSet(bkg)), LineColor(kRed), LineStyle(kDashed), Name("background"));
  model.plotOn(frame1, Name("model"));

  // Removing Errors bars when bin value is zero
  RooHist *RH = (RooHist *)frame1->findObject("dataSet");

  N = RH->GetN();
  for (int i = 0; i < N; i++)
  {
    RH->GetPoint(i, x, y);
    if (y == 0.0)
    {
      RH->SetPointError(i, 0., 0., 0., 0.);
    }
  }

  // Start new canvas (default)
  TCanvas *c0 = new TCanvas("c0", "c0", 1000, 900);
  c0->GetPad(0)->SetLogy();

  frame1->GetXaxis()->SetTitleSize(0.0425);
  frame1->GetYaxis()->SetTitleSize(0.0425);
  frame1->GetXaxis()->SetTitle("Track y slope");

  frame1->GetXaxis()->SetTitleOffset(1.0);
  frame1->GetYaxis()->SetTitleOffset(1.1);

  frame1->SetTitle("");
  


  frame1->SetMaximum(1e5);
  frame1->SetMinimum(0.5);

  frame1->Draw();

  TLegend *leg1 = new TLegend(0.58,0.70,0.84,0.87);


//https://root-forum.cern.ch/t/legend-with-rooplot/10966

  leg1->SetFillColor(kWhite);
  leg1->SetLineColor(kWhite);
  leg1->AddEntry("dataSet","Data", "P");
  leg1->AddEntry("signal","Template from VdM fit", "L");
  leg1->AddEntry("background","Accidentals", "L");
  leg1->AddEntry("model","Total fit","L");
  leg1->SetTextSize(0.035);
  leg1->Draw();

  // Add CMS Preliminary 2016 text aligned using GUI
  pt = new TPaveText(-0.04470191,30819.76,-0.02422345,61907.79,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetTextFont(61);
  pt->SetTextAlign(12);
  pt->SetTextSize(0.05);
  pt_LaTex = pt->AddText("CMS");
  pt->Draw();

  pt = new TPaveText(-0.04413828,10273.93,-0.02365982,30287.01,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetTextAlign(12);
  pt->SetTextFont(61);
  pt->SetTextSize(0.05);
  pt_LaTex = pt->AddText("2016");
  pt->Draw();

  c0->SaveAs("MaxL_CMS.png");
  c0->SaveAs("MaxL_CMS.pdf");
  c0->SaveAs("MaxL_CMS.root");

  pt = new TPaveText(-0.02666583,27758.24,0.01466683,56738.89,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetTextFont(52);
  pt->SetTextAlign(12);
  pt->SetTextSize(0.05);
  pt_LaTex = pt->AddText("Preliminary");
  pt->Draw();
  
  c0->SaveAs("MaxL_CMSpreliminary.png");
  c0->SaveAs("MaxL_CMSpreliminary.pdf");
  c0->SaveAs("MaxL_CMSpreliminary.root");
}
