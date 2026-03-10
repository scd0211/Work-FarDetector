

void DrawVetoCharge_kicker()
{

	std::vector<int> *PMT_nPulse = nullptr;
	std::vector<int> *PMT_Pulse_Time = nullptr;
	std::vector<double> *PMT_Pulse_Charge = nullptr;
	std::vector<int> PulseTime[220];
	std::vector<double> PulseCharge[220];


	TChain* chain = new TChain("tree");

	int run=684;

	chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/10FADC_differential_trigger/R%06d/Reco.tree_jsns2_far.r%06d.f00000.root",run,run));
	chain->LoadTree(0);
	chain->SetBranchAddress("PMT_nPulse", &PMT_nPulse);
	chain->SetBranchAddress("PMT_Pulse_Time", &PMT_Pulse_Time);
	chain->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge);

	int nTot = chain->GetEntries();
	cout << "Total entries : " << nTot << endl;


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

			for(int ipulse=0; ipulse<PMT_nPulse->at(172); ipulse++){
				hVetoTime1->Fill(PulseTime[172][ipulse]);
				hTopVeto->Fill(PulseCharge[172][ipulse]);
			}

			for(int ipulse=0; ipulse<PMT_nPulse->at(173); ipulse++){
				hVetoTime2->Fill(PulseTime[173][ipulse]);
				hBottomVeto->Fill(PulseCharge[172][ipulse]);
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

