{


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



	TFile * a1 = new TFile("hist/ME_after_N2.root");	
	TFile * a2 = new TFile("hist/FN_after_N2.root");	

	int PMT=2;

	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);


	TCanvas * c1 = new TCanvas();

	TH1D * hWF1[172];
	TH1D * hWF2[172];

	//for(int ipmt=0; ipmt<1; ipmt++){
	for(int ipmt=0; ipmt<172; ipmt++){
		hWF1[ipmt] = (TH1D*)a1->Get(Form("hWF_%03d",ipmt));
		hWF2[ipmt] = (TH1D*)a2->Get(Form("hWF_%03d",ipmt));
	}

	
	TH1D * hME = (TH1D*)hWF1[0]->Clone();
	TH1D * hFN = (TH1D*)hWF2[0]->Clone();

	for(int ipmt=1; ipmt<172; ipmt++){
		if(MaskTag[ipmt]==1) continue;
		if(ipmt==49) continue;
		if(ipmt==105) continue;
		hME->Add(hWF1[ipmt]);
		hFN->Add(hWF2[ipmt]);
	}

	hME->SetLineWidth(2);
	hFN->SetLineWidth(2);

	hME->SetLineColor(1);
	hFN->SetLineColor(2);

	hME->Scale(1./hME->GetMaximum());
	hFN->Scale(1./hFN->GetMaximum());

	hME->Draw("hist");
	hFN->Draw("histsame");

	hME->SetName("hME");
	hFN->SetName("hFN");

	TFile * fout = new TFile("WF_far.root","RECREATE");
	hME->Write();
	hFN->Write();
	fout->Close();

}
