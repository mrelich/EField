
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// ZHS runs for a long time. I am going to try to make a fake //
// profile using two gaussians to match Jaime's figure 5 in   //
// 1106.6283                                                  //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "myHist.C"

void Fake100PeVShower(int opt)
{

  // opt == 0 implies save to root file
  // opt == 1 implies plot and save to canvas
  TFile* file = NULL;
  TCanvas* c  = NULL;

  if(opt == 0)
    file = new TFile("fake100PeVShower.root","recreate");
  if(opt == 1)
    c = makeCanvas("c");

  TString gaus = "1.5e7*TMath::Exp(-(pow(x-10,2)/9))";
  TString bump = "5e6*TMath::Exp(-(pow(x-17,2)/20))";
  TF1* f = new TF1("f",(gaus+"+"+bump).Data(),0,30);

  int nbins = 3000;
  float xmin  = 0;
  float xmax  = 30;
  TProfile* prof = new TProfile("prof","",nbins,xmin,xmax);
  prof->SetStats(0);
  prof->SetTitle("");
  prof->GetYaxis()->SetTitle("Charge excess / 1e7");
  prof->GetXaxis()->SetTitle("z [m]");

  float step = xmax / nbins;
  for(int i =0; i<nbins; ++i){
    float x = step * i;
    if(opt == 1 ) prof->Fill(x, f->Eval(x)/1e7);
    else          prof->Fill(x, f->Eval(x));
  }
    
  if(opt == 1){
    prof->Draw();
    c->SaveAs("JaimeCheck/FakeProfile100PeV.png");
  }
  if(opt == 0){
    file->Write();
    file->Close();
  }
}
