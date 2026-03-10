#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <stdio.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TLine.h>
#include <iostream>

using namespace std;


void DrawScoreAcci2(int RefBin=5670){


	TFile * fPED = new TFile("baseline_cut.root");
	TH1D * hPEDm = (TH1D*)fPED->Get("hPed_m");
	TH1D * hPEDr = (TH1D*)fPED->Get("hPed_r");

	FILE * fmask = fopen("/home/mlf/cdshin/FarDetector/EventReco/10FADC_20us_develop_differential/dat/PMT_masking_list_v20250702_8FADC.txt","r");

	int MaskTag[172];

	int pmtid, mtag;
	for(int ipmt=0; ipmt<172; ipmt++){
		fscanf(fmask,"%d %d",&pmtid, &mtag);

		if(ipmt!=pmtid){
			cout<<"mismatch masked pmt"<<endl;
			return;
		}
		MaskTag[ipmt]=mtag;
	}



	float PSDscore_1D;
	float Score_1Ds[172];
	double DVTX;
	double DeltaT;
	double pedestal[172];
	double pedestalVar[172];
	double VertX;
	double VertY;
	double VertZ;

	TChain * chain1 = new TChain("tree");
	//chain1->Add("root/SCORE/ME_BeforeN2.root");
	chain1->Add("root/SCORE/ME_AfterN2.root");
	chain1->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain1->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain1->SetBranchAddress("DVTX",&DVTX);
	chain1->SetBranchAddress("DeltaT",&DeltaT);
	chain1->SetBranchAddress("pedestal",pedestal);
	chain1->SetBranchAddress("pedestal_var",pedestalVar);
	chain1->SetBranchAddress("VertX",&VertX);
	chain1->SetBranchAddress("VertY",&VertY);
	chain1->SetBranchAddress("VertZ",&VertZ);

	TChain * chain2 = new TChain("tree");
	//chain2->Add("root/SCORE/FN_BeforeN2.root");
	chain2->Add("root/SCORE/FN_AfterN2.root");
	chain2->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain2->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain2->SetBranchAddress("DVTX",&DVTX);
	chain2->SetBranchAddress("DeltaT",&DeltaT);
	chain2->SetBranchAddress("pedestal",pedestal);
	chain2->SetBranchAddress("pedestal_var",pedestalVar);

	TChain * chain3 = new TChain("tree");
	//chain3->Add("root/SCORE/ME_Acci_BeforeN2.root");
	chain3->Add("root/SCORE/ME_Acci_AfterN2.root");
	chain3->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain3->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain3->SetBranchAddress("DVTX",&DVTX);
	chain3->SetBranchAddress("DeltaT",&DeltaT);
	chain3->SetBranchAddress("pedestal",pedestal);
	chain3->SetBranchAddress("pedestal_var",pedestalVar);
	chain3->SetBranchAddress("VertX",&VertX);
	chain3->SetBranchAddress("VertY",&VertY);
	chain3->SetBranchAddress("VertZ",&VertZ);

	TChain * chain4 = new TChain("tree");
	//chain4->Add("root/SCORE/FN_Acci_BeforeN2.root");
	chain4->Add("root/SCORE/FN_Acci_AfterN2.root");
	chain4->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain4->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain4->SetBranchAddress("DVTX",&DVTX);
	chain4->SetBranchAddress("DeltaT",&DeltaT);
	chain4->SetBranchAddress("pedestal",pedestal);
	chain4->SetBranchAddress("pedestal_var",pedestalVar);


	float xxmin=-3000;
	float xxmax=3000;

	TH1D * h_1Dscore_ME = new TH1D("h_1Dscore_ME","h_1Dscore_ME",10000,xxmin,xxmax);
	TH1D * h_1Dscore_FN = new TH1D("h_1Dscore_FN","h_1Dscore_FN",10000,xxmin,xxmax);
	TH1D * h_1Dscore_ME_acci = new TH1D("h_1Dscore_ME_acci","h_1Dscore_ME_acci",10000,xxmin,xxmax);
	TH1D * h_1Dscore_FN_acci = new TH1D("h_1Dscore_FN_acci","h_1Dscore_FN_acci",10000,xxmin,xxmax);


	float tME=0;
	float tFN=0;


	cout<<chain1->GetEntries()<<endl;
	cout<<chain2->GetEntries()<<endl;
	cout<<chain3->GetEntries()<<endl;
	cout<<chain4->GetEntries()<<endl;


	TH2D * hNpmtPSD_ME = new TH2D("hNpmtPSD_ME","hNpmtPSD_ME",172,-0.5,171.5,10000,xxmin,xxmax);

	TH1D * hNrej_ME = new TH1D("hNrej_ME","hNrej_ME",172,0,172);
	TH1D * hNrej_FN = new TH1D("hNrej_FN","hNrej_FN",172,0,172);



	float R0,Z0;
	float en;

	int count=0;

	float refx;
	float refy;

	float diff;

	int nn1=0;
	int nn2=0;




	//////////////////////////////////////////////////////////////////
	////// Filling of 1D PSD score histogram 
	for(int iEntry=0; iEntry<chain1->GetEntries(); iEntry++){
		chain1->GetEntry(iEntry);

		if(DeltaT<2) continue;

		int n1=0;
		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;
			if(pedestalVar[iPMT]>10) n1++;
		}

		hNrej_ME->Fill(n1);
		hNpmtPSD_ME->Fill(n1,PSDscore_1D);
		h_1Dscore_ME->Fill(PSDscore_1D);

		tME++;

	}

	for(int iEntry=0; iEntry<chain2->GetEntries(); iEntry++){
		chain2->GetEntry(iEntry);


		int n_High_PED_PMT=0;

		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;

			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}

		if( !(n_High_PED_PMT<4) ) continue;


		int n1=0;
		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;
			if(pedestalVar[iPMT]>10) n1++;
		}

		hNrej_FN->Fill(n1);
		h_1Dscore_FN->Fill(PSDscore_1D);


		tFN++;

	}

	///////////////////////////////////////////////////////////////////

	////// Finding reference point for best efficiency

	float Best_ME_Eff=0;
	float Best_FN_Rej=0; 

//	int RefBin=h_1Dscore_ME->FindBin(0.01);


	float ME_Eff;
	float FN_Rej;

	refx=0;
	refy=0;


	for(int iEntry=0; iEntry<chain3->GetEntries(); iEntry++){
		chain3->GetEntry(iEntry);

		if( DeltaT<50 ) continue;

		h_1Dscore_ME_acci->Fill(PSDscore_1D);

	}



	for(int iEntry=0; iEntry<chain4->GetEntries(); iEntry++){
		chain4->GetEntry(iEntry);


		if( DeltaT<500 ) continue;

		int n_High_PED_PMT=0;

		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;

			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}

		if( !(n_High_PED_PMT<4) ) continue;

		h_1Dscore_FN_acci->Fill(PSDscore_1D);

	}

	int sbin=RefBin+1;
	//cout<<sbin<<endl;


	TGraphErrors * gEff_ME = new TGraphErrors();
	


	float val1,val2;
	float frac1;
	float err1;
	float berr_m;
	float frac2;
	float err2;
	float berr_f;
	float frac;
	float err;


	float frac3;
	float err3;

	float nerr_m; 
	float nerr_f; 

	//float acci_m=0.0563229;
	//float acci_f=0.00543157;
	//float acci_e_m=0.000653016;
	//float acci_e_f=0.000142993;


	// after purging
	float acci_m=0.0507312;
	float acci_f=0.00504684;
	float acci_e_m=0.000385159;
	float acci_e_f=0.000167762;

	cout<<"accidental fraction"<<endl;
	cout<<acci_m<<" "<<acci_e_m<<endl;
	cout<<acci_f<<" "<<acci_e_f<<endl;



// survived accidental in the ME by PSD cut 	
	val1 = h_1Dscore_ME_acci->Integral(sbin,h_1Dscore_ME->GetNbinsX());
	val2 = h_1Dscore_ME_acci->Integral();		
	float sur_acci_m = val1/val2;
	float sur_acci_e_m = sqrt(sur_acci_m*(1-sur_acci_m)/val2);

	val1 = h_1Dscore_FN_acci->Integral(1,sbin-1);
	val2 = h_1Dscore_FN_acci->Integral();		
	float rej_acci_f = val1/val2;
	float rej_acci_e_f = sqrt(rej_acci_f*(1-rej_acci_f)/val2);


	float nME1=h_1Dscore_ME->Integral(sbin,h_1Dscore_ME->GetNbinsX());
	float nME2=h_1Dscore_ME->Integral();
	float nAcci_m = nME2*acci_m;
	float nAcci_e_m = nME2*acci_e_m;
	float nSurAcci_m = nME2*acci_m*sur_acci_m;
	float nSurAcci_e_m = nME2 * acci_m * sur_acci_m * sqrt( pow(acci_e_m/acci_m,2) + pow(sur_acci_e_m/sur_acci_m,2) );

	float nFN1=h_1Dscore_FN->Integral(1,sbin-1);
	float nFN2=h_1Dscore_FN->Integral();
	float nAcci_f = nFN2*acci_f;
	float nAcci_e_f = nFN2*acci_e_f;
	float nRejAcci_f = nFN2*acci_f*rej_acci_f;
	float nRejAcci_e_f = nFN2 * acci_f * rej_acci_f * sqrt( pow(acci_e_f/acci_f,2) + pow(rej_acci_e_f/rej_acci_f,2) );

	val1 = nME1 - nSurAcci_m;
	val2 = nME2 - nAcci_m;		
	frac1 = val1/val2;
	berr_m = sqrt(frac1*(1-frac1)/val2);
	nerr_m = sqrt( pow(nSurAcci_e_m/val1,2) + pow(nAcci_e_m/val2,2) )*val1/val2;

	cout<<nSurAcci_m/nAcci_m<<endl;

	val1 = nFN1 - nRejAcci_f;
	val2 = nFN2 - nAcci_f;		
	frac2 = val1/val2;
	berr_f = sqrt(frac2*(1-frac2)/val2);
	nerr_f = sqrt( pow(nRejAcci_e_f/val1,2) + pow(nAcci_e_f/val2,2) )*val1/val2;

	cout<<nRejAcci_f/nAcci_f<<endl;



	// survived accidental in the ME by PSD cut 	

	int endbin=40;
	//int endbin=hNrej_FN->GetNbinsX();
	
	for(int ibin=0; ibin<40; ibin++){
	//for(int ibin=0; ibin<hNpmtPSD_ME->GetNbinsX(); ibin++){
		val1=hNpmtPSD_ME->Integral(ibin+1,ibin+1,sbin,10000);
		val2=hNpmtPSD_ME->Integral(ibin+1,ibin+1,1,10000);
		//cout<<ibin<<" "<<val1<<" "<<val2<<endl;
		frac3=val1/val2;
		err3 = sqrt(frac3*(1-frac3)/val2);

		gEff_ME->SetPoint(ibin,ibin,frac3);
		gEff_ME->SetPointError(ibin,0,err3);

	}

	TGraphErrors * gFrac_ME = new TGraphErrors();
	TGraphErrors * gFrac_FN = new TGraphErrors();

	//for(int ibin=0; ibin<40; ibin++){
	for(int ibin=0; ibin<endbin; ibin++){
		val1=hNrej_FN->GetBinContent(ibin+1);
		val2=hNrej_FN->Integral(1,endbin);
		frac3=val1/val2;
		err3 = sqrt(frac3*(1-frac3)/val2);
		gFrac_FN->SetPoint(ibin,ibin,frac3);
		gFrac_FN->SetPointError(ibin,0,err3);

		val1=hNrej_ME->GetBinContent(ibin+1);
		val2=hNrej_ME->Integral(1,endbin);
		frac3=val1/val2;
		err3 = sqrt(frac3*(1-frac3)/val2);
		gFrac_ME->SetPoint(ibin,ibin,frac3);
		gFrac_ME->SetPointError(ibin,0,err3);

	}


	double x1,y1,ex1,ey1;
	double x2,y2,ex2,ey2;

	double toteff1=0;
	double toteff2=0;
	double toterr1=0;
	double toterr2=0;

	//for(int ipoint=0; ipoint<gEff_ME->GetN(); ipoint++){
	for(int ipoint=0; ipoint<endbin; ipoint++){
		gEff_ME->GetPoint(ipoint,x1,y1);
		ex1=gEff_ME->GetErrorX(ipoint);
		ey1=gEff_ME->GetErrorY(ipoint);

		gFrac_FN->GetPoint(ipoint,x2,y2);
		ex2=gFrac_FN->GetErrorX(ipoint);
		ey2=gFrac_FN->GetErrorY(ipoint);

		if(isnan(y1)) continue;
		if(isnan(y2)) continue;
		if(y2==0) continue;
		//cout<<y1<<" "<<y2<<endl;
		double eff = y1*y2;
		double err = eff*sqrt( pow(ey1/y1,2) + pow(ey2/y2,2));
		//cout<<ipoint<<" "<<eff<<" "<<err<<endl;

		toteff1 += eff;
		toterr1 = sqrt( toterr1*toterr1 + err*err );

	}

	nME1=1.*toteff1;
	nME2=1.;
	nAcci_m = nME2*acci_m;
	nAcci_e_m = nME2*acci_e_m;
	nSurAcci_m = nME2*acci_m*sur_acci_m;
	nSurAcci_e_m = nME2 * acci_m * sur_acci_m * sqrt( pow(acci_e_m/acci_m,2) + pow(sur_acci_e_m/sur_acci_m,2) );

	val1 = nME1 - nSurAcci_m;
	val2 = nME2 - nAcci_m;		
	frac1 = val1/val2;
	berr_m = toterr1;
	nerr_m = sqrt( pow(nSurAcci_e_m/val1,2) + pow(nAcci_e_m/val2,2) )*frac1;

	toteff1 = frac1;
	toterr1 = sqrt( berr_m*berr_m + nerr_m*nerr_m );

	toteff2 = frac2;
	toterr2 = sqrt(berr_f*berr_f+nerr_f*nerr_f);

	cout<<Form("ME efficiency : %.3f +/- %.3f %%",toteff1*100,toterr1*100)<<endl;
	cout<<Form("FN rejection : %.3f +/- %.3f %%",toteff2*100,toterr2*100)<<endl;



	FILE * fout = fopen(Form("acci/scan_bin/PSD_%04d.txt",RefBin),"w");
	fprintf(fout,"%d %f %f %f %f\n",RefBin,toteff1,toterr1,toteff2,toterr2);


	TCanvas * c0 = new TCanvas();
	gEff_ME->Draw("alp");
	gEff_ME->SetTitle("");
	gEff_ME->GetXaxis()->SetTitle("# of PMTs with high pedestal var.");
	gEff_ME->GetYaxis()->SetTitle("ME efficiency at 99% FN rej. [%]");

	TCanvas * c1 = new TCanvas();
	gFrac_FN->Draw("alp");
	gFrac_ME->Draw("lp");

	gFrac_FN->SetTitle("");
	gFrac_FN->GetXaxis()->SetTitle("# of PMTs with high pedestal var.");

	gFrac_ME->SetLineColor(1);
	gFrac_FN->SetLineColor(2);

	gFrac_ME->SetLineWidth(2);
	gFrac_FN->SetLineWidth(2);

	TLegend * leg1 = new TLegend(0.7,0.7,0.89,0.89);
	leg1->SetFillColor(0);
	leg1->SetBorderSize(0);
	leg1->SetTextFont(132);
	leg1->AddEntry(gFrac_ME,"ME","l");
	leg1->AddEntry(gFrac_FN,"FN","l");
	leg1->Draw();
	

}
