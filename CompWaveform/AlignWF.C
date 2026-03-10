
double wave[172][250]={0};
double wave_align[172][250]={0};
double pedestal[220];
double pedestal_var[220];

//void AlignWF()
void AlignWF(int runno, int subrunno, int trigger, int evtid)
{
/*
	int	runno=422;
	int subrunno=0;
	int trigger=3;
	int evtid=0;
*/

	int FADCmod[220];
	int FADCch[220];

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


	std::vector<int> *EventTime = nullptr;

	TFile *f0 = TFile::Open(Form("/home/mlf/pjs/FAREC/output/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root",runno,runno,subrunno), "READ");
	TTree *tree0 = (TTree*)f0->Get("tree");
	tree0->SetBranchAddress("pedestal", pedestal);
	tree0->SetBranchAddress("pedestal_var", pedestal_var);
	tree0->SetBranchAddress("EventTime",&EventTime);
	tree0->GetEntry(trigger);
	int evttime=EventTime->at(evtid);
	f0->Close();
	delete f0;
	for(int ipmt=0; ipmt<172; ipmt++){
		//cout<<ipmt<<" "<<pedestal[ipmt]<<" "<<pedestal_var[ipmt]<<endl;
	}

	int nsample;

	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	chain->Add(Form("/home/mlf/pjs/FAREC/input/R%06d/tree_jsns2_far.r%06d.f%05d.root",runno,runno,subrunno));
	chain->SetBranchAddress("nsample", &nsample);
	chain->GetEntry(0);

	const int NMOD      = 15;
	const int NCH       = 16;


	int t_start=evttime-75;
	int t_end=evttime+175;

	if(t_start<0) t_start=0;
	if(t_end >= nsample) t_end=nsample;



	const int NSAM = nsample;
	std::vector<uint16_t> FADC_vec;
	FADC_vec.resize(NMOD * NCH * NSAM);

	chain->SetBranchAddress("FADC", FADC_vec.data());

	auto FADC = [&](int mod, int ch, int s) -> uint16_t& {
		// mod:   0 ~ 14
		// ch:    0 ~ 15
		// s:     0 ~ (NSAM-1)
		int idx = (mod * NCH + ch) * NSAM + s;
		return FADC_vec[idx];
	};

	chain->GetEntry(trigger);

	for(int ipmt=0; ipmt<172; ipmt++){
		int mod=FADCmod[ipmt];
		int ch=FADCch[ipmt];
		for(int isample=0; isample<nsample; isample++){
			int adc = (int)FADC(mod, ch, isample);
			double val = pedestal[ipmt] - adc;
			//double val = adc;
			wave[ipmt][isample]=val;
		}


		const int TARGET_POS = 50;

		int peak_index = 0;
		double peak_value = wave[ipmt][0];

		for (int i = 1; i < nsample; i++) {
			if (wave[ipmt][i] > peak_value) {
				peak_value = wave[ipmt][i];
				peak_index = i;
			}
		}

		int shift = TARGET_POS - peak_index;

		for (int i = 0; i < nsample; i++) {
			int new_index = i + shift;

			if (0 <= new_index && new_index < 250) {
				wave_align[ipmt][new_index] = wave[ipmt][i];
			}
		}

	}
/*
	TH1D * hWF = new TH1D("hWF","hWF",250,0,250);
	for (int i = 0; i < nsample; i++) {
		hWF->SetBinContent(i+1,wave_align[6][i]);
	}

	hWF->Draw("hist");
*/


}
