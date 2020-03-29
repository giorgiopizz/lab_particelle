//c++ istogrammi_mv.cpp -o istogrammi_mv `root-config --cflags --glibs`

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
        ifstream InFile("test.txt");
        if(InFile.good()==0)
                return false;

        int x,min,bin;
        vector<string> files;
        vector <int> values;
        vector<int> mins;
        vector<int> bins;
        vector<string> energies;
        string line,energy;
        while(true){
                InFile >>line>> x>>min >> bin >>energy;
                files.push_back(line);
                values.push_back(x);
                mins.push_back(min);
                bins.push_back(bin);
                energies.push_back(energy);
                //myHisto->Fill(x[3]);

                if(InFile.eof()==true)
                        break;
        }

        string file;
        int val;

        /*for(int i=0;i<values.size();i++){
            if (files.at(i)==("/Users/giorgio/Desktop/particelle/primo_compito/"+string(argv[1]))){
                file=string(argv[1]);
                val=values.at(i);
                min=mins.at(i);
                bin=bins.at(i);
                energy=energies.at(i);
            }
        }*/

        vector<int> valori;
        vector<float> peaks;
        vector<float> errors;
        for (int i=0;i<files.size();i++){
            valori.clear();
            file=files.at(i);
            val=values.at(i);
            min=mins.at(i);
            bin=bins.at(i);
            energy=energies.at(i);
            //TApplication * app = new TApplication("myApp",NULL,NULL);
            TCanvas* cnv=new TCanvas("myCanv","myCanv",0,0,1200,800);
            gStyle->SetOptFit(1111);
            int infos[2];

            if(!ReadData(valori,file,val,infos)){
                cout << "Errore" <<endl;
            }
            cout << "Estremi"<<endl;
            cout << infos[0]<<endl;
            cout << infos[1]<<endl;
            TH1D * histo= new TH1D("name","title",bin, min ,infos[1]);
            for(int j=0;j<valori.size();j++){
                histo->Fill(valori.at(j));
            }
            histo->SetTitle(energy.c_str());
            histo->GetXaxis()->SetTitle("Channels[mV]");
            histo->GetYaxis()->SetTitle("Counts");
            cnv->cd();
            histo->Draw("same");

            TFitResultPtr result = histo->Fit("gaus","S");
            cout << "\n\n\nRisultati per " << file <<endl;
            cout << result->Parameter(1) <<"\t" << result->ParError(1)<<"\n\n\n"<<endl;
            peaks.push_back(result->Parameter(1));
            errors.push_back(result->Parameter(2));
            cnv->Modified();
            cnv->Update();
            cnv->Print(("fit_"+energies.at(i)+".png").c_str(), "png");
            //cnv->Print(("fit_"+energy+".png").c_str(), "png");
            delete cnv;
            delete histo;
        }

        //salvo i picchi
        //struttura del file x y xerr yerr con x = peaks e y = energies

        ofstream myfile;
        myfile.open("results.txt");
        for(int i=0;i<peaks.size();i++){
            myfile << peaks.at(i) <<"\t"<<stof(energies.at(i).substr(0,4))*1000<<"\t"<<errors.at(i)<<"\t"<<100<<endl;
        }
        myfile.close();




        //analisi per Americio
        /*
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
        TH1D * histo= new TH1D("name","title",170, 3930 ,4100);
        for(int i=0;i<valori.size();i++){
            histo->Fill(valori.at(i));
        }
        cnv->cd();
        histo->Draw("same");
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

        fun->SetParameter(3,1800);
        fun->SetParLimits(3,100,2300);

        fun->SetParameter(4,4002);
        fun->SetParLimits(4,3990,4015);

        fun->SetParameter(5,5);
        fun->SetParLimits(5,0.5,20);

        fun->SetParameter(6,11000);
        fun->SetParLimits(6,8000,12000);

        fun->SetParameter(7,4036);
        fun->SetParLimits(7,4025,4045);

        fun->SetParameter(8,10);
        fun->SetParLimits(8,1,50);

        fun->SetParameter(9,500);
        fun->SetParLimits(9,10,2000);

        fun->SetParameter(10,4065);
        fun->SetParLimits(10,4045,4085);

        fun->SetParameter(11,5);
        fun->SetParLimits(11,1,50);


        TFitResultPtr r = histo->Fit("fun","S");
        float m=1.31722e-03;
        float m_err=1.45970e-05;
        float q=2.13569e-01;
        float q_err=6.73541e-02;
        float propagazione,s_mx;
        float real_energies[4]={5388,5443,5486,5545};
        for(int i=0;i<4;i++){
            s_mx=abs(m*r->Parameter(3*i+1))*sqrt(pow(m_err/m,2)+pow(r->ParError(3*i+1)/r->Parameter(3*i+1),2));
            propagazione = sqrt(pow(s_mx,2)+pow(q_err,2));
            cout << r->Parameter(3*i+1) << "\t" << (r->Parameter(3*i+1)*m+q)*1000 << "\t" <<  r->ParError(3*i+1) << "\t" << propagazione*1000 << endl;
            cout << "Errore:\t"<<(real_energies[i]-(r->Parameter(3*i+1)*m+q)*1000)/(propagazione*1000) <<endl;
        }
        cnv->Modified();
        cnv->Update();

        */
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

        //app->Run();

}
