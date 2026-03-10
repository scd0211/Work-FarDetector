#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TList.h"

void checkRecoFile()
{
		std::string dir="/home/mlf/cdshin/Tape/FarDetector/v20260309/R000676";
		std::string treename="tree";
    std::vector<Long64_t> entries_list;

    TSystemDirectory directory("dir", dir);
    TList *files = directory.GetListOfFiles();

    if (!files) {
        std::cout << "Cannot open directory" << std::endl;
        return;
    }

    TSystemFile *file;
    TString fname;

    TIter next(files);

    while ((file=(TSystemFile*)next())) {

        fname = file->GetName();

        if (!fname.EndsWith(".root")) continue;
        if (!fname.Contains("Reco.tree_jsns2_far")) continue;

        TString fullpath = TString(dir) + "/" + fname;

        std::cout << "\nChecking : " << fullpath << std::endl;

        TFile *f = TFile::Open(fullpath);

        if (!f) {
            std::cout << "  -> FAILED to open" << std::endl;
            continue;
        }

        if (f->IsZombie()) {
            std::cout << "  -> ZOMBIE file" << std::endl;
            f->Close();
            continue;
        }

        if (f->TestBit(TFile::kRecovered)) {
            std::cout << "  -> RECOVERED file (possibly corrupted)" << std::endl;
        }

        TTree *tree = (TTree*)f->Get(treename);

        if (!tree) {
            std::cout << "  -> Tree not found : " << treename << std::endl;
            f->Close();
            continue;
        }

        Long64_t nentries = tree->GetEntries();

        std::cout << "  Entries : " << nentries << std::endl;

        entries_list.push_back(nentries);

        f->Close();
    }

    std::cout << "\n===== Summary =====" << std::endl;

    if(entries_list.size()==0){
        std::cout << "No valid files found." << std::endl;
        return;
    }

    Long64_t min = entries_list[0];
    Long64_t max = entries_list[0];
    Long64_t sum = 0;

    for(auto e : entries_list){
        if(e < min) min = e;
        if(e > max) max = e;
        sum += e;
    }

    double avg = (double)sum / entries_list.size();

    std::cout << "Files checked : " << entries_list.size() << std::endl;
    std::cout << "Min entries   : " << min << std::endl;
    std::cout << "Max entries   : " << max << std::endl;
    std::cout << "Average       : " << avg << std::endl;
}
