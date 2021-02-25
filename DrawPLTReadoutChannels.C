// DrawPLTReadoutChanels.C
//
// A simple script to draw the position of the PLT readout channels on the PLT schematic.  This was originally
// just something I did in PowerPoint, but I decided to create it in ROOT to get the positioning of the
// individual scopes exactly right.
//
// Paul Lujan, February 25, 2021

// Note: all calculations are in pixel coordinates but to draw we have to convert to pad coordinates
// (divide by w or h as appropriate).

// overall width and height of canvas
float w = 980;
float h = 480;

// margin on left and at the top
float mleft = 20;
float mtop = 80;
float gap = 40;

// inner and outer radius of a single quadrant
float rout = 180;
float rin = 120;

// line width
int lw = 4;

// angular position of each channel
float chanAng[16] = {90+22.5, 90+67.5, 180+22.5, 180+67.5, -67.5, -22.5, 22.5, 67.5,
		     -67.5, -22.5, 22.5, 67.5, 90+22.5, 90+67.5, 180+22.5, 180+67.5};

void DrawScope(float cx, float cy, int n) {
  char buf[16];
  sprintf(buf, "%d", n);
  TText *t = new TText(cx/w, cy/h, buf);
  t->SetTextAlign(22);
  t->SetTextSize(0.06);
  t->Draw();

  float bsize = (rout - rin - 6)/2;
  float x1 = cx + bsize*sin(TMath::DegToRad()*chanAng[n]);
  float y1 = cy + bsize*cos(TMath::DegToRad()*chanAng[n]);
  float x2 = cx + bsize*sin(TMath::DegToRad()*(chanAng[n]+90));
  float y2 = cy + bsize*cos(TMath::DegToRad()*(chanAng[n]+90));
  float x3 = cx + bsize*sin(TMath::DegToRad()*(chanAng[n]+180));
  float y3 = cy + bsize*cos(TMath::DegToRad()*(chanAng[n]+180));
  float x4 = cx + bsize*sin(TMath::DegToRad()*(chanAng[n]+270));
  float y4 = cy + bsize*cos(TMath::DegToRad()*(chanAng[n]+270));
  TLine *l1 = new TLine(x1/w, y1/h, x2/w, y2/h);
  TLine *l2 = new TLine(x2/w, y2/h, x3/w, y3/h);
  TLine *l3 = new TLine(x3/w, y3/h, x4/w, y4/h);
  TLine *l4 = new TLine(x4/w, y4/h, x1/w, y1/h);
  l1->Draw();
  l2->Draw();
  l3->Draw();
  l4->Draw();
}

void DrawQuadrant(float cx, float cy, int ang1, int ang2, int startChan) {
  TEllipse *ei = new TEllipse(cx/w, cy/h, rin/w, rin/h, ang1, ang2);
  TEllipse *eo = new TEllipse(cx/w, cy/h, rout/w, rout/h, ang1, ang2);
  ei->SetNoEdges();
  eo->SetNoEdges();
  ei->SetLineWidth(lw);
  eo->SetLineWidth(lw);
  eo->Draw();
  ei->Draw();

  TLine *lu = new TLine(cx/w, (cy+rin-lw/2)/h, cx/w, (cy+rout+lw/2)/h);
  lu->SetLineWidth(lw);
  lu->Draw();

  TLine *ll = new TLine(cx/w, (cy-rin+lw/2)/h, cx/w, (cy-rout-lw/2)/h);
  ll->SetLineWidth(lw);
  ll->Draw();

  for (int i=startChan; i<startChan+4; ++i) {
    float tx = cx + (rin+rout)*cos(TMath::DegToRad()*chanAng[i])/2;
    float ty = cy + (rin+rout)*sin(TMath::DegToRad()*chanAng[i])/2;
    DrawScope(tx, ty, i);
  }
}

void DrawPLTReadoutChannels(void) {
  TCanvas *c1 = new TCanvas("c1", "c1", w, h);

  float cq1x = mleft + rout;
  float cqy = (h-mtop)/2;
  DrawQuadrant(cq1x, cqy, 90, 270, 0);

  float cq2x = cq1x + gap;
  DrawQuadrant(cq2x, cqy, -90, 90, 4);

  float cq3x = cq2x + 2*rout + 140;
  DrawQuadrant(cq3x, cqy, 90, 270, 12);

  float cq4x = cq3x + gap;
  DrawQuadrant(cq4x, cqy, -90, 90, 8);

  TText *tzm = new TText((cq1x + gap/2)/w, (h-mtop+40)/h, "-z side");
  tzm->SetTextSize(0.08);
  tzm->SetTextAlign(21);
  tzm->Draw();

  TText *tzmn = new TText((cq1x - rout/2)/w, (h-mtop)/h, "near");
  tzmn->SetTextSize(0.06);
  tzmn->SetTextAlign(21);
  tzmn->SetTextFont(42);
  tzmn->Draw();

  TText *tzmf = new TText((cq2x + rout/2)/w, (h-mtop)/h, "far");
  tzmf->SetTextSize(0.06);
  tzmf->SetTextAlign(21);
  tzmf->SetTextFont(42);
  tzmf->Draw();

  TText *tzp = new TText((cq3x + gap/2)/w, (h-mtop+40)/h, "+z side");
  tzp->SetTextSize(0.08);
  tzp->SetTextAlign(21);
  tzp->Draw();

  TText *tzpn = new TText((cq4x + rout/2)/w, (h-mtop)/h, "near");
  tzpn->SetTextSize(0.06);
  tzpn->SetTextAlign(21);
  tzpn->SetTextFont(42);
  tzpn->Draw();

  TText *tzpf = new TText((cq3x - rout/2)/w, (h-mtop)/h, "far");
  tzpf->SetTextSize(0.06);
  tzpf->SetTextAlign(21);
  tzpf->SetTextFont(42);
  tzpf->Draw();

  TLatex *tln1 = new TLatex((cq1x + gap/2)/w, (cqy+14)/h, "looking");
  tln1->SetTextSize(0.06);
  tln1->SetTextAlign(22);
  tln1->SetTextFont(52);
  tln1->Draw();
  TLatex *tln2 = new TLatex((cq1x + gap/2)/w, (cqy-14)/h, "towards IP");
  tln2->SetTextSize(0.06);
  tln2->SetTextAlign(22);
  tln2->SetTextFont(52);
  tln2->Draw();

  TLatex *tlf1 = new TLatex((cq3x + gap/2)/w, (cqy+14)/h, "looking");
  tlf1->SetTextSize(0.06);
  tlf1->SetTextAlign(22);
  tlf1->SetTextFont(52);
  tlf1->Draw();
  TLatex *tlf2 = new TLatex((cq3x + gap/2)/w, (cqy-14)/h, "towards IP");
  tlf2->SetTextSize(0.06);
  tlf2->SetTextAlign(22);
  tlf2->SetTextFont(52);
  tlf2->Draw();

  c1->Print("PLTReadoutChannels.png");
  c1->Print("PLTReadoutChannels.pdf");
}
