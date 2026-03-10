#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <stdio.h>

using namespace std;


float ScalePDFME[172][25][248]; // normalization factor for ME PDF for 1D
float ScalePDFFN[172][25][248]; // normalization factor for FN PDF for 1D
TH3D * hPDF_ME[172]; // ME PDF for 1D
TH3D * hPDF_FN[172]; // FN PDF for both 1D and 2D

void GetPDF(const char *ME_PDFname, const char *FN_PDFname)
{

	TFile * fME = new TFile(ME_PDFname);
	TFile * fFN = new TFile(FN_PDFname);

	char hname[255];
	for(int i=0; i<172; i++){
		sprintf(hname,"hPDF%03d",i);
		hPDF_ME[i] = (TH3D*)fME->Get(hname);

		sprintf(hname,"hPDF%03d",i);
		hPDF_FN[i] = (TH3D*)fFN->Get(hname);
	}


	float sum;

	int nPMT=172;
	int Qbins = hPDF_ME[0]->GetNbinsX();
	int nFADC = hPDF_ME[0]->GetNbinsY();
	int nQrbins = hPDF_ME[0]->GetNbinsZ();

	for(int i=0; i<nPMT; i++){
		for(int j=0; j<Qbins; j++){
			for(int k=0; k<nFADC; k++){
				sum = hPDF_ME[i]->Integral(j+1,j+1,k+1,k+1,1,nQrbins);
				ScalePDFME[i][j][k] = 1./sum;
				if(sum==0) ScalePDFME[i][j][k] = 0;

				sum = hPDF_FN[i]->Integral(j+1,j+1,k+1,k+1,1,nQrbins);
				ScalePDFFN[i][j][k] = 1./sum;
				if(sum==0) ScalePDFFN[i][j][k] = 0;
			}
		}
	}

}
