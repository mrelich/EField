//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// We want to simulate high energy showers using superposition   //
// of many small showers.  It seems like it should work but I    //
// have found that the shower profile peaks at a lower radiation //
// length when the energy lowers.  I want to use a toy example   //
// of a gaussian and just calculate what happens if I move the   //
// central position along the logitudinal direction.             //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "myHist.C"

const int nbins = 80;
const float min = 0;
const float max = 40;

const float X0 = 0.390522; // Radiation length in ice

const int m_colors[] = {kBlack, kRed, kBlue, kMagenta,
			kGreen+3,
			kOrange+7, kViolet-6};

TString savedir = "../plots/Toy/";

// Need some physics constants for the calculation
// of p in the exponent:
float pi      = TMath::Pi();
float nICE    = 1.79; // index of refraction
float c_speed = 3e8;  // speed of light
double DEG_RAD = 57.2957795;
double e0  = 8.8541e-12;  // Permitivity of Free space
double c2  = c_speed*c_speed;         // c^2 is used quite a bit
double e   = 1.602e-19;   // Electron charge coloumbs
double MHz = 1.e6;
float freq    = 300*MHz; // Hz

// Const
double RE_const = 0;

//----------------------------------------//
// Main
//----------------------------------------//
ToyShowerExample()
{

  // Specify the const
  RE_const = (e) / 
    (2 * pi * e0 * c2);

  // Specify the initial positions of the gaussian
  vector<TString> x0s;
  x0s.push_back("2");
  x0s.push_back("8");
  x0s.push_back("14");
  x0s.push_back("20");
  x0s.push_back("26");
  x0s.push_back("32");
  x0s.push_back("38");

  /*  x0s.push_back("14");
  x0s.push_back("14");
  x0s.push_back("14");
  x0s.push_back("14");
  x0s.push_back("14");
  x0s.push_back("14");
  x0s.push_back("14");
  */

  vector<TString> pars;
  pars.push_back("4");
  pars.push_back("4");
  pars.push_back("4");
  pars.push_back("4");
  pars.push_back("4");
  pars.push_back("4");
  pars.push_back("4");


  // Draw the gaussians
  //drawGaussians(x0s, pars);

  // THe main event
  drawIntegralResult(x0s, pars);

}

//----------------------------------------//
// Draw the gaussians to be integrated
//----------------------------------------//
void drawGaussians(vector<TString> x0s,
		   vector<TString> pars)
{
  
  // Make canvas
  TCanvas* c = makeCanvas("c");
  
  // Titles
  TString xtitle = "Radiation Length";
  TString ytitle = "N(e-p)";

  // Get histograms
  TH1F* hists[10];
  float maximum = -999;
  for(unsigned int i=0; i<x0s.size(); ++i){
    hists[i] = makeGaussian(x0s.at(i),pars.at(i));
    setAtt(hists[i],xtitle,ytitle,m_colors[i]);
  }

  //hists[0]->SetMaximum(1.2*maximum);
  hists[0]->Draw();
  for(unsigned int i=1; i<x0s.size(); ++i)
    hists[i]->Draw("same");

  c->SaveAs((savedir+"inputGaussian.png").Data());
  //c->SaveAs((savedir+"inputGaussian_varHeight.png").Data());
  //c->SaveAs((savedir+"inputGaussian_varWidth.png").Data());

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
void drawIntegralResult(vector<TString> x0s,
			vector<TString> pars)
{

  // Make a canvas
  TCanvas* c = makeCanvas("c");
  c->SetLogy();

  // Need a dummy histogram to plot the graphs on
  TH1F* dummy = new TH1F("dum","",1,25,85);
  dummy->SetMaximum(1e-4);
  dummy->SetMinimum(1e-11);
  setAtt(dummy, "Angle [deg]", "Rx|E(f,#theta)|",kBlack);
  dummy->Draw();  

  // specify the number of points, the step size,
  // and the starting angle to look at
  const int np  = 100;
  float step    = 0.6;
  float theta_i = 25;

  // Make Legend
  TLegend* leg = makeLegend(0.15,0.35,0.55,0.92);
  leg->Clear();
  leg->SetHeader("Gaussian Center");

  // Then integrate the profiles according to Jaime's 
  // paper and make a Graph of the results.
  for(unsigned int i=0; i<x0s.size(); ++i){
    
    // Get the charge profile
    TH1F* Qz = makeGaussian(x0s.at(i),pars.at(i));
    
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
      RE[p]       = RE_const * 2*pi*freq * sqrt(real*real+imag*imag) * 
	TMath::Sin(theta[p]/DEG_RAD) * MHz;
      
    }// end loop over angles
    
    TGraph* gr = new TGraph(np,theta,RE);
    gr->SetLineColor(m_colors[i]);
    gr->SetMarkerColor(m_colors[i]);
    gr->SetLineWidth(2);
    gr->Draw("same");

    leg->AddEntry(gr,("x_{0}= "+x0s.at(i)+"X_{0}").Data(),"l");
    //leg->AddEntry(gr,("#sigma^{2} = "+pars.at(i)).Data(),"l");
  }

  // Draw legend
  leg->Draw("same");

  // Save
  c->SaveAs((savedir+"EfieldFromGaussians.png").Data());
  //c->SaveAs((savedir+"EfieldFromGaussians_varHeight.png").Data());
  //c->SaveAs((savedir+"EfieldFromGaussians_varWidth.png").Data());
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

  return 2*pi*freq*(1-nICE*TMath::Cos(theta/DEG_RAD))/c_speed;

}

//----------------------------------------//
// Make Gaussian hist centered at 
// a variable point
//----------------------------------------//
TH1F* makeGaussian(TString x0, TString par)
{

  // make histogram
  TH1F* hist = new TH1F(("hist_"+x0+"_"+par).Data(),"",nbins,min,max);
  
  // Make gaussian function
  TString f = "exp(-0.5*((x-"+x0+")**2)/"+par+")";
  TF1 gaus = TF1("func",f.Data(),0,max);

  // Fill histogram and return
  float nEvt = 10000;
  hist->FillRandom("func",nEvt);
  //float sf = norm / hist->GetMaximum();
  //hist->Scale(sf);
  //hist->Scale(1/sqrt(nEvt));
  cout<<"int; "<<hist->Integral()<<endl;
  return hist;

}
