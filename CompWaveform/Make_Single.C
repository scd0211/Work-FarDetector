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


void AlignWaveforms(TChain* chain, Long64_t entry,int evtid, int nsample, int etime)
{
    chain->GetEntry(entry);

    for (int ipmt = 0; ipmt < NPMT; ++ipmt) {

        int mod = FADCmod[ipmt];
        int ch  = FADCch[ipmt];

				//cout<<ipmt<<" "<<mod<<" "<<ch<<endl;
        for (int i = 0; i < 250; ++i) wave_align[ipmt][i] = 0.0;

        for (int i = 0; i < nsample; ++i) {
            int adc = (int)FADC(mod, ch, i);
            wave[ipmt][i] = pedestal[ipmt] - adc;
				//		if(ipmt==0)	cout<<ipmt<<" "<<i<<" "<<pedestal[ipmt]<<" "<<adc<<endl;
        }

				int t_start=etime-75;
				int t_end=etime+175;
				if(t_start<0) t_start=0;
				if(t_end >= nsample) t_end=nsample;

        int peak_index = 0;
        double peak_value = wave[ipmt][t_start];

        for (int i = t_start; i < t_end; ++i) {
            if (wave[ipmt][i] > peak_value) {
                peak_value = wave[ipmt][i];
                peak_index = i;
            }
        }

        int shift = TARGET_POS - peak_index;

        for (int i = 0; i < nsample; ++i) {
            int new_index = i + shift;
            if (0 <= new_index && new_index < NSAMPLEMAX)
                wave_align[ipmt][new_index] = wave[ipmt][i];
        }
    }
}



void Make_Single(int run=422, int subrun=0, const char *outdir="tree/ME")
{


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


	double pmtQ[172];
	double pmtQ1[172];
	double pmtQ2[172];

	TH1D * hWF = new TH1D("hWF","hWF",250,0,250);


	TGraph * gTTT = new TGraph();

	TChain* chain = new TChain("tree");


	chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/FarREC_8FADC_old/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root",run,run,subrun));

	chain->LoadTree(0);
	chain->SetBranchAddress("TriggerID", &TriggerID);
	chain->SetBranchAddress("TTT", TTT);
	chain->SetBranchAddress("Num_Events", &Num_Events);
	chain->SetBranchAddress("PMT_Time", &PMT_Time);
	chain->SetBranchAddress("PMT_Charge", &PMT_Charge);
	chain->SetBranchAddress("pedestal", pedestal);
	chain->SetBranchAddress("pedestal_var", pedestal_var);
	chain->SetBranchAddress("PMT_nPulse", &PMT_nPulse);
	chain->SetBranchAddress("PMT_Pulse_Time", &PMT_Pulse_Time);
	chain->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge);
	chain->SetBranchAddress("EventTime",&EventTime);
	chain->SetBranchAddress("EventTotalCharge",&EventTotalCharge);
	chain->SetBranchAddress("CentVertX", &CentVertX);
	chain->SetBranchAddress("CentVertY", &CentVertY);
	chain->SetBranchAddress("CentVertZ", &CentVertZ);
	chain->SetBranchAddress("CorrVertX", &CorrVertX);
	chain->SetBranchAddress("CorrVertY", &CorrVertY);
	chain->SetBranchAddress("CorrVertZ", &CorrVertZ);
	chain->SetBranchAddress("CorrFlux", &CorrFlux);
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

	int RunNumber;
	int SubRunNumber;
	int TrigID;
	int EventID;

	double totQ;
	double VertX;
	double VertY;
	double VertZ;
	int nPed;

	TFile * fntp = new TFile(Form("%s/pair_r%06d.f%05d.root",outdir,run,subrun),"RECREATE");
	TTree * ntp = new TTree("tree","tree");
	ntp->Branch("RunNumber",&RunNumber,"RunNumber/I");
	ntp->Branch("SubRunNumber",&SubRunNumber,"SubRunNumber/I");
	ntp->Branch("TrigID",&TrigID,"TrigID/I");
	ntp->Branch("EventID",&EventID,"EventID/I");
	ntp->Branch("totQ",&totQ,"totQ/D");
	ntp->Branch("VertX",&VertX,"VertX/D");
	ntp->Branch("VertY",&VertY,"VertY/D");
	ntp->Branch("VertZ",&VertZ,"VertZ/D");
	ntp->Branch("nPed",&nPed,"nPed/I");


	int nsample;

	TChain * chain1 = new TChain("tree");
	chain1->LoadTree(0);
	chain1->Add(Form("/home/mlf/pjs/FAREC/input/R%06d/tree_jsns2_far.r%06d.f%05d.root",run,run,subrun));
	chain1->SetBranchAddress("nsample", &nsample);
	chain1->GetEntry(0);


	NSAM = nsample;
	FADC_vec.resize(NMOD * NCH * NSAM);

	chain1->SetBranchAddress("FADC", FADC_vec.data());


	bool stop=false;


	for (int iEntry = 0; iEntry < nTot; iEntry++) {
		chain->GetEntry(iEntry);

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


		for(int ievt=0; ievt<Num_Events; ievt++){
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

			int n_High_PED_PMT=0;

			for(int iPMT=0; iPMT<172; iPMT++){
				if(iPMT==49) continue;
				if(iPMT==105) continue;
				if(MaskTag[iPMT]==1) continue;

				float PEDcut = hPEDm->GetBinContent(iPMT+1) + 3.5*hPEDr->GetBinContent(iPMT+1);
				if( pedestal[iPMT] > PEDcut ) n_High_PED_PMT++;
			}

			nPed=n_High_PED_PMT;



			if(!veto_tag){
				totQ = EventTotalCharge->at(ievt);
				VertX=v3.X();
				VertY=v3.Y();
				VertZ=v3.Z();
				ntp->Fill();
			}

			}


		}


	fntp->cd();
	ntp->Write();
	fntp->Close();

	return;
}

