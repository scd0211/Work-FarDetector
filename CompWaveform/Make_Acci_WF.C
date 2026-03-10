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


void AlignWaveforms(TChain* chain_0, TChain* chain_1, Long64_t entry, int nsample, int etime)
{
	chain_0->GetEntry(entry);
	chain_1->GetEntry(entry);

	for (int ipmt = 0; ipmt < NPMT; ++ipmt) {

		int mod = FADCmod[ipmt];
		int ch  = FADCch[ipmt];

		//cout<<ipmt<<" "<<mod<<" "<<ch<<endl;
		for (int i = 0; i < 250; ++i) wave_align[ipmt][i] = 0.0;

		for (int i = 0; i < nsample; ++i) {
			int adc = (int)FADC(mod, ch, i);
			wave[ipmt][i] = pedestal[ipmt] - adc;
			//if(ipmt==0)	cout<<ipmt<<" "<<i<<" "<<pedestal[ipmt]<<" "<<adc<<endl;
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



void Make_Acci_WF(int period=1, int run=422, int subrun=0, const char *outdir=".")
{


	double Eprompt1;
	double Eprompt2;
	double Edelayed1;
	double Edelayed2;

	if(period==1){
		Eprompt1=2000;
		Eprompt2=7000;
		Edelayed1=700;
		Edelayed2=1000;
	}
	if(period==2)
		Eprompt1=3000;
		Eprompt2=8000;
		Edelayed1=800;
		Edelayed2=1100;
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
double ped[172];
double pedvar[172];
double pmtQ1[172];
double pmtQ2[172];
double prompt_pmtQ[172];


vector<int>    v_trig;
vector<int>    v_evt;
vector<double> v_time;
vector<double> v_charge;
vector<int>    v_isPrompt;
vector<int>    v_isDelayed;
vector<int>    v_run;
vector<int>    v_subrun;
vector<double>    v_vertx;
vector<double>    v_verty;
vector<double>    v_vertz;

std::vector<std::vector<double>> all_pmtQ;
std::vector<std::vector<double>> all_pedestal;
std::vector<std::vector<double>> all_pedvar;


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
ntp->Branch("PMTCharge",pmtQ1,"PMTCharge[172]/D");
ntp->Branch("CorPMTCharge",pmtQ2,"CorPMTCharge[172]/D");
ntp->Branch("pedestal",pedestal,"pedestal[220]/D");
ntp->Branch("pedestal_var",pedestal_var,"pedestal_var[220]/D");
ntp->Branch("wave_align",wave_align,"wave_align[172][250]/D");


int nsample;

TChain * chain1 = new TChain("tree");
chain1->LoadTree(0);
chain1->Add(Form("/home/mlf/pjs/FAREC/input/R%06d/tree_jsns2_far.r%06d.f%05d.root",run,run,subrun));
chain1->SetBranchAddress("nsample", &nsample);
chain1->GetEntry(0);


NSAM = nsample;
FADC_vec.resize(NMOD * NCH * NSAM);

chain1->SetBranchAddress("FADC", FADC_vec.data());




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
		v1.SetXYZ(CentVertX->at(ievt),CentVertY->at(ievt),CentVertZ->at(ievt));
		v2.SetXYZ(CorrVertX->at(ievt),CorrVertY->at(ievt),CorrVertZ->at(ievt));
		v3.SetXYZ(RecoVertX->at(ievt),RecoVertY->at(ievt),RecoVertZ->at(ievt));

		bool veto_tag=false;

		for (int ipmt = 0; ipmt < inner_pmt; ipmt++) {
			pmtQ[ipmt] = PMT_Charge->at(ipmt + ievt * nPMT);
			ped[ipmt] = pedestal[ipmt];
			pedvar[ipmt] = pedestal_var[ipmt];
		}

		for(int ipulse=0; ipulse<PMT_nPulse->at(172); ipulse++){
			if(PulseCharge[172][ipulse]>400) veto_tag=true;
		}

		for(int ipulse=0; ipulse<PMT_nPulse->at(173); ipulse++){
			if(PulseCharge[173][ipulse]>400) veto_tag=true;
		}


		int isPrompt=0;
		int isDelayed=0;

		double totQ=EventTotalCharge->at(ievt);

		if(!veto_tag && totQ>Eprompt1 && totQ<Eprompt2){
			isPrompt=1;
		}
		if(!veto_tag && totQ>Edelayed1 && totQ<Edelayed2){
			isDelayed=1;
		}

		if(isPrompt==1 || isDelayed==1){
			v_trig.push_back(iEntry);
			v_evt.push_back(ievt);
			v_time.push_back(trigtime+evttime);
			v_charge.push_back(totQ);
			v_run.push_back(run);
			v_subrun.push_back(subrun);
			v_vertx.push_back(v3.X());
			v_verty.push_back(v3.Y());
			v_vertz.push_back(v3.Z());
			v_isPrompt.push_back(isPrompt);
			v_isDelayed.push_back(isPrompt);
			all_pmtQ.emplace_back(pmtQ,pmtQ + NPMT);
			all_pedestal.emplace_back(ped,ped + NPMT);
			all_pmtQ.emplace_back(pedvar,pedvar + NPMT);
		}


//				AlignWaveforms(chain,chain1, TrigID, nsample, ptime);
//				ntp->Fill();
//				FNprompt=false;


	}

}

fntp->cd();
ntp->Write();
fntp->Close();

return;
}

