#ifndef Physics_h
#define Physics_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Class to house the physics constants needed to calculate //
// the electric field.                                      //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

// Some useful typedefs
typedef unsigned int uint;

// Constants
namespace Phys
{
  
  // Fundamental Constants
  //const double mur = 1.256627e-6; // Permeability of water -- ok for ice?
  const double mu0 = 1.256637e-6; // permeability of free space
  const double mur = 0.999992;    // relative permeability of water from wikipedia
  const double e0  = 8.8541e-12;  // Permitivity of Free space
  const double c   = 2.99792e8;   // Speed of light m/s
  const double c2  = c*c;         // c^2 is used quite a bit
  const double pi  = 3.141592654; // Pi
  const double e   = 1.602e-19;   // Electron charge coloumbs

  // Constants related to ice
  const double nICE  = 1.79;      // Index of refraction for ice
  const double X0ICE = 0.390522;  // Rad length of ice in m

  // Convenient conversions
  const double RAD_DEG = 0.017453292; // rads -> deg
  const double DEG_RAD = 57.2957795;  // deg -> rads
  
  const double MHz = 1.e6; // Hz

};

#endif
