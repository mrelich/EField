
#include "Analysis3D.h"

using namespace std;

//--------------------------------------------------//
// Constructor
//--------------------------------------------------//
Analysis3D::Analysis3D(double beamEnergy)
{

  // Create Dim3 ana object
  m_dim3 = new Dim3(beamEnergy);
  
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
void Analysis3D::PlotTimeDependence()
{


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
      beamEnergy = atof(argv[i++]);
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

  // Execute options
  ana->PlotTimeDependence();

  // Delete
  delete ana;
  return 0;


}
