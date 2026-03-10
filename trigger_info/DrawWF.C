void DrawWF(int run=549, int subrun=16, int trig=900)
{

	int pmt=0;
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
	uint16_t FADC[15][16][1000];

	TChain * chain = new TChain("tree");
	chain->LoadTree(0);
	chain->Add(Form("/home/mlf/cdshin/Tape/FarDetector/Tree_for_FAREC/R%06d/tree_jsns2_far.r%06d.f%05d.root",run,run,subrun));
  chain->SetBranchAddress("nmod", &nmod);
  chain->SetBranchAddress("nsample", &nsample);
  chain->SetBranchAddress("TimeTag", TimeTag);
  chain->SetBranchAddress("EventCount", EventCount);
  chain->SetBranchAddress("FADC", FADC);

	TH1D * hWF= new TH1D("hWF","hWF",1000,0,1000);


		chain->GetEntry(trig);
				//int mod=FADCmod[pmt];
				//int ch=FADCch[pmt];
				int mod=0;
				int ch=0;
				cout<<mod<<" "<<ch<<endl;

			for(int isample=0; isample<nsample; isample++){

				hWF->SetBinContent(isample+1, (int)FADC[mod][ch][isample]);
			}


	gStyle->SetOptStat(0);

	TCanvas * c1= new TCanvas();
	c1->SetTopMargin(0.12);
	c1->SetBottomMargin(0.12);
	c1->SetLeftMargin(0.12);
	c1->SetRightMargin(0.12);
	c1->SetGridx();
	c1->SetGridy();
	hWF->Draw("hist");
	hWF->SetTitle("");
	hWF->GetXaxis()->SetTitle("time tick");
	hWF->GetYaxis()->SetTitle("ADC");
	hWF->SetLineWidth(2);
	hWF->SetLineColor(1);

}
