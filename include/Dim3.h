#ifndef Dim3_h
#define Dim3_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=// 
// This class will have the methods to carry out, at first, the //
// parameterized version of the 3D shower given a charge excess //
// profile from either ZHS or G4.  The calculation is based on  //
// Jaime Alvarez Muniz's paper: arxiv:1106.6283                 //
//                                                              //
// Below FF = far-field and NF = near-field                     //
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
  double getRAFF(double time,             // seconds
		 double theta,            // radians
		 std::vector<double> Qz,  // longitudinal distribution
		 double step);            // step size in rad lengths

  // Get the near-field approximation following equation 22
  // given in Jaime's paper. Unlike the far-field, the user
  // will need to specify more parameters, including the exact
  // location of the obersvers
  double getANF(double time,             // seconds
		double zObs,             // observer z position
		double rObs,             // observer r position
		std::vector<double> Qz,  // longitudinal distribution
		double step);            // step size in rad lengths
  
    

 private:

    
  // Calculate R*A(theta_C, t) following Jaime's 
  // parameterization given in equation 16.
  double getRAatThetaC(double time);   // t in seconds

  // Get the retarded time taking into account
  // the longitudinal position of the shower
  // for the far-field approximation
  double getTRetardedFF(double time,   // seconds
			double z,      // m
			double v,      // m/s
			double theta); // radians
  
  // Get the retarded time for the near-field
  // approximation following the arguments for Fp
  // given in equation 22.
  double getTRetardedNF(double time,    // seconds
			double zObs,    // observer z pos [m]
			double zShower, // shower z [m]
			double rObs,    // observer r pos [m]
			double v);      // shower velocity [m/s]

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
