//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// We want to simulate high energy showers using superposition   //
// of many small showers.  It seems like it should work but I    //
// have found that the shower profile peaks at a lower radiation //
// length when the energy lowers.  I want to use a toy example   //
// of a gaussian and just calculate what happens if I move the   //
// central position along the logitudinal direction.             //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "myHist.C"

const int nbins = 40;
const float min = 0;
const float max = 20;

const float X0 = 39.0522; // Radiation length in ice

//----------------------------------------//
// Main
//----------------------------------------//
ToyShowerExample()
{
  // Make Canvas
  TCanvas* c = makeCanvas("c");

  // Specify the initial positions of the gaussian
  vector<TString> x0s;
  x0s.push_back("2");
  x0s.push_back("4");
  x0s.push_back("6");
  x0s.push_back("8");
  x0s.push_back("10");
  x0s.push_back("12");
  x0s.push_back("14");
  x0s.push_back("16");
  x0s.push_back("18");

  // Then integrate the profiles according to Jaime's 
  // paper and make a Graph of the results.
  for(unsigned int i=0; i<x0s.size(); ++i){
    float x0 = 


}

//----------------------------------------//
// Make Gaussian hist centered at 
// a variable point
//----------------------------------------//
TH1F* makeGaussian(TString x0)
{

  // make histogram
  TH1F* hist = new TH1F(("hist_"+x0).Data(),"",nbins,min,max);
  
  // Make gaussian function
  TString f = "exp(-(x-"+x0+")**2)";
  TF1 gaus = TF1("func",f.Data(),0,20);

  // Fill histogram and return
  hist->FillRandom("func",10000);
  hist->Scale(1000/hist->Integral());

  return hist;

}
