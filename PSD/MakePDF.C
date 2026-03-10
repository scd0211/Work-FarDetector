
void MakePDF(int ptag, int runno, int subrunno,const char *indir, const char *outdir)
{
	double e7=700;
	double e12=1200;
	double e20=2000;
	double e60=7000;

//	double e7=750;
//	double e12=1250;
//	double e20=3000;
//	double e60=8000;

	double wave[172][250]={0};
	double wave_align[172][250]={0};
	double pedestal[220];
	double pedestal_var[220];
	double PMTCharge[172];

	double promptQ;
	double delayedQ;
	double time_diff;
	double p_vertx;
	double p_verty;
	double p_vertz;
	double d_vertx;
	double d_verty;
	double d_vertz;

	double p_vertx1;
	double p_verty1;
	double p_vertz1;
	double d_vertx1;
	double d_verty1;
	double d_vertz1;

	double p_vertx2;
	double p_verty2;
	double p_vertz2;
	double d_vertx2;
	double d_verty2;
	double d_vertz2;

	double p_vertx3;
	double p_verty3;
	double p_vertz3;
	double d_vertx3;
	double d_verty3;
	double d_vertz3;

	double nevt=0;


	TChain* chain = new TChain("tree");
	chain->Add(Form("%s/pair_r%06d.f%05d.root",indir,runno,subrunno));
	chain->LoadTree(0);
	chain->SetBranchAddress("promptQ", &promptQ);
	chain->SetBranchAddress("delayedQ", &delayedQ);
	chain->SetBranchAddress("p_vertx", &p_vertx);
	chain->SetBranchAddress("p_verty", &p_verty);
	chain->SetBranchAddress("p_vertz", &p_vertz);
	chain->SetBranchAddress("d_vertx", &d_vertx);
	chain->SetBranchAddress("d_verty", &d_verty);
	chain->SetBranchAddress("d_vertz", &d_vertz);
	chain->SetBranchAddress("time_diff", &time_diff);
	chain->SetBranchAddress("PMTCharge",PMTCharge);
	chain->SetBranchAddress("pedestal",pedestal);
	chain->SetBranchAddress("pedestal_var",pedestal_var);
	chain->SetBranchAddress("wave_align",wave_align);

	TH3D * hPDF[172]; // PDF for the 1D method

	char hname[255];

	for(int i=0; i<172; i++){
		sprintf(hname,"hPDF%03d",i);
		hPDF[i] = new TH3D(hname,hname,25,0,500,248,0,248,100,-0.4,1.1);
	}

	

	TVector3 v1;
	TVector3 v2;
	TVector3 v3;

	TVector3 v1_1;
	TVector3 v2_1;
	TVector3 v3_1;

	TVector3 v1_2;
	TVector3 v2_2;
	TVector3 v3_2;

	TVector3 v1_3;
	TVector3 v2_3;
	TVector3 v3_3;

	//for(int iEntry=0; iEntry<1; iEntry++){
	int ntot=chain->GetEntries();
	for(int iEntry=0; iEntry<ntot; iEntry++){

		if(iEntry%10000==0) cout<<iEntry<<"/"<<ntot<<" events"<<endl;
		chain->GetEntry(iEntry);

		v1.SetXYZ(p_vertx,p_verty,p_vertz);
		v2.SetXYZ(d_vertx,d_verty,d_vertz);

		v3 = v2-v1;
		if( !( v2.Perp()<1.0 && fabs(v2.Z())<1.0 ) ) continue;

		if(ptag==1){
			if(delayedQ<e20) continue;
			if(delayedQ>e60) continue;
		}

		if(ptag==2){
			if(time_diff<10) continue;
			if(v3.Mag()>0.6) continue;
			if(promptQ<e20) continue;
			if(promptQ>e60) continue;
			if(delayedQ<e7) continue;
			if(delayedQ>e12) continue;
		}

		for(int ipmt=0; ipmt<172; ipmt++){

			if(PMTCharge[ipmt]<20) continue;
			if(PMTCharge[ipmt]>500) continue;
			if(pedestal_var[ipmt]>10) continue;
			for(int isam=0; isam<250; isam++){
				//hWF[ipmt]->Fill(isam,wave_align[ipmt][isam]);
				double Q_ratio = wave_align[ipmt][isam]/wave_align[ipmt][50];
				hPDF[ipmt]->Fill(PMTCharge[ipmt],isam,Q_ratio);
			}

		}

		nevt++;
	}

	gSystem->mkdir(Form("%s",outdir), true);
	TFile * fout = new TFile(Form("%s/S%05d.root",outdir,subrunno),"RECREATE");

	for(int i=0; i<172; i++){
		hPDF[i]->Write();
	}
	fout->Close();




}
