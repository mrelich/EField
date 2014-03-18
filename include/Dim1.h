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

  // The methods below will calculate Rx|E(f,theta)|
  // given the N Particle distribution vs. shower depth
  // in terms of radiation length. Right now these profiles
  // are taken from Geant4 simulations.
  double getRE(double theta,             // radians
	       double freq,              // MHz
	       std::vector<double> Qz,   // Qz distribution
	       double step               // step size in rad lengths
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


 private:
  
  double RE_const; // product of constants. Initialize once to avoid recalc.
  double X0;       // Radiation Length 

};

#endif
