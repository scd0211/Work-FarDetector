void DrawBaseline()
{

	double wave[172][250]={0};
	double wave_align[172][250]={0};
	double pedestal1[220];
	double pedestal2[220];
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
	chain->Add(Form("tree/Baseline/ME_R000454.root"));
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
	chain->SetBranchAddress("pedestal1",pedestal1);
	chain->SetBranchAddress("pedestal2",pedestal2);

	TH1D * hPed1[172];
	TH1D * hPed2[172];
 
	char hname[255];

	for(int ipmt=0; ipmt<172; ipmt++){
		sprintf(hname,"hPed1_%03d",ipmt);
		hPed1[ipmt]= new TH1D(hname,hname,10000,13000,15000);	
		sprintf(hname,"hPed2_%03d",ipmt);
		hPed2[ipmt]= new TH1D(hname,hname,10000,13000,15000);	
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

	int ntot=chain->GetEntries();
	for(int iEntry=0; iEntry<ntot; iEntry++){

		if(iEntry%10000==0) cout<<iEntry<<"/"<<ntot<<" events"<<endl;
		chain->GetEntry(iEntry);

		v1.SetXYZ(p_vertx,p_verty,p_vertz);
		v2.SetXYZ(d_vertx,d_verty,d_vertz);

		v3 = v2-v1;
		if( !( v2.Perp()<1.0 && fabs(v2.Z())<1.0 ) ) continue;

		if(promptQ<10000) continue;
		if(delayedQ<3000) continue;
		if(delayedQ>8000) continue;
		if(time_diff<2) continue;

		for(int ipmt=0; ipmt<172; ipmt++){

			if(PMTCharge[ipmt]==0) continue;
//			if(PMTCharge[ipmt]>500) continue;
//			if(pedestal_var[ipmt]>10) continue;

				hPed1[ipmt]->Fill(pedestal1[ipmt]);
				hPed2[ipmt]->Fill(pedestal2[ipmt]);

		}

		nevt++;
	}


	gSystem->mkdir(Form("hist/Baseline"), true);
	TFile * fout = new TFile(Form("hist/Baseline/R000454.root"),"RECREATE");

	for(int ipmt=0; ipmt<172; ipmt++){
		hPed1[ipmt]->Write();
		hPed2[ipmt]->Write();
	}
	fout->Close();


}
