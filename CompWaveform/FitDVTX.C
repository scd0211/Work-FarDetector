{
	TFile * a1 = new TFile("hist/FN_after_N2_woDVTX.root");
	TFile * a2 = new TFile("hist/FN_after_N2_acci.root");

	gStyle->SetOptStat(0);
	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");


	TH1D * h1 = (TH1D*)a1->Get("hDVTX");
	TH1D * h2 = (TH1D*)a2->Get("hDVTX");


	cout<<h1->GetBinWidth(1)<<endl;


	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();

	h1->SetLineWidth(2);
	h2->SetLineWidth(2);
	h1->SetLineColor(1);
	h2->SetLineColor(2);


	h1->Draw("hist");
	h2->Draw("histsame");

	h2->Scale(h1->Integral(41,60)/h2->Integral(41,60));

	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("#DeltaVTX [m]");

	float num1 = h1->Integral(1,10);
	float num2 = h2->Integral(1,10);


	TLegend * leg = new TLegend(0.6,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(h1,"FN candidates","l");
	leg->AddEntry(h2,"accidental","l");
	leg->Draw();

	cout<<num1<<" "<<num2<<endl;
	cout<<num2/num1<<endl;

	

}
