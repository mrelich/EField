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

const int m_colors[] = {kBlack, kRed, kBlue, kMagenta,
			kGreen+3,
			kOrange+7, kViolet-6};

TString savedir = "../plots/Toy/";

// Need some physics constants for the calculation
// of p in the exponent:
float freq    = 300000; // Hz
float pi      = TMath::Pi();
float nICE    = 1.79; // index of refraction
float c_speed = 3e8;  // speed of light

//----------------------------------------//
// Main
//----------------------------------------//
ToyShowerExample()
{

  // Specify the initial positions of the gaussian
  vector<TString> x0s;
  x0s.push_back("2");
  //x0s.push_back("4");
  x0s.push_back("6");
  //x0s.push_back("8");
  x0s.push_back("10");
  //x0s.push_back("12");
  x0s.push_back("14");
  //x0s.push_back("16");
  x0s.push_back("18");

  // Draw the gaussians
  //drawGaussians(x0s);

  // THe main event
  drawIntegralResult(x0s);

}

//----------------------------------------//
// Draw the gaussians to be integrated
//----------------------------------------//
void drawGaussians(vector<TString> x0s)
{
  
  // Make canvas
  TCanvas* c = makeCanvas("c");
  
  // Titles
  TString xtitle = "Radiation Length";
  TString ytitle = "N(e-p)";

  // Get histograms
  TH1F* hist = NULL;
  for(unsigned int i=0; i<x0s.size(); ++i){
    hist = makeGaussian(x0s.at(i));
    setAtt(hist,xtitle,ytitle,m_colors[i]);
    if( i == 0) hist->Draw();
    else        hist->Draw("same");
  }

  c->SaveAs((savedir+"inputGaussian.png").Data());

}

//----------------------------------------//
// Set histogram attributes
//----------------------------------------//
void setAtt(TH1F* &h, TString x, TString y, 
	    int color)
{

  h->GetXaxis()->SetTitle(x.Data());
  h->GetYaxis()->SetTitle(y.Data());
  h->SetTitle("");
  h->SetStats(0);
  h->SetLineColor(color);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->GetYaxis()->SetTitleOffset(1.5);
  h->SetLineWidth(2);
}
	      

//----------------------------------------//
// Draw the integral result to test the
// hypothesis
//----------------------------------------//
void drawIntegralResult(vector<TString> x0s)
{

  // Make a canvas
  TCanvas* c = makeCanvas("c");
  c->SetLogy();

  // Need a dummy histogram to plot the graphs on
  TH1F* dummy = new TH1F("dum","",1,25,85);
  dummy->SetMaximum(1e6);
  dummy->SetMinimum(1e-1);
  dummy->Draw();  

  // specify the number of points, the step size,
  // and the starting angle to look at
  const int np  = 100;
  float step    = 0.6;
  float theta_i = 25;

  // Then integrate the profiles according to Jaime's 
  // paper and make a Graph of the results.
  for(unsigned int i=0; i<x0s.size(); ++i){
    
    // Get the charge profile
    TH1F* Qz = makeGaussian(x0s.at(i));
    
    // Make placeholders for the graph
    float theta[np];
    float RE[np];

    // Now loop over the angles and build the graph
    for(int p=0; p<np; ++p){
      theta[p] = theta_i + step*p;

      // get p
      double P = getP(theta[p]);

      // Get integral results
      double real = excessInt(P,Qz,true);
      double imag = excessInt(P,Qz,false);
      RE[p]       = sqrt(real*real+imag*imag) * -1*TMath::Sin(theta[p]);
    }// end loop over angles
    
    cout<<"Working on "<<x0s.at(i)<<" with "<< RE[50] << endl;

    TGraph* gr = new TGraph(np,theta,RE);
    gr->SetLineColor(m_colors[i]);
    gr->Draw("same");

  }


}

//----------------------------------------//
// Get integral
//----------------------------------------//
double excessInt(double p, TH1F* Qz, bool isReal)
{

  // Set some things up for the integral. Just 
  // basically copying from Dim1.cxx
  double step = Qz->GetBinWidth(1);

  // Some place holders
  double integral   = 0;
  double stepResult = 0;
  double dZ         = step * X0;

  // Loop over the bins
  int nbins = Qz->GetNbinsX();
  for(int bin=1; bin<=nbins; ++bin){
    float bc = Qz->GetBinContent(bin);
    stepResult = bc * dZ;
    if( isReal ) stepResult *= TMath::Cos(p*step*bin*X0);
    else         stepResult *= TMath::Sin(p*step*bin*X0);
    // record
    integral += stepResult;
  }// end loop

  return integral;
}

//----------------------------------------//
// Get P in exponent
//----------------------------------------//
double getP(double theta)
{

  return 2*pi*freq*(1-nICE*TMath::Cos(theta))/c_speed;

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
