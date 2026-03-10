double func_exp1(double *x, double *v)
{
	double term_1 = v[0]*exp(-1*x[0]/v[1]);
	return term_1;
}


void FitDT()
{



	gROOT->SetStyle("Plain");
	//gStyle->SetOptStat(0);
	gStyle->SetOptFit(1);
	gStyle->SetPalette(1);
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");
	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");




	TFile * a1 = new TFile("hist/R000454.root");
	TFile * a2 = new TFile("hist/FN_before_N2.root");


	TH1D * h1 = (TH1D*)a1->Get("hDT");
	TH1D * h2 = (TH1D*)a2->Get("hDT");




	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();
	c0->SetLogy();


	h1->SetLineWidth(2);
	h1->SetLineColor(1);

	h1->Draw("hist");
	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("#DeltaT [#musec]");
	h1->GetXaxis()->SetRangeUser(2.5,10);

	TF1 * f1 = new TF1("f1",func_exp1,2.5,10,2);
	f1->SetParameter(1,2.2);
	h1->Fit(f1,"R0");
	f1->Draw("same");
	f1->SetLineColor(2);


	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();
	c1->SetLogy();


	h2->SetLineWidth(2);
	h2->SetLineColor(1);

	h2->Draw("hist");
	h2->SetTitle("");
	h2->GetXaxis()->SetTitle("#DeltaT [#musec]");
	//h2->GetXaxis()->SetRangeUser(2.5,10);

	TF1 * f2 = new TF1("f2",func_exp1,10,100,2);
	f2->SetParameter(1,30);
	h2->Fit(f2,"R0");
	f2->Draw("same");
	f2->SetLineColor(2);



}
