#ifndef Analysis_h
#define Analysis_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// The main to calculate the electric field using varius //
// methods. Right now only 1-D is implemented, but that  //
// will change shortly.                                  //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

// My goodies
#include "Dim1.h"
#include "TreeAnalyzer.h"
#include "Tools.h"

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
  void Average1DResult(TFile* outfile);

  // Use the analytic approximation for 1-D 
  // parameterization. This involves looking at
  // the simple case around Cherenkov angle
  // and applying a correction factor.
  void Analytic1DResult(TFile* outfile);

  // Method to look at the 1-D parameterization
  // on an event by event basis.
  void Event1DResult(TFile* outfile);

  // Convert the TProfile for some results into
  // a simple vector<double>
  std::vector<double> convert(TProfile* prof);

  // Simple print method
  void print(std::vector<double> v){
    for(uint i=0; i<v.size(); ++i)
      std::cout<<"i: "<<i<<" "<<v.size()<<std::endl;
  };

 private:
  
  Dim1* m_dim1;            // 1*D ana object
  Tools* m_tool;           // Tool object
  TreeAnalyzer* m_treeAna; // Analyze Tree object

  std::vector<std::string> m_fnames; // File names to consider
  std::vector<std::string> m_gnames; // Names to save graphs
  std::vector<std::string> m_tnames; // File names for trees
  std::vector<std::string> m_tgnames; // Names to save graphs for tree plots
  
};

#endif
