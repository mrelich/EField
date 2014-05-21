
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// This script will look to compare the near and far field results    //
// for a fixed angle. Right now I am working with 100PeV shower from  //
// Jaime's paper which I have approximated using two gaussians.  This //
// gives similar results to figure 5 in paper, so I am semi-          //
// confident that the near field is ok.  The far field still has no   //
// comparison to gain confidence...  100 EeV shower might be out of   //
// reach...                                                           //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "myHist.C"
#include <sstream>

//TString savedir = "../plots/ELSStudy/";
TString savedir = "../plots/ELSStudy_40e3TeV/";

//--------------------------------------------//
// Main
//--------------------------------------------//
void NearField(int opt)
{

  //TFile* file   = new TFile("ELStest.root");
  TFile* file   = new TFile("ELStest_40e3TeV.root");
  TString ytitle = "";
  float maximum = 1;
  TString savename = "";
  float threshold = 0;
  double angle = (55.8-0.3)/57.2957795;
  float scale     = 1; // Scale A by R for option 0
  
  if( opt == 0 ){
    //file = new TFile("../results/Dim3_100PeV_FF_NFFA_RA.root");
    ytitle = "R|A|";
    maximum = 4e-9;
    savename = "tVsRA.png";
    threshold = 8e-12;
  }
  else if( opt == 1 ){
    //file = new TFile("../results/Dim3_100PeV_FF_NFFA_AOnly.root");    
    ytitle    = "|A|";
    maximum   = 8e-10;
    savename  = "tVsA.png";
    threshold = 1e-13; 
  }
  else{
    return;
  }
  
  // Specify the plots to load
  vector<int> zpts;
  //zpts.push_back(1);
  //zpts.push_back(2);
  zpts.push_back(3);
  zpts.push_back(4);
  zpts.push_back(5);
  zpts.push_back(6);
  zpts.push_back(7);
  zpts.push_back(8);
  zpts.push_back(9);
  zpts.push_back(10);
  //zpts.push_back(20);
  //zpts.push_back(30);
  //zpts.push_back(40);
  //zpts.push_back(50);
  
  
  // specify plot labels and colors
  TString xtitle = "time [ns]";
  int colors[] = {kRed,kBlue,kMagenta,kAzure+9,
		  kViolet,kGreen+3,kOrange,
		  kGreen-6, kOrange+10, kViolet+2};
  
  // Make Canvas
  TCanvas* c = makeCanvas("c");
  //c->SetLogy();

  // Frame
  TH1F* frame = makeHist("frame",1,0,120,xtitle,ytitle,kBlack,20);
  frame->SetMaximum(maximum);
  frame->SetMinimum(threshold);
  frame->Draw();
  
  // Make legend
  TLegend* leg = makeLegend(0.72,0.9,0.5,0.9);
  leg->SetTextSize(0.03);
  leg->SetHeader("Observer (x,y,z) [m]");
  
  // Loop and get the graphs and plot
  float shiftOffset = 10;
  float offset = 80;
  stringstream ss;
  stringstream titles;
  for(unsigned int i=0; i<zpts.size(); ++i){

    // Build pname
    int zpt = zpts.at(i);
    ss.str("");
    ss << "gr_" << zpt;

    // Get Graph
    TGraph* gr = getGraph(file,TString(ss.str().c_str()),xtitle,ytitle,colors[i]);

    // Reset the graph and scale if needed
    if(opt == 0 ) scale = (zpt)/cos(angle);
    gr = resetGraph(gr, threshold,offset,colors[i],scale);
    gr->SetLineWidth(2);

    // Draw
    gr->Draw("same");

    // Add to legend
    titles.str("");
    titles << "(" << Form("%3.2f", zpt * tan(angle)) << ",0," << zpt <<")";
    leg->AddEntry(gr,titles.str().c_str(),"l");
    offset -= shiftOffset;
    
    // Print the threshold
    double width = getWidth(gr,threshold);
    cout<<"R: "<<((zpt)/cos(angle))<<" width = "<<width<<endl;

  }
  
  leg->Draw("same");

  // Save
  c->SaveAs((savedir+savename).Data());

}

//--------------------------------------------//
// Reset the graph
//--------------------------------------------//
TGraph* resetGraph(TGraph* gr, float thresh, 
		   float offset, int color,
		   float scale = 1)
{

  double xp[10000];
  double yp[10000];
  int counter = 0;
  
  int np = gr->GetN();
  double x=0, y=0;
  double prev = 0;
  for(int p=0; p<np; ++p){    
    gr->GetPoint(p,x,y);
    y *= scale;
    if( y > thresh ){
      xp[counter] = offset + counter*(x-prev);
      yp[counter] = y;
      counter++;
    }
    prev = x;
  }
  TGraph* graph = new TGraph(counter,xp,yp);
  graph->SetLineColor(color);
  delete gr;
  
  return graph;
  
}

//--------------------------------------------//
// Give the time window
//--------------------------------------------//
double getWidth(TGraph* gr, double thresh)
{

  int np = gr->GetN();
  double x=0, y=0;
  double xi = -999;
  double xf = 0;
  for(int p=0; p<np; ++p){
    gr->GetPoint(p,x,y);
    if( y > thresh && xi < 0 )
      xi = x;
    if( y > thresh )
      xf = x;
  }
  
  return xf - xi;
      
}
