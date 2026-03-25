void ReadTree(int trig=116, int pmtID=23)
{
	const int NUM_MOD = 15;
	const int NUM_CHAN = 16;
	const int MAX_LEN = 100000;

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




	int nmod;
	int nsample;
	Long64_t TimeTag[NUM_MOD];
	int EventCount[NUM_MOD];
	uint16_t FADC[15][16][2500];

	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	chain->Add("/group/mlf/nu/data/RawData_leedh_Far/wavedumpMB/WAVEDUMPMB_64ch_All/tree/R000676/tree_jsns2_far.r000676.f00000.root");
  chain->SetBranchAddress("nmod", &nmod);
  chain->SetBranchAddress("nsample", &nsample);
  chain->SetBranchAddress("TimeTag", TimeTag);
  chain->SetBranchAddress("EventCount", EventCount);
  chain->SetBranchAddress("FADC", FADC);

	TH1D * hWF = new TH1D("hWF","hWF",2500,0,2500);

		chain->GetEntry(trig);

		int mod=FADCmod[pmtID];
		int ch=FADCch[pmtID];
		for(int isample=0; isample<nsample; isample++){
				hWF->SetBinContent(isample+1, (int)FADC[mod][ch][isample]);
		}

	
	TCanvas * c1 = new TCanvas();
	hWF->Draw("hist");

}
