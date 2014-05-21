
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
void Analysis3D::PlotTimeDependence(TFile* outfile,
				    TFile* infile)
{

  // Set the profile
  //string pname = "NPartDiff";
  string pname = "prof";
  
  // Get Rx|A| vs. time for fixed angle
  float angle = (55.8 - 0.3)/Phys::DEG_RAD;
  int np      = 10000;
  float step  = 1e-4; //0.00001;  // ns
  float t_i   = 0;     // initial time
  float time[np];
  float RA[np];
  
  // Get profile
  TProfile* prof = (TProfile*) infile->Get(pname.c_str());
  vector<double> Qz = m_tool->convert(prof);
  double stepSize   = prof->GetBinWidth(1);  // assuming uniform binning  

  // Delete profile and close file
  delete prof;
  
  // Now loop and get info
  for(int i=0; i<np; ++i){

    // Save time and Rx|A|
    time[i] = t_i + step*i; // in ns
    RA[i]   = m_dim3->getRAFF(time[i]*pow(10,-9), // convert to s 
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

//--------------------------------------------------//
// Plot time dependence
//--------------------------------------------------//
void Analysis3D::PlotNearField(TFile* outfile,
			       TFile* infile)
{

  // Set the profile
  //string pname = "NPartDiff";
  string pname = "prof";

  // Get Rx|A| vs. time for fixed angle
  int np      = 600;
  float step  = 0.5;  // ns
  float t_i   = 0;     // initial time
  float time[np];
  float A[np];

  // Specify (r,z) coordinates of our observer. In 
  // Jaime's paper, he sets x,y, and z however y
  // is zero.  This means r = x, phi = 0, and z
  // is just z.
  vector< pair<double,double> > points;
  points.push_back( pair<double,double> (10, 40) );
  points.push_back( pair<double,double> (10, 30) );
  points.push_back( pair<double,double> (10, 20) );
  points.push_back( pair<double,double> (10, 10) );
  points.push_back( pair<double,double> (10, 0) );

  // Get profile
  TProfile* prof    = (TProfile*) infile->Get(pname.c_str());
  vector<double> Qz = m_tool->convert(prof);
  double stepSize   = prof->GetBinWidth(1);  // assuming uniform binning

  // Delete profile and close file
  delete prof;

  // String stream for saving graphs
  stringstream ss;

  // So now we can loop over the points, create a graph,
  // save it, and then delete the graph.
  for(uint p=0; p<points.size(); ++p){

    // Get the locations
    double r = points.at(p).first;
    double z = points.at(p).second;
    
    // Now loop over the times
    for(int i=0; i<np; ++i){
    
      // Get the time
      time[i] = t_i + step * i;
      A[i]    = m_dim3->getANF(time[i]*pow(10,-9),
			       z,
			       r,
			       Qz,
			       stepSize);
      //cout<<"Time is: "<<time[i]<<" with A= "<<A[i]<<endl;
    }// end loop over points

    // Create graph and save
    outfile->cd();
    ss.str("");
    ss << "gr_" << r << "_" << z;
    TGraph* gr = new TGraph(np,time,A);
    gr->SetName(ss.str().c_str());
    gr->Write();
    delete gr;

  }// end loop over points

}

//--------------------------------------------------//
// Plot NF for fixed angle 
//--------------------------------------------------//
void Analysis3D::PlotNearFieldFixedAngle(TFile* outfile,
					 TFile* infile)
{

  // Set the profile
  //string pname = "NPartDiff";
  string pname = "prof";

  // Get Rx|A| vs. time for fixed angle
  int np      = 10000;
  float step  = 1e-3; //0.00001;  // ns
  // 100 PeV settings
  //int np      = 10000;
  //float step  = 0.01;  // ns
  //float t_i   = 0;     // initial time
  float time[np];
  float RA[np];
  double angle = (55.8-0.3)/Phys::DEG_RAD;

  // Specify (r,z) coordinates of our observer. In 
  // Jaime's paper, he sets x,y, and z however y
  // is zero.  This means r = x, phi = 0, and z
  // is just z.
  vector<double> zpoints;  vector<double> t_is;
  zpoints.push_back(1);  t_is.push_back(8);
  zpoints.push_back(2);  t_is.push_back(18);
  zpoints.push_back(3);  t_is.push_back(30);
  zpoints.push_back(4);  t_is.push_back(40);
  zpoints.push_back(5);  t_is.push_back(50);
  zpoints.push_back(6);  t_is.push_back(60);
  zpoints.push_back(7);  t_is.push_back(70);
  zpoints.push_back(8);  t_is.push_back(80);
  zpoints.push_back(9);  t_is.push_back(92);
  zpoints.push_back(10);  t_is.push_back(103);
  zpoints.push_back(20);  t_is.push_back(208);
  zpoints.push_back(30);  t_is.push_back(314);
  zpoints.push_back(40);  t_is.push_back(420);
  zpoints.push_back(50);  t_is.push_back(525);

  /*
  zpoints.push_back(10);   t_is.push_back(50);
  zpoints.push_back(20);   t_is.push_back(200);
  zpoints.push_back(30);   t_is.push_back(300);
  zpoints.push_back(40);   t_is.push_back(410);
  zpoints.push_back(50);   t_is.push_back(510);
  zpoints.push_back(60);   t_is.push_back(620);
  zpoints.push_back(70);   t_is.push_back(725);
  zpoints.push_back(80);   t_is.push_back(830);
  zpoints.push_back(90);   t_is.push_back(935);
  zpoints.push_back(100);   t_is.push_back(1040);
  zpoints.push_back(200);   t_is.push_back(2100);
  zpoints.push_back(300);   t_is.push_back(3150);
  zpoints.push_back(400);   t_is.push_back(4200);
  zpoints.push_back(500);   t_is.push_back(5260);
  zpoints.push_back(600);   t_is.push_back(6320);
  zpoints.push_back(700);   t_is.push_back(7375);
  zpoints.push_back(800);   t_is.push_back(8430);
  zpoints.push_back(900);   t_is.push_back(9480);
  zpoints.push_back(1000);   t_is.push_back(10536);
  zpoints.push_back(2000);   t_is.push_back(21080);
  zpoints.push_back(3000);   t_is.push_back(31620);
  zpoints.push_back(4000);   t_is.push_back(42160);  
  zpoints.push_back(5000);   t_is.push_back(52700);
  zpoints.push_back(6000);   t_is.push_back(63240);
  zpoints.push_back(7000);   t_is.push_back(73785);
  zpoints.push_back(8000);   t_is.push_back(84320);
  zpoints.push_back(9000);   t_is.push_back(94870);
  zpoints.push_back(10000);   t_is.push_back(105410);
  */

  // Get profile
  TProfile* prof    = (TProfile*) infile->Get(pname.c_str());
  vector<double> Qz = m_tool->convert(prof);
  double stepSize   = prof->GetBinWidth(1);  // assuming uniform binning

  // Delete profile and close file
  delete prof;

  // String stream for saving graphs
  stringstream ss;

  // So now we can loop over the points, create a graph,
  // save it, and then delete the graph.
  for(uint p=0; p<zpoints.size(); ++p){

    // Get the locations
    double z   = zpoints.at(p);
    double r   = z * TMath::Tan(angle);
    double R   = TMath::Sqrt(z*z + r*r);
    double t_i = t_is.at(p);
    cout<<"Workign on point: "<<z<<" "<<r<<" "<<R<<endl;
    /*
    if( z < 150 )
      t_i += 100;
    else if( z < 400)
      t_i += 1000;
    else if( z < 500)
      t_i += 1200;
    else if( z < 800)
      t_i += 1000;
    else if( z < 850)
      t_i += 1200;
    else 
      t_i += 1000;
    */
    // Now loop over the times
    for(int i=0; i<np; ++i){

      // Get the time
      time[i] = t_i + step * i;
      RA[i]   = m_dim3->getANF(time[i]*pow(10,-9),
			       z,
			       r,
			       Qz,
			       stepSize);
      //RA[i] *= R;
    }// end loop over points
    
    // Create graph and save
    outfile->cd();
    ss.str("");
    ss << "gr_" << z;
    TGraph* gr = new TGraph(np,time,RA);
    gr->SetName(ss.str().c_str());
    gr->Write();
    delete gr;

  }// end loop over points

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

  double beamEnergy        = -999;
  bool nearField           = false;
  bool farField            = false;
  bool nearFieldFixedAngle = false;
  string inputfile         = "";
  string outputfile        = "";

  // Loop over options (none now)
  for(int i=1; i<argc; ++i){
    if(strcmp(argv[i], "-e") == 0)
      beamEnergy = atof(argv[++i]);
    else if(strcmp(argv[i], "-ff") == 0)
      farField = true;
    else if(strcmp(argv[i], "-nf") == 0)
      nearField = true;
    else if(strcmp(argv[i], "-nffa") == 0)
      nearFieldFixedAngle = true;
    else if(strcmp(argv[i], "-i") == 0)
      inputfile = argv[++i];
    else if(strcmp(argv[i], "-o") == 0)
      outputfile = argv[++i];
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
  if( inputfile.compare("") == 0 ){
    cout<<"No Input file specified"<<endl;
    return 0;
  }
  if( outputfile.compare("") == 0 ){
    cout<<"No Input file specified"<<endl;
    return 0;
  }

  // Create analysis object
  Analysis3D* ana = new Analysis3D(beamEnergy);

  // Specify file for output
  //TFile* output = new TFile("testout.root","RECREATE");
  //TFile* output = new TFile("testout2.root","RECREATE");
  TFile* output = new TFile(outputfile.c_str(),"RECREATE");
  TFile* infile = new TFile(inputfile.c_str());

  // Execute options
  if(farField)  ana->PlotTimeDependence(output,infile);
  if(nearField) ana->PlotNearField(output,infile);
  if(nearFieldFixedAngle) ana->PlotNearFieldFixedAngle(output,infile);
  // close output
  output->Write();
  output->Close();
  delete output;
  infile->Close();
  delete infile;

  // Delete
  delete ana;
  return 0;


}
