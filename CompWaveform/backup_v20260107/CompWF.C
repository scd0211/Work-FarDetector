{
	TFile * a1 = new TFile("hist/R000423.root");	
	TFile * a2 = new TFile("hist/R000454.root");	

	int PMT=2;

	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");


	TH1D * hQ1 = (TH1D*)a1->Get(Form("hDelayedQ"));
	TH1D * hQ2 = (TH1D*)a2->Get(Form("hDelayedQ"));


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);


	TCanvas * c0 = new TCanvas("c0","c0",1000,700);
	c0->SetGridx();
	c0->SetGridy();

	hQ1->Scale(1./hQ1->GetMaximum());	
	hQ2->Scale(1./hQ2->GetMaximum());	

	hQ1->SetLineWidth(2);	
	hQ2->SetLineWidth(2);	

	hQ1->SetLineColor(1);
	hQ2->SetLineColor(2);
	

	hQ1->Draw("hist");
	hQ2->Draw("histsame");

	hQ1->SetTitle("");
	hQ1->GetXaxis()->SetTitle("total charge [p.e.]");
	hQ1->GetYaxis()->SetTitle("a.u.");

	TLegend * leg = new TLegend(0.7,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(hQ1,"before purging","l");
	leg->AddEntry(hQ2,"after purging","l");
	leg->Draw();


	c0->Print("png/ME_charge.png");

	TLatex * lat = new TLatex();
	lat->SetTextAlign(12);
	lat->SetTextFont(132);
	lat->SetTextSize(0.07);
	lat->SetNDC(1);


	TCanvas * c1 = new TCanvas("c1","c1",2000,700);
	c1->Divide(2,1);

	for(int ipmt=0; ipmt<172; ipmt++){

	TH1D * h1 = (TH1D*)a1->Get(Form("hWF_%03d",ipmt));
	TH1D * h2 = (TH1D*)a2->Get(Form("hWF_%03d",ipmt));

	h1->Scale(1./h1->GetMaximum());	
	h2->Scale(1./h2->GetMaximum());	

	h1->SetLineWidth(2);	
	h2->SetLineWidth(2);	
	h1->SetLineColor(1);
	h2->SetLineColor(2);


	c1->cd(1);
	c1_1->SetGridx();
	c1_1->SetGridy();

	h1->Draw("hist");
	h2->Draw("histsame");

	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("time tick");
	h1->GetYaxis()->SetTitle("a.u.");

	leg->Draw();

	lat->DrawLatex(0.15,0.95,Form("PMT %03d",ipmt));


	c1->cd(2);
	c1_2->SetGridx();
	c1_2->SetGridy();
	c1_2->SetLogy();

	h1->Draw("hist");
	h2->Draw("histsame");

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
