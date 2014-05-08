
#include "Analysis3D.h"

using namespace std;

//--------------------------------------------------//
// Constructor
//--------------------------------------------------//
Analysis3D::Analysis3D(double beamEnergy) :
  m_dim3(NULL),
  m_tool(NULL)
{

  // Create Dim3 ana object
  m_dim3 = new Dim3(beamEnergy);
  
  // Make tool object
  m_tool = new Tools();
}

//--------------------------------------------------//
// Destructor
//--------------------------------------------------//
Analysis3D::~Analysis3D()
{

  delete m_dim3;

}

//--------------------------------------------------//
// Plot time dependence
//--------------------------------------------------//
void Analysis3D::PlotTimeDependence(TFile* outfile)
{

  // Set the profile
  string pname = "NPartDiff";

  // Get Rx|A| vs. time for fixed angle
  float angle = (55.8 - 0.3)/Phys::DEG_RAD;
  int np      = 300;
  float step  = 0.01;  // ns
  float t_i   = 0;     // initial time
  float time[np];
  float RA[np];
  
  // Load profiles and save the vector potential
  // make this a loop later, but for now just handle
  // one file
  //TFile* infile = new TFile("/home/mrelich/work/ara/IceBlock/rootfiles/TrkAna_20_10000000_ice_eBeam_1MeV.root");
  TFile* infile = new TFile("/home/mrelich/work/ara/MyZHS/rootfiles/test.root");
  
  // Get profile
  TProfile* prof = (TProfile*) infile->Get(pname.c_str());
  vector<double> Qz = m_tool->convert(prof);
  double stepSize   = prof->GetBinWidth(1);  // assuming uniform binning
  
  // Delete profile and close file
  delete prof;
  infile->Close();
  delete infile;
  
  // Now loop and get info
  for(int i=0; i<np; ++i){
    
    // Save time and Rx|A|
    time[i] = t_i + step*i; // in ns
    RA[i]   = m_dim3->getRA(time[i]*pow(10,-9), // convert to s 
			    angle, 
			    Qz, 
			    stepSize);
  }// end loop over points

  // Now make TGraph and save
  
  outfile->cd();
  TGraph* gr = new TGraph(np,time,RA);
  gr->SetName("gr1");
  gr->Write();
  delete gr;

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
//                    --- END OF ANALYSIS CODE ---                        //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

//--------------------------------------------------//
// Help
//--------------------------------------------------//
void help()
{
  
  cout<<endl;
  cout<<"**************************************"<<endl;
  cout<<"Help Menu"                             <<endl;
  cout<<"**************************************"<<endl;
  cout<<endl;

}

//--------------------------------------------------//
// Main
//--------------------------------------------------//

int main(int argc, char** argv)
{

  double beamEnergy = -999;

  // Loop over options (none now)
  for(int i=1; i<argc; ++i){
    if(strcmp(argv[i], "-e") == 0)
      beamEnergy = atof(argv[++i]);
    else{
      help();
      return 0;
    }
  } // end loop over options

  // Check options
  if( beamEnergy < 0 ){
    cout<<"Beam energy is less than 0"<<endl;
    cout<<"Beam Energy = "<<beamEnergy<<endl;
    cout<<"This is unacceptable!!!!!"<<endl;
    return 0;
  }

  // Create analysis object
  Analysis3D* ana = new Analysis3D(beamEnergy);

  // Specify file for output
  TFile* output = new TFile("testout.root","RECREATE");

  // Execute options
  ana->PlotTimeDependence(output);

  // Delete
  delete ana;
  return 0;


}
