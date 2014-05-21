
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
double Dim3::getRAFF(double time, double theta,
		     vector<double> Qz, double step)
{

  // First get LQtot given this particular 
  // charge distribution
  double LQtot = getLQtot(Qz,step);
  
  // Calculate the constant
  double C = TMath::Sin(theta) / (LQtot * TMath::Sin(m_thetaC));

  // Fix velocity at speed of light for now
  // When looking at ELS showers, will need
  // to do this dynamically
  double v = Phys::c;
  
  // Now the next process is to integrate and 
  // convolute the Fp part with charge distribution.
  double integral = 0;
  double z = 0, tR = 0, RA =0;
  for(uint i=0; i<Qz.size(); ++i){
    z  = step * (i+1); // in m
    tR = getTRetardedFF(time, z, v, theta);
    RA = getRAatThetaC(tR);
    integral += step * Qz.at(i) * RA;
  }

  
  //cout<<"\t Result: "<<fabs(C*integral)<<endl;
  
  // Now we can return the result
  return fabs(C * integral);
  
}

//-----------------------------------------------//
// Calculate |A(r,z,t)| for near-field
//-----------------------------------------------//
double Dim3::getANF(double time, double zObs, double rObs,
		    vector<double> Qz, double step)
{

  // First get LQtot given this particular 
  // charge distribution.
  double LQtot = getLQtot(Qz,step);
  
  // Calculate the constant
  //double C = Phys::mur * Phys::mu0 / (4 * Phys::pi);
  double C = 1/(LQtot*TMath::Sin(m_thetaC)); 

  // Set the velocity. Might want to do this 
  // dynamically based on energy.. think about
  // it for a little
  double v = Phys::c;
  
  // Now the next process is to integrate and 
  // convolute the Fp part with charge distribution.
  double integral = 0;
  double z=0, tR=0, RA=0, denom=0, magp=0;  
  for(uint i=0; i<Qz.size(); ++i){
    z         = step * (i+1); // in m
    tR        = getTRetardedNF(time, zObs, z, rObs, v);
    RA        = getRAatThetaC(tR);
    denom     = 1/TMath::Sqrt(rObs*rObs+pow(zObs-z,2)); 
    magp      = rObs/TMath::Sqrt(rObs*rObs+pow(zObs-z,2));
    integral += step * Qz.at(i) * RA * denom * magp;
  }
  
  // Now we can return the result
  return fabs(C * integral);
  
}


//-----------------------------------------------//
// Calculate RA at the cherenkov angle
//-----------------------------------------------//
double Dim3::getRAatThetaC(double time)
{
  
  // Get constant term
  double C = -4.5e-14 * m_showerE;

  // Put time into ns
  double t = time * 1e9;
  
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
// Get the retarded time for far-field
//-----------------------------------------------//
double Dim3::getTRetardedFF(double time, double z, 
			    double v, double theta)
{
  return time + z/v - z*Phys::nICE*TMath::Cos(theta)/Phys::c;
}

//-----------------------------------------------//
// Get the retarded time for near-field
//-----------------------------------------------//
double Dim3::getTRetardedNF(double time, double zObs, 
			    double zShower, double rObs,
			    double v)
{
  
  double n  = Phys::nICE;
  double c  = Phys::c;

  double tR = time - zShower/v;
  tR -= n*TMath::Sqrt(rObs*rObs + pow(zObs-zShower,2))/c;
  
  return tR;
}

//-----------------------------------------------//
// Get L*Qtot which represents the track length
// from the charge excess      
//-----------------------------------------------//
double Dim3::getLQtot(vector<double> Qz,
		      double step)
{
  
  // Just simply integrate and return the total...
  double tot = 0;
  for(uint i=0; i<Qz.size(); ++i){
    //tot += step * m_X0 * Qz.at(i);
    tot += step * Qz.at(i);
  }

  // Return the integral
  return tot;

}
