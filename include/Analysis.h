#ifndef Analysis_h
#define Analysis_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// The main to calculate the electric field using varius //
// methods. Right now only 1-D is implemented, but that  //
// will change shortly.                                  //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

// My goodies
#include "Dim1.h"

// Root shiz
#include "TFile.h"
#include "TString.h"
#include "TProfile.h"
#include "TGraph.h"

// Std
#include <vector>

class Analysis
{

 public:

  // Constructor/Destructor
  Analysis();
  virtual ~Analysis();

  // Look at the profile averaged over many G4
  // showers in order to determine the E field
  void Average1DResult();

  // Convert the TProfile for some results into
  // a simple vector<double>
  std::vector<double> convert(TProfile* prof);

 private:
  
  Dim1* m_dim1; // 1*D ana object

};

#endif
