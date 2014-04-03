//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// This script will make some quick plots in order to   //
// study the expected impact from transition radiation. //
// So far I will concentrate on angular dependence that //
// we expect at our two boundaries of Aluminum to air   //
// and air to ice.                                      //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "myHist.C"
#include <cmath>
#include <complex>

// 
//// Physics Constants
//

double hbar  = 1.0545717e-34; // J s
double alpha = 7.29735257e-3; // dimensionless
double pi    = TMath::Pi();
double e_0   = 8.854187817e-12;
double e_air = 1.00058986;// * e_0;
double e_ice = 88;// * e_0;
double e_Al  = 10;// * e_0; 
double e_Si  = 3.;
double DEG_RAD = 57.2957795;

//
//// Options
//
int Al_Air   = 0;
int Air_Ice  = 1;
int Sand_Air = 2;
int Air_Sand = 3;
int Al_Sand  = 4;

//
//// For plots
//
float xmin = 0;
float xmax = 90;

//
//// Shower Energy
//
double E0 = 15; // MeV
//double E0 = 1e9; // MeV

//-----------------------------------//
// Main
//-----------------------------------//
void TRAnalysis()
{

  //plotVsTheta(Al_Air);
  //plotVsTheta(Air_Ice);
  plotVsTheta(Air_Sand);
  //plotVsTheta(Sand_Air);
  //plotVsTheta(Al_Sand);
}

//-----------------------------------//
// Make the plot
//-----------------------------------//
void plotVsTheta(int interface)
{

  // TCanvas
  TCanvas* c = makeCanvas("c");
  //c->SetLogy();

  // Get dummy histogram
  TH1F* h = dummy();

  // Get the function
  TF1* function = getFunc(interface);
  //function->Draw("same");
  //function->Draw();

  const int np = 100000;
  double stepSize = (xmax-xmin)/np;
  double x[np], y[np];
  double maximum = -999;
  double minimum = 999;
  for(int n=0; n<np; ++n){
    x[n] = stepSize * (n+1);
    y[n] = function->Eval(x[n]);
    if( maximum < y[n] )
      maximum = y[n];
    if( minimum > y[n] )
      minimum = y[n];
  }

  h->SetMaximum(maximum*0.8);
  h->SetMinimum(minimum);
  h->Draw();

  TGraph* gr = new TGraph(np,x,y);
  gr->SetLineWidth(2);
  gr->Draw("same");

}

//-----------------------------------//
// Define my user function
//-----------------------------------//
Double_t myfunc(Double_t *x, Double_t *par)
{

  // The parameters for the function are
  // par[0] = Constant
  // par[1] = e1
  // par[2] = e2
  // par[3] = beta
  // par[4] = Deg-->Rad conversion factor
  
  // Put angle in Radiants
  Double_t th = x[0]/par[4];


  // Specify the other params so it is 
  // more intuitive to read
  Double_t C  = par[0];
  Double_t e1 = par[1];
  Double_t e2 = par[2];
  Double_t B  = par[3];

  // Get the first part of equation
  Double_t num = sqrt(e2)*sin(th)*sin(th)*cos(th)*cos(th);
  Double_t den = pow(1-B*B*e2*cos(th)*cos(th),2);

  // Get Zeta
  Double_t Zeta_sq = 0;
  if( e1 >= e2*sin(th)*sin(th) ){ // sqrt is real
    Double_t phi = sqrt(e1-e2*sin(th)*sin(th));
    Double_t z_num = (e2-e1)*(1-B*B*e2-B*phi);
    Double_t z_den = (e1*cos(th)+sqrt(e2)*phi)*(1-B*phi);
    Zeta_sq = fabs(z_num/z_den);
  }
  else{ // sqrt is imaginary, need complex math
    Double_t phi = sqrt(e2*sin(th)*sin(th)-e1);
    Double_t z_num = sqrt(pow((e2-e1)*(1-B*B*e2),2) + pow(phi*B*(e2-e1),2));
    Double_t z_den = sqrt(pow(e1*cos(th)-sqrt(e2)*phi*phi*B,2) +
			  pow(sqrt(e2)*phi - e1*cos(th)*phi*B,2));
    Zeta_sq = pow(z_num/z_den,2);
  }

  // return the final result
  //return C * (num/den) * Zeta_sq;
  return log10(C * (num/den) * Zeta_sq);

}

//-----------------------------------//
// Get function (Eq 2.1 from 
// hep-ex/0004007)
//-----------------------------------//
TF1* getFunc(int interface)
{

  TF1* func = new TF1("func",myfunc,xmin,xmax,5);
  func->SetParameters(1,1,1,1,1);

  // Set constants
  double C = hbar*alpha/(pi*pi) * 3e8*(0.18-0.11)/0.18**2;
  double e1 = 0;
  double e2 = 0;
  double beta = Beta(); 

  // Set Epsilon
  getEpsilon(interface,e1,e2);    

  // Set parameters
  func->FixParameter(0,C);
  func->FixParameter(1,e1);
  func->FixParameter(2,e2);
  func->FixParameter(3,beta);
  func->FixParameter(4,DEG_RAD);

  // return that guy
  return func;

}


//-----------------------------------//
// Get epsilon
//-----------------------------------//
void getEpsilon(int interface, double &e1, double &e2)
{

  if(interface == Al_Air){
    e1 = e_Al;
    e2 = e_air;
  }
  else if(interface == Air_Ice){
    e1 = e_air;
    e2 = e_ice;
  }
  else if(interface == Air_Sand){
    e1 = e_air;
    e2 = e_Si;
  }
  else if(interface == Sand_Air){
    e1 = e_Si;
    e2 = e_air;
  }
  else if(interface == Al_Sand){
    e1 = e_Al;
    e2 = e_Si;
  }
  else{
    cout<<"Interface not supported"<<endl;
    e1 = 0;
    e2 = 0;
  }

}

//-----------------------------------//
// Get Dummy histogram for labels
//-----------------------------------//
TH1F* dummy()
{

  TH1F* h = new TH1F("h","",1,xmin,xmax);
  h->SetStats(0);
  h->SetMaximum(10);
  h->SetMinimum(-8);
  return h;

}

//-----------------------------------//
// Get Beta
//-----------------------------------//
double Beta()
{

  return sqrt(E0*E0/(E0*E0 + 0.511));

}
