#include "GetPDF.C"

void MakeScore_FN(int runno, int subrunno,const char *ME_PDFname, const char *FN_PDFname, const char *indir, const char *outdir)
{

	GetPDF(ME_PDFname, FN_PDFname);

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
	int TrigID;

	float PSDscore_1D;
	float Score_1D[172][250];
	float Score_1Ds[172];

	TChain* chain = new TChain("tree");
	chain->Add(Form("%s/pair_r%06d.f%05d.root",indir,runno,subrunno));
	chain->LoadTree(0);
	chain->SetBranchAddress("TrigID", &TrigID);
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

	double DVTX;
	double recoflux;
	double Correcoflux;
	double VertX;
	double VertY;
	double VertZ;

	gSystem->mkdir(Form("%s",outdir), true);
	TFile * fntp = new TFile(Form("%s/S%05d.root",outdir,subrunno),"RECREATE");
	TTree * ntp = new TTree("tree","tree");
	ntp->Branch("TrigID",&TrigID,"TrigID/I");
	ntp->Branch("DVTX",&DVTX,"DVTX/D");
	ntp->Branch("VertX",&VertX,"VertX/D");
	ntp->Branch("VertY",&VertY,"VertY/D");
	ntp->Branch("VertZ",&VertZ,"VertZ/D");
	ntp->Branch("recoflux",&recoflux,"recoflux/D");
	ntp->Branch("Correcoflux",&Correcoflux,"Correcoflux/D");
	ntp->Branch("DeltaT",&time_diff,"DeltaT/D");
	ntp->Branch("PSDscore_1D",&PSDscore_1D,"PSDscore_1D/F");
	ntp->Branch("RunNum",&runno,"RunNum/I");
	ntp->Branch("SubrunNum",&subrunno,"SubrunNum/I");
	ntp->Branch("PMTCharge",PMTCharge,"PMTCharge[172]/D");
	ntp->Branch("Score_1Ds",Score_1Ds,"Score_1Ds[172]/F");



	//for(int iEntry=0; iEntry<1; iEntry++){
	int ntot=chain->GetEntries();
	for(int iEntry=0; iEntry<ntot; iEntry++){

		if(iEntry%10000==0) cout<<iEntry<<"/"<<ntot<<" events"<<endl;
		chain->GetEntry(iEntry);


		v1.SetXYZ(p_vertx,p_verty,p_vertz);
		v2.SetXYZ(d_vertx,d_verty,d_vertz);

		v3 = v2-v1;

		VertX=d_vertx;
		VertY=d_verty;
		VertZ=d_vertz;
		DVTX=v3.Mag();

		if( !( v2.Perp()<1.0 && fabs(v2.Z())<1.0 ) ) continue;
		if(promptQ<2000) continue;
		if(promptQ>7000) continue;
		if(delayedQ<700) continue;
		if(delayedQ>1000) continue;

		//if(promptQ<3000) continue;
		//if(promptQ>8000) continue;
		//if(delayedQ<800) continue;
		//if(delayedQ>1200) continue;

		if(time_diff<10) continue;
		if(v3.Mag()>0.6) continue;



		recoflux=delayedQ;
		Correcoflux=promptQ;

		PSDscore_1D = 0;

		//cout<<iEntry<<" "<<TrigID<<" "<<time_diff<<" "<<PSDscore_1D<<endl;

		for(int ipmt=0; ipmt<172; ipmt++){
			Score_1Ds[ipmt] = 0;

			for(int isam=0; isam<250; isam++){
				Score_1D[ipmt][isam] = 0;
			}

			if(PMTCharge[ipmt]<20) continue;
			if(PMTCharge[ipmt]>500) continue;
			if(pedestal_var[ipmt]>10) continue;
			if(MaskTag[ipmt]==1) continue;
			for(int isam=0; isam<250; isam++){

				if(isam<40) continue;
				if(isam>200) continue;
				//hWF[ipmt]->Fill(isam,wave_align[ipmt][isam]);
				int nbx = hPDF_ME[ipmt]->GetXaxis()->GetNbins();
				int nbz = hPDF_ME[ipmt]->GetZaxis()->GetNbins();

				double denom = wave_align[ipmt][50];
				if (denom == 0) continue;
				double Q_ratio = wave_align[ipmt][isam] / denom;
				if (!std::isfinite(Q_ratio)) continue;


				int qbin = hPDF_ME[ipmt]->GetXaxis()->FindBin(PMTCharge[ipmt]);
				int ybin = hPDF_ME[ipmt]->GetZaxis()->FindBin(Q_ratio);

				if (qbin < 1 || qbin > nbx) continue;
				if (ybin < 1 || ybin > nbz) continue;
				int qidx = qbin - 1; 

				float P_ME = ScalePDFME[ipmt][qbin-1][isam]*hPDF_ME[ipmt]->GetBinContent(qbin, isam+1, ybin); // probability for the ME
				float P_FN = ScalePDFFN[ipmt][qbin-1][isam]*hPDF_FN[ipmt]->GetBinContent(qbin, isam+1, ybin); // probability for the FN

				if( P_ME>0. && P_FN>0.){
					Score_1D[ipmt][isam] = PMTCharge[ipmt]*( log(P_ME) - log(P_FN) ); // 1D PSD score for each PMT and each FADC timing
					Score_1Ds[ipmt] += Score_1D[ipmt][isam]; 

					PSDscore_1D += PMTCharge[ipmt]*( log(P_ME) - log(P_FN) ); // 1D PSD score
				}

			}

		}
		cout<<iEntry<<" "<<TrigID<<" "<<time_diff<<" "<<PSDscore_1D<<endl;

		ntp->Fill();
		nevt++;
	}



	ntp->Write();

	fntp->Close();




}
