#ifndef Analysis3D_h
#define Analysis3D_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// This code will be a spin off of the initial 1D parameterization //
// where we will look at the 3D parameterized result from Jaime.   //
// Again, for reference we are followign arxiv:1106.6283.          //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "Dim3.h"
#include <iostream>

class Analysis3D
{

 public:

  // Constructor / Destructor
  Analysis3D(double energy);
  virtual ~Analysis3D();

  // Method to make the time dependent response given
  // a fixed angle. Later build the electric field
  // from this result, similar to figure 3 in Jaime's
  // paper.
  void PlotTimeDependence();

 private:

  Dim3* m_dim3;              // 3D analysis object

};

#endif
