
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


