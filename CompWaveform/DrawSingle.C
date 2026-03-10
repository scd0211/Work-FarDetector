void DrawSingle()
{

	double totQ;
	double VertX;
	double VertY;
	double VertZ;
	int nPed;


	TChain* chain = new TChain("tree");
	chain->Add(Form("tree/Single/R000454.root"));
	chain->LoadTree(0);
	chain->SetBranchAddress("VertX", &VertX);
	chain->SetBranchAddress("VertY", &VertY);
	chain->SetBranchAddress("VertZ", &VertZ);
	chain->SetBranchAddress("totQ", &totQ);
	chain->SetBranchAddress("nPed", &nPed);


	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");




	TH1D * hQ1 = new TH1D("hQ1","hQ1",100,0,10000);
	TH1D * hQ2 = new TH1D("hQ2","hQ2",100,0,10000);
	TH1D * hQ3 = new TH1D("hQ3","hQ3",100,0,10000);

	TVector3 v1;

	int ntot=chain->GetEntries();
	for(int iEntry=0; iEntry<ntot; iEntry++){

		if(iEntry%10000==0) cout<<iEntry<<"/"<<ntot<<" events"<<endl;
		chain->GetEntry(iEntry);

		v1.SetXYZ(VertX,VertY,VertZ);

		if( !( v1.Perp()<1.0 && fabs(v1.Z())<1.0 ) ) continue;


		hQ1->Fill(totQ);

		if(nPed<4) hQ2->Fill(totQ);
		else hQ3->Fill(totQ); 

	}

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	
	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();

	hQ1->SetLineWidth(2);
	hQ2->SetLineWidth(2);
	hQ3->SetLineWidth(2);

	hQ1->SetLineColor(1);
	hQ2->SetLineColor(2);
	hQ3->SetLineColor(4);


	hQ1->Draw("hist");
	hQ2->Draw("histsame");
	hQ3->Draw("histsame");


	hQ1->SetTitle("");
	hQ1->GetXaxis()->SetTitle("Total charge [p.e.]");

	TLegend * leg = new TLegend(0.6,0.7,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(132);
	leg->AddEntry(hQ1,"all","l");
	leg->AddEntry(hQ2,"after baseline cut","l");
	leg->AddEntry(hQ3,"rejected","l");
	leg->Draw();




}
