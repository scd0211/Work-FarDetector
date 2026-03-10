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



void FitFN()
{

	TFile * fPED = new TFile("baseline_cut.root");

	TH1D * hPEDm = (TH1D*)fPED->Get("hPed_m");
	TH1D * hPEDr = (TH1D*)fPED->Get("hPed_r");

	double DVTX;
	double DeltaT; 
	double pedestal[172];

	float xx[2];
	float xe[2];

	float RecoEnergy;
	TVector3 vec;


	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	chain->Add(Form("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/SCORE/Acci/FN_Acci_AfterN2.root"));
	chain->SetBranchAddress("DVTX", &DVTX);
	chain->SetBranchAddress("DeltaT", &DeltaT);
	chain->SetBranchAddress("pedestal",pedestal);


	TH1D * hDT = new TH1D("hDT","hDT",990,10,1000);

	int Ntot = chain->GetEntries();
	float R0,Z0;

	for(int iEntry=0; iEntry<Ntot; iEntry++){
		chain->GetEntry(iEntry);

		if(iEntry%10000==0) cout<<iEntry<<" / "<<Ntot<<" processed"<<endl;

		if(DeltaT<10) continue;

		int n_High_PED_PMT=0;

		for(int iPMT=0; iPMT<96; iPMT++){
			float PEDcut = hPEDm->GetBinContent(iPMT+1) + 8*hPEDr->GetBinContent(iPMT+1);
			if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
		}
//		if(n_High_PED_PMT>=4) continue; // baseline cut ( # of high pedestal PMT < 2 )


		if(DVTX<0.6) hDT->Fill(DeltaT);

	}

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);


	TCanvas * c1 = new TCanvas();
	c1->SetLogy();

	hDT->Rebin(10);
	hDT->Draw("e1p");

	hDT->SetLineWidth(2);
	hDT->SetLineColor(1);
	hDT->SetMarkerStyle(20);

	hDT->SetTitle("");
	hDT->GetXaxis()->SetTitle("#DeltaT [#mus]");
	hDT->GetXaxis()->SetTitleFont(133);
	hDT->GetYaxis()->SetTitleFont(133);
	hDT->GetXaxis()->SetLabelFont(132);
	hDT->GetYaxis()->SetLabelFont(132);
	hDT->GetXaxis()->SetTitleSize(19);


	TF1 * f1 = new TF1("f1",func_exp1,200,1000,2);
	TF1 * f2 = new TF1("f2",func_exp2,10,1000,4);

	TF1 * f1_1 = new TF1("f1_1",func_exp1,1,1000,2);
	TF1 * f1_2 = new TF1("f1_2",func_exp1,1,1000,2);
	TF1 * f1_3 = new TF1("f1_3",func_exp1,1,1000,2);



	f1->SetParameter(1,1000000);

	hDT->Fit(f1,"R0");

	f2->SetParameter(1,30);
	f2->SetParameter(2,f1->GetParameter(0));;
	f2->SetParameter(3,f1->GetParameter(1));;


	while(1){
		TFitResultPtr r = hDT->Fit(f2, "LR0S");  // "S" option: store result
		if (r.Get() == nullptr || r->Status() != 0 || !r->IsValid()) {
	   std::cout << "Fit failed, not using this result." << std::endl;
		}
		else{
			break;
		}
	}



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

	int bin1 = hDT->FindBin(2+0.0001);
	int bin2 = hDT->FindBin(100-0.0001);


	double livetime = 11945.4;


	double nn = hDT->Integral(bin1,bin2);
	double nn_hz = nn/livetime;

	double nn1 = f1_1->Integral(10,100)/hDT->GetBinWidth(1);
	double ne1 = f1_1->IntegralError(10,100,par_1,covMatS_1)/hDT->GetBinWidth(1);
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
	xx[1] = nn3;
	xe[1] = ne3;

	cout<<"Accidental fraction by DeltaT fitting"<<endl;
	cout<<100*nn3<<" +/- "<<100*ne3<<endl;


	TLatex * lat1 = new TLatex();
	lat1->SetTextAlign(12);
	lat1->SetTextFont(132);
	lat1->SetNDC(1);
	lat1->DrawLatex(0.5,0.5,Form("Acci : %.3f +/- %.3f%%",100*nn3,100*ne3));


//	c1->Print(Form("acci/FN_%s_%02d.png",filename,group_num));

//	FILE * fout = fopen(Form("acci/FN_%s_%02d_fit.txt",filename,group_num),"w");
//	fprintf(fout,"%lf %lf\n",nn3,ne3);



}
