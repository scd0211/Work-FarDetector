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



void FitME()
{

	double DVTX;
	double DeltaT; 


	float xx[2];
	float xe[2];

	TVector3 vec;


	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	chain->Add(Form("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/SCORE/ME_Acci_BeforeN2.root"));
	//chain->Add(Form("/gpfs/home/mlf/cdshin/FarDetector/Analysis/PSD/root/SCORE/Acci/ME_Acci_AfterN2.root"));
	chain->SetBranchAddress("DVTX", &DVTX);
	chain->SetBranchAddress("DeltaT", &DeltaT);


	TH1D * hDT = new TH1D("hDT","hDT",98,2,100);

	int Ntot = chain->GetEntries();

	for(int iEntry=0; iEntry<Ntot; iEntry++){
		chain->GetEntry(iEntry);

		if(iEntry%10000==0) cout<<iEntry<<" / "<<Ntot<<" processed"<<endl;

		//if( !(DVTX<1.3) ) continue;
		//if(DVTX<1.3) 
			
		hDT->Fill(DeltaT);

	}

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);


	TCanvas * c1 = new TCanvas();
	c1->SetLogy();

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


	TF1 * f1 = new TF1("f1",func_exp1,20,100,2);
	TF1 * f2 = new TF1("f2",func_exp2,3,100,4);

	TF1 * f1_1 = new TF1("f1_1",func_exp1,1,100,2);
	TF1 * f1_2 = new TF1("f1_1",func_exp1,1,100,2);
	TF1 * f1_3 = new TF1("f1_1",func_exp1,1,100,2);



	f1->SetParameter(1,1000000);

	hDT->Fit(f1,"R0");

	f1->SetRange(0,100);
	f1->Draw("same");
	f1->SetLineColor(2);

	f2->SetParameter(1,2);
	f2->SetParameter(2,f1->GetParameter(0));;
	f2->SetParameter(3,f1->GetParameter(1));;


//	hDT->Fit(f2,"R0");

	while(1){
		TFitResultPtr r = hDT->Fit(f2, "R0S");  // "S" option: store result
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

	f1->SetRange(1,500);
	f2->SetRange(1,500);

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
	leg1->AddEntry(f1_3,"ME","l");
	leg1->Draw();


	TVirtualFitter * fitter = TVirtualFitter::GetFitter();
	double * covMatrix = fitter->GetCovarianceMatrix();

	double par_1[2] = {f2->GetParameter(2),f2->GetParameter(3)};
  double covMatS_1[4] = {covMatrix[10],covMatrix[11],covMatrix[14],covMatrix[15]};

	int bin1 = hDT->FindBin(2+0.0001);
	int bin2 = hDT->FindBin(10-0.0001);


	double livetime = 11945.4;


	double nn = hDT->Integral(bin1,bin2);
	double nn_hz = nn/livetime;

	double nn1 = f1_1->Integral(2,10)/hDT->GetBinWidth(1);
	double ne1 = f1_1->IntegralError(2,10,par_1,covMatS_1)/hDT->GetBinWidth(1);
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


//	c1->Print(Form("acci/ME_%s_%02d.png",filename,group_num));

//	FILE * fout = fopen(Form("acci/ME_%s_%02d_fit.txt",filename,group_num),"w");
//	fprintf(fout,"%lf %lf\n",nn3,ne3);


}
