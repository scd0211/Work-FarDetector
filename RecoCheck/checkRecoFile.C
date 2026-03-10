#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TList.h"
#include "TString.h"

void checkRecoFile()
{
    const char *dir="/home/mlf/cdshin/Tape/FarDetector/v20260309/R000676";
    const char *treename="tree";

    TSystemDirectory directory(dir, dir);
    TList *files = directory.GetListOfFiles();

    if (!files) {
        std::cout << "Cannot open directory : " << dir << std::endl;
        return;
    }

    Long64_t refEntries = -1;
    bool foundReference = false;
    int nChecked = 0;
    int nBad = 0;

    std::cout << "=== Files with different event counts or bad status ===" << std::endl;

    TIter next(files);
    TSystemFile *file = 0;

    while ((file = (TSystemFile*)next())) {
        TString fname = file->GetName();

        if (file->IsDirectory()) continue;
        if (!fname.EndsWith(".root")) continue;
        if (!fname.BeginsWith("Reco.tree_jsns2_far.")) continue;

        TString fullpath = TString(dir) + "/" + fname;

        TFile *f = TFile::Open(fullpath.Data(), "READ");
        if (!f) {
            std::cout << fname << "  -> FAILED TO OPEN" << std::endl;
            nBad++;
            continue;
        }

        if (f->IsZombie()) {
            std::cout << fname << "  -> ZOMBIE FILE" << std::endl;
            nBad++;
            f->Close();
            delete f;
            continue;
        }

        if (f->TestBit(TFile::kRecovered)) {
            std::cout << fname << "  -> RECOVERED FILE" << std::endl;
            nBad++;
            f->Close();
            delete f;
            continue;
        }

        TTree *tree = (TTree*)f->Get(treename);
        if (!tree) {
            std::cout << fname << "  -> TREE NOT FOUND (" << treename << ")" << std::endl;
            nBad++;
            f->Close();
            delete f;
            continue;
        }

        Long64_t nentries = tree->GetEntries();
        nChecked++;

        if (!foundReference) {
            refEntries = nentries;
            foundReference = true;
        } else {
            if (nentries != refEntries) {
                std::cout << fname
                          << "  -> DIFFERENT ENTRIES : "
                          << nentries
                          << " (reference = " << refEntries << ")"
                          << std::endl;
                nBad++;
            }
        }

        f->Close();
        delete f;
    }

    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Reference entries : " << refEntries << std::endl;
    std::cout << "Checked files     : " << nChecked << std::endl;
    std::cout << "Bad/different     : " << nBad << std::endl;
}
