#ifndef Dim1_h
#define Dim1_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// This class will have methods to perform the parameterized  //
// 1-D calculation of the electric field given a distribution //
// of the charge excess as a function of distance.            //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "TMath.h"
#include "Physics.h"
#include <vector>
#include <iostream>

class Dim1 
{

 public:
  
  // Constructor/Destructor
  Dim1();
  virtual ~Dim1();  

  // The method below will calculate Rx|E(f,theta)|
  // given the N Particle distribution vs. shower depth
  // in terms of radiation length. Right now these profiles
  // are taken from Geant4 simulations.
  double getRE(double theta,             // radians
	       double freq,              // MHz
	       std::vector<double> Qz,   // Qz distribution
	       double step               // step size in rad lengths
	       );

  // There also exists an analytic approximation that is
  // solely based on the expected shower energy.  For this
  // you don't need the Qz, but rather you pass the shower
  // energy, frequency, and angle
  double getAnalyticRE(double theta,     // radians
		       double freq,      // MHz
		       double showerE   // TeV
		       );

 private:
  
  // Get Integral result
  double excessInt(double p,                // p(w,theta)
		   std::vector<double> Qz,  // Q(z) distribution
		   double step,             // step size
		   bool isReal              // Real or imaginary part
		   );

  // Get p(w,theta) from astro-ph/0003315
  double getP(double theta, double freq);

  // Get theta_Cherenkov
  // Here assuming beta factor of 1.
  // According to astro-ph/0003315, 
  // seems they used ~0.995, so it's close 
  // enough :P
  // return value in radians!!
  double getThetaC(){ return TMath::ACos(1/Phys::nICE); };
    
  // Get the analytic value Rx|E| evaluated
  // at the cherenkov angle
  double getAnalyticRE(double freq, double showerE);

  // Get the angulare correction factor
  double getAngleCorrection(double theta, double freq, double showerE);


 private:
  
  double RE_const; // product of constants. Initialize once to avoid recalc.
  double X0;       // Radiation Length 
  double nu_0;     // Some frequency norm parameter from astro-ph/0003315

};

#endif
