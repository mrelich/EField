
#include "myHist.C"

void FarFieldPlot()
{

  // Specify file
  //TFile* file = new TFile("../results/Dim3_100PeV_FF_NFFA_AOnly.root");
  //TFile* file = new TFile("test.root");
  TFile* file = new TFile("ELStest_40e3TeV.root");

  // Specify plot name
  TString pname = "gr1";
  
  // Make canvas
  TCanvas* c = makeCanvas("c");

  // Specify plot attributes
  TString xtitle = "time [ns]";
  TString ytitle = "R|A|";
  
  // Make frame
  TH1F* frame = makeHist("h",1,0,0.3,xtitle,ytitle,kBlack,20);
  frame->SetMaximum(4e-9);
  //frame->SetMaximum(4e-12);
  frame->Draw();
  
  // Get Graph
  TGraph* gr = getGraph(file,pname,xtitle,ytitle,kBlack);
  gr->Draw("same");

  //c->SaveAs("../plots/Study100PeV/FarFieldResult.png");
  c->SaveAs("../plots/ELSStudy_40e3TeV/FarFieldResult.png");

}
