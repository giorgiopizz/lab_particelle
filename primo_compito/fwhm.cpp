/*
c++ -o fwhm fwhm.cpp `root-config --cflags --glibs`
*/

#include <iostream>
#include <fstream>
#include <cmath>

#include "TStyle.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMath.h"

using namespace std;


int main(int argc, char** argv)
{


  TApplication* myApp = new TApplication("myApp", NULL, NULL);


  // ########################################
  // # Choice of the minimization algorithm #
  // ########################################
  // Use numerical minimization algorithm: MIGRAD


  // TGraphErrors constructor from file
  /*myGraph->GetXaxis()->SetTitle("lambda (nm)");
  myGraph->GetYaxis()->SetTitle("Refractive index");
  myGraph->SetMarkerStyle(20);
  myGraph->SetMarkerSize(0.4);

  myC0->cd();
  myGraph->Draw("AP");
  TFitResultPtr r = myGraph->Fit("myFun","S");
  myC0->Modified();
  myC0->Update();*/
/*
  cout << "\nReduced Chi-2: " << myFun->GetChisquare()/myFun->GetNDF() << endl;
  cout << "p-value: " << myFun->GetProb() << endl;

  double Chi2, NDF, pvalue;
  computeChi2(myGraph,myFun,Chi2,NDF,pvalue);
  cout << "\nMy Chi-2: " << Chi2 << endl;
  cout << "My NDF: " << NDF << endl;
  cout << "My reduced Chi-2: " << Chi2/NDF << endl;
  cout << "My p-value: " << pvalue << endl;

  r->Print("V");

*/
  TCanvas* cnv = new TCanvas("myC1","myC1",10,10,1200,800);
  gStyle->SetOptFit(1111);
  //TF1 * func = new TF1("fun", "sqrt(([0]/sqrt(x))^2+[1]^2+([2]*x)^2)",0, 10000);
  TF1 * func = new TF1("fun", "[2]*sqrt([3]*x+[0]*x^2+[1])",0, 10000);
  func->SetParameter(1,0.1684);
  func->SetParameter(0,-3e-10);
  func->SetParameter(2,67);
  func->SetParameter(3,pow(2.35,2)*0.115*3.6e-3);
  //TF1 * func = new TF1("fun", "pol1",0, 10000);
  //da provare questo qui sotto
  //func->FixParameter(0,pow(2.35,2)*0.115*3.6);
  //func->SetParameter(0,pow(2.35,2)*0.115*3.6);
  TGraphErrors*  graph= new TGraphErrors(argv[1]);
  graph->SetTitle("FWHM Americio");
  cnv->cd();
  graph->Draw("AP");
  graph->GetXaxis()->SetTitle("Energies[KeV]");
  graph->GetYaxis()->SetTitle("FWHM");
  TFitResultPtr r = graph->Fit("fun","S");
  float x[4]={3.96971e+03,4.00621e+03,4.03594e+03,4.06157e+03};
  float y_real[4]={5388,5443,5486,5545};
  for(int i=0;i<4;i++){
      cout<< x[i]<<" :\t"<<func->Eval(x[i])<<"\t"<<y_real[i]-func->Eval(x[i])<<endl;
  }


  /*ofstream myfile;
  myfile.open("results_cal.txt");
  myfile <<r->Parameter(0)<< r->ParError(0)<<endl;
  myfile <<r->Parameter(1)<< r->ParError(1)<<endl;
  myfile <<r->Parameter(2)<< r->ParError(2)<<endl;
  myfile.close();*/

  cnv->Modified();
  cnv->Update();
  //cnv->Print("fit_calibrazione_pulser.png", "png");
  myApp->Run();
  return 0;
}
