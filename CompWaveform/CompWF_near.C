{
	TFile * a1 = new TFile("WF_near.root");	
	TFile * a2 = new TFile("WF_far.root");	

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


	TH1D * h1 = (TH1D*)a1->Get(Form("hME"));
	TH1D * h2 = (TH1D*)a1->Get(Form("hFN"));
	TH1D * h3 = (TH1D*)a2->Get(Form("hME"));
	TH1D * h4 = (TH1D*)a2->Get(Form("hFN"));

	h1->Scale(1./h1->GetMaximum());	
	h2->Scale(1./h2->GetMaximum());	
	h3->Scale(1./h3->GetMaximum());	
	h4->Scale(1./h4->GetMaximum());	

	h1->SetLineWidth(2);	
	h2->SetLineWidth(2);	
	h3->SetLineWidth(2);	
	h4->SetLineWidth(2);	
	h2->SetLineStyle(1);	
	h4->SetLineStyle(1);	
	h1->SetLineColor(1);
	h2->SetLineColor(2);
	h3->SetLineColor(4);
	h4->SetLineColor(6);


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
	leg->AddEntry(h1,"ME (near)","l");
	leg->AddEntry(h2,"FN (near)","l");
	leg->AddEntry(h3,"ME (far)","l");
	leg->AddEntry(h4,"FN (far)","l");


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

	h1->GetXaxis()->SetRangeUser(40,248);

	leg->Draw();




}
