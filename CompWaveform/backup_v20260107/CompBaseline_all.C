{
	TFile * a1 = new TFile("hist/Baseline/R000454.root");	

	int PMT=0;

	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");




	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	TLatex * lat = new TLatex();
	lat->SetTextAlign(12);
	lat->SetTextFont(132);
	lat->SetTextSize(0.07);
	lat->SetNDC(1);


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




	TH1D * hPedFrac = new TH1D("hPedFrac","hPedFrac",100,95,100);
	TH1D * hPed_m = new TH1D("hPed_m","hPed_m",172,0,172);
	TH1D * hPed_r = new TH1D("hPed_r","hPed_r",172,0,172);

	TGraphErrors * gr_diff = new TGraphErrors();
	int npoint=0;

	TCanvas * c1 = new TCanvas();

	for(int ipmt=0; ipmt<172; ipmt++){
	//for(int ipmt=0; ipmt<1; ipmt++){

	if(MaskTag[ipmt]==1) continue;
	if(ipmt==49) continue;
	if(ipmt==105) continue;

	TH1D * h1 = (TH1D*)a1->Get(Form("hPed1_%03d",ipmt));
	TH1D * h2 = (TH1D*)a1->Get(Form("hPed2_%03d",ipmt));

//	h1->Scale(1./h1->GetMaximum());	
//	h2->Scale(1./h2->GetMaximum());	

	h1->SetLineWidth(2);	
	h2->SetLineWidth(2);	
	h1->SetLineColor(1);
	h2->SetLineColor(2);


	c1->SetGridx();
	c1->SetGridy();

	h1->Draw("hist");
	h2->Draw("histsame");

	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("pedestal");
//	h1->GetYaxis()->SetTitle("a.u.");

	h1->Rebin(2);
	h2->Rebin(2);

	float center = h1->GetBinCenter(h1->GetMaximumBin());
	float center1 = h2->GetBinCenter(h2->GetMaximumBin());

	gr_diff->SetPoint(npoint,ipmt,center1-center);
	npoint++;

	TF1 * f1 = new TF1("f1","gaus",0,100);

	float rms = h1->GetRMS();
	float xmin = center - rms;
	float xmax = center + rms;

	f1->SetRange(xmin,xmax);

	h1->Fit(f1,"R0");

	f1->SetLineColor(4);

	f1->Draw("same");

	float mean = f1->GetParameter(1);
	float sig = f1->GetParameter(2);

	hPed_m->SetBinContent(ipmt+1,mean);
	hPed_r->SetBinContent(ipmt+1,sig);

	float xbin1 = h1->GetXaxis()->FindBin(mean-3.5*sig);
	float xbin2 = h1->GetXaxis()->FindBin(mean+3.5*sig);

	float nn1 = h1->Integral(xbin1,xbin2);
	float nn2 = h1->Integral();

	cout<<"pedcut  : "<<nn1<<" "<<nn2<<" "<<100*nn1/nn2<<endl;


	hPedFrac->Fill(100*nn1/nn2);

	h1->GetXaxis()->SetRangeUser(center-10*rms,center+20*rms);

	TLegend * leg = new TLegend(0.6,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(h1,"parent muon","l");
	leg->AddEntry(h2,"ME","l");
	leg->Draw();

	lat->DrawLatex(0.15,0.95,Form("PMT %03d",ipmt));

	c1->Update();
	c1->Print(Form("png/Baseline_PMT%03d.png",ipmt));

	h1->Delete();
	h2->Delete();
	f1->Delete();

	}

	TCanvas * c2 = new TCanvas();
	hPedFrac->Draw("hist");
	hPedFrac->SetTitle("");
	hPedFrac->GetXaxis()->SetTitle("fraction of pedestal cut");
	c2->Print("png/Baseline_frac.png");
	cout<<hPedFrac->GetMean()<<endl;

	TCanvas * c3 = new TCanvas();
	c3->SetGridx();
	c3->SetGridy();
	gr_diff->Draw("ap");
	gr_diff->SetTitle("");
	gr_diff->GetXaxis()->SetTitle("PMT ID");
	gr_diff->GetYaxis()->SetTitle("peak diff");
	gr_diff->SetMarkerStyle(20);
	gr_diff->SetMarkerColor(4);
	c3->Print("png/Comp_baseline.png");


	TFile * fout = new TFile("baseline_cut.root","RECREATE");
	hPed_m->Write();
	hPed_r->Write();
	fout->Close();

}
