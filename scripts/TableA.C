
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Table for the vector field as a function of distance //
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//

#include "myHist.C"

void TableA()
{
  
  // Input file
  TFile* file = new TFile("test.root");
  
  // Z positions
  vector<int> zpts;
  zpts.push_back(100);
  zpts.push_back(200);
  zpts.push_back(300);
  zpts.push_back(400);
  zpts.push_back(500);
  zpts.push_back(600);
  zpts.push_back(700);
  zpts.push_back(800);
  zpts.push_back(900);
  zpts.push_back(1000);

  // Calculate R distance
  double angle = (55.8 - 0.3)/57.2957795; // convert to radians

  // string stream
  stringstream ss;

  // Loop 
  TGraph* gr = NULL;
  for(unsigned int i=0; i<zpts.size(); ++i){

    // Get graph maximum
    ss.str("");
    ss << "gr_" << zpts.at(i);
    gr = getGraph(file,TString(ss.str().c_str()),"","",kBlack);
    double maximum = getMax(gr);
		  
    double R = (zpts.at(i)+10) * cos(angle);
    
    cout<<"R = "<<R<<" max: "<<maximum<<" "<<maximum*R<<endl;

  }

}

double getMax(TGraph* gr)
{
  
  int np = gr->GetN();
  double maximum = -999;
  double x=0, y=0;
  for(int p=0; p<np; ++p){
    gr->GetPoint(p,x,y);
    if( y > maximum)
      maximum = y;
  }
  
  return maximum;

}
