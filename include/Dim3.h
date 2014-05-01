#ifndef Dim3_h
#define Dim3_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=// 
// This class will have the methods to carry out, at first, the //
// parameterized version of the 3D shower given a charge excess //
// profile from either ZHS or G4.  The calculation is based on  //
// Jaime Alvarez Muniz's paper: arxiv:1106.6283                 //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=// 

#include "TMath.h"
#include "Physics.h"
#include <vector>
#include <iostream>

class Dim3
{

 public:
  
  // Constructor / Destructor
  Dim3(double showerE);
  virtual ~Dim3();

  // The method given below will calculate the vector
  // potnetial following equation 17 given the N 
  // particle distribution, the time,  and some angle 
  // theta
  double getRA(double time,             // seconds
	       double theta,            // radians
	       std::vector<double> Qz,  // longitudinal distribution
	       double step);            // step size in rad lengths
  
    

 private:

  // Calculate the parameterized Fp following equation
  // 17 in Jaime's paper.  This is the form factor that
  // takes care of the lateral spread.
  double getFp(double time,       // seconds
	       double LQtot);     // L*Qtot

    
  // Calculate R*A(theta_C, t) following Jaime's 
  // parameterization given in equation 16.
  double getRAatThetaC(double time); // t in seconds

  // We need to calculate L*Qtot which is just the 
  // excess track length. Have a simple method to 
  // grab this from the profile that is sent in
  double getLQtot(std::vector<double> Qz,  // Qz
		  double step);            // Step in rad lengths

  // Some variables
  double m_showerE;              // energy of the shower
  double m_X0;                   // Raditaion lengths
  double m_thetaC;               // Cherenkov Angle

};

#endif
