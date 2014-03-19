#ifndef TreeAnalyzer_h
#define TreeAnalyzer_h

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// This class will read in the simple tree of the G4 events //
// and return some useful objects for the analysis of       //
// electric field produced form the particle shower.        //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "Physics.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include <iostream>

class TreeAnalyzer
{

 public:
  // Constructor / Destructor
  TreeAnalyzer();
  virtual ~TreeAnalyzer();

  // Method to set the tree
  void initialize(std::string fname);
  
  // Method to close file and tree
  void finalize();

  // Retrieve profile
  void getNextQz(float stepSize,
		 std::vector<double> &Qz);

 private:
  
  TTree* m_tree;      // Current tree to analyze
  TFile* m_file;    
  int m_currentEvent; // The current event to analyze
  int m_entry;        // Position in the tree
  int m_entries;      // Number of entries in the tree

  // Variables in the tree
  int    m_evtNum;
  double m_E;
  int    m_pdg;
  double m_zi;
  double m_zf;

};

#endif
