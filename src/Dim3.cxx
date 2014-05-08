
#include "Dim3.h"

using namespace std;

//-----------------------------------------------//
// Constructor
//-----------------------------------------------//
Dim3::Dim3(double showerE) :
  m_showerE(showerE),
  m_X0(0.)
{

  // Set the radiation length here to whatever it is
  // in ice.  Note that the units are in m.
  m_X0 = Phys::X0ICE;

  // Right now this is hardcoded but should be made
  // a parameter in the future.
  m_thetaC = 55.8 / Phys::DEG_RAD;  // in Radians
  
}

//-----------------------------------------------//
// Destructor
//-----------------------------------------------//
Dim3::~Dim3()
{

}

//-----------------------------------------------//
// Calculate RA(theta,t)
//-----------------------------------------------//
double Dim3::getRA(double time, double theta,
		   vector<double> Qz, double step)
{

  // First get LQtot given this particular 
  // charge distribution
  double LQtot = getLQtot(Qz,step);

  // Calculate the constant
  double C = TMath::Sin(theta) / (LQtot * TMath::Sin(m_thetaC));

  // Set the velocity. Might want to do this 
  // dynamically based on energy.. think about
  // it for a little
  //double v = 0.99 * Phys::c;
  double v = 0.95 * Phys::c;
  
  // Now the next process is to integrate and 
  // convolute the Fp part with charge distribution.
  double integral = 0;
  double z = 0, tR = 0, RA =0;
  for(uint i=0; i<Qz.size(); ++i){
    z  = step * m_X0 * (i+1); // in m
    tR = getTRetarded(time, z, v, theta);
    RA = getRAatThetaC(tR);
    integral += step * m_X0 * Qz.at(i) * RA;
    //cout<<"Z: "<<z<<" tr: "<<tR<<" RA: "<<RA<<" C: "<<C<<endl;
  }

  //cout<<"\t Result: "<<fabs(C*integral)<<endl;

  // Now we can return the result
  return fabs(C * integral);
  
}


//-----------------------------------------------//
// Calculate RA at the cherenkov angle
//-----------------------------------------------//
double Dim3::getRAatThetaC(double time)
{
  
  // Get constant term
  double C = -4.5*pow(10,-14) * m_showerE;

  // Put time into ns
  double t = time * pow(10,9);
  
  // Get piecewise portion
  double tDep = 0;
  if( t > 0 ) 
    tDep = TMath::Exp(-fabs(t)/0.057) + pow(1+2.87*fabs(t),-3);
  else
    tDep = TMath::Exp(-fabs(t)/0.030) + pow(1+3.05*fabs(t),-3.5);
  
  // Now return
  return C * tDep;
  
}

//-----------------------------------------------//
// Get the retarded time
//-----------------------------------------------//
double Dim3::getTRetarded(double time, double z, 
		    double v, double theta)
{

  // Want to return t - z[1/v - ncos(th)/c]
  return time - z * (1/v - Phys::nICE*TMath::Cos(theta)/Phys::c);

}

//-----------------------------------------------//
// Get L*Qtot which represents the track length
// from the charge excess      
//-----------------------------------------------//
double Dim3::getLQtot(vector<double> Qz,
		      double step)
{
  
  // Just simply integrate and return the total
  double tot = 0;
  for(uint i=0; i<Qz.size(); ++i)
    tot += step * m_X0 * Qz.at(i);

  // Return the integral
  return tot;

}
