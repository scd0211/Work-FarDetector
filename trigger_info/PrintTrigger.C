{

	std::vector<int> *EventTime = nullptr;
	std::vector<double> *EventTotalCharge = nullptr;
	std::vector<int> *PMT_Time = nullptr;
	std::vector<double> *PMT_Charge = nullptr;
	std::vector<double> *PMT_PredCharge = nullptr;
	std::vector<double> *PMT_PredChargeCorr = nullptr;
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

	int Trigger_type;
	int Num_Events;
	Long64_t TTT[15];
	int TriggerID;


	int nPMT = 220;
	int inner_pmt = 172;


	bool FlagOverlap; 
	bool FlagKicker;
	bool FlagSP;
	bool FlagSD;

	double nevt=0;
	//int runno=549;
	int runno=683;


	//TString dirPath  = Form("/home/mlf/cdshin/Tape/FarDetector/10FADC_differential_trigger/R%06d",runno);
	TString dirPath  = Form("/home/mlf/cdshin/Tape/FarDetector/v20260304_trigger_update_study/R%06d",runno);
	TString prefix   = Form("Reco.tree_jsns2_far.r%06d.f",runno);
	TString ext      = ".root";

	TSystemDirectory dir("dir", dirPath);
	TList *files = dir.GetListOfFiles();
	if (!files) {
		std::cerr << "No files found in directory: " << dirPath << std::endl;
		return -1;
	}

	cout<<dirPath<<endl;
	int maxIndex = -1;

	TIter next(files);
	while (TSystemFile *file = (TSystemFile*)next()) {
		TString name = file->GetName();

		if (file->IsDirectory()) continue;
		if (!name.BeginsWith(prefix)) continue;
		if (!name.EndsWith(ext))      continue;

		TString tmp = name;
		tmp.ReplaceAll(ext, "");

		TString numStr = tmp;
		numStr.Remove(0, prefix.Length());

		int idx = numStr.Atoi();
		if (idx > maxIndex) maxIndex = idx;
	}
	delete files;

	cout<<maxIndex<<endl;

	//for (int isubrun = 0; isubrun < maxIndex; isubrun++) {
	//for (int isubrun = 0; isubrun < 1; isubrun++) {


	TH1D * hFlag = new TH1D("hFlag","hFlag",5,0,5);

	for (int isubrun = 0; isubrun < 1; isubrun++) {
	//for (int isubrun = 0; isubrun < 100; isubrun++) {

		//TString fname = Form("/home/mlf/cdshin/Tape/FarDetector/10FADC_differential_trigger/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root",runno, runno, isubrun);
		TString fname = Form("/home/mlf/cdshin/Tape/FarDetector/v20260304_trigger_update_study/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root",runno, runno, isubrun);

		TFile *fin = TFile::Open(fname);
		if (!fin || fin->IsZombie()) {
			std::cout << "[Skip] cannot open: " << fname << std::endl;
			if (fin) { fin->Close(); delete fin; }
			continue;
		}

		TTree *chain = (TTree*)fin->Get("tree");
		if (!chain) {
			std::cout << "[Skip] no tree in: " << fname << std::endl;
			fin->Close();
			delete fin;
			continue;
		}

		chain->LoadTree(0);
		chain->SetBranchAddress("TriggerID", &TriggerID);
		chain->SetBranchAddress("TTT", TTT);
		chain->SetBranchAddress("Trigger_type", &Trigger_type);
		chain->SetBranchAddress("Num_Events", &Num_Events);
		chain->SetBranchAddress("PMT_Time", &PMT_Time);
		chain->SetBranchAddress("PMT_Charge", &PMT_Charge);
		chain->SetBranchAddress("PMT_nPulse", &PMT_nPulse);
		chain->SetBranchAddress("PMT_Pulse_Time", &PMT_Pulse_Time);
		chain->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge);
		chain->SetBranchAddress("EventTime",&EventTime);
		chain->SetBranchAddress("EventTotalCharge",&EventTotalCharge);
		chain->SetBranchAddress("CentVertX", &CentVertX);
		chain->SetBranchAddress("CentVertY", &CentVertY);
		chain->SetBranchAddress("CentVertZ", &CentVertZ);
		chain->SetBranchAddress("RecoVertX", &RecoVertX);
		chain->SetBranchAddress("RecoVertY", &RecoVertY);
		chain->SetBranchAddress("RecoVertZ", &RecoVertZ);
		chain->SetBranchAddress("RecoFlux", &RecoFlux);
		chain->SetBranchAddress("FlagOverlap", &FlagOverlap);
		chain->SetBranchAddress("FlagKicker", &FlagKicker);
		chain->SetBranchAddress("FlagSP", &FlagSP);
		chain->SetBranchAddress("FlagSD", &FlagSD);

		Long64_t nEntries = chain->GetEntries();



		for (Long64_t iEntry = 0; iEntry < nEntries; iEntry++) {
			chain->GetEntry(iEntry);

			int flagtot=0;

			if(FlagKicker){
				flagtot++;
				hFlag->Fill(1);
			}
			if(FlagSP){
				flagtot++;
				hFlag->Fill(2);
			}
			if(FlagSD){
				flagtot++;
				hFlag->Fill(3);
			}
			if(FlagOverlap){
				flagtot++;
				hFlag->Fill(4);
			}

			cout<<isubrun<<" "<<iEntry<<" "<<FlagKicker<<" "<<FlagSP<<" "<<FlagSD<<" "<<FlagOverlap<<endl;
			//continue;
			//if(FlagOverlap) cout<<"Overlap flag : "<<isubrun<<" "<<iEntry<<" "<<FlagKicker<<" "<<FlagSP<<" "<<FlagSD <<endl;
			//if(flagtot>1) cout<<"multi flag : "<<isubrun<<" "<<iEntry<<" "<<FlagKicker<<" "<<FlagSP<<" "<<FlagSD<<endl;
			if(flagtot<1){
				cout<<"no flag : "<<isubrun<<" "<<iEntry<<endl;
				hFlag->Fill(0);
			}


		}

		fin->Close();
		delete fin;
	}

	gStyle->SetOptStat(0);
	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	hFlag->SetTitle("");
	hFlag->SetLineWidth(2);
	hFlag->SetLineColor(1);

	hFlag->GetXaxis()->SetBinLabel(1, "empty");
	hFlag->GetXaxis()->SetBinLabel(2, "kicker");
	hFlag->GetXaxis()->SetBinLabel(3, "SP");
	hFlag->GetXaxis()->SetBinLabel(4, "SD");
	hFlag->GetXaxis()->SetBinLabel(5, "overlap");

	gStyle->SetPaintTextFormat("4.0f"); // 표시 형식 (정수)


	hFlag->Draw("hist");
	hFlag->GetXaxis()->SetTitle("Trigger");
	hFlag->GetXaxis()->SetLabelSize(0.05);
	hFlag->GetXaxis()->SetTitleSize(0);


	
double ymax = hFlag->GetMaximum();

for (int i = 1; i <= hFlag->GetNbinsX(); i++) {
    double x = hFlag->GetBinCenter(i);
    double y = hFlag->GetBinContent(i);

    double ytext = y + 0.02*ymax;  // 항상 일정한 offset

    TLatex *t = new TLatex(x, ytext, Form("%.0f", y));
    t->SetTextAlign(22);
    t->SetTextSize(0.04);
    t->Draw();
}

}
