R__LOAD_LIBRARY(libbz2)

#include <bzlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include "TSystem.h"
#include "Compression.h"

const int num_boards = 10;
const int num_channels = 16;
const int total_channels = num_boards * num_channels;

#pragma pack(push, 1)  // Ensure no struct padding
struct EventData {
    uint32_t board;              // 4 bytes
    uint32_t ch;                 // 4 bytes
    uint32_t event_number;       // 4 bytes (mapped to header[3])
    uint32_t TriggerTimeStamp;   // 4 bytes (mapped to header[4])
    uint32_t SAM;                // 4 bytes (mapped to header[5], e.g. 5000)
};  // Total: 20 bytes
#pragma pack(pop)

void Convert_160ch_binarybz2_to_Tree_auto(int run, int subrun) {
    // Construct file names for part1 and part2

  std::string filename1 = Form("/ghi/fs02/mlf/nu/JSNS2_RawData_dhlee_Far/wavedumpMB/Raw_bz2file/R%06d/jsns2_far.r%06d.f%05d.dat.part1.bz2",run , run, subrun);
  std::string filename2 = Form("/ghi/fs02/mlf/nu/JSNS2_RawData_dhlee_Far/wavedumpMB/Raw_bz2file/R%06d/jsns2_far.r%06d.f%05d.dat.part1.bz2",run , run, subrun);

    // to check the time window automatically
   	int num_samples = 0;

    auto detect_num_samples = [&](const std::string &fname) -> bool {
        FILE *file = fopen(fname.c_str(), "rb");
        if (!file) {
            std::cerr << "[detect_num_samples] Cannot open file: " << fname << std::endl;
            return false;
        }
        int bzerror = BZ_OK;
        BZFILE *bzFile = BZ2_bzReadOpen(&bzerror, file, 0, 0, NULL, 0);
        if (bzerror != BZ_OK) {
            std::cerr << "[detect_num_samples] Error opening bz2 stream for file: " << fname << std::endl;
            fclose(file);
            return false;
        }

        EventData tmpHeader;
        int bytesRead = BZ2_bzRead(&bzerror, bzFile, &tmpHeader, sizeof(EventData));
        if (bytesRead != sizeof(EventData)) {
            std::cerr << "[detect_num_samples] Failed to read first EventData from: " << fname << std::endl;
            BZ2_bzReadClose(&bzerror, bzFile);
            fclose(file);
            return false;
        }

        BZ2_bzReadClose(&bzerror, bzFile);
        fclose(file);

        num_samples = static_cast<int>(tmpHeader.SAM);

        std::cout << "[detect_num_samples] From file " << fname
                  << ": SAM = " << tmpHeader.SAM
                  << "  --> num_samples = " << num_samples << std::endl;

        if (num_samples <= 0) {
            std::cerr << "[detect_num_samples] Invalid num_samples: " << num_samples << std::endl;
            return false;
        }
        return true;
    };

    if (!detect_num_samples(filename1)) {
        if (!detect_num_samples(filename2)) {
            std::cerr << "Failed to determine num_samples from both part1 and part2. Abort." << std::endl;
            return;
        }
    }    // end of check the time window automatically 

		
    std::vector<int> time_bin(num_samples, 0);
    std::vector<int> FADC_merged(num_boards * num_channels * num_samples, 0);
    //std::vector<uint16_t> FADC_merged(num_boards * num_channels * num_samples, 0);

		int TrigID;
    int event_number = 0;
		int nsample;
    long TriggerTimeStamp = 0;
    long TriggerTimeStamp_each[num_boards] = {0};
    int  event_number_each[num_boards]     = {0};
		int nmod=num_boards;

    // reset time_bin
    for (int i = 0; i < num_samples; i++) {
        time_bin[i] = i;
    }

    #define FADC(board, ch, sample) \
        FADC_merged[(board) * num_channels * num_samples + (ch) * num_samples + (sample)]
    
		// Create ROOT output file
    TFile *outputFile = new TFile(Form("tree_jsns2_far.r%06d.f%05d.root", run, subrun),"RECREATE");
//    TFile *outputFile = new TFile(Form("tree_jsns2_far.r%06d.f%05d_ZLIB_l4.root", run, subrun),"RECREATE");
//    TFile *outputFile = new TFile(Form("tree_jsns2_far.r%06d.f%05d_LZMA_l3.root", run, subrun),"RECREATE");
//		outputFile->SetCompressionAlgorithm(ROOT::kZLIB);
//		outputFile->SetCompressionLevel(4);
    outputFile->SetCompressionAlgorithm(ROOT::kLZMA);
	  outputFile->SetCompressionLevel(1);

    TTree *mergedTree = new TTree("tree", "tree");

//		ntp -> Branch("TrigID", &TrigID, "TrigID/I");
//		ntp -> Branch("nmod", &nmod, "nmod/I");
//		ntp -> Branch("nsample", &nsample, "nsample/I");
//		ntp -> Branch("TimeTag", TimeTag, "TimeTag[nmod]/L");
//		ntp -> Branch("EventCount", EventCount, "EventCount[nmod]/I");
//		ntp -> Branch("FADC", FADC, Form("FADC[15][16][%d]/s",num_samples));	// need to modify for time window 



    mergedTree->Branch("nmod", &nmod, "nmod/I");
    mergedTree->Branch("TrigID", &TrigID, "TrigID/I");
    mergedTree->Branch("nsample", &nsample, "nsample/I");
    mergedTree->Branch("TimeTag", TriggerTimeStamp_each,Form("TimeTag[%d]/L", num_boards));
    mergedTree->Branch("TTT", &TriggerTimeStamp, "TTT/L");
    mergedTree->Branch("EventCount", event_number_each, Form("EventCount[%d]/I", num_boards));
    mergedTree->Branch("FADC", FADC_merged.data(),Form("FADC[%d][%d][%d]/I", num_boards, num_channels, num_samples));
    mergedTree->Branch("event_number", &event_number, "event_number/I");
    mergedTree->Branch("time_bin", time_bin.data(), Form("time_bin[%d]/I", num_samples));

    
		int count = 0, count2 = 0, count3 = 0;
    EventData eventData;
    std::vector<uint16_t> waveform(num_samples); 
    
		// Lambda to process one bzip2 file
    auto process_file = [&](const std::string &fname) {
        FILE *file = fopen(fname.c_str(), "rb");
        if (!file) {
            std::cerr << "Error opening file: " << fname << std::endl;
            return;
        }
        int bzerror;
        BZFILE *bzFile = BZ2_bzReadOpen(&bzerror, file, 0, 0, NULL, 0);
        if (bzerror != BZ_OK) {
            std::cerr << "Error opening bz2 stream for file: " << fname << std::endl;
            fclose(file);
            return;
        }

        while (true) {
            int bytesRead = BZ2_bzRead(&bzerror, bzFile, &eventData, sizeof(EventData));
            if (bytesRead != sizeof(EventData)) {
                if (bzerror == BZ_STREAM_END) {
                    break;
                } else {
                    std::cerr << "Error reading eventData from " << fname << std::endl;
                    break;
                }
            }

            bytesRead = BZ2_bzRead(&bzerror, bzFile, waveform.data(),
                                   sizeof(uint16_t) * num_samples);
            if (bytesRead != static_cast<int>(sizeof(uint16_t) * num_samples)) {
                std::cerr << "Error reading waveform data from " << fname
                          << " (bytesRead=" << bytesRead
                          << ", expected=" << sizeof(uint16_t) * num_samples << ")"
                          << std::endl;
                break;
            }

            int board = eventData.board;
            int ch    = eventData.ch;
            event_number      = eventData.event_number;
            TriggerTimeStamp  = eventData.TriggerTimeStamp;

							nsample=eventData.SAM;
            // Debug output every 200 events for board 0 and channel 0
            if ( ((count3 % 200 == 0) && board == 0 && ch == 0) ||
                 (count3 > 1498 && count3 < 1503) ) {
                std::cout << "Event: " << event_number
                          << " | Board: " << board
                          << " | Ch: " << ch
                          << " | TTT: " << TriggerTimeStamp
                          << " | SAM: " << eventData.SAM
                          << " | FADC[0]: " << waveform[0] << std::endl;
            }

            if (board >= num_boards || ch >= num_channels) {
                std::cerr << "ERROR: Out-of-bounds board/channel: board="
                          << board << ", ch=" << ch << std::endl;
                continue;
            }

            // copy waveform to FADC_merged
            for (int k = 0; k < num_samples; ++k) {
                FADC(board, ch, k) = waveform[k];
            }

            if (ch == 0) {
                event_number_each[board]     = event_number;
                TriggerTimeStamp_each[board] = TriggerTimeStamp;
            }

            count++;
            count2++;
            if (count >= total_channels) {
                count = 0;
                mergedTree->Fill();
                if (count3 % 10000 == 0) {
                    mergedTree->AutoSave("SaveSelf");
                }
								TrigID++;
                count3++;
            }
        }

        BZ2_bzReadClose(&bzerror, bzFile);
        fclose(file);
    };
    // Process both parts sequentially
    process_file(filename1);
    process_file(filename2);

    outputFile->Write();
    outputFile->Close();

    std::cout << "Finished processing and merging waveform files from both bzip2 parts."
              << std::endl;

    #undef FADC
}
