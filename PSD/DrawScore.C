{

	TFile * fPED = new TFile("baseline_cut.root");
	//TFile * fPED = new TFile("baseline_cut_BeforeN2.root");


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
	chain1->Add("root/SCORE/ME_AfterN2_backup.root");
	//chain1->Add("root/SCORE/ME_AfterN2.root");
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


	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");

	TH1D * h_1Dscore_ME = new TH1D("h_1Dscore_ME","h_1Dscore_ME",10000,-3000,3000);
	TH1D * h_1Dscore_FN = new TH1D("h_1Dscore_FN","h_1Dscore_FN",10000,-3000,3000);

	TH2D * h_1Dscore_MEs = new TH2D("h_1Dscore_MEs","h_1Dscore_MEs",172,0,172,100,-30,30);
	TH2D * h_1Dscore_FNs = new TH2D("h_1Dscore_FNs","h_1Dscore_FNs",172,0,172,100,-30,30);

	TH2D * hNpmt_PSD = new TH2D("hNpmt_PSD","hNpmt_PSD",172,0,172,10000,-3000,3000);
	TH1D * hNpmt1 = new TH1D("hNpmt1","hNpmt1",120,0,120);
	TH1D * hNpmt2 = new TH1D("hNpmt2","hNpmt2",120,0,120);

	TH1D * h_1Dscore_FN1 = new TH1D("h_1Dscore_FN1","h_1Dscore_FN1",10000,-3000,3000);
	TH1D * h_1Dscore_FN2 = new TH1D("h_1Dscore_FN2","h_1Dscore_FN2",10000,-3000,3000);

	TH2D * hVTX = new TH2D("hVTX","hVTX",100,0,1.5,100,-1.5,1.5);

	for(int iEntry=0; iEntry<chain1->GetEntries(); iEntry++){
		chain1->GetEntry(iEntry);

		TVector3 v1;
		v1.SetXYZ(VertX,VertY,VertZ);
		if(DeltaT<2) continue;

		int n_High_PED_PMT=0;

		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;

			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}

		hNpmt1->Fill(n_High_PED_PMT);	


		//if( !(n_High_PED_PMT<4) ) continue;

		hVTX->Fill(v1.Perp()*v1.Perp(),v1.Z());

		h_1Dscore_ME->Fill(PSDscore_1D);
		for(int ipmt=0; ipmt<172; ipmt++){
			h_1Dscore_MEs->Fill(ipmt,Score_1Ds[ipmt]);
		}
	}

	for(int iEntry=0; iEntry<chain2->GetEntries(); iEntry++){
		chain2->GetEntry(iEntry);
		if(DVTX>0.6) continue;


		int n_High_PED_PMT=0;

		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;

			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}

		hNpmt_PSD->Fill(n_High_PED_PMT,PSDscore_1D);	
		hNpmt2->Fill(n_High_PED_PMT);	


		if(n_High_PED_PMT<4){
			h_1Dscore_FN->Fill(PSDscore_1D);
			h_1Dscore_FN1->Fill(PSDscore_1D);
		}
		else{
			h_1Dscore_FN2->Fill(PSDscore_1D);
		}

		for(int ipmt=0; ipmt<172; ipmt++){
			h_1Dscore_FNs->Fill(ipmt,Score_1Ds[ipmt]);
		}
	}

	double FN_Rej;
	int RefBin;


	//double Thr_FN_Rej=0.972;
	double Thr_FN_Rej=0.99;

		for(int i=0; i<h_1Dscore_ME->GetNbinsX(); i++){
			FN_Rej = h_1Dscore_FN->Integral(1,i+1)/h_1Dscore_FN->Integral();

			if( FN_Rej < Thr_FN_Rej){
				RefBin = i+1;
			}
		}


	float val1,val2;
	float frac1;
	float err1;
	float frac2;
	float err2;
	float frac;
	float err;

	int sbin=RefBin+1;

	cout<<h_1Dscore_FN->GetBinCenter(sbin) - h_1Dscore_FN->GetBinWidth(sbin)<<endl;

	val1 = h_1Dscore_ME->Integral(sbin,h_1Dscore_ME->GetNbinsX());		
	val2 = h_1Dscore_ME->Integral();		
	frac1 = val1/val2;
	err1 = sqrt(frac1*(1-frac1)/val2);

	float nME=val2;
	val1 = h_1Dscore_FN->Integral(1,sbin-1);		
	val2 = h_1Dscore_FN->Integral();		
	frac2 = val1/val2;
	err2 = sqrt(frac2*(1-frac2)/val2);
	float nFN=val2;




	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetPalette(1);

	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();
	
	h_1Dscore_ME->Scale(1./h_1Dscore_ME->Integral());
	h_1Dscore_FN->Scale(1./h_1Dscore_FN->Integral());
	
	h_1Dscore_ME->Rebin(100);
	h_1Dscore_FN->Rebin(100);

	h_1Dscore_ME->SetLineWidth(2);
	h_1Dscore_FN->SetLineWidth(2);

	h_1Dscore_ME->SetLineColor(1);
	h_1Dscore_FN->SetLineColor(2);
	
	h_1Dscore_ME->Draw("hist");
	h_1Dscore_FN->Draw("histsame");

	float yymax;

	if(h_1Dscore_ME->GetBinContent(h_1Dscore_ME->GetMaximumBin()) < h_1Dscore_FN->GetBinContent(h_1Dscore_FN->GetMaximumBin())){
		yymax=1.1*h_1Dscore_FN->GetBinContent(h_1Dscore_FN->GetMaximumBin());
		h_1Dscore_ME->GetYaxis()->SetRangeUser(0,yymax);
	}

	h_1Dscore_ME->SetTitle("");
	h_1Dscore_ME->GetXaxis()->SetTitle("Score");
	h_1Dscore_ME->GetXaxis()->SetTitleFont(132);
	h_1Dscore_ME->GetXaxis()->SetLabelFont(132);
	h_1Dscore_ME->GetYaxis()->SetTitleFont(132);
	h_1Dscore_ME->GetYaxis()->SetLabelFont(132);

	TLegend * leg = new TLegend(0.11,0.7,0.4,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(h_1Dscore_ME,"ME candidates","l");
	leg->AddEntry(h_1Dscore_FN,"FN candidates","l");
	leg->Draw();

	TLatex * lat = new TLatex();
	lat->SetTextFont(132);
	lat->SetTextAlign(12);
	lat->SetTextSize(0.04);
	lat->SetNDC(1);

	cout<<"1D likelihood results"<<endl;
	cout<<"ME : "<<nME<<", FN : "<<nFN<<endl; 
	cout<<Form("ME efficiency : %.2f +/- %.2f %%",frac1*100,err1*100)<<endl;
	cout<<Form("FN rejection : %.2f +/- %.2f %%",frac2*100,err2*100)<<endl;
	lat->DrawLatex(0.60,0.85,Form("ME eff. : %.2f +/- %.2f %%",frac1*100,err1*100));
	lat->DrawLatex(0.60,0.75,Form("FN rej. : %.2f +/- %.2f %%",frac2*100,err2*100));


	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();
	h_1Dscore_MEs->Draw("colz");

	TCanvas * c2 = new TCanvas();
	c2->SetGridx();
	c2->SetGridy();
	h_1Dscore_FNs->Draw("colz");

	TCanvas * c3 = new TCanvas();
	c3->SetGridx();
	c3->SetGridy();
	hNpmt_PSD->Draw("colz");	
	hNpmt_PSD->SetTitle("");
	hNpmt_PSD->GetXaxis()->SetTitle("# of PMTs with abnormal pedestal");
	hNpmt_PSD->GetYaxis()->SetTitle("PSD score");
	hNpmt_PSD->GetXaxis()->SetRangeUser(0,120);

	TCanvas * c41 = new TCanvas();
	c41->SetGridx();
	c41->SetGridy();

	hNpmt1->Scale(1./hNpmt1->Integral());
	hNpmt2->Scale(1./hNpmt2->Integral());

	hNpmt1->Draw("hist");
	hNpmt2->Draw("histsame");
	hNpmt1->SetLineWidth(2);
	hNpmt2->SetLineWidth(2);
	hNpmt1->SetLineColor(1);
	hNpmt2->SetLineColor(2);
	hNpmt1->SetTitle("");
	hNpmt1->GetXaxis()->SetTitle("# of PMTs with abnormal pedestal");
	hNpmt1->GetYaxis()->SetRangeUser(0,1.1*hNpmt2->GetMaximum());

	cout<<"ME"<<endl;
	cout<<hNpmt1->Integral(1,4)<<endl;
	cout<<hNpmt1->Integral()<<endl;
	cout<<hNpmt1->Integral(1,4)/hNpmt1->Integral()<<endl;

	leg->Draw();




	TCanvas * c4 = new TCanvas();
	c4->SetGridx();
	c4->SetGridy();
	hNpmt2->Draw("hist");
//	hNpmt2->SetLineWidth(2);
//	hNpmt2->SetLineColor(1);
	hNpmt2->SetTitle("");
	hNpmt2->GetXaxis()->SetTitle("# of PMTs with abnormal pedestal");


	cout<<"FN"<<endl;
	cout<<hNpmt2->Integral(1,4)<<endl;
	cout<<hNpmt2->Integral()<<endl;
	cout<<hNpmt2->Integral(1,4)/hNpmt2->Integral()<<endl;



	TCanvas * c5 = new TCanvas();
	c5->SetGridx();
	c5->SetGridy();

	h_1Dscore_FN1->Rebin(100);
	h_1Dscore_FN2->Rebin(100);

	h_1Dscore_FN1->Scale(1./h_1Dscore_FN1->Integral());
	h_1Dscore_FN2->Scale(1./h_1Dscore_FN2->Integral());

	h_1Dscore_FN1->SetLineWidth(2);
	h_1Dscore_FN2->SetLineWidth(2);
	h_1Dscore_FN1->SetLineColor(1);
	h_1Dscore_FN2->SetLineColor(2);

	h_1Dscore_FN1->Draw("hist");
	h_1Dscore_FN2->Draw("histsame");

	h_1Dscore_FN1->SetTitle("");
	h_1Dscore_FN1->GetXaxis()->SetTitle("PSD score");

	TLegend * leg5 = new TLegend(0.7,0.7,0.89,0.89);
	leg5->SetFillColor(0);
	leg5->SetBorderSize(0);
	leg5->SetTextFont(132);

	leg5->AddEntry(h_1Dscore_FN1,"passed","l");
	leg5->AddEntry(h_1Dscore_FN2,"rejected","l");
	leg5->Draw();

	TCanvas *c6 = new TCanvas();
	c6->SetGridx();
	c6->SetGridy();

	hVTX->Draw("colz");
	hVTX->SetTitle("");
	hVTX->GetXaxis()->SetTitle("R^{2} [m^{2}]");
	hVTX->GetYaxis()->SetTitle("Z [m]");

	TFile * fout = new TFile("Npmt_hist.root","RECREATE");
	hNpmt1->Write();
	hNpmt2->Write();
	fout->Close();

}
