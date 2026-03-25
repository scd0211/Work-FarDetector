std::vector<int> *PMT_nPulse1 = nullptr;
std::vector<double> *PMT_Pulse_Charge1 = nullptr;
std::vector<double> *RecoVertX1 = nullptr;
std::vector<double> *RecoVertY1 = nullptr;
std::vector<double> *RecoVertZ1 = nullptr;
std::vector<double> *RecoFlux1 = nullptr;

std::vector<int> *PMT_nPulse2 = nullptr;
std::vector<double> *PMT_Pulse_Charge2 = nullptr;
std::vector<double> *RecoVertX2 = nullptr;
std::vector<double> *RecoVertY2 = nullptr;
std::vector<double> *RecoVertZ2 = nullptr;
std::vector<double> *RecoFlux2 = nullptr;

const int inner_pmt=172;
const int nPMT=220;

void CheckCharge()
{
	int run=676;


	TChain* chain1 = new TChain("tree");
	TChain* chain2 = new TChain("tree");

	TString baseDir = Form("/home/mlf/cdshin/Tape/FarDetector/v20260309_ori/R%06d", run);

	void *dirp = gSystem->OpenDirectory(baseDir);
	if (!dirp) {
		cout << "Cannot open directory : " << baseDir << endl;
		return;
	}

	const char *entry;
	int nSubrun = 1;
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

	TH1D * hPMTcharge1 = new TH1D("hPMTcharge1","hPMTcharge1",100,-1000,2000);
	TH1D * hPMTcharge2 = new TH1D("hPMTcharge2","hPMTcharge2",100,-1000,2000);
	TH1D * hDiffcharge = new TH1D("hDiffcharge","hDiffcharge",100,-1000,2000);
	for (int isub = 0; isub < nSubrun; isub++) {

		chain1->Reset();
		chain1->Add(Form("/home/mlf/cdshin/Tape/FarDetector/v20260309_ori/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root", run, run, isub));
		chain1->LoadTree(0);
		chain1->SetBranchAddress("PMT_nPulse", &PMT_nPulse1);
		chain1->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge1);
		chain1->SetBranchAddress("RecoVertX", &RecoVertX1);
		chain1->SetBranchAddress("RecoVertY", &RecoVertY1);
		chain1->SetBranchAddress("RecoVertZ", &RecoVertZ1);
		chain1->SetBranchAddress("RecoFlux", &RecoFlux1);

		chain2->Reset();
		chain2->Add(Form("/home/mlf/cdshin/Tape/FarDetector/v20260309/R%06d/Reco.tree_jsns2_far.r%06d.f%05d.root", run, run, isub));
		chain2->LoadTree(0);
		chain2->SetBranchAddress("PMT_nPulse", &PMT_nPulse1);
		chain2->SetBranchAddress("PMT_Pulse_Charge", &PMT_Pulse_Charge2);
		chain2->SetBranchAddress("RecoVertX", &RecoVertX2);
		chain2->SetBranchAddress("RecoVertY", &RecoVertY2);
		chain2->SetBranchAddress("RecoVertZ", &RecoVertZ2);
		chain2->SetBranchAddress("RecoFlux", &RecoFlux2);



		int nTot = chain1->GetEntries();

		for (int iEntry = 0; iEntry < nTot; iEntry++) {
			chain1->GetEntry(iEntry);
			chain2->GetEntry(iEntry);

			int ivec=0;
			for(int ipmt=0; ipmt<220; ipmt++){
				for(int ipulse=0; ipulse<PMT_nPulse1->at(ipmt); ipulse++){
					double pmtcharge1=PMT_Pulse_Charge1->at(ivec);
					double pmtcharge2=PMT_Pulse_Charge2->at(ivec);
					if(pmtcharge1!=0) hPMTcharge1->Fill(pmtcharge1);
					if(pmtcharge2!=0) hPMTcharge2->Fill(pmtcharge2);

					double diffcharge = pmtcharge2-pmtcharge1;
					hDiffcharge->Fill(diffcharge);

				//	if(pmtcharge2<0) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<ipulse<<" "<<pmtcharge2<<endl;
					//if(diffcharge<-300) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<ipulse<<endl;
					if(diffcharge>1000) cout<<run<<" "<<isub<<" "<<iEntry<<" "<<ipmt<<" "<<ipulse<<endl;
					ivec++;
				}
			}
		}
	}

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	hPMTcharge1->SetLineWidth(2);
	hPMTcharge2->SetLineWidth(2);
	hPMTcharge1->SetLineColor(1);
	hPMTcharge2->SetLineColor(2);

	hPMTcharge1->Draw("hist");
	hPMTcharge2->Draw("histsame");

	hPMTcharge1->SetMaximum(1.1*hPMTcharge2->GetMaximum());

	hPMTcharge1->GetXaxis()->SetTitle("Charge [p.e.]");

	TLegend * leg1 = new TLegend(0.7,0.7,0.89,0.89);
	leg1->SetFillColor(0);
	leg1->SetBorderSize(0);
	leg1->SetTextFont(132);
	leg1->AddEntry(hPMTcharge1,"previous","l");
	leg1->AddEntry(hPMTcharge2,"new","l");
	leg1->Draw();

	TCanvas * c2 = new TCanvas();
	c2->SetGridx();
	c2->SetGridy();
	c2->SetLogy();

	hDiffcharge->Draw("hist");
	hDiffcharge->GetXaxis()->SetTitle("Difference (new-previous) [p.e.]");

		return;
	}
