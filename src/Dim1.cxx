
#include "Dim1.h"

using namespace std;

//-------------------------------------------------//
// Constructor
//-------------------------------------------------//
Dim1::Dim1() :
  RE_const(0.)
{

  // Caculate the product of many physics 
  // constants that appear in front of Eq.4
  // found in Jaime's paper: astro-ph/0003315
  RE_const = (Phys::e * Phys::mur) / 
    (2 * Phys::pi * Phys::e0 * Phys::c2);
  
  // Specify the radiation length.  Right now only
  // ice is available, but this could become an 
  // option later
  X0 = Phys::X0ICE;


  // There is some frequency normalization parameter
  // that needs to be set for the analytical 1-D 
  // approximation
  //nu_0 =1015.;  // this is in MHz.
  nu_0 =500.;  // this is in MHz. from astro-ph/9706064 
}

//-------------------------------------------------//
// Destructor
//-------------------------------------------------//
Dim1::~Dim1()
{


}

//-------------------------------------------------//
// Get Rx|E(f,theta)|
//-------------------------------------------------//
double Dim1::getRE(double theta, double freq, 
		   vector<double> Qz, double step)
{

  // Get p for this freq and angle
  double p = getP(theta,freq);
  cout<<"Theta: "<<theta/Phys::RAD_DEG<<" p: "<<p<<endl;
  // Get real and imaginary part for charge integral
  double real = excessInt(p, Qz, step, true);
  double imag = excessInt(p, Qz, step, false);

  // Take magnitude of real and imaginary
  double mag  = TMath::Sqrt(real*real+imag*imag);
  
  // Angular frequency
  double w = 2*Phys::pi*freq*Phys::MHz;

  // Get total result
  double result = RE_const * w * TMath::Sin(theta) * mag;

  // Result is in V/Hz, convert to V/MHz
  return result * Phys::MHz;
}

//-------------------------------------------------//
// Get Real part of integral from
// astro-ph/0003315
//-------------------------------------------------//
double Dim1::excessInt(double p, vector<double> Qz, 
		       double step, bool isReal)
{

  // Result of integrating
  double integral   = 0;
  double stepResult = 0;
  double dZ         = step * X0;

  // Loop over and calculate
  for(uint i=0; i<Qz.size(); ++i){
    
    // Calculate result for one step depending
    // on whether real or imaginary part 
    // is desired
    stepResult = Qz.at(i) * dZ;
    if(isReal) stepResult *= TMath::Cos(p*step*(i+1)*X0);
    else       stepResult *= TMath::Sin(p*step*(i+1)*X0);
    
    // record
    integral += stepResult;
  }
  
  // return integral
  return integral;
 
}

//-------------------------------------------------//
// Calculate p(w,theta)
//-------------------------------------------------//
double Dim1::getP(double theta, double freq)
{
  
  freq *= Phys::MHz; // Convert to Hz
  return 2 * Phys::pi * freq * (1 - Phys::nICE*TMath::Cos(theta))/Phys::c;

}

//-------------------------------------------------//
// Get Analytic Rx|E|
//-------------------------------------------------//
double Dim1::getAnalyticRE(double theta, double freq, double showerE)
{

  // Get the analytic result for the cherenkov angle
  double anaAtCherenkov = getAnalyticRE(freq,showerE);

  // Now get the correction
  double angleCorr = getAngleCorrection(theta,freq,showerE);

  // Now return the result which is product
  // following astro-ph/0003315
  return anaAtCherenkov * angleCorr;  

}

//-------------------------------------------------//
// Get Analytic Rx|E| at theta_Cherenkov
//-------------------------------------------------//
double Dim1::getAnalyticRE(double freq, double showerE)
{

  // Some constants following astro-ph/0003315
  //double C    = 2.53e-7;  // no units
  // Calculate Eq.6 result
  //return C * (showerE/1.) * (freq/nu_0) * 1/(1+pow(freq/nu_0,1.44));

  // UPDATE TO USING astro-ph/9706064 
  // Some constants following astro-ph/9706064
  double C = 1.1e-7;

  // Now calculate Eq.1 from  astro-ph/9706064
  return C * (showerE/1.) * (freq/nu_0) * 1/(1+0.4*pow(freq/nu_0,2));

}

//-------------------------------------------------//
// Get Correction factor
//-------------------------------------------------//
double Dim1::getAngleCorrection(double theta, double freq, double showerE)
{

  // Only real parameter is dTheta
  double dTheta  = 1;
  double theta_C = getThetaC();

  // Shower energy is assumed to be in TeV
  if(showerE < 1000) dTheta = 2.7 * (nu_0/freq) * pow(showerE/1000, -0.03);
  else{
    // I don't know what to put for E_LPM if this is a known parameter
    // or if it is something that I need to estimate/extract.  
    // Ignore for now, since my showers are < 1 PeV.
    cout<<"In Dim1::getAngleCorrection(...)"<<endl;
    cout<<"The shower energy is currently not supported for 1D"<<endl;
    cout<<"analytic calculation, which you are trying to use for"<<endl;
    cout<<"Setting dTheta = 1, which should give bogus results"<<endl;
    dTheta = 1;
  }
  double theta_diff = (theta-theta_C)/Phys::RAD_DEG;
  double exponent = -1 * TMath::Log(2) * pow(theta_diff/dTheta,2);
  
  return TMath::Exp(exponent);   
  
}
