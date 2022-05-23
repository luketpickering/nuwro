#pragma once

#include "event1.h"
#include "eventstream.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TTree.h"

class ROOTEventOutStream : public EventOutStream {
public:
  ROOTEventOutStream(std::string filename)
      : f1(NULL), t1(NULL), evout(NULL) {

    TDirectory *ogdir = gDirectory;

    f1 = TFile::Open(filename.c_str(), "RECREATE");
    t1 = new TTree("treeout", "Tree of events");
    t1->Branch("e", "event", &evout);

    if (ogdir) {
      ogdir->cd();
    }
  }

  void PushEvent(event const &ev) {
    *evout = ev;
    t1->Fill();
  }

  event *GetEntry(Long64_t i) {
    t1->GetEntry(i);
    return evout;
  }

  Long64_t GetEntries() { return t1->GetEntries(); }

  Long64_t Write() { return f1->Write(); }

  ~ROOTEventOutStream() {
    if (f1) {
      f1->Write();
      f1->Close();
    }
    delete f1;
  }

private:
  TFile *f1;
  TTree *t1;
  event *evout;
};

class ROOTEventInStream : public EventInStream {
public:
  ROOTEventInStream(std::string filename)
      : f1(NULL), t1(NULL), CEntry(0), evin(NULL) {

    TDirectory *ogdir = gDirectory;

    f1 = TFile::Open(filename.c_str(), "READ");
    t1 = dynamic_cast<TTree *>(f1->Get("treeout"));
    if (!t1) {
      std::cerr << "tree \"treeout\" not found in file \"" << filename << "\""
                << std::endl;
      exit(6);
    }
    evin = NULL;
    t1->SetBranchAddress("e", &evin);

    NEntries = t1->GetEntries();

    if (ogdir) {
      ogdir->cd();
    }
  }

  bool ReadEvent(event &ev) {

    if(CEntry >= NEntries){
      return false;
    }

    ev = *GetEntry(CEntry++);
    return true;
  }

  event *GetEntry(Long64_t i) {
    if(i >= NEntries){
      return NULL;
    }

    t1->GetEntry(i);
    return evin;
  }

  Long64_t GetEntries() { return NEntries; }

  ~ROOTEventInStream() {
    if (f1) {
      f1->Close();
    }
    delete f1;
  }

private:
  TFile *f1;
  TTree *t1;

  Long64_t CEntry;
  Long64_t NEntries;

  event *evin;
};