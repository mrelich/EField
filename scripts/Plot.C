
#include "myHist.C"
#include <string>
#include <sstream>

// Some constants
TFile* m_file = NULL;

int colors[] = {kBlack, kBlue, kRed, 
		kBlack, kBlue, kRed};

TString freq = "f = 300 MHz";

//---------------------------------------------//
// Main
//---------------------------------------------//
void Plot()
{
  
  m_file = new TFile("../results/AnaOutput.root");

  basic();
  //event();
}

//---------------------------------------------//
// Plot basic
//---------------------------------------------//
void basic()
{

  // Get Canvas
  TCanvas* c = makeCanvas("c");
  c->SetLogy();

  // Get dummy histogram
  float xmin = 25;
  float xmax = 85;
  TH1F* h = makeHist("dummy",1,xmin,xmax,"Observation Angle [deg]",
		     "Rx|E(f,#theta)| [V/MHz]",kBlack,20);
  h->SetMaximum(2e-4);
  h->SetMinimum(1e-11);
  h->Draw();

  // TLegend
  TLegend* leg = makeLegend(0.15,0.3,0.7,0.9);
  leg->SetTextSize(0.035);
  
  // TLatex
  TLatex* lat = makeLatex();
  lat->DrawLatex(0.7,0.88,freq.Data());
  
  // Graphs to plot
  vector<TString> gnames;
  gnames.push_back("g4_100GeV");
  gnames.push_back("g4_1TeV");
  gnames.push_back("g4_10TeV");
  gnames.push_back("ana_100GeV");
  gnames.push_back("ana_1TeV");
  gnames.push_back("ana_10TeV");

  // Line Styles
  int styles[] = {1,1,1,2,2,2};

  // Names for legend
  vector<TString> names;
  names.push_back("G4 1D Approx: 100 GeV");
  names.push_back("G4 1D Approx: 1 TeV");
  names.push_back("G4 1D Approx: 10 TeV");
  names.push_back("Analytic Approx: 100 GeV");
  names.push_back("Analytic Approx: 1 TeV");
  names.push_back("Analytic Approx: 10 TeV");

  // Loop and plot
  for(unsigned int i=0; i<gnames.size(); ++i){
    TString gname = gnames.at(i);
    TString name = names.at(i);
    cout<<"Getting graph: "<<gname<<endl;
    TGraph* gr = getGraph(m_file, gname, colors[i], styles[i]);
    gr->Draw("same");
    leg->AddEntry(gr, name.Data(),"l");
  }

  leg->Draw("same");


}

//---------------------------------------------//
// Plot Event by Event result
//---------------------------------------------//
void event()
{

  // Specify the energies
  vector<string> energies;
  energies.push_back("1TeV");
  energies.push_back("100GeV");
  
  // Specify names for legend
  vector<string> names;
  names.push_back("G4 1D Approx: 1 TeV");
  names.push_back("G4 1D Approx: 100 GeV");

  // Canavas
  TCanvas* c = makeCanvas("c");
  c->SetLogy();
  
  // Dummy histogram
  float xmin = 25;
  float xmax = 85;
  TH1F* h = makeHist("dummy",1,xmin,xmax,"Observation Angle [deg]",
		     "Rx|E(f,#theta)| [V/MHz]",kBlack,20);
  h->SetMaximum(2e-4);
  h->SetMinimum(1e-11);
  h->Draw();
  
  // Legend
  TLegend* leg = makeLegend(0.15,0.3,0.8,0.9);
  leg->SetTextSize(0.035);
  
  // Latex
  TLatex* lat = makeLatex();
  lat->DrawLatex(0.7,0.88,freq.Data());  
  
  for(unsigned int i=0; i<energies.size(); ++i){
    string energy = energies.at(i);
    string name   = names.at(i);
    
    int nGraphs = 0;
    string base = "g4_"+energy+"_event_";
    
    if( energy == "100GeV" )    nGraphs = 100;
    else if( energy == "1TeV" ) nGraphs = 20;
    else return;
    
    // Loop and plot graphs
    TGraph* graph = NULL;
    TString gname = "";
    for(int n=0; n<nGraphs; ++n){
      
      // Make graph name
      stringstream ss;
      ss << base;
      ss << n;
      gname = TString(ss.str().c_str());
      
      // Get Graph and Draw
      graph = getGraph(m_file, gname, colors[i], 1);
      graph->Draw("same");
    }// end loop over graphs

    // Add last one to the Legend
    leg->AddEntry(graph, name.c_str(), "l");

  }// end loop over energies

  leg->Draw("same");

}
