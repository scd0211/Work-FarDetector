void StoreWF(int runno=422, int subrunno=0)
{

	double promptQ;
	double delayedQ;
	double time_diff;
	double p_vertx;
	double p_verty;
	double p_vertz;
	double d_vertx;
	double d_verty;
	double d_vertz;

	double nevt=0;


	TChain* chain = new TChain("tree");
	chain->Add(Form("tree/ME/R%06d/pair_r%06d.f%05d.root",runno,runno,subrunno));
	chain->LoadTree(0);
	chain->SetBranchAddress("promptQ", &promptQ);
	chain->SetBranchAddress("delayedQ", &delayedQ);
	chain->SetBranchAddress("p_vertx3", &p_vertx);
	chain->SetBranchAddress("p_verty3", &p_verty);
	chain->SetBranchAddress("p_vertz3", &p_vertz);
	chain->SetBranchAddress("d_vertx3", &d_vertx);
	chain->SetBranchAddress("d_verty3", &d_verty);
	chain->SetBranchAddress("d_vertz3", &d_vertz);
	chain->SetBranchAddress("time_diff", &time_diff);

	TVector3 v1;
	TVector3 v2;
	TVector3 v3;


	for(int iEntry=0; iEntry<chain->GetEntries(); iEntry++){
		chain->GetEntry(iEntry);

		v1.SetXYZ(p_vertx,p_verty,p_vertz);
		v2.SetXYZ(d_vertx,d_verty,d_vertz);

		v3 = v2-v1;

		if( !( v2.Perp()<1.0 && fabs(v2.Z())<1.0 ) ) continue;

		//if(delayedQ<2000) continue;
		//if(delayedQ>10000) continue;


		nevt++;
	}


}
