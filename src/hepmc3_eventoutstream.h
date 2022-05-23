#pragma once

#include "event1.h"
#include "eventstream.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenRunInfo.h"
#include "HepMC3/GenVertex.h"

#include "HepMC3/WriterRootTree.h"

#include <memory>

template <typename WriterType = HepMC3::WriterRootTree>
class HepMC3EventOutStream : public EventOutStream {
public:
  HepMC3EventOutStream(std::string filename)
      : fWriter(new WriterType(filename)) {
    std::shared_ptr<HepMC3::GenRunInfo> gri(new HepMC3::GenRunInfo());

    HepMC3::GenRunInfo::ToolInfo ti;
    ti.name = "nuwro";
    ti.version = VERSION;
    ti.description =
        "These events brought to you by the NuWro neutrino event generator";
    gri->tools().push_back(ti);

    fWriter->set_run_info(gri);
  }

  void PushEvent(event const &ev) {
    HepMC3::GenVertexPtr vtx(new HepMC3::GenVertex(
        HepMC3::FourVector{ev.r.x, ev.r.y, ev.r.z, 0}));

    for (auto &p : ev.in) {
      vtx->add_particle_in(HepMC3::GenParticlePtr(new HepMC3::GenParticle(
          HepMC3::FourVector{p.x, p.y, p.z, p.t}, p.pdg, 4)));
    }

    for (auto &p : ev.post) {
      vtx->add_particle_out(HepMC3::GenParticlePtr(new HepMC3::GenParticle(
          HepMC3::FourVector{p.x, p.y, p.z, p.t}, p.pdg, 1)));
    }

    HepMC3::GenEvent hepmc;
    hepmc.add_attribute("HardScatterMode",
                        std::shared_ptr<HepMC3::IntAttribute>(
                            new HepMC3::IntAttribute(ev.dyn)));

    hepmc.add_vertex(vtx);

    fWriter->write_event(hepmc);
  }

  ~HepMC3EventOutStream() {
    fWriter->close();
    delete fWriter;
  }

private:
  HepMC3::Writer *fWriter;
};