
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
  m_thetaC = 55. / Phys::DEG_RAD;  // in Radians
  
  //
  
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
  double C = Phys::mur * TMath::Sin(theta) / (4 * Phys::pi);
  
  // Now the next process is to integrate. 
  // We should go from -infinity to inifinity,
  // but our charge distribution is (in reality) 
  // finite, so we just go over the Qz bins.
  double integral = 0;
  for(uint i=0; i<Qz.size(); ++i)    
    integral += (i+1) * step * m_X0 * Qz.at(i) * getFp(time,LQtot);
    
  // Now we can return the result
  return C * integral;
  
}

//-----------------------------------------------//
// Calculate the form factor, Fp(time)
//-----------------------------------------------//
double Dim3::getFp(double time,
		   double LQtot)
{
  
  // Get the constant factor
  double C = 4 * Phys::pi / (Phys::mur * TMath::Sin(m_thetaC) * LQtot);
  
  // Now the final piece is RA at ThetaC
  return C * getRAatThetaC(time);
  
}

//-----------------------------------------------//
// Calculate RA at the cherenkov angle
//-----------------------------------------------//
double Dim3::getRAatThetaC(double time)
{

  // Get constant term
  double C = -4.5*pow(10,-14) * m_showerE;

  // Get time dependent part
  double tDep = 0;
  if( time > 0 ) 
    tDep = TMath::Exp(-fabs(time)/0.057) + pow(1+2.87*fabs(time),-3);
  else
    tDep = TMath::Exp(-fabs(time)/0.030) + pow(1+3.05*fabs(time),-3.5);
  
  // Now return
  return C * tDep;

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
    tot += (i+1) * step * m_X0 * Qz.at(i);

  // Return the integral
  return tot;

}
