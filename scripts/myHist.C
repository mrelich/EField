
#include "TH1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TFile.h"

#include <vector>

//------------------------------------------------------------//
// Get a graph from file and format
//------------------------------------------------------------//
TGraph* getGraph(TFile* file, TString pname, TString xtitle,
		 TString ytitle, int color)
{

  TGraph* gr = (TGraph*) file->Get(pname.Data());
  gr->SetTitle("");
  gr->GetXaxis()->SetTitle(xtitle.Data());
  gr->GetYaxis()->SetTitle(ytitle.Data());
  gr->SetLineColor(color);
  
  return gr;

}

//------------------------------------------------------------//
// Make TLatex
//------------------------------------------------------------//
TLatex* makeLatex()
{

  TLatex* lat = new TLatex();
  lat->SetTextSize(0.04);
  lat->SetNDC();
  lat->SetTextFont(42);
  lat->SetTextColor(kBlack);
  return lat;
}

//------------------------------------------------------------//
// Make legend
//------------------------------------------------------------//
TLegend* makeLegend(float x0, float x1, float y0, float y1)
{

  TLegend* leg = new TLegend(x0, y0, x1, y1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  return leg;

}

//------------------------------------------------------------//
// Make canvas
//------------------------------------------------------------//
TCanvas* makeCanvas(TString name)
{

  TCanvas* c = new TCanvas(name.Data(), name.Data(), 600, 500);
  c->SetTicks(1,1);
  c->SetTopMargin(0.05);
  c->SetRightMargin(0.05);
  c->SetBottomMargin(0.12);
  c->SetLeftMargin(0.12);
  
  return c;
}

//------------------------------------------------------------//
// Get TProfile
//------------------------------------------------------------//
TProfile* getProfile(TFile* file, TString pname, TString xtitle,
		     TString ytitle, int color, int marker)
{
  
  TProfile* prof = (TProfile*) (file->Get(pname.Data())->Clone(Form("%s_%i",pname.Data(),color)));
  prof->GetXaxis()->SetTitle(xtitle.Data());
  prof->GetYaxis()->SetTitle(ytitle.Data());
  prof->SetMarkerStyle(marker);
  prof->SetMarkerColor(color);
  prof->SetMarkerSize(0.5);
  prof->SetLineColor(color);
  prof->SetTitle("");
  prof->SetStats(0);
  prof->GetYaxis()->SetTitleOffset(1.5);
  
  return prof;

}

//------------------------------------------------------------//
// Get histogram
//------------------------------------------------------------//
TH1F* getHist(TFile* file, TString pname, TString xtitle,
		  TString ytitle, int color, int marker)
{
  
  TH1F* hist = (TH1F*) (file->Get(pname.Data())->Clone(Form("%s_%i",pname.Data(),color)));
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitle.Data());
  hist->SetMarkerStyle(marker);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize(0.5);
  hist->SetLineColor(color);
  hist->SetTitle("");
  hist->SetStats(0);
  hist->GetYaxis()->SetTitleOffset(1.5);
  hist->SetLineWidth(2);
  return hist;

}

//------------------------------------------------------------//
// Get graph
//------------------------------------------------------------//
TGraph* getGraph(TFile* file, TString pname, int color, int style)
{
  
  TGraph* gr = (TGraph*) (file->Get(pname.Data()));
  gr->SetLineColor(color);
  gr->SetLineStyle(style);
  return gr;

}

//------------------------------------------------------------//
// Get 2D histogram
//------------------------------------------------------------//
TH2F* getHist2(TFile* file, TString pname, TString xtitle,
	       TString ytitle, TString ztitle)
{
  
  TH2F* hist = (TH2F*) (file->Get(pname.Data())->Clone());
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitle.Data());
  hist->GetZaxis()->SetTitle(ztitle.Data());
  hist->SetTitle("");
  hist->SetStats(0);
  hist->GetYaxis()->SetTitleOffset(1.5);
  hist->GetZaxis()->SetTitleOffset(1.);
  hist->SetLineWidth(2);

  return hist;

}

//------------------------------------------------------------//
// Make Histogram
//------------------------------------------------------------//
TH1F* makeHist(TString name, int nbins, float xmin, float xmax,
	       TString xtitle, TString ytitle, int color, int marker)
{

  TH1F* h = new TH1F(name.Data(),"",nbins,xmin,xmax);
  h->GetXaxis()->SetTitle(xtitle.Data());
  h->GetYaxis()->SetTitle(ytitle.Data());
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetMarkerStyle(marker);
  h->SetStats(0);
  h->SetTitle("");
  h->GetYaxis()->SetTitleOffset(1.5);
  return h;

}

