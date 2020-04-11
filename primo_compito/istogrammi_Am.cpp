//c++ istogrammi_Am.cpp -o istogrammi_Am `root-config --cflags --glibs`

#include <iostream>
#include <fstream>
#include <string>
#include <TApplication.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFitResult.h>
using namespace std;
bool ReadData(vector<int> &values, string file,int val, int * infos){
        ifstream InFile(file.c_str());
        if(InFile.good()==0)
                return false;

        double x[5];
        string line;

        getline(InFile,line);
        getline(InFile,line);
        getline(InFile,line);
        int min=1000;
        int max=0;
        while(true){
                InFile >> x[0] >> x[1] >> x[2]>>  x[3] >> x[4];
                if(x[3]<min){
                    min=x[3];
                }
                if(x[3]>max){
                    max=x[3];
                }
                values.push_back(x[3]);
                //myHisto->Fill(x[3]);
                if(x[0]==(val-1)){
                    break;
                }
                /*if(InFile.eof()==true)
                        break;*/
        }
        infos[0]=min;
        infos[1]=max;
        return true;
}
double convert(double x,double m, double q){
    return m*x+q;
}
/*void computeChi2(TH1D* myHisto, TF1* myFun, double& Chi2, double& NDF, double& pvalue)
{
  double result = 0;
  NDF = 0.;
  for (unsigned int i = 1; i <= myHisto->GetNbinsX(); i++)
    {
      if (myHisto->GetBinContent(i) != 0.)
	{
	  result += pow((myHisto->GetBinContent(i) - myFun->Eval(myHisto->GetBinCenter(i))),2.) / myHisto->GetBinContent(i);
	  NDF++;
	}
    }

  Chi2   = result;
  NDF   -= myFun->GetNpar();
  pvalue = TMath::Prob(Chi2,NDF);
}
double gauss(double * x, double * par){
        double gauss =par[0]*exp(-0.5*pow(((x[0]-par[1])/par[2]),2));
        return gauss;
}
double parabola(double * x, double * par){
        //double parabola = par[3]+par[4]*x[0]+par[5]*pow(x[0],2);
        double parabola = par[0]+par[1]*x[0]+par[2]*pow(x[0],2);
        return parabola;
}
double fun(double * x, double * par){
        return gauss(x,&par[0])+parabola(x,&par[3]);
}*/
int main(int argc, char ** argv){
        //leggo i nomi dei file da leggere e i sample massimi
        float m,m_err,q,q_err;
        ifstream myfile("results_cal.txt");
        myfile >> m >> m_err;
        myfile >> q >> q_err;
        myfile.close();

        ifstream InFile("test2.txt");
        if(InFile.good()==0)
                return false;

        int x,min;
        vector<string> files;
        vector <int> values;
        vector<int> mins;
        vector<string> energies;
        string line,energy;
        while(true){
                InFile >>line>> x>>min>>energy;
                files.push_back(line);
                values.push_back(x);
                mins.push_back(min);
                energies.push_back(energy);
                //myHisto->Fill(x[3]);

                if(InFile.eof()==true)
                        break;
        }

        string file;
        int val;

        for(int i=0;i<values.size();i++){
            if (files.at(i)==("/Users/giorgio/Desktop/particelle/primo_compito/"+string(argv[1]))){
                file=string(argv[1]);
                val=values.at(i);
                min=mins.at(i);
            }
        }
        TApplication * app = new TApplication("myApp",NULL,NULL);
        TCanvas* cnv=new TCanvas("myCanv","myCanv",0,0,1200,800);
        //double nBins=80,xMin=0, xMax=10;

        gStyle->SetOptFit(1111);
        int infos[2];
        vector<int>valori;
        if(!ReadData(valori,file,val,infos)){
            cout << "Errore" <<endl;
        }
        cout << "Estremi"<<endl;
        cout << infos[0]<<endl;
        cout << infos[1]<<endl;
        TH1D * histo= new TH1D("name","Am - Counts(Energy)",160, 3940 ,4100);
        for(int i=0;i<valori.size();i++){
            histo->Fill(valori.at(i));
        }
        cnv->cd();
        histo->Draw("same");
        TF1 * gaussians[4];
        for (int i=0;i<4;i++){
            gaussians[i]=new TF1(("function" + to_string(i)).c_str(),"gaus",3000,4200);
        }
        TF1 * fun = new TF1("fun", "gaus(0)+gaus(3)+gaus(6)+gaus(9)",3000,4200);

        fun->SetParName(0,"Ampl1");
        fun->SetParName(1,"Mean1");
        fun->SetParName(2,"Sigma1");

        fun->SetParName(3,"Ampl2");
        fun->SetParName(4,"Mean2");
        fun->SetParName(5,"Sigma2");

        fun->SetParName(6,"Ampl3");
        fun->SetParName(7,"Mean3");
        fun->SetParName(8,"Sigma3");

        fun->SetParName(9,"Ampl4");
        fun->SetParName(10,"Mean4");
        fun->SetParName(11,"Sigma4");

        fun->SetParameter(0,300);
        fun->SetParLimits(0,1,500);

        fun->SetParameter(1,3963);
        fun->SetParLimits(1,3950,3975);

        fun->SetParameter(2,10);
        fun->SetParLimits(2,0.5,50);


        gaussians[0]->SetParameter(0,300);
        gaussians[0]->SetParLimits(0,1,500);

        gaussians[0]->SetParameter(1,3963);
        gaussians[0]->SetParLimits(1,3950,3975);

        gaussians[0]->SetParameter(2,10);
        gaussians[0]->SetParLimits(2,0.5,50);



        fun->SetParameter(3,1800);
        fun->SetParLimits(3,100,2300);

        fun->SetParameter(4,4002);
        fun->SetParLimits(4,3990,4015);

        fun->SetParameter(5,5);
        fun->SetParLimits(5,0.5,20);


        gaussians[1]->SetParameter(0,1800);
        gaussians[1]->SetParLimits(0,100,2300);

        gaussians[1]->SetParameter(1,4002);
        gaussians[1]->SetParLimits(1,3990,4015);

        gaussians[1]->SetParameter(2,5);
        gaussians[1]->SetParLimits(2,0.5,20);




        fun->SetParameter(6,11000);
        fun->SetParLimits(6,8000,12000);

        fun->SetParameter(7,4036);
        fun->SetParLimits(7,4025,4045);

        fun->SetParameter(8,10);
        fun->SetParLimits(8,1,50);


        gaussians[2]->SetParameter(0,11000);
        gaussians[2]->SetParLimits(0,8000,12000);

        gaussians[2]->SetParameter(1,4036);
        gaussians[2]->SetParLimits(1,4025,4045);

        gaussians[2]->SetParameter(2,10);
        gaussians[2]->SetParLimits(2,1,50);



        fun->SetParameter(9,500);
        fun->SetParLimits(9,10,2000);

        fun->SetParameter(10,4065);
        fun->SetParLimits(10,4045,4085);

        fun->SetParameter(11,5);
        fun->SetParLimits(11,1,50);


        gaussians[3]->SetParameter(0,500);
        gaussians[3]->SetParLimits(0,10,2000);

        gaussians[3]->SetParameter(1,4065);
        gaussians[3]->SetParLimits(1,4045,4085);

        gaussians[3]->SetParameter(2,5);
        gaussians[3]->SetParLimits(2,1,50);


        TFitResultPtr r = histo->Fit("fun","S");

        //histo->Fit("function0+function1+function2+function3");

        for (int i=0;i<4;i++){
            cout <<"Integrale gaussiana "+to_string(i)+"\t"<<gaussians[i]->Integral(0,5000)<<endl;
        }

        double sum_gaussians=0;
        for (int i=0;i<4;i++){
            sum_gaussians+= gaussians[i]->Integral(0,5000);
        }
        for (int i=0;i<4;i++){
            cout <<"BR "+to_string(i)+"\t"<<gaussians[i]->Integral(0,5000)*100/sum_gaussians<<endl;
        }
        // float m=1.31729e+00;
        // float m_err=1.45970e-05;
        // float q=2.13569e-01;
        // float q_err=6.73541e-02;






        float m,m_err,q,q_err;
        ifstream myfile("results_cal.txt");
        myfile >> m >> m_err;
        myfile >> q >> q_err;
        myfile.close();

        float propagazione,s_mx;
        float real_energies[4]={5388,5443,5486,5545};
        for(int i=0;i<4;i++){
            s_mx=abs(m*r->Parameter(3*i+1))*sqrt(pow(m_err/m,2)+pow(r->ParError(3*i+1)/r->Parameter(3*i+1),2));
            propagazione = sqrt(pow(s_mx,2)+pow(q_err,2));
            cout << "Picco americio(Canale):" << "\t" << r->Parameter(3*i+1) <<endl;
            cout << "Energia calcolata(KeV):" << "\t" << (q+r->Parameter(3*i+1)*m) <<endl;
            cout << "Errore picco(unità canale):" << "\t" << r->ParError(3*i+1) <<endl;
            cout << "Errore energia(KeV):"<< "\t" << propagazione<<endl;
            //cout << r->Parameter(3*i+1) << "\t" << (r->Parameter(3*i+1)*m+q) << "\t" <<  r->ParError(3*i+1) << "\t" <<  << endl;
            cout << "Difference(val-val_true):\t"<< (real_energies[i]-(r->Parameter(3*i+1)*m+q)) <<endl;
            cout << "t:\t"<<abs(real_energies[i]-(r->Parameter(3*i+1)*m+q))/(propagazione) <<"\n\n"<<endl;
        }
        cnv->Modified();
        cnv->Update();

        ofstream outfile;
        myfile.open("results_Am.txt");
        for(int i=0;i<4;i++){
            myfile << r->Parameter(3*i+1) <<"\t"<<(q+r->Parameter(3*i+1)*m)<<"\t"<< r->Parameter(3*i+2) <<"\t"<<100<<endl;
        }
        myfile.close();

        outfile.open("risoluzione_am.txt");
        for(int i=0;i<4;i++){
            s_mx=abs(m*r->Parameter(3*i+1))*sqrt(pow(m_err/m,2)+pow(r->ParError(3*i+1)/r->Parameter(3*i+1),2));
            propagazione = sqrt(pow(s_mx,2)+pow(q_err,2));
            outfile << (q+r->Parameter(3*i+1)*m) << "\t" << 2.35*r->Parameter(3*i+2)/r->Parameter(3*i+1) <<"\t"<< propagazione <<"\t"<< \
            2.35*r->Parameter(3*i+2)/r->Parameter(3*i+1)*sqrt(pow((r->ParError(3*i+2)/r->Parameter(3*i+2)),2)+pow((r->ParError(3*i+1)/r->Parameter(3*i+1)),2)) <<endl;
        }
        outfile.close();

        outfile.open("fwhm_am.txt");
        for(int i=0;i<4;i++){
            s_mx=abs(m*r->Parameter(3*i+1))*sqrt(pow(m_err/m,2)+pow(r->ParError(3*i+1)/r->Parameter(3*i+1),2));
            propagazione = sqrt(pow(s_mx,2)+pow(q_err,2));
            outfile << (q+r->Parameter(3*i+1)*m) << "\t" << 2.35*r->Parameter(3*i+2) <<"\t"<< propagazione <<"\t"<< 2.35*r->ParError(3*i+2)<<endl;
        }
        outfile.close();


        //cnv->Print("es.png", "png");
        /*
        cout<< myFun->GetChisquare()/myFun->GetNDF()<<endl;
        cout << "Il chi-quadro di ROOT: "<<myFun->GetChisquare()<<"\nIl ridotto: "<<myFun->GetChisquare()/myFun->GetNDF()<<"Il p-value: "<<myFun->GetProb()<<endl;
        double chi2, ndf, prob;
        computeChi2(histo,myFun,chi2,ndf,prob);
        cout << "Il mio chi-quadro: "<<chi2<<"\nIl ridotto: "<<chi2/ndf<<"Il p-value: "<<prob<<endl;
        TF1* mySignal = new TF1("mySignal", gauss, xMin, xMax, 3);
        mySignal->SetLineColor(kBlack);
        mySignal->SetParameter(0,myFun->GetParameter(0));
        mySignal->SetParameter(1,myFun->GetParameter(1));
        mySignal->SetParameter(2,myFun->GetParameter(2));
        cout << "\nNumber of entries for the signal (i.e. Gaussian): ";
        cout << nBins / (xMax - xMin) * mySignal->Integral(xMin, xMax);
        cout << endl;

        // Compute number of entries for the background (i.e. Parabola)
        TF1* myBackground = new TF1("myBackground", parabola, xMin, xMax, 3);
        myBackground->SetLineColor(kBlue);
        myBackground->SetParameter(0,myFun->GetParameter(3));
        myBackground->SetParameter(1,myFun->GetParameter(4));
        myBackground->SetParameter(2,myFun->GetParameter(5));
        cout << "Number of entries for the background (i.e. Parabola): ";
        cout << nBins / (xMax - xMin) * myBackground->Integral(xMin, xMax)<<endl;
        cout << endl;
        mySignal->Draw("same");
        myBackground->Draw("same");*/

        app->Run();

}
