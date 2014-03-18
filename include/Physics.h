#ifndef Physics_h
#define Physics_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Class to house the physics constants needed to calculate //
// the electric field.                                      //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

namespace Phys
{
  
  // Fundamental Constants
  const double mur = 1.256627e-6; // Permeability of water -- ok for ice?
  const double e0  = 8.8541e-12;  // Permitivity of Free space
  const double c   = 2.99792e8;   // Speed of light m/s
  const double c2  = pow(c,2);    // c^2 is used quite a bit
  const double pi  = 3.141592654; // Pi
  const double e   = 1.602e-19;   // Electron charge coloumbs


};

#endif
