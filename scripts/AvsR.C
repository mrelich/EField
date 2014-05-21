
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// Don't fully understand why A doesn't go like 1/R, so plot //
// A vs R for various points to confirm.                     //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "myHist.C"

//TString savedir = "../plots/ELSStudy/";
TString savedir = "../plots/ELSStudy_40e3TeV/";

void AvsR(bool RA)
{

  // input file
  //TFile* file = new TFile("test.root");
  //TFile* file = new TFile("ELStest.root");
  TFile* file = new TFile("ELStest_40e3TeV.root");

  // Points to consider
  vector<int> zpts; 
  /*zpts.push_back(10);
  zpts.push_back(20);
  zpts.push_back(30);
  zpts.push_back(40);
  zpts.push_back(50);
  zpts.push_back(60);
  zpts.push_back(70);
  zpts.push_back(80);
  zpts.push_back(90);
  zpts.push_back(100);
  zpts.push_back(200);
  zpts.push_back(300);
  zpts.push_back(400);
  zpts.push_back(500);
  zpts.push_back(600);
  zpts.push_back(700);
  zpts.push_back(800);
  zpts.push_back(900);
  zpts.push_back(1000);
  zpts.push_back(2000);
  zpts.push_back(3000);
  zpts.push_back(4000);
  zpts.push_back(5000);
  zpts.push_back(6000);
  zpts.push_back(7000);
  zpts.push_back(8000);
  zpts.push_back(9000);
  zpts.push_back(10000);
  */
  zpts.push_back(1);
  zpts.push_back(2);
  zpts.push_back(3);
  zpts.push_back(4);
  zpts.push_back(5);
  zpts.push_back(6);
  zpts.push_back(7);
  zpts.push_back(8);
  zpts.push_back(9);
  zpts.push_back(10);
  zpts.push_back(20);
  zpts.push_back(30);
  zpts.push_back(40);
  zpts.push_back(50);


  // Angle
  double angle = (55.8-0.3)/57.2957795;

  // Placeholders
  int points = 0;
  double A[100];
  double R[100];

  // Loop
  stringstream ss;
  TGraph* gr = NULL;
  for(unsigned int i=0; i<zpts.size(); ++i){

    // Get R
    double z = zpts.at(i);
    R[i]     = z / cos(angle);

    // Load Graph and get maximum A
    ss.str(""); ss << "gr_" << z;
    gr   = getGraph(file,TString(ss.str().c_str()),"","",kBlack);
    A[i] = getMaximum(gr);
    if( RA ) A[i] *= R[i];

    points++;

  }

  // Make a canvas
  TCanvas* c = makeCanvas("c");

  // Titles
  TString xt = "R [m]";
  TString yt = "A [V s m^{-1}]";
  if( RA ) yt = "R|A| [V s]";

  // Make frame
  //TH1F* frame = makeHist("h",1,0,20000,xt,yt,kBlack,20);
  TH1F* frame = makeHist("h",1,0,100,xt,yt,kBlack,20);
  if(RA){
    //frame->SetMaximum(7e-9);
    //frame->SetMinimum(1e-10);
    frame->SetMaximum(4e-9);
    frame->SetMinimum(2.5e-9);
  }
  else{
    //frame->SetMaximum(9e-12);
    //frame->SetMinimum(1e-13);
    frame->SetMaximum(2e-9);
    frame->SetMinimum(1e-14);
  }

  frame->Draw();

  // Make a graph
  TGraph* graph = new TGraph(points,R,A);
  graph->SetLineColor(kBlue);
  graph->SetLineWidth(2);
  graph->SetMarkerColor(kBlue);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1);
  graph->Draw("lpsame");
  
  // Save
  TString save = "AvsR.png";
  if(RA) save = "RAvsR.png";
  c->SaveAs((savedir+save).Data());

}

double getMaximum(TGraph* gr)
{

  int np = gr->GetN();
  double x=0, y=0, maximum=0;
  for(int p=0; p<np; ++p){
    gr->GetPoint(p,x,y);
    if( y > maximum )
      maximum = y;
  }

  return maximum;

}  
