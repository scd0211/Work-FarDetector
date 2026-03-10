void Draw_FN_acci_WF(int runno, int subrunno)
{

	double wave[172][250]={0};
	double wave_align[172][250]={0};
	double pedestal[220];
	double pedestal_var[220];
	double PMTCharge[172];

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

	double nevt=0;


	TChain* chain = new TChain("tree");
	chain->Add(Form("tree/Acci/FN/R%06d/pair_r%06d.f%05d.root",runno,runno,subrunno));
	chain->LoadTree(0);
	chain->SetBranchAddress("promptQ", &promptQ);
	chain->SetBranchAddress("delayedQ", &delayedQ);
	chain->SetBranchAddress("p_vertx", &p_vertx);
	chain->SetBranchAddress("p_verty", &p_verty);
	chain->SetBranchAddress("p_vertz", &p_vertz);
	chain->SetBranchAddress("d_vertx", &d_vertx);
	chain->SetBranchAddress("d_verty", &d_verty);
	chain->SetBranchAddress("d_vertz", &d_vertz);
	chain->SetBranchAddress("time_diff", &time_diff);
	chain->SetBranchAddress("PMTCharge",PMTCharge);
	chain->SetBranchAddress("pedestal",pedestal);
	chain->SetBranchAddress("pedestal_var",pedestal_var);
	chain->SetBranchAddress("wave_align",wave_align);

	TH1D * hPromptQ = new TH1D("hPromptQ","hPromptQ",100,0,10000);
	TH1D * hDelayedQ = new TH1D("hDelayedQ","hDelayedQ",100,0,2000);
	TH1D * hDT = new TH1D("hDT","hDT",100,0,100);
	TH1D * hDVTX = new TH1D("hDVTX","hDVTX",100,0,6);

	TH2D * hVert1 = new TH2D("hVert1","hVert1",100,0,3.0*3.0,100,-3.0,3.0);
	TH2D * hVert2 = new TH2D("hVert2","hVert2",100,0,3.0*3.0,100,-3.0,3.0);

	TH1D * hVertX1 = new TH1D("hVertX1","hVertX1",100,-2.0,2.0);
	TH1D * hVertY1 = new TH1D("hVertY1","hVertY1",100,-2.0,2.0);
	TH1D * hVertZ1 = new TH1D("hVertZ1","hVertZ1",100,-2.0,2.0);

	TH1D * hVertX2 = new TH1D("hVertX2","hVertX2",100,-2.0,2.0);
	TH1D * hVertY2 = new TH1D("hVertY2","hVertY2",100,-2.0,2.0);
	TH1D * hVertZ2 = new TH1D("hVertZ2","hVertZ2",100,-2.0,2.0);

	TH1D * hPedVar = new TH1D("hPedVar","hPedVar",100,0,100);

	TH1D * hPMTCharge[172];

	TH1D * hWF[172];
 
	char hname[255];

	for(int ipmt=0; ipmt<172; ipmt++){
		sprintf(hname,"hWF_%03d",ipmt);
		hWF[ipmt]= new TH1D(hname,hname,250,0,250);	
		sprintf(hname,"hPMTCharge_%03d",ipmt);
 		hPMTCharge[ipmt]= new TH1D(hname,hname,100,0,100);
	}
	

	TVector3 v1;
	TVector3 v2;
	TVector3 v3;

	TVector3 v1_1;
	TVector3 v2_1;
	TVector3 v3_1;

	TVector3 v1_2;
	TVector3 v2_2;
	TVector3 v3_2;

	TVector3 v1_3;
	TVector3 v2_3;
	TVector3 v3_3;

	//for(int iEntry=0; iEntry<1; iEntry++){
	int ntot=chain->GetEntries();
	for(int iEntry=0; iEntry<ntot; iEntry++){

		if(iEntry%10000==0) cout<<iEntry<<"/"<<ntot<<" events"<<endl;
		chain->GetEntry(iEntry);

		v1.SetXYZ(p_vertx,p_verty,p_vertz);
		v2.SetXYZ(d_vertx,d_verty,d_vertz);

		v3 = v2-v1;
		if( !( v1.Perp()<1.0 && fabs(v1.Z())<1.0 ) ) continue;
		//if(promptQ<2000) continue;
		//if(promptQ>7000) continue;
		//if(delayedQ<700) continue;
		//if(delayedQ>1200) continue;

		if(promptQ<3000) continue;
		if(promptQ>8000) continue;
		if(delayedQ<750) continue;
		if(delayedQ>1250) continue;
		
		if(time_diff<500) continue;
		//if(v3.Mag()>0.6) continue;

		for(int ipmt=0; ipmt<172; ipmt++){
			hPedVar->Fill(pedestal_var[ipmt]);

			if(PMTCharge[ipmt]!=0) hPMTCharge[ipmt]->Fill(PMTCharge[ipmt]);

			if(PMTCharge[ipmt]<20) continue;
			if(PMTCharge[ipmt]>500) continue;
			if(pedestal_var[ipmt]>10) continue;
			for(int isam=0; isam<250; isam++){
				hWF[ipmt]->Fill(isam,wave_align[ipmt][isam]);
			}

		}



		hVert1->Fill(v1.Perp()*v1.Perp(),v1.Z());
		hVert2->Fill(v2.Perp()*v2.Perp(),v2.Z());
		hVertX2->Fill(d_vertx);
		hVertY2->Fill(d_verty);
		hVertZ2->Fill(d_vertz);
		hPromptQ->Fill(promptQ);
		hDelayedQ->Fill(delayedQ);
		hDT->Fill(time_diff);
		hDVTX->Fill(v3.Mag());

		nevt++;
	}


	gROOT->SetStyle("Plain");
	//	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1);
	gStyle->SetPalette(1);
	gStyle->SetLabelFont(132,"x");
	gStyle->SetLabelFont(132,"y");
	gStyle->SetLabelFont(132,"z");
	gStyle->SetTitleFont(132,"x");
	gStyle->SetTitleFont(132,"y");
	gStyle->SetTitleFont(132,"z");
/*

	TCanvas * c0 = new TCanvas();
	c0->SetGridx();
	c0->SetGridy();
	c0->SetLogy();


	hDT->SetLineWidth(2);
	hDT->SetLineColor(1);

	hDT->Draw("hist");
	hDT->SetTitle("");
	hDT->GetXaxis()->SetTitle("#DeltaT [#musec]");

	TCanvas * c1 = new TCanvas();
	c1->SetGridx();
	c1->SetGridy();

	hPromptQ->SetLineWidth(2);
	hPromptQ->SetLineColor(1);

	hPromptQ->Draw("hist");
	hPromptQ->SetTitle("");
	hPromptQ->GetXaxis()->SetTitle("Total Charge [p.e.]");

	TCanvas * c2 = new TCanvas();
	c2->SetGridx();
	c2->SetGridy();

	hDelayedQ->SetLineWidth(2);
	hDelayedQ->SetLineColor(1);

	hDelayedQ->Draw("hist");
	hDelayedQ->SetTitle("");
	hDelayedQ->GetXaxis()->SetTitle("Total Charge [p.e.]");


	TLine * line = new TLine();
	line->SetLineWidth(2);
	line->SetLineColor(2);

	TCanvas * c3 = new TCanvas();
	c3->SetGridx();
	c3->SetGridy();
	hVert1->Draw("colz");
	hVert1->SetTitle("");
	hVert1->GetXaxis()->SetTitle("R^{2} [m^{2}]");
	hVert1->GetYaxis()->SetTitle("Z [m]");
	line->DrawLine(0,-1.65,1.8*1.8,-1.65);
	line->DrawLine(0,1.65,1.8*1.8,1.65);
	line->DrawLine(1.8*1.8,-1.65,1.8*1.8,1.65);


	TCanvas * c4 = new TCanvas();
	c4->SetGridx();
	c4->SetGridy();
	hVert2->Draw("colz");
	hVert2->SetTitle("");
	hVert2->GetXaxis()->SetTitle("R^{2} [m^{2}]");
	hVert2->GetYaxis()->SetTitle("Z [m]");

	line->DrawLine(0,-1.65,1.8*1.8,-1.65);
	line->DrawLine(0,1.65,1.8*1.8,1.65);
	line->DrawLine(1.8*1.8,-1.65,1.8*1.8,1.65);


	TCanvas * c5 = new TCanvas();
	c5->SetGridx();
	c5->SetGridy();

	hDVTX->SetLineWidth(2);
	hDVTX->SetLineColor(1);
	hDVTX->Draw("hist");
	hDVTX->SetTitle("");
	hDVTX->GetXaxis()->SetTitle("#DeltaVTX [m]");

	TCanvas * c6 = new TCanvas();
	c6->SetGridx();
	c6->SetGridy();
	hVertX2->Draw("histe");
	hVertX2->SetLineWidth(2);
	hVertX2->SetLineColor(1);
	hVertX2->SetTitle("");
	hVertX2->GetXaxis()->SetTitle("X [m]");

	TCanvas * c7 = new TCanvas();
	c7->SetGridx();
	c7->SetGridy();
	hVertY2->Draw("histe");
	hVertY2->SetLineWidth(2);
	hVertY2->SetLineColor(1);
	hVertY2->SetTitle("");
	hVertY2->GetXaxis()->SetTitle("Y [m]");


	TCanvas * c8 = new TCanvas();
	c8->SetGridx();
	c8->SetGridy();
	hVertZ2->Draw("hist");
	hVertZ2->Draw("histe");
	hVertZ2->SetLineWidth(2);
	hVertZ2->SetLineColor(1);
	hVertZ2->SetTitle("");
	hVertZ2->GetXaxis()->SetTitle("Z [m]");

	TCanvas * c9 = new TCanvas();
	c9->SetGridx();
	c9->SetGridy();

//	hWF->Draw("hist");
	hPedVar->Draw("hist");
*/

	hWF[0]->Draw();

	gSystem->mkdir(Form("hist/R%06d", runno), true);
	TFile * fout = new TFile(Form("hist/R%06d/S%05d.root",runno,subrunno),"RECREATE");

	hVert1->Write();
	hVert2->Write();
	hVertX2->Write();
	hVertY2->Write();
	hVertZ2->Write();
	hPromptQ->Write();
	hDelayedQ->Write();
	hDT->Write();
	hDVTX->Write();


	for(int ipmt=0; ipmt<172; ipmt++){
		hWF[ipmt]->Write();
		hPMTCharge[ipmt]->Write();
	}
	fout->Close();


}
