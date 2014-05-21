#ifndef Analysis3D_h
#define Analysis3D_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// This code will be a spin off of the initial 1D parameterization //
// where we will look at the 3D parameterized result from Jaime.   //
// Again, for reference we are followign arxiv:1106.6283.          //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

// My Goodies
#include "Dim3.h"
#include "Tools.h"

// Extras
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

// Root stuff
#include "TFile.h"
#include "TProfile.h"
#include "TGraph.h"

typedef unsigned int uint;

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
  void PlotTimeDependence(TFile* outfile, TFile* infile);

  // Now want to plot vector potential versus time
  // for the near-field. I will try to reproduce 
  // results in figure 5.
  void PlotNearField(TFile* outfile, TFile* infile);

  // Now want to plot vector potential versus time
  // for the near-field. In this function I fix for
  // specific angle. I want to see comparison with
  // far-field prediction
  void PlotNearFieldFixedAngle(TFile* outfile, TFile* infile);
  
 private:

  Dim3* m_dim3;              // 3D analysis object
  Tools* m_tool;             // Tool object
};

#endif
