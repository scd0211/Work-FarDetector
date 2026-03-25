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
std::vector<double> *RecoVertX = nullptr;
std::vector<double> *RecoVertY = nullptr;
std::vector<double> *RecoVertZ = nullptr;
std::vector<double> *RecoFlux = nullptr;

std::vector<int> PulseTime[220];
std::vector<double> PulseCharge[220];

int Num_Events;
Long64_t TTT[15];
int TriggerID;

const int inner_pmt=172;
const int nPMT=220;

void CheckPedestal(int run=676, int subrun=0)
{
	double pmtQ[172];
	double pedestal[172];
	double pedestal_var[172];

	TGraph * gTTT = new TGraph();

	TChain* chain = new TChain("tree");

	TString baseDir = Form("/home/mlf/cdshin/Tape/FarDetector/v20260309_ori/R%06d", run);

	void *dirp = gSystem->OpenDirectory(baseDir);
	if (!dirp) {
		cout << "Cannot open directory : " << baseDir << endl;
		return;
	}

	const char *entry;
	int nSubrun = 0;
	while ((entry = gSystem->GetDirEntry(dirp))) {
		TString fname = entry;
		if (fname.BeginsWith(Form("Reco.tree_jsns2_far.r%06d.f", run)) && fname.EndsWith(".root")) {
			nSubrun++;
		}
	}
	gSystem->FreeDirectory(dirp);

	cout << "Number of subruns : " << nSubrun << endl;
	nSubrun=1;

gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);
gStyle->SetTitleFont(132, "XYZ");
gStyle->SetLabelFont(132, "XYZ");
gStyle->SetStatFont(132);
gStyle->SetTextFont(132);
gStyle->SetOptTitle(0);
gStyle->SetPalette(1);

	TH1D * hPedvar = new TH1D("hPedvar","hPedvar",100,0,100);


	for (int isub = 0; isub < nSubrun; isub++) {

		chain->Reset();
		chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/v20260309_ori/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root", run, run, isub));
		chain->LoadTree(0);

		chain->SetBranchAddress("TriggerID", &TriggerID);
		chain->SetBranchAddress("TTT", TTT);
		chain->SetBranchAddress("Num_Events", &Num_Events);
		chain->SetBranchAddress("PMT_Time", &PMT_Time);
		chain->SetBranchAddress("PMT_Charge", &PMT_Charge);
		chain->SetBranchAddress("PMT_Pedestal", &PMT_Ped);
		chain->SetBranchAddress("PMT_Pedvar", &PMT_Pedvar);
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
//		cout << "Subrun : " << isub << " , Total entries : " << nTot << endl;

		for (int iEntry = 0; iEntry < nTot; iEntry++) {
			chain->GetEntry(iEntry);

			for(int ievt=0; ievt<Num_Events; ievt++){
				if((*EventTotalCharge)[ievt]<1000) continue;
				for (int ipmt = 0; ipmt < inner_pmt; ipmt++) {
					pmtQ[ipmt] = PMT_Charge->at(ipmt + ievt * nPMT);
					pedestal[ipmt] = PMT_Ped->at(ipmt + ievt * nPMT);
					pedestal_var[ipmt] = PMT_Pedvar->at(ipmt + ievt * nPMT);
					if(pedestal_var[ipmt]!=0)	hPedvar->Fill(pedestal_var[ipmt]);
					if(pedestal_var[ipmt]>100) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<pedestal_var[ipmt]<<" "<<pmtQ[ipmt]<<" "<<(*EventTime)[ievt]-75<<" "<<(*EventTime)[ievt]+175<<endl;
					//if(pedestal_var[ipmt]>100 && pmtQ[ipmt]<0 ) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<pedestal_var[ipmt]<<" "<<pmtQ[ipmt]<<" "<<(*EventTime)[ievt]-75<<" "<<(*EventTime)[ievt]+175<<endl;
					//if(pedestal_var[ipmt]<20) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<pedestal_var[ipmt]<<" "<<pmtQ[ipmt]<<" "<<(*EventTime)[ievt]<<endl;
				}
			}
		}
	}

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	hPedvar->SetLineWidth(2);
	hPedvar->SetLineColor(1);

	hPedvar->Draw("hist");

	hPedvar->SetTitle("");
	hPedvar->GetXaxis()->SetTitle("pedestal variance");


	return;
}
