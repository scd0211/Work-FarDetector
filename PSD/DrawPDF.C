{

	TFile * ff1 = new TFile("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/ME_beforeN2.root");
	TFile * ff2 = new TFile("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/PDF/FN_beforeN2.root");




	int PMT=0;
	int Bin=100; // for paper
//	int Bin=55; // test
	float charge1=50;
	float charge2=50;


	TH3D * hhPDF1 = (TH3D*)ff1->Get(Form("hPDF%03d",PMT));
	TH3D * hhPDF2 = (TH3D*)ff2->Get(Form("hPDF%03d",PMT));
//	hhPDF1->Scale(1./hhPDF1->Integral(1,1,51,51,1,300));

	//hhPDF1->Draw("colz");

	TH1D * h1 = new TH1D("h1","h1",100,-0.4,1.1);
	TH1D * h2 = new TH1D("h2","h2",100,-0.4,1.1);
	TH1D * h3 = new TH1D("h3","h3",100,-0.4,1.1);
	TH1D * h4 = new TH1D("h4","h4",100,-0.4,1.1);

	TH2D * hPDF_2D_1 = new TH2D("hPDF_2D_1","hPDF_2D_1",248,0,248,100,-0.4,1.1);
	TH2D * hPDF_2D_2 = new TH2D("hPDF_2D_2","hPDF_2D_2",248,0,248,100,-0.4,1.1);

	int Qbin1 = hhPDF1->GetXaxis()->FindBin(charge1);
	int Qbin2 = hhPDF1->GetXaxis()->FindBin(charge2);


	for(int i=0; i<100; i++){

		float yy1 = hhPDF1->GetBinContent(Qbin1,Bin+1,i+1);
		float yy2 = hhPDF2->GetBinContent(Qbin1,Bin+1,i+1);
		float yy3 = hhPDF1->GetBinContent(Qbin2,Bin+1,i+1);
		float yy4 = hhPDF2->GetBinContent(Qbin2,Bin+1,i+1);
//		cout<<i<<" "<<Qbin<<" "<<Bin+1<<" "<<yy<<endl;
		h1->SetBinContent(i+1,yy1);
		h2->SetBinContent(i+1,yy2);
		h3->SetBinContent(i+1,yy3);
		h4->SetBinContent(i+1,yy4);
	}


	for(int i=0; i<248; i++){
		for(int j=0; j<100; j++){
			double content1 = hhPDF1->GetBinContent(Qbin1,i+1,j+1);
			double content2 = hhPDF2->GetBinContent(Qbin1,i+1,j+1);
			//if(i==60) cout<<j<<" "<<hPDF_2D_1->GetYaxis()->GetBinCenter(j+1)<<" "<<content1<<endl;
			//if(i==55) cout<<j<<" "<<hPDF_2D_1->GetYaxis()->GetBinCenter(j+1)<<" "<<content2<<endl;
			//if(i!=50 && j==93 && content1!=0) continue;
			//if(i!=50 && j==93 && content2!=0) continue;
			hPDF_2D_1->SetBinContent(i+1,j+1,content1);
			hPDF_2D_2->SetBinContent(i+1,j+1,content2);
		}
	}


	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetPalette(1);


	TCanvas * c1 = new TCanvas("c1","c1",1400,500);
	c1->Divide(2,1);
	c1->cd(1);
	h1->Scale(1./h1->Integral());
	h2->Scale(1./h2->Integral());

	h1->SetLineWidth(2);
	h2->SetLineWidth(2);
	h1->SetLineColor(1);
	h2->SetLineColor(2);

	h1->Draw("hist");
	h2->Draw("histsame");
	h1->SetTitle("");
	h1->GetXaxis()->SetTitle("PH");
	h1->GetYaxis()->SetTitle("probability");
	h1->GetXaxis()->SetLabelFont(133);
	h1->GetYaxis()->SetLabelFont(133);
	h1->GetXaxis()->SetTitleFont(133);
	h1->GetYaxis()->SetTitleFont(133);
	h1->GetXaxis()->SetLabelSize(18);
	h1->GetYaxis()->SetLabelSize(18);
	h1->GetXaxis()->SetTitleSize(20);
	h1->GetYaxis()->SetTitleSize(20);


	TLegend * leg = new TLegend(0.11,0.7,0.3,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(h1,"ME","l");
	leg->AddEntry(h2,"FN","l");
	leg->Draw();



/*
	TPad * pad1 = new TPad("pad1","pad1",0.45,0.45,0.89,0.89);
 	pad1->Draw();
	pad1->cd();	

	TH1D *hh1 = (TH1D*)h1->Clone();
	TH1D *hh2 = (TH1D*)h2->Clone();
	hh1->Draw("HIST");
	hh2->Draw("HISTsame");

	hh1->GetXaxis()->SetRangeUser(-0.1,0.4);

	hh1->GetXaxis()->SetTitle("");
	hh1->GetYaxis()->SetTitle("");
*/

	c1->cd(2);
	h3->Scale(1./h3->Integral());
	h4->Scale(1./h4->Integral());

	h3->SetLineWidth(2);
	h4->SetLineWidth(2);
	h3->SetLineColor(1);
	h4->SetLineColor(2);

	h3->Draw("hist");
	h4->Draw("histsame");
	h3->SetTitle("");
	h3->GetXaxis()->SetTitle("PH");
	h3->GetYaxis()->SetTitle("probability");
	h3->GetXaxis()->SetLabelFont(133);
	h3->GetYaxis()->SetLabelFont(133);
	h3->GetXaxis()->SetTitleFont(133);
	h3->GetYaxis()->SetTitleFont(133);
	h3->GetXaxis()->SetLabelSize(18);
	h3->GetYaxis()->SetLabelSize(18);
	h3->GetXaxis()->SetTitleSize(20);
	h3->GetYaxis()->SetTitleSize(20);


	leg->Draw();


/*

	TPad * pad2 = new TPad("pad2","pad2",0.45,0.45,0.89,0.89);
 	pad2->Draw();
	pad2->cd();	

	TH1D *hh3 = (TH1D*)h3->Clone();
	TH1D *hh4 = (TH1D*)h4->Clone();
	hh3->Draw("HIST");
	hh4->Draw("HISTsame");

	hh3->GetXaxis()->SetRangeUser(-0.1,0.4);

	hh3->GetXaxis()->SetTitle("");
	hh3->GetYaxis()->SetTitle("");
*/



	c1->Print("typical_PDF.pdf");


	TCanvas * c2 = new TCanvas("c2","c2",1400,500);
	c2->Divide(2,1);
	c2->cd(1);

	hPDF_2D_1->Scale(1./hPDF_2D_1->Integral());
	hPDF_2D_2->Scale(1./hPDF_2D_2->Integral());

	TLatex *lat2 = new TLatex();
	lat2->SetTextAlign(22);
	lat2->SetTextFont(133);
	lat2->SetTextSize(20);
	lat2->SetNDC(1);

	gPad->SetGrid();
	gPad->SetTopMargin(0.15);
	gPad->SetBottomMargin(0.15);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.15);
	hPDF_2D_1->Draw("colz");
	hPDF_2D_1->SetTitle("");
	hPDF_2D_1->GetXaxis()->SetTitle("FADC bin");
	hPDF_2D_1->GetYaxis()->SetTitle("PH");
	hPDF_2D_1->GetZaxis()->SetTitle("probability");
	hPDF_2D_1->GetZaxis()->SetRangeUser(0,0.005);
	hPDF_2D_1->GetZaxis()->SetTitleOffset(1.5);

	lat2->DrawLatex(0.2,0.9,"ME");


	c2->cd(2);
	gPad->SetGrid();
	gPad->SetTopMargin(0.15);
	gPad->SetBottomMargin(0.15);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.15);
	hPDF_2D_2->Draw("colz");
	hPDF_2D_2->SetTitle("");
	hPDF_2D_2->GetXaxis()->SetTitle("FADC bin");
	hPDF_2D_2->GetYaxis()->SetTitle("PH");
	hPDF_2D_2->GetZaxis()->SetTitle("probability");
	hPDF_2D_2->GetZaxis()->SetTitleOffset(1.5);
	hPDF_2D_2->GetZaxis()->SetRangeUser(0,0.005);
	lat2->DrawLatex(0.2,0.9,"FN");

}
