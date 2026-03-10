{
	TFile * a1 = new TFile("hist/ME_before_N2.root");	
	TFile * a2 = new TFile("hist/ME_after_N2.root");	
	TFile * a3 = new TFile("hist/FN_before_N2.root");	
	TFile * a4 = new TFile("hist/FN_after_N2.root");	

	int PMT=2;

	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");


	TH1D * hME1 = (TH1D*)a1->Get(Form("hDelayedQ"));
	TH1D * hME2 = (TH1D*)a2->Get(Form("hDelayedQ"));
	TH1D * hFN1 = (TH1D*)a3->Get(Form("hPromptQ"));
	TH1D * hFN2 = (TH1D*)a4->Get(Form("hPromptQ"));
	TH1D * hGd1 = (TH1D*)a3->Get(Form("hDelayedQ"));
	TH1D * hGd2 = (TH1D*)a4->Get(Form("hDelayedQ"));


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	TCanvas * c0 = new TCanvas("c0","c0",1000,700);
	c0->SetGridx();
	c0->SetGridy();

	hME1->Scale(1./hME1->GetMaximum());	
	hME2->Scale(1./hME2->GetMaximum());	

	hME1->SetLineWidth(2);	
	hME2->SetLineWidth(2);	

	hME1->SetLineColor(1);
	hME2->SetLineColor(2);
	

	hME1->Draw("hist");
	hME2->Draw("histsame");

	hME1->SetTitle("");
	hME1->GetXaxis()->SetTitle("total charge [p.e.]");
	hME1->GetYaxis()->SetTitle("a.u.");

	TLegend * leg = new TLegend(0.7,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(hME1,"before purging","l");
	leg->AddEntry(hME2,"after purging","l");
	leg->Draw();

	TCanvas * c1 = new TCanvas("c1","c1",1000,700);
	c1->SetGridx();
	c1->SetGridy();

	hFN1->Rebin(2);
	hFN2->Rebin(2);

	hFN1->Scale(1./hFN1->GetMaximum());	
	hFN2->Scale(1./hFN2->GetMaximum());	

	hFN1->SetLineWidth(2);	
	hFN2->SetLineWidth(2);	

	hFN1->SetLineColor(1);
	hFN2->SetLineColor(2);
	

	hFN1->Draw("hist");
	hFN2->Draw("histsame");

	hFN1->SetTitle("");
	hFN1->GetXaxis()->SetTitle("total charge [p.e.]");
	hFN1->GetYaxis()->SetTitle("a.u.");
	leg->Draw();

	TCanvas * c2 = new TCanvas("c2","c2",1000,700);
	c2->SetGridx();
	c2->SetGridy();

	hGd1->Scale(1./hGd1->GetMaximum());	
	hGd2->Scale(1./hGd2->GetMaximum());	

	hGd1->SetLineWidth(2);	
	hGd2->SetLineWidth(2);	

	hGd1->SetLineColor(1);
	hGd2->SetLineColor(2);
	

	hGd1->Draw("hist");
	hGd2->Draw("histsame");

	hGd1->SetTitle("");
	hGd1->GetXaxis()->SetTitle("total charge [p.e.]");
	hGd1->GetYaxis()->SetTitle("a.u.");

	leg->Draw();


}
