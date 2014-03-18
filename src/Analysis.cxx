//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//
// The main to calculate the electric field using varius //
// methods. Right now only 1-D is implemented, but that  //
// will change shortly.                                  //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-//

#include "Analysis.h"

using namespace std;

//-----------------------------------------------//
// Constructor
//-----------------------------------------------//
Analysis::Analysis()
{

  // Create 1-D Analysis object
  m_dim1 = new Dim1();

}

//-----------------------------------------------//
// Destructor
//-----------------------------------------------//
Analysis::~Analysis()
{
  
  delete m_dim1;

}

//-----------------------------------------------//
// Average 1D Result
//-----------------------------------------------//
void Analysis::Average1DResult(TFile* outfile)
{

  string indir = "rootfiles/";

  // Open up a root file. Hardcode for now, but
  // make it an option later
  vector<string> fnames;
  fnames.push_back(indir+"TrkAna_100_100000_ice_eBeam.root");
  fnames.push_back(indir+"TrkAna_20_1000000_ice_eBeam.root");

  // names to save
  vector<string> gnames;
  gnames.push_back("g4_100GeV");
  gnames.push_back("g4_1TeV");

  // Set the profile name
  string pname      = "NPartDiff";

  // Get Rx|E| vs. Degree for constant freq
  float freq    = 300; // MHz
  int np        = 100;
  float step    = 0.55;
  float theta_i = 25; // start at 25 degrees
  float theta[np];
  float RE[np];

  // Load profiles and save electric field
  TFile* infile  = NULL;
  for(uint f=0; f<fnames.size(); ++f){
    string fname = fnames.at(f);
    string gname = gnames.at(f);
    
    // Open file
    infile = new TFile(fname.c_str());
    
    // Get Profile
    TProfile* prof    = (TProfile*) infile->Get(pname.c_str()); 
    vector<double> Qz = convert(prof);
    double stepSize   = prof->GetBinWidth(1);
    
    // Delete profile and close file
    delete prof;
    infile->Close();
    infile = NULL;
    
    for(int i=0; i<np; ++i){
      // Save theta
      theta[i] = theta_i + step*i;
      // Save Rx|E|
      RE[i] = m_dim1->getRE(theta[i]/Phys::DEG_RAD,
			    freq,
			    Qz,
			    stepSize);
      
    }// end loop over points 
    
    outfile->cd();
    TGraph* gr = new TGraph(np,theta,RE);
    gr->SetName(gname.c_str());
    gr->Write();
    delete gr;
    
  }// end loop over files

  delete infile;

}

//-----------------------------------------------//
// Analytic 1-D Result
//-----------------------------------------------//
void Analysis::Analytic1DResult(TFile* outfile)
{

  // In this case I need to simply loop over theta
  // for a fixed frequency and save the results. We
  // will loop over two shower energies: 100 GeV, 1TeV
  // this is because that is all I have by way of simulation
  // at the moment.
  vector<double> showerEs; // in TeV
  showerEs.push_back(0.1); 
  showerEs.push_back(1);

  vector<string> gnames;
  gnames.push_back("ana_100GeV");
  gnames.push_back("ana_1TeV");

  // Get Rx|E| vs. Degree for constant freq                                                                                        
  float freq    = 300; // MHz
  int np        = 100;
  float step    = 0.55;
  float theta_i = 25;  // start at 25 degrees 
  float theta[np];
  float RE[np];  

  // Loop over energies
  for(uint s=0; s<showerEs.size(); ++s){
    double showerE = showerEs.at(s);
    string gname   = gnames.at(s);

    // Loop and get Rx|E|
    for(int i=0; i<np; ++i){
      // Save theta
      theta[i] = theta_i + step*i;
      // Save Rx|E|
      RE[i] = m_dim1->getAnalyticRE(theta[i]/Phys::DEG_RAD,
				    freq,
				    showerE);
      
    }// end loop over points 

    // Make graph and save
    outfile->cd();
    TGraph* gr = new TGraph(np,theta,RE);
    gr->SetName(gname.c_str());
    gr->Write();
    delete gr;    

  }// end loop over shower energies

}

//-----------------------------------------------//
// Convert profile to vector
//-----------------------------------------------//
vector<double> Analysis::convert(TProfile* prof)
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


//********************************************************************//
//                 --- END OF ANALYSIS CODE ---                       //
//********************************************************************//

//-----------------------------------------------//
// Main
//-----------------------------------------------//
int main(int argc, char** argv)
{
 

  // Create Ana object
  Analysis* ana = new Analysis();

  // Create an output for this run
  TFile* outfile = new TFile("../results/AnaOutput.root","recreate");
  
  // Execute commands
  ana->Average1DResult(outfile);
  ana->Analytic1DResult(outfile);
  
  // Clean up, we're done here.
  outfile->Write();
  outfile->Close();
  delete outfile;

  delete ana;  
  return 0;
  
}