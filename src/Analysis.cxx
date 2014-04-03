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

  // Create Tree analyzer
  m_treeAna = new TreeAnalyzer();

  // Specify the directory where the Geant4 output is located
  string indir = "rootfiles/";

  // Initialize the file names to look at
  m_fnames.push_back(indir+"TrkAna_100_100000_ice_eBeam_1MeV.root");
  //m_fnames.push_back(indir+"TrkAna_50_1000000_ice_eBeam_1MeV.root");
  //m_fnames.push_back(indir+"TrkAna_20_10000000_ice_eBeam_1MeV.root");
  //m_fnames.push_back(indir+"TrkAna_100_10000_ice_eBeam_np10_1MeV.root");
  //m_fnames.push_back(indir+"TrkAna_50_100000_ice_eBeam_np10_1MeV.root");
  //m_fnames.push_back(indir+"TrkAna_50_10000_ice_eBeam_np100_1MeV.root");

  // Initialize the output graph names
  m_gnames.push_back("g4_100GeV");
  m_gnames.push_back("g4_1TeV");
  m_gnames.push_back("g4_10TeV");

  m_gnames.push_back("g4_10GeV_np10");
  m_gnames.push_back("g4_100GeV_np10");
  m_gnames.push_back("g4_10GeV_np100");

  // Initialize the file names to look at
  m_tnames.push_back(indir+"TrkTree_100_100000_ice_eBeam.root");
  //m_tnames.push_back(indir+"TrkTree_50_1000000_ice_eBeam.root");
  //m_tnames.push_back(indir+"TrkTree_20_10000000_ice_eBeam.root");
  //m_tnames.push_back(indir+"TrkTree_100_10000_ice_eBeam_np10.root");
  //m_tnames.push_back(indir+"TrkTree_50_100000_ice_eBeam_np10.root");
  //m_tnames.push_back(indir+"TrkTree_50_10000_ice_eBeam_np100.root");

  // Initialize the output graph names
  m_tgnames.push_back("g4_100GeV_event");
  m_tgnames.push_back("g4_1TeV_event");
  m_tgnames.push_back("g4_10TeV_event");

  m_tgnames.push_back("g4_10GeV_np10_event");
  m_tgnames.push_back("g4_100GeV_np10_event");
  m_tgnames.push_back("g4_10GeV_np100_event");



}

//-----------------------------------------------//
// Destructor
//-----------------------------------------------//
Analysis::~Analysis()
{
  
  delete m_dim1;
  //delete m_treeAna;

}

//-----------------------------------------------//
// Average 1D Result
//-----------------------------------------------//
void Analysis::Average1DResult(TFile* outfile)
{

  // Set the profile name
  string pname      = "NPartDiff";

  // Get Rx|E| vs. Degree for constant freq
  float freq    = 300; // MHz
  int np        = 100;
  float step    = 0.6;
  float theta_i = 25; // start at 25 degrees
  float theta[np];
  float RE[np];

  // Load profiles and save electric field
  TFile* infile  = NULL;
  for(uint f=0; f<m_fnames.size(); ++f){
    string fname = m_fnames.at(f);
    string gname = m_gnames.at(f);
    
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
  showerEs.push_back(10);

  vector<string> gnames;
  gnames.push_back("ana_100GeV");
  gnames.push_back("ana_1TeV");
  gnames.push_back("ana_10TeV");

  // Get Rx|E| vs. Degree for constant freq                                                                                        
  float freq    = 300; // MHz
  int np        = 100;
  float step    = 0.6;
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

//-----------------------------------------------//
// Event by event result
//-----------------------------------------------//
void Analysis::Event1DResult(TFile* outfile)
{

  // Loop over files
  for(uint f=0; f<m_tnames.size(); ++f){
    string fname = m_tnames.at(f);
    string gname = m_tgnames.at(f);

    // Pass tree to tree analyzer
    m_treeAna->initialize(fname);
    
    // Place holders
    float freq    = 300; // MHz
    int np        = 100;
    float step    = 0.6;
    float theta_i = 25;  // start at 25 degrees 
    float theta[np];
    float RE[np];
    
    // Specify the step size for getting charge
    // distribution.
    float stepSize = 0.5;
    
    // Load first charge profile
    vector<double> Qz (20,0);
    m_treeAna->getNextQz(stepSize, Qz);
    
    // Now loop for as long as the length is not zero
    int count = 0;
    while( Qz.size() != 0 ){

      // Loop and calculate electric field
      for(int i=0; i<np; ++i){
	theta[i] = theta_i + step*i;
	RE[i] = m_dim1->getRE(theta[i]/Phys::DEG_RAD,
			      freq,
			      Qz,
			      stepSize);
      }// end loop over points       
      
      // Make TGraph and save
      outfile->cd();
      TGraph* gr = new TGraph(np,theta,RE);
      gr->SetName(Form("%s_%i",gname.c_str(),count));
      gr->Write();
      delete gr;
      
      // Increment counter and get next charge
      count++;
      m_treeAna->getNextQz(stepSize, Qz);
      
    }// end while loop

    m_treeAna->finalize();
  
  }// end loop over files

}

//********************************************************************//
//                 --- END OF ANALYSIS CODE ---                       //
//********************************************************************//

//-----------------------------------------------//
// Help Menu
//-----------------------------------------------//
void help()
{

  cout<<endl;
  cout<<"**********************************"<<endl;
  cout<<"Help Menu"                         <<endl;
  cout<<"-a 1D vs. Theta using Avg Profile "<<endl;
  cout<<"-b 1D vs. Theta using Analytic Est"<<endl;
  cout<<"-c 1D vs. Theta from profiles on  "<<endl;
  cout<<"\tan event by event basis."        <<endl;
  cout<<"**********************************"<<endl;
  cout<<endl;


}

//-----------------------------------------------//
// Main
//-----------------------------------------------//
int main(int argc, char** argv)
{
 
  // Define options
  bool run1DVsTheta      = false;
  bool run1DVsThetaAna   = false;
  bool run1DVsThetaEvent = false;

  // Loop over options
  for(int i=1; i<argc; ++i){
    if( strcmp(argv[i], "-a") == 0 )
      run1DVsTheta = true;
    else if( strcmp(argv[i], "-b") == 0 )
      run1DVsThetaAna = true;
    else if( strcmp(argv[i], "-c") == 0 )
      run1DVsThetaEvent = true;
    else{
      help();
      return 0;
    }
  }// end loop over options

  // Create Ana object
  Analysis* ana = new Analysis();

  // Create an output for this run
  //TFile* outfile = new TFile("../results/AnaOutput.root","recreate");
  TFile* outfile = new TFile("../results/AnaOutput_Mar25.root","recreate");
  
  // Execute commands
  if(run1DVsTheta)      ana->Average1DResult(outfile);
  if(run1DVsThetaAna)   ana->Analytic1DResult(outfile);
  if(run1DVsThetaEvent) ana->Event1DResult(outfile);

  // Clean up, we're done here.
  outfile->Write();
  outfile->Close();
  delete outfile;

  delete ana;  
  return 0;
  
}
