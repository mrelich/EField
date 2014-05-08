#ifndef Tools_h
#define Tools_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Expanding the analysis code to handle the 3D case and realize  //
// it would be a good idea to have some common tools used in each //
// analysis code.                                                 //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "TProfile.h"
#include <vector>


class Tools
{

 public:
  
  // Constructor / Destructor
  Tools();
  virtual ~Tools();

  // Method to convert TProfile to 
  // a vector of doubles
  std::vector<double> convert(TProfile* prof);

};

#endif
