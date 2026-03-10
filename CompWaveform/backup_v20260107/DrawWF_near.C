{

	//TFile * ff1 = new TFile("../backup/v20210705/ME_PDF_R1563Odd.root");
	//TFile * ff2 = new TFile("../backup/v20210705/FN_PDF_R1563Odd.root");

	TFile * ff1 = new TFile("/home/mlf/cdshin/Tape/PSD/root/1D_likelihood/PDF/ME_R1563_with_baseline_cut.root");
	TFile * ff2 = new TFile("/home/mlf/cdshin/Tape/PSD/root/1D_likelihood/PDF/FN_R1563_with_baseline_cut.root");

	TH3D * hhWF1 = (TH3D*)ff1->Get("hWF");
	TH3D * hhWF2 = (TH3D*)ff2->Get("hWF");


	int PMT1=0;

	TH1D * hME = new TH1D("hME","hME",250,0,250);
	TH1D * hFN = new TH1D("hFN","hFN",250,0,250);


	cout<<hhWF1->GetNbinsX()<<endl;
	cout<<hhWF1->GetNbinsY()<<endl;
	cout<<hhWF1->GetNbinsZ()<<endl;


	for(int ipmt=0; ipmt<96; ipmt++){
	for(int iq=0; iq<25; iq++){
		for(int ibin=0; ibin<248; ibin++){
			hME->Fill(ibin, hhWF1->GetBinContent(ipmt+1,iq+1,ibin+1));
			hFN->Fill(ibin, hhWF2->GetBinContent(ipmt+1,iq+1,ibin+1));
		}
	}
	}

	hME->Scale(1./hME->GetMaximum());
	hFN->Scale(1./hFN->GetMaximum());


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetPalette(1);


	TCanvas * c1 = new TCanvas();
	//c1->SetLogy();
	//
	
	c1->SetGridx();
	c1->SetGridy();
	


	hME->Draw("HIST");
	hFN->Draw("HISTsame");
	hME->SetTitle("");
	hME->GetXaxis()->SetTitle("time [ns]");
	//hME->GetYaxis()->SetTitle("arb. units");
	hME->GetYaxis()->SetTitle("PH");
	hME->GetXaxis()->SetLabelFont(133);
	hME->GetYaxis()->SetLabelFont(133);
	hME->GetXaxis()->SetTitleFont(133);
	hME->GetYaxis()->SetTitleFont(133);
	hME->GetXaxis()->SetLabelSize(19);
	hME->GetYaxis()->SetLabelSize(19);
	hME->GetXaxis()->SetTitleSize(21);
	hME->GetYaxis()->SetTitleSize(21);


	//hME->GetYaxis()->SetRangeUser(-0.01,0.01);

	hME->SetLineWidth(2);
	hFN->SetLineWidth(2);

	hME->SetLineColor(1);
	hFN->SetLineColor(2);

	TLegend * leg = new TLegend(0.7,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(hME,"ME","l");
	leg->AddEntry(hFN,"FN","l");
	leg->Draw();

///*
	TPad * pad1 = new TPad("pad1","pad1",0.40,0.25,0.89,0.7);
	pad1->SetLogy();
 	pad1->Draw();
	pad1->cd();	

	pad1->SetLeftMargin(0.15);
	pad1->SetBottomMargin(0.15);

	TH1D *hhME = (TH1D*)hME->Clone();
	TH1D *hhFN = (TH1D*)hFN->Clone();
	hhME->Draw("HIST");
	hhFN->Draw("HISTsame");


	hhME->GetXaxis()->SetNdivisions(705);


	hhME->GetXaxis()->SetTitle("");
	hhME->GetYaxis()->SetTitle("");


	TCanvas * c2 = new TCanvas();
	c2->SetLogy();
	hME->Draw("HIST");
	hFN->Draw("HISTsame");


	TFile * fout = new TFile("WF_near.root","RECREATE");
	hME->Write();
	hFN->Write();
	fout->Close();



}
