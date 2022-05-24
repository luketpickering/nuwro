#include "event1.h"
#include "root_eventstream.h"
#include "hepmc3_eventoutstream.h"

int main(int argc, char const *argv[]) {
  ROOTEventInStream is(argv[1]);

  event const *ev = is.GetEntry(0);

#ifdef HEPMC3_ENABLED
#ifdef HEPMC3_ENABLE_PROTOBUFIO
  HepMC3EventOutStream<HepMC3::Writerprotobuf> os(argv[2], ev->par);
#else
  HepMC3EventOutStream<HepMC3::WriterAscii> os(argv[2], ev->par);
#endif

  Long64_t entries = is.GetEntries();

  for (Long64_t i = 0; i < entries; ++i) {
    os.PushEvent(*is.GetEntry(i));
  }
#endif
}