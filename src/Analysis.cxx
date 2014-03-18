//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// The main to calculate the electric field using varius //
// methods. Right now only 1-D is implemented, but that  //
// will change shortly.                                  //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "Analysis.h"

//-----------------------------------------------//
// Constructor
//-----------------------------------------------//
Analysis::Analysis()
{
  // Dummy
}

//-----------------------------------------------//
// Destructor
//-----------------------------------------------//
Analysis::~Analysis()
{
  // Dummy
}

//-----------------------------------------------//
// Main
//-----------------------------------------------//
int main(int argc, char** argv)
{
  
  Dim1* dim1Ana = new Dim1();
  
  delete dim1Ana;
  
  return 0;
  
}
