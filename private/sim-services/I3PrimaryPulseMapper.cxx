/**
 * Copyright (c) 2013
 * Juan Carlos Diaz-Velez <juancarlos@icecube.wisc.edu>
 * and the IceCube Collaboration <http://www.icecube.wisc.edu>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * $Id$
 *
 * @file I3PrimaryPulseMapper.cxx
 * @version $Revision$
 * @date $Date$
 * @author Juan Carlos Diaz-Velez
 */

#include <boost/foreach.hpp>
#include <icetray/I3Units.h>
#include <icetray/I3Module.h>
#include <simclasses/I3MCPulse.h>
#include <sim-services/MCPEMCPulseTools.hpp>
#include <dataclasses/physics/I3MCTree.h>

namespace{
        typedef std::map<I3ParticleID, I3ParticlePtr> I3ParticleIDParticleMap;

	I3ParticlePtr getPrimary(I3ParticleIDParticleMap& map, const I3ParticleID& pid) {
	    I3ParticleIDParticleMap::iterator iter2 = map.find(pid);
	    if (iter2 != map.end()) {
		return iter2->second;
	    } else {
		return I3ParticlePtr();
	    }
	}

	I3ParticleIDParticleMap buildPrimaryMap (I3MCTreeConstPtr tree) 
	{
	    I3ParticleIDParticleMap map = I3ParticleIDParticleMap();

	    I3MCTree::const_iterator iter;
	    I3ParticlePtr latest_primary;
	    for (iter=tree->begin(); iter != tree->end(); ++iter) {
		if (tree->depth(iter) == 0) { // primary
		    latest_primary = I3ParticlePtr(new I3Particle(*iter));
		}
		// add to map under latest primary
		I3ParticleID id = iter->GetID();
		map[id] = latest_primary;
	    }
	    return map;
	}
}


/**
 * @brief Removes photo-electron hits that are separated in time by a factor 
 * larger than maxDT/2 from the median time (where maxDT is the maximum size 
 * of the trigger window).
 * 
 * The purpose is to eliminate hits that, while physically related to a triggered
 * event, would never be associated to that event by the DAQ.
 * The long gaps will otherwise be filled by noise and beacon hits in DOMLauncher
 * and will unnecessarily blow up memory consumption.
 * 
 * Clips any PEs later than MaxDeltaT, taken with respect to the earliest
 * PE in the frame.
 */
class I3PrimaryPulseMapper: public I3Module {
public:
  I3PrimaryPulseMapper(const I3Context& ctx);
  ~I3PrimaryPulseMapper(){};

  void Configure();
  void DAQ(I3FramePtr frame);
  void Finish(){};

private:
  std::string inputParticleIDMapName_;
  std::string outputParticleIDMapName_;
  std::string mcTreeName_;

  SET_LOGGER("I3PrimaryPulseMapper");
};

I3PrimaryPulseMapper::I3PrimaryPulseMapper(const I3Context& ctx) :
  I3Module(ctx),
  inputParticleIDMapName_("I3MCPulseSeriesMapParticleIDMap"),
  outputParticleIDMapName_("I3MCPulseSeriesMapPrimaryIDMap"),
  mcTreeName_("I3MCTree")
{
  AddParameter("InputParticleIDMapname",
               "Name of I3ParticleIDMap to read",
               inputParticleIDMapName_);

  AddParameter("OutputParticleIDMapname",
               "Name of I3ParticleIDMap to write",
               outputParticleIDMapName_);

  AddParameter("I3MCTreeName",
               "Name of I3MCTree to get particle relations from",
               mcTreeName_);

  AddOutBox("OutBox");
}

void I3PrimaryPulseMapper::Configure()
{
    GetParameter("InputParticleIDMapname", inputParticleIDMapName_);
    GetParameter("OutputParticleIDMapname", outputParticleIDMapName_);
    GetParameter("I3MCTreeName", mcTreeName_);
}

void I3PrimaryPulseMapper::DAQ(I3FramePtr frame)
{
  if(!frame->Has(mcTreeName_)){ // push and return
    log_fatal("I3MCTree '%s' doesn't exist in the frame.", 
	mcTreeName_.c_str());
  }
  I3MCTreeConstPtr mctree = frame->Get<I3MCTreeConstPtr>(mcTreeName_);

  if(!frame->Has(inputParticleIDMapName_)){ // push and return
    log_fatal("ParticleIDMap'%s' doesn't exist in the frame.", 
	inputParticleIDMapName_.c_str());
  } 
  I3ParticleIDMapConstPtr pIDMap = frame->Get<I3ParticleIDMapConstPtr>(inputParticleIDMapName_);

  I3ParticleIDParticleMap primary_map = buildPrimaryMap(mctree);
  I3ParticleIDMapPtr primaryPIDMap(new I3ParticleIDMap());

  //iterate over DOMs 
  for(I3ParticleIDMap::const_iterator domIt=pIDMap->begin(), domEnd=pIDMap->end();
	    domIt!=domEnd; domIt++){

	ParticlePulseIndexMap primary_pulse_map; 
	for(ParticlePulseIndexMap::const_iterator pIt=domIt->second.begin(), pEnd=domIt->second.end(); pIt!=pEnd; pIt++) 
	{ 
		I3ParticlePtr primary = getPrimary(primary_map, pIt->first); 
		if (primary) {
		    primary_pulse_map[primary->GetID()]=pIt->second; 
		} else { // source not found (noise)
		    primary_pulse_map[pIt->first]=pIt->second; 
		} 
	} 
	primaryPIDMap->insert(std::make_pair(domIt->first,primary_pulse_map));
  }

  frame->Put(outputParticleIDMapName_,primaryPIDMap);

  PushFrame(frame);
}

I3_MODULE(I3PrimaryPulseMapper);


