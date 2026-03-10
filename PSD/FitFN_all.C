
double nData[400];
double nAcci[400];
int nBins;
int nBins_first;

void fcn2(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, int iflag)
{
	double chi2 = 0;
	double ll = 0;
	for (int i=nBins_first;i<nBins;i++) {
		double nExp = par[0]*nAcci[i];
		if(nData[i] != 0) ll += (nData[i]*log(nExp)-nExp);
		if(nData[i] != 0) chi2 += pow(nData[i]-nExp,2)/(nData[i]);
	}
	//f = -ll;
	f = chi2;
	return;
}// NLL


double func_exp1(double *x, double *v)
{
	double term_1 = v[0]*exp(-1*x[0]/v[1]);
	return term_1;
}

double func_exp2(double *x, double *v)
{
	double term_1 = v[0]*exp(-1*x[0]/v[1]);
	double term_2 = v[2]*exp(-1*x[0]/v[3]);
	return term_1 + term_2;
}

void FitFN_all()
{

	double range = 2.001;


	TFile * fPED = new TFile("baseline_cut.root");

	TH1D * hPEDm = (TH1D*)fPED->Get("hPed_m");
	TH1D * hPEDr = (TH1D*)fPED->Get("hPed_r");

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





	double DVTX;
	double DeltaT; 
	double pedestal[172];




	TVector3 vec;

	float xx[2];
	float xe[2];


	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	//chain->Add(Form("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/SCORE/FN_Acci_BeforeN2.root"));
	chain->Add(Form("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/SCORE/FN_Acci_AfterN2.root"));
	chain->SetBranchAddress("DVTX", &DVTX);
	chain->SetBranchAddress("DeltaT", &DeltaT);
	chain->SetBranchAddress("pedestal",pedestal);





	TH1D * hDR1 = new TH1D("hDR1","hDR1",100,0,5.0);
	TH1D * hDR2 = new TH1D("hDR2","hDR2",100,0,5.0);
	TH1D * hDT = new TH1D("hDT","hDT",990,10,1000);

	int Ntot = chain->GetEntries();


	for(int iEntry=0; iEntry<Ntot; iEntry++){
		chain->GetEntry(iEntry);

		if(iEntry%10000==0) cout<<iEntry<<" / "<<Ntot<<" processed"<<endl;

		if(DeltaT<10) continue;

		int n_High_PED_PMT=0;


		for(int iPMT=0; iPMT<172; iPMT++){
			if(iPMT==49) continue;
			if(iPMT==105) continue;
			if(MaskTag[iPMT]==1) continue;

			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}



		if( !(n_High_PED_PMT<4) ) continue;





		if(DeltaT<100)
			hDR1->Fill(DVTX);

		if(DeltaT>200)
			hDR2->Fill(DVTX);

		if(DVTX<0.6) hDT->Fill(DeltaT);


	}

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	TCanvas * c1 = new TCanvas();

	cout<<hDR2->Integral()<<endl;
	hDR2->Scale(1./hDR2->Integral());
	hDR2->Draw();

	//set Ranges
	nBins = hDR1->GetNbinsX();
	//  cout << "what is NbinsX() ? " << hDR1->GetNbinsX() << endl;

	nBins_first = hDR1->FindBin(range); // Set Minimum range : input value
	//Restering Bin Content each Datas...
	for (int i=0;i<nBins;i++) {
		nData[i] = hDR1->GetBinContent(i+1);
		nAcci[i] = hDR2->GetBinContent(i+1);
		//    cout<< i << " " << nData[i] << " " << nAcci[i] <<endl;
	}



	TMinuit *gMinuit = new TMinuit(1);//#ofpara.
	gMinuit->SetFCN(fcn2);
	Double_t arglist[10];
	Int_t ierflg = 0;
	arglist[0] = 0.5;
	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
	gMinuit->mnparm(0,"Acci",0,0.1,0,10000000,ierflg);

	arglist[0] = 50000;
	// do minimisation with MIGRAD
	gMinuit->mnexcm("MIGRAD", arglist ,1,ierflg);
	Double_t p1,p2,p3;
	Int_t i1,i2,i3;
	gMinuit->mnstat(p1,p2,p3,i1,i2,i3);
	TString chnam;
	double val, err, xlolim,xuplim;
	gMinuit->mnpout(0, chnam, val, err, xlolim, xuplim, ierflg);
	hDR2->Scale(val);

	cout<<val<<endl;

	double acci_val = val;
	double acci_err = err;


	hDR1->Draw("hist");
	hDR2->Draw("histsame");

	hDR1->SetLineWidth(2);
	hDR2->SetLineWidth(2);
	hDR1->SetLineColor(1);
	hDR2->SetLineColor(2);

	int bin1 = hDR1->FindBin(2.0);
	int bin2 = hDR1->FindBin(5.0);

	//hDR2->Scale(hDR1->Integral(bin1,bin2)/hDR2->Integral(bin1,bin2));

	hDR1->SetTitle("");
	hDR1->GetXaxis()->SetTitle("#DeltaVTX [m]");
	hDR1->GetXaxis()->SetTitleFont(133);
	hDR1->GetYaxis()->SetTitleFont(133);
	hDR1->GetXaxis()->SetLabelFont(132);
	hDR1->GetYaxis()->SetLabelFont(132);
	hDR1->GetXaxis()->SetTitleSize(19);

	TLegend * leg = new TLegend(0.6,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(hDR1,"FN candidates","l");
	leg->AddEntry(hDR2,"#DeltaT > 200#mus","l");
	leg->Draw();


	int bin3 = hDR1->FindBin(0.0+0.0001);
	int bin4 = hDR1->FindBin(0.6-0.0001);

	cout<<bin3<<" "<<bin4<<endl;


	double n_DR1 = hDR1->Integral(bin3,bin4);
	double ne_DR1 = sqrt(n_DR1);
	double n_DR2 = hDR2->Integral(bin3,bin4);
	double ne_DR2 = n_DR2*(acci_err/acci_val);

	double dr_frac = n_DR2/n_DR1;
	double dr_frac_err = dr_frac * sqrt( pow(ne_DR1/n_DR1,2) + pow(ne_DR2/n_DR2,2) );

	cout<<"Accidental fraction by dVTX fitting"<<endl;
	cout<<100*dr_frac<<" +/- "<<100*dr_frac_err<<endl;

	xx[0] = dr_frac;
	xe[0] = dr_frac_err;

	TLatex * lat1 = new TLatex();
	lat1->SetTextAlign(12);
	lat1->SetTextFont(132);
	lat1->SetNDC(1);
	lat1->DrawLatex(0.6,0.5,Form("Acci : %.3f +/- %.3f%%",100*dr_frac,100*dr_frac_err));


	TCanvas * c2 = new TCanvas();
	c2->SetLogy();

	float aa2 = hDT->Integral(2,100);

	hDT->Rebin(10);

	hDT->Draw();

	hDT->SetTitle("");
	hDT->GetXaxis()->SetTitle("#DeltaT [#mus]");
	hDT->GetXaxis()->SetTitleFont(133);
	hDT->GetYaxis()->SetTitleFont(133);
	hDT->GetXaxis()->SetLabelFont(132);
	hDT->GetYaxis()->SetLabelFont(132);
	hDT->GetXaxis()->SetTitleSize(19);


	TF1 * f1 = new TF1("f1",func_exp1,200,1000,2);
	TF1 * f2 = new TF1("f2",func_exp2,2,1000,4);

	TF1 * f1_1 = new TF1("f1_1",func_exp1,1,1000,2);
	TF1 * f1_2 = new TF1("f1_2",func_exp1,1,1000,2);
	TF1 * f1_3 = new TF1("f1_3",func_exp1,1,1000,2);


	f1->SetParameter(1,10000);

	hDT->Fit(f1,"R0");

	f2->SetParameter(1,30);
	f2->SetParameter(2,f1->GetParameter(0));;
	f2->SetParameter(3,f1->GetParameter(1));;


	hDT->Fit(f2,"LR0");

	f1->SetParameter(0,f2->GetParameter(2));;
	f1->SetParameter(1,f2->GetParameter(3));;

	f1_1->SetLineColor(3);
	f1_2->SetLineColor(4);
	f1_3->SetLineColor(6);
	f2->SetLineColor(2);

	f1_1->SetParameter(0,f2->GetParameter(2));;
	f1_1->SetParameter(1,f2->GetParameter(3));;
	f1_3->SetParameter(0,f2->GetParameter(0));;
	f1_3->SetParameter(1,f2->GetParameter(1));;


	f1->SetLineColor(4);
	f2->SetLineColor(2);

	f1->SetRange(1,1000);
	f2->SetRange(1,1000);

	f1_1->Draw("same");
	f1_3->Draw("same");
	f2->Draw("same");

	TLegend * leg1 = new TLegend(0.6,0.6,0.89,0.89);
	leg1->SetFillColor(0);
	leg1->SetBorderSize(0);
	leg1->SetTextFont(132);
	leg1->AddEntry(hDT,"Data","l");
	leg1->AddEntry(f2,"Fit (total)","l");
	leg1->AddEntry(f1_1,"Accidental","l");
	leg1->AddEntry(f1_3,"FN","l");
	leg1->Draw();



	TVirtualFitter * fitter = TVirtualFitter::GetFitter();
	double * covMatrix = fitter->GetCovarianceMatrix();

	double par_1[2] = {f2->GetParameter(2),f2->GetParameter(3)};
	double covMatS_1[4] = {covMatrix[10],covMatrix[11],covMatrix[14],covMatrix[15]};

	bin1 = hDT->FindBin(2+0.0001);
	bin2 = hDT->FindBin(100-0.0001);


	double livetime = 11945.4;


	double nn = hDT->Integral(bin1,bin2);
	double nn_hz = nn/livetime;

	double nn1 = f1_1->Integral(1,100)/hDT->GetBinWidth(1);
	double ne1 = f1_1->IntegralError(1,100,par_1,covMatS_1)/hDT->GetBinWidth(1);
	double nn1_hz = nn1/livetime;
	double ne1_hz = ne1/livetime;

	double nn2 = nn-nn1;
	double ne2 = nn + ne1*ne1;
	ne2 = sqrt(ne2);
	double nn2_hz = nn2/livetime;
	double ne2_hz = ne2/livetime;


	double nn3 = nn1/nn;
	double ne3 = nn3*sqrt(1./nn + (ne1*ne1)/(nn1*nn1));
	double nn3_hz = nn3/livetime;
	double ne3_hz = ne3/livetime;

	cout<<100*nn3<<" +/- "<<100*ne3<<endl;

	xx[1] = nn3;
	xe[1] = ne3;


	lat1->DrawLatex(0.6,0.5,Form("Acci : %.3f +/- %.3f%%",100*nn3,100*ne3));




	c1->Print("FN_DR.pdf");
	c2->Print("FN_DT.pdf");

	double wmean;
	double werr;

	double up_sig = 0;
	double down_sig = 0;


	for(int i=0; i<2; i++){
		up_sig += xx[i]/xe[i]/xe[i];
		down_sig += 1/xe[i]/xe[i];
	}

	wmean = up_sig/down_sig;
	werr = 1/down_sig;


	cout<<"======================================================"<<endl;
	cout<<"                                                      "<<endl;
	for(int i=0; i<2; i++) cout<<xx[i]<<" +- "<<xe[i]<<endl;

	cout<<"                                                      "<<endl;
	cout<<"======================================================"<<endl;
	cout<<"weighted xx xeor "<<wmean<<endl;
	cout<<100*wmean<<" +- "<<100*sqrt(werr)<<endl;




}
