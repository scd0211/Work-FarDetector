std::vector<int> *EventTime = nullptr;
std::vector<double> *EventTotalCharge = nullptr;
std::vector<int> *PMT_Time = nullptr;
std::vector<double> *PMT_Charge = nullptr;
std::vector<double> *PMT_Ped = nullptr;
std::vector<double> *PMT_Pedvar = nullptr;
std::vector<int> *PMT_nPulse = nullptr;
std::vector<int> *PMT_Pulse_Time = nullptr;
std::vector<double> *PMT_Pulse_Charge = nullptr;
std::vector<double> *RecoVertX = nullptr;
std::vector<double> *RecoVertY = nullptr;
std::vector<double> *RecoVertZ = nullptr;
std::vector<double> *RecoFlux = nullptr;


std::vector<int> PulseTime[220];
std::vector<double> PulseCharge[220];

int Num_Events;
Long64_t TTT[15];
int TriggerID;


int nPMT = 220;
int inner_pmt = 172;


void DrawVetoCharge()
{


	const char * filemapping="/gpfs/home/mlf/cdshin/FarDetector/EventReco/Default/dat/MappingTable.txt";
	std::ifstream FADCMapFile;
	FADCMapFile.open(filemapping);

	int Num_Events;
	Long64_t TTT[15];
	int TriggerID;


	int nPMT = 220;
	int inner_pmt = 172;

	double pmtQ[172];
	double prompt_pmtQ[172];



	TGraph * gTTT = new TGraph();

	TChain* chain = new TChain("tree");

	int run=549;
	//int run=684;

	chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/10FADC_differential_trigger/R%06d/Reco.tree_jsns2_far.r%06d.f*.root",run,run));
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

	TH1D * hTopVeto = new TH1D("hTopVeto","hTopVeto",100,0,2000);
	TH1D * hBottomVeto = new TH1D("hBottomVeto","hBottomVeto",100,0,2000);

	TH1D * hVetoTime1 = new TH1D("hVetoTime1","hVetoTime1",110,0,11000);
	TH1D * hVetoTime2 = new TH1D("hVetoTime2","hVetoTime2",110,0,11000);

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

			for(int ipulse=0; ipulse<PMT_nPulse->at(172); ipulse++){
				hVetoTime1->Fill(PulseTime[172][ipulse]);
				//if(PulseTime[172][ipulse]<5000) continue;
				hTopVeto->Fill(PulseCharge[172][ipulse]);
			}

			for(int ipulse=0; ipulse<PMT_nPulse->at(173); ipulse++){
				hVetoTime2->Fill(PulseTime[173][ipulse]);
				//if(PulseTime[173][ipulse]<5000) continue;
				hBottomVeto->Fill(PulseCharge[172][ipulse]);
			}


		}
	}

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();
	hTopVeto->SetLineWidth(2);
	hTopVeto->SetLineColor(1);
	hTopVeto->Draw("hist");
	hTopVeto->GetXaxis()->SetTitle("Charge [p.e.]");


	TCanvas * c2 = new TCanvas();
	c2->SetGridx();
	c2->SetGridy();
	hBottomVeto->SetLineWidth(2);
	hBottomVeto->SetLineColor(1);
	hBottomVeto->Draw("hist");
	hBottomVeto->GetXaxis()->SetTitle("Charge [p.e.]");


	TCanvas * c3 = new TCanvas();
	hVetoTime1->Draw("hist");

	TCanvas * c4 = new TCanvas();
	hVetoTime2->Draw("hist");

	return;
}

