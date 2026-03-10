std::vector<int> *EventTime = nullptr;
std::vector<double> *EventTotalCharge = nullptr;
std::vector<int> *PMT_Time = nullptr;
std::vector<double> *PMT_Charge = nullptr;
std::vector<double> *PMT_Ped = nullptr;
std::vector<double> *PMT_Pedvar = nullptr;
std::vector<int> *PMT_nPulse = nullptr;
std::vector<int> *PMT_Pulse_Time = nullptr;
std::vector<double> *PMT_Pulse_Charge = nullptr;
std::vector<double> *CentVertX = nullptr;
std::vector<double> *CentVertY = nullptr;
std::vector<double> *CentVertZ = nullptr;
std::vector<double> *CorrVertX = nullptr;
std::vector<double> *CorrVertY = nullptr;
std::vector<double> *CorrVertZ = nullptr;
std::vector<double> *CorrFlux = nullptr;
std::vector<double> *RecoVertX = nullptr;
std::vector<double> *RecoVertY = nullptr;
std::vector<double> *RecoVertZ = nullptr;
std::vector<double> *RecoFlux = nullptr;


std::vector<int> PulseTime[220];
std::vector<double> PulseCharge[220];

int Num_Events;
Long64_t TTT[15];
int TriggerID;



const int NMOD      = 15;
const int NCH       = 16;
const int NPMT       = 172;
const int NSAMPLEMAX = 250;
const int TARGET_POS = 50;
double wave[NPMT][10000];
double wave_align[NPMT][250];
int NSAM;
std::vector<uint16_t> FADC_vec;

int    FADCmod[220];
int    FADCch[220];
double pedestal[220];
double pedestal_var[220];

double pedestal1[220];
double pedestal_var1[220];
double pedestal2[220];
double pedestal_var2[220];

int nPMT = 220;
int inner_pmt = 172;

uint16_t& FADC(int mod, int ch, int s)
{
	int idx = (mod * NCH + ch) * NSAM + s;
	return FADC_vec[idx];
}



void MakeME(int run=549, int subrun=0, const char *outdir="./")
{


	const char * filemapping="/gpfs/home/mlf/cdshin/FarDetector/EventReco/Default/dat/MappingTable.txt";
	std::ifstream FADCMapFile;
	FADCMapFile.open(filemapping);

	std::string line;
	while ( std::getline(FADCMapFile, line) ) {
		if (line == "" || line.at(0) == '#') {
			continue;
		}

		std::stringstream channelMap(line);
		int pmtid, BoardNum, ChannelNum;
		channelMap >> pmtid >> BoardNum >> ChannelNum ;
		FADCmod[pmtid]=BoardNum;
		FADCch[pmtid]=ChannelNum;

	}

	int Num_Events;
	Long64_t TTT[15];
	int TriggerID;


	int nPMT = 220;
	int inner_pmt = 172;

	double pmtQ[172];
	double prompt_pmtQ[172];



	TGraph * gTTT = new TGraph();

	TChain* chain = new TChain("tree");


	chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/FarREC_20us_develop_v20251202_diffREC/Reco.tree_jsns2_far.r%06d.f%05d.root",run,subrun));
	chain->LoadTree(0);
	chain->SetBranchAddress("TriggerID", &TriggerID);
	chain->SetBranchAddress("TTT", TTT);
	chain->SetBranchAddress("Num_Events", &Num_Events);
	chain->SetBranchAddress("PMT_Time", &PMT_Time);
	chain->SetBranchAddress("PMT_Charge", &PMT_Charge);
	chain->SetBranchAddress("PMT_nPulse", &PMT_nPulse);
	chain->SetBranchAddress("PMT_Pulse_Time", &PMT_Pulse_Time);
	chain->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge);
	chain->SetBranchAddress("EventTime",&EventTime);
	chain->SetBranchAddress("EventTotalCharge",&EventTotalCharge);
	chain->SetBranchAddress("RecoVertX", &RecoVertX);
	chain->SetBranchAddress("RecoVertY", &RecoVertY);
	chain->SetBranchAddress("RecoVertZ", &RecoVertZ);
	chain->SetBranchAddress("RecoFlux", &RecoFlux);

	int nTot = chain->GetEntries();
	cout << "Total entries : " << nTot << endl;


	double nSpill_no_kicker=0;
	double nME=0;
	double nMuon=0;
	double nnGd=0;

	double nFNp=0;

	int nhit[20];

	TVector3 v1;
	TVector3 v2;
	TVector3 v3;

	TVector3 vPrompt;
	TVector3 vDelayed;
	TVector3 vDVTX;


	double livetime=0;
	double prevtime;
	double trigtime;


	bool FNprompt;
	bool FNdelayed;

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


	double p_time;
	double d_time;
	double sum_trig;

	int RunNumber;
	int SubRunNumber;
	int TrigID;
	int EventID;
	int ptime;

	TFile * fntp = new TFile(Form("%s/pair_r%06d.f%05d.root",outdir,run,subrun),"RECREATE");
	TTree * ntp = new TTree("tree","tree");
	ntp->Branch("RunNumber",&RunNumber,"RunNumber/I");
	ntp->Branch("SubRunNumber",&SubRunNumber,"SubRunNumber/I");
	ntp->Branch("TrigID",&TrigID,"TrigID/I");
	ntp->Branch("EventID",&EventID,"EventID/I");
	ntp->Branch("promptQ",&promptQ,"promptQ/D");
	ntp->Branch("delayedQ",&delayedQ,"delayedQ/D");
	ntp->Branch("time_diff",&time_diff,"time_diff/D");
	ntp->Branch("p_vertx",&p_vertx,"p_vertx/D");
	ntp->Branch("p_verty",&p_verty,"p_verty/D");
	ntp->Branch("p_vertz",&p_vertz,"p_vertz/D");
	ntp->Branch("d_vertx",&d_vertx,"d_vertx/D");
	ntp->Branch("d_verty",&d_verty,"d_verty/D");
	ntp->Branch("d_vertz",&d_vertz,"d_vertz/D");
	ntp->Branch("sum_trig",&sum_trig,"sum_trig/D");
	ntp->Branch("PMTCharge",prompt_pmtQ,"PMTCharge[172]/D");
	ntp->Branch("pedestal",pedestal,"pedestal[220]/D");
	ntp->Branch("pedestal_var",pedestal_var,"pedestal_var[220]/D");
	ntp->Branch("wave_align",wave_align,"wave_align[172][250]/D");


	int nsample;
	uint16_t FADC[15][16][1000];

	TChain * chain1 = new TChain("tree");
	chain1->LoadTree(0);
	chain1->Add(Form("/home/mlf/cdshin/Tape/FarDetector/Tree_for_FAREC/R%06d/tree_jsns2_far.r%06d.f%05d.root",run,run,subrun));
	chain1->SetBranchAddress("nsample", &nsample);
	chain1->SetBranchAddress("FADC", FADC);
	chain1->GetEntry(0);

	TH1D * hWF= new TH1D("hWF","hWF",1000,0,1000);

	for (int iEntry = 0; iEntry < nTot; iEntry++) {
		//if(iEntry!=0) continue;
		chain->GetEntry(iEntry);
		chain1->GetEntry(iEntry);

		float mean_trig=0;
		for(int isample=0; isample<50; isample++){
			mean_trig += (int)FADC[0][0][isample];
		}
		mean_trig = mean_trig/50.;

		sum_trig=0;

		for(int isample=0; isample<nsample; isample++){
			hWF->SetBinContent(isample+1, (int)FADC[0][0][isample]);
			float adc = (int)FADC[0][0][isample];
			sum_trig += mean_trig-adc;
		}

		if(sum_trig>200000) continue;

		int ivec=0;
		for(int ipmt=0; ipmt<220; ipmt++){
			PulseTime[ipmt].clear();
			PulseCharge[ipmt].clear();
			for(int ipulse=0; ipulse<PMT_nPulse->at(ipmt); ipulse++){
				PulseTime[ipmt].push_back(PMT_Pulse_Time->at(ivec));
				PulseCharge[ipmt].push_back(PMT_Pulse_Charge->at(ivec));
				ivec++;
			}
		}

		trigtime=8*TTT[1]/pow(10,3);
		if(iEntry==0){
			prevtime=trigtime;
			FNprompt=false;
			FNdelayed=false;
		}
		else{
			double trig_time_diff = trigtime-prevtime;
			if(trig_time_diff>0){
				livetime+=trig_time_diff;
			}
			else{
				FNprompt=false;
				FNdelayed=false;
			}
			prevtime=trigtime;
		}

		gTTT->SetPoint(iEntry,iEntry,8*TTT[1]/pow(10,6));


		for(int ievt=0; ievt<Num_Events; ievt++){
			int time1=EventTime->at(ievt);
			int evttime=2*EventTime->at(ievt)/1000.;
			double recof = RecoFlux->at(ievt);
			v3.SetXYZ(RecoVertX->at(ievt),RecoVertY->at(ievt),RecoVertZ->at(ievt));

			bool veto_tag=false;

			for (int ipmt = 0; ipmt < inner_pmt; ipmt++) {
				pmtQ[ipmt] = PMT_Charge->at(ipmt + ievt * nPMT);
			}

			for(int ipulse=0; ipulse<PMT_nPulse->at(172); ipulse++){
				if(PulseCharge[172][ipulse]>400) veto_tag=true;
			}

			for(int ipulse=0; ipulse<PMT_nPulse->at(173); ipulse++){
				if(PulseCharge[173][ipulse]>400) veto_tag=true;
			}


			if(!FNprompt && veto_tag && recof>30000){
				p_time = trigtime+evttime;
				//promptQ = EventTotalCharge->at(ievt);
				promptQ = recof;
				p_vertx = v3.X();
				p_verty = v3.Y();
				p_vertz = v3.Z();
				RunNumber=run;
				SubRunNumber=subrun;
				TrigID=iEntry;
				EventID=ievt;
				ptime=time1;

				FNprompt=true;
			}
			else{
				d_time = trigtime+evttime;
				time_diff	= d_time-p_time;

				if(time_diff>10 && !veto_tag &&  recof>30000){
					p_time = trigtime+evttime;
					//promptQ = EventTotalCharge->at(ievt);
					promptQ = recof;
					p_vertx = v3.X();
					p_verty = v3.Y();
					p_vertz = v3.Z();
					RunNumber=run;
					SubRunNumber=subrun;
					TrigID=iEntry;
					EventID=ievt;
					ptime=time1;

				}
				else if(time_diff<10 && !veto_tag && recof>30000 && recof<100000){
					//delayedQ = EventTotalCharge->at(ievt);
					delayedQ = recof;
					d_vertx = v3.X();
					d_verty = v3.Y();
					d_vertz = v3.Z();
					ntp->Fill();
					FNprompt=false;
				}

			}


		}

	}

//	TCanvas * c1 = new TCanvas();
//	hWF->Draw("hist");

	fntp->cd();
	ntp->Write();
	fntp->Close();



	return;
}

