{
	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");

	TFile * fPED = new TFile("baseline_cut.root");
	TH1D * hPEDm = (TH1D*)fPED->Get("hPed_m");
	TH1D * hPEDr = (TH1D*)fPED->Get("hPed_r");

	TFile * fPMT1 = new TFile("/home/mlf/cdshin/FarDetector/Analysis/CompWaveform/hist/Baseline/AfterN2_npmt.root");
	TFile * fPMT2 = new TFile("Npmt_hist.root");

	TH1D * hPMT1 = (TH1D*)fPMT1->Get("hNpmt1");
	TH1D * hPMT2 = (TH1D*)fPMT2->Get("hNpmt1");
	TH1D * hPMT3 = (TH1D*)fPMT2->Get("hNpmt2");


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);


	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();
	c0->SetLogy();

	hPMT1->Scale(1./hPMT1->Integral());
	hPMT2->Scale(1./hPMT2->Integral());
	hPMT3->Scale(1./hPMT3->Integral());

	hPMT1->Draw("hist");
	hPMT2->Draw("histsame");
	hPMT3->Draw("histsame");
	hPMT1->SetLineWidth(2);
	hPMT2->SetLineWidth(2);
	hPMT3->SetLineWidth(2);
	hPMT1->SetLineColor(1);
	hPMT2->SetLineColor(2);
	hPMT3->SetLineColor(4);
	hPMT1->SetTitle("");
	hPMT1->GetXaxis()->SetTitle("# of PMTs with abnormal pedestal");
	//hPMT1->SetMaximum(1.1*hPMT2->GetMaximum());


	TLegend * leg0 = new TLegend(0.65,0.65,0.89,0.89);
	leg0->SetFillColor(0);
	leg0->SetBorderSize(0);
	leg0->SetTextFont(132);
	leg0->AddEntry(hPMT1,"muon","l");
	leg0->AddEntry(hPMT2,"ME","l");
	leg0->AddEntry(hPMT3,"FN","l");
	leg0->Draw();

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	hPEDm->Draw("p");
	hPEDm->SetMarkerStyle(20);

	hPEDm->SetTitle("");
	hPEDm->GetXaxis()->SetTitle("PMT ID");
	hPEDm->GetYaxis()->SetTitle("Pedestal mean");
	hPEDm->GetYaxis()->SetRangeUser(13200,13750);
	hPEDm->GetXaxis()->SetTitleFont(132);
	hPEDm->GetXaxis()->SetLabelFont(132);
	hPEDm->GetYaxis()->SetTitleFont(132);
	hPEDm->GetYaxis()->SetLabelFont(132);
	hPEDm->GetYaxis()->SetTitleOffset(1.2);

	TCanvas * c2 = new TCanvas();
	c2->SetGridx();
	c2->SetGridy();

	hPEDr->Draw("p");
	hPEDr->SetMarkerStyle(20);

	hPEDr->SetTitle("");
	hPEDr->GetXaxis()->SetTitle("PMT ID");
	hPEDr->GetYaxis()->SetTitle("#sigma of pedestal");
	hPEDr->GetYaxis()->SetRangeUser(0,2);
	hPEDr->GetXaxis()->SetTitleFont(132);
	hPEDr->GetXaxis()->SetLabelFont(132);
	hPEDr->GetYaxis()->SetTitleFont(132);
	hPEDr->GetYaxis()->SetLabelFont(132);
	hPEDr->GetYaxis()->SetTitleOffset(1.2);



}
