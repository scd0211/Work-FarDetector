void DrawScoreAcci(int RefBin=5670){

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

	TChain * chain3 = new TChain("tree");
	chain3->Add("root/SCORE/Acci/ME_Acci_AfterN2.root");
	chain3->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain3->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain3->SetBranchAddress("DVTX",&DVTX);
	chain3->SetBranchAddress("DeltaT",&DeltaT);
	chain3->SetBranchAddress("pedestal",pedestal);
	chain3->SetBranchAddress("VertX",&VertX);
	chain3->SetBranchAddress("VertY",&VertY);
	chain3->SetBranchAddress("VertZ",&VertZ);

	TChain * chain4 = new TChain("tree");
	chain4->Add("root/SCORE/Acci/FN_Acci_AfterN2.root");
	chain4->SetBranchAddress("PSDscore_1D",&PSDscore_1D);
	chain4->SetBranchAddress("Score_1Ds",Score_1Ds);
	chain4->SetBranchAddress("DVTX",&DVTX);
	chain4->SetBranchAddress("DeltaT",&DeltaT);
	chain4->SetBranchAddress("pedestal",pedestal);


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


	//////////////////////////////////////////////////////////////////
	////// Filling of 1D PSD score histogram 
	for(int iEntry=0; iEntry<chain1->GetEntries(); iEntry++){
		chain1->GetEntry(iEntry);

		if(DeltaT<2) continue;

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



	for(int iEntry=0; iEntry<chain3->GetEntries(); iEntry++){
		chain3->GetEntry(iEntry);
		if( DeltaT<50 ) continue;
		h_1Dscore_ME_acci->Fill(PSDscore_1D);
	}



	for(int iEntry=0; iEntry<chain4->GetEntries(); iEntry++){
		chain4->GetEntry(iEntry);

//		if( !(DVTX<0.6) ) continue;
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

	float toteff1;
	float toteff2;
	float toterr1;
	float toterr2;

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

	toteff1 = frac1;
	toterr1 = sqrt(berr_m*berr_m+nerr_m*nerr_m);

	val1 = nFN1 - nRejAcci_f;
	val2 = nFN2 - nAcci_f;		
	frac2 = val1/val2;
	berr_f = sqrt(frac2*(1-frac2)/val2);
	nerr_f = sqrt( pow(nRejAcci_e_f/val1,2) + pow(nAcci_e_f/val2,2) )*val1/val2;

	cout<<nRejAcci_f/nAcci_f<<endl;
	toteff2 = frac2;
	toterr2 = sqrt(berr_f*berr_f+nerr_f*nerr_f);


	cout<<Form("ME efficiency : %.3f +/- %.3f %%",toteff1*100,toterr1*100)<<endl;
	cout<<Form("FN rejection : %.3f +/- %.3f %%",toteff2*100,toterr2*100)<<endl;



	FILE * fout = fopen(Form("acci/scan_bin/PSD_%04d.txt",RefBin),"w");
	fprintf(fout,"%d %f %f %f %f\n",RefBin,toteff1,toterr1,toteff2,toterr2);


	gStyle->SetOptStat(0);

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	h_1Dscore_ME->Scale(1./h_1Dscore_ME->Integral());
	h_1Dscore_FN->Scale(1./h_1Dscore_FN->Integral());

	h_1Dscore_ME_acci->Scale(1./h_1Dscore_ME_acci->Integral());
	h_1Dscore_FN_acci->Scale(1./h_1Dscore_FN_acci->Integral());

	h_1Dscore_ME->Rebin(100);
	h_1Dscore_FN->Rebin(100);

	h_1Dscore_ME_acci->Rebin(100);
	h_1Dscore_FN_acci->Rebin(100);

	h_1Dscore_ME->SetLineColor(1);
	h_1Dscore_FN->SetLineColor(2);
	h_1Dscore_ME_acci->SetLineColor(4);
	h_1Dscore_FN_acci->SetLineColor(6);

	h_1Dscore_ME->SetLineWidth(2);
	h_1Dscore_FN->SetLineWidth(2);
	h_1Dscore_ME_acci->SetLineWidth(2);
	h_1Dscore_FN_acci->SetLineWidth(2);


	h_1Dscore_ME->Draw("hist");
	h_1Dscore_FN->Draw("histsame");
	h_1Dscore_ME_acci->Draw("histsame");
	h_1Dscore_FN_acci->Draw("histsame");

	h_1Dscore_ME->SetMaximum(1.1*h_1Dscore_FN_acci->GetMaximum());

	h_1Dscore_ME->SetTitle("");
	h_1Dscore_ME->GetXaxis()->SetTitle("PSD score");

	TLegend * leg1 = new TLegend(0.11,0.65,0.5,0.89);
	leg1->SetBorderSize(0);
	leg1->SetFillColor(0);
	leg1->SetTextFont(132);
	leg1->AddEntry(h_1Dscore_ME,"ME","l");
	leg1->AddEntry(h_1Dscore_FN,"FN","l");
	leg1->AddEntry(h_1Dscore_ME_acci,"Acci. (ME)","l");
	leg1->AddEntry(h_1Dscore_FN_acci,"Acci. (FN)","l");
	leg1->Draw();
	

}
