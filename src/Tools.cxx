
#include "Tools.h"

using namespace std;

//---------------------------------------------------//
// Constructor
//---------------------------------------------------//
Tools::Tools()
{
  // dummy for now
}

//---------------------------------------------------//
// Destructor
//---------------------------------------------------//
Tools::~Tools()
{
  // dummy for now
}

//---------------------------------------------------//
// Convert profile
//---------------------------------------------------//
vector<double> Tools::convert(TProfile* prof)
{

  // New vector                                                                                       
  vector<double> steps;

  // Loop over bins and save                                                                          
  int nbins = prof->GetNbinsX();
  for(int bin=1; bin<=nbins; ++bin)
    steps.push_back( prof->GetBinContent(bin) );

  // return converted result                                                                          
  return steps;

}
