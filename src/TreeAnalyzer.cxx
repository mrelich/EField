
#include "TreeAnalyzer.h"

using namespace std;

//--------------------------------------------------------/
// Constructor
//--------------------------------------------------------/
TreeAnalyzer::TreeAnalyzer() :
  m_tree(NULL),
  m_file(NULL),
  m_currentEvent(0),
  m_entry(0),
  m_entries(0),
  m_evtNum(0),
  m_E(0),
  m_zi(0),
  m_zf(0)
{  

}

//--------------------------------------------------------/
// Destructor
//--------------------------------------------------------/
TreeAnalyzer::~TreeAnalyzer()
{

  if( m_tree ) delete m_tree;
  if( m_file ) delete m_file;

}

//--------------------------------------------------------/
// Set Tree
//--------------------------------------------------------/
void TreeAnalyzer::initialize(string fname)
{

  // Open the file
  m_file = new TFile(fname.c_str());

  // Set the tree
  m_tree = (TTree*) m_file->Get("ntuple");

  // Set Branch Address
  m_tree->SetBranchAddress("evtNum", &m_evtNum);
  m_tree->SetBranchAddress("E",      &m_E);
  m_tree->SetBranchAddress("pdg",    &m_pdg);
  m_tree->SetBranchAddress("zi",     &m_zi);
  m_tree->SetBranchAddress("zf",     &m_zf);

  // Save N Entries and load first one
  m_entry   = 0;
  m_entries = m_tree->GetEntries();
  if( m_entries > 0 ) m_tree->GetEntry(m_entry);

  // Output some notice
  cout<<"Tree inialized with "<<m_entries<<" entries"<<endl;
}

//--------------------------------------------------------/
// Finalize
//--------------------------------------------------------/
void TreeAnalyzer::finalize()
{
  
  // Close the file
  m_file->Close();

  // Reset tree location
  m_tree = NULL;

  // Reset variables
  m_currentEvent = 0;
  m_evtNum = 0;
  m_E      = 0;
  m_pdg    = 0;
  m_zi     = 0;
  m_zf     = 0;
  
}

//--------------------------------------------------------/
// Retrieve profile for next event
//--------------------------------------------------------/
void TreeAnalyzer::getNextQz(float stepSize,
			     vector<double> &Qz)
{

  // Reset previous points
  for(uint q=0; q<Qz.size(); ++q)
    Qz[q] = 0;

  // Check to make sure you are not at the end of the tree
  if( m_entry >= m_entries){
    cout<<"Reached end of file: "<<m_entry<<" >= "<<m_entries<<endl;
    Qz.clear();
    return;
  }

  // Loop over current event
  while( m_currentEvent == m_evtNum ){

    // Require e/p and E > 1 MeV
    if( TMath::Abs(m_pdg) == 11 && m_E > 1.){

      // Analyze this line
      int radi = (int) (m_zi /(stepSize*Phys::X0ICE*100));
      int radf = (int) (m_zf /(stepSize*Phys::X0ICE*100));

      // Make sure we are in bounds
      if(radf > Qz.size())
	radf = Qz.size();
      if(radi > Qz.size())
	radi = Qz.size();

      // Save results
      for(int i=radi; i<radf; ++i)
	Qz.at(i) += m_pdg == 11 ? 1 : -1;

    }
    
    // Increment to next entry
    m_entry++;
    if( m_entry >= m_entries ) break;
    m_tree->GetEntry(m_entry);
    
  }

  m_currentEvent = m_evtNum;

}

