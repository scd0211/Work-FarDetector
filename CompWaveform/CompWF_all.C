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




	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	TLatex * lat = new TLatex();
	lat->SetTextAlign(12);
	lat->SetTextFont(132);
	lat->SetTextSize(0.07);
	lat->SetNDC(1);



	TCanvas * c1 = new TCanvas("c1","c1",2000,700);
	c1->Divide(2,1);

	//for(int ipmt=0; ipmt<1; ipmt++){
	for(int ipmt=0; ipmt<172; ipmt++){

	TH1D * h1 = (TH1D*)a1->Get(Form("hWF_%03d",ipmt));
	TH1D * h2 = (TH1D*)a2->Get(Form("hWF_%03d",ipmt));
	TH1D * h3 = (TH1D*)a3->Get(Form("hWF_%03d",ipmt));
	TH1D * h4 = (TH1D*)a4->Get(Form("hWF_%03d",ipmt));

	h1->Scale(1./h1->GetMaximum());	
	h2->Scale(1./h2->GetMaximum());	
	h3->Scale(1./h3->GetMaximum());	
	h4->Scale(1./h4->GetMaximum());	

	h1->SetLineWidth(2);	
	h2->SetLineWidth(2);	
	h3->SetLineWidth(2);	
	h4->SetLineWidth(2);	
	h2->SetLineStyle(2);	
	h4->SetLineStyle(2);	
	h1->SetLineColor(1);
	h2->SetLineColor(1);
	h3->SetLineColor(2);
	h4->SetLineColor(2);


	c1->cd(1);
	c1_1->SetGridx();
	c1_1->SetGridy();

	h1->Draw("hist");
	h2->Draw("histsame");
	h3->Draw("histsame");
	h4->Draw("histsame");

	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("time tick");
	h1->GetYaxis()->SetTitle("a.u.");

	TLegend * leg = new TLegend(0.7,0.5,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(h1,"ME (before N2)","l");
	leg->AddEntry(h2,"ME (after N2)","l");
	leg->AddEntry(h3,"FN (before N2)","l");
	leg->AddEntry(h4,"FN (after N2)","l");



	leg->Draw();

	lat->DrawLatex(0.15,0.95,Form("PMT %03d",ipmt));


	c1->cd(2);
	c1_2->SetGridx();
	c1_2->SetGridy();
	c1_2->SetLogy();

	h1->Draw("hist");
	h2->Draw("histsame");
	h3->Draw("histsame");
	h4->Draw("histsame");

	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("time tick");
	h1->GetYaxis()->SetTitle("a.u.");

	leg->Draw();
	c1->Update();
	c1->Print(Form("png/WF_PMT%03d.png",ipmt));

	h1->Delete();
	h2->Delete();

	}


}
