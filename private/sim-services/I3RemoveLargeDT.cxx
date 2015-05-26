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
 * $Id: I3RemoveLargeDT.cxx 108207 2013-07-13 15:19:02Z juancarlos $
 *
 * @file I3RemoveLargeDT.cxx
 * @version $Revision: $
 * @date $Date: $
 * @author Juan Carlos Diaz-Velez
 */

#include <icetray/I3Units.h>
#include "icetray/I3Module.h"
#include "simclasses/I3MCPE.h"

namespace{
  bool compare(const I3MCPE& lhs, const I3MCPE& rhs){
    if(lhs.time < rhs.time) return true;
    return false;
  }
}

/**
 * Clips any PEs later than MaxDeltaT, taken with respect to the earliest
 * PE in the frame.
 */
class I3RemoveLargeDT: public I3Module {
public:
  I3RemoveLargeDT(const I3Context& ctx);
  ~I3RemoveLargeDT(){};
  
  void Configure();
  void DAQ(I3FramePtr frame);
  void Finish(){};
  
private:
  std::string inputResponse_;
  std::string outputResponse_;
  double maxdt_;
  bool presorted_;
  
  SET_LOGGER("I3RemoveLargeDT");
};

I3RemoveLargeDT::I3RemoveLargeDT(const I3Context& ctx) :
  I3Module(ctx),
  inputResponse_("I3MCPESeriesMap"),
  outputResponse_("CleanedI3MCPESeriesMap"),
  maxdt_(100*I3Units::ms),
  presorted_(true)
{
  AddParameter("MaxDeltaT",
               "Maximum gap between adjecent hits",
               maxdt_); 
  AddParameter("InputResponse",
               "Name of the input response series",
               inputResponse_); 
  
  AddParameter("OutputResponse",
               "Name of the output response series",
               outputResponse_); 
  
  AddParameter("PreSorted",
               "PEs are already sorted in time",
               presorted_); 
  
  AddOutBox("OutBox");
}

void I3RemoveLargeDT::Configure()
{
    GetParameter("MaxDeltaT", maxdt_); 
    GetParameter("InputResponse", inputResponse_); 
    GetParameter("OutputResponse", outputResponse_); 
    GetParameter("PreSorted", presorted_); 
}

void I3RemoveLargeDT::DAQ(I3FramePtr frame)
{
    I3MCPESeriesMapConstPtr input = frame->Get<I3MCPESeriesMapConstPtr>(inputResponse_);
    if(!input)
    {
            log_fatal("Frame is missing input response");
    }

    // determine the earliest and latest hits
    // also sorting in-place, which is why we're iterating
    // over the output map, which at this point is just a 
    // copy of the input map.
    double earliest_time(input->begin()->second.front().time);
    double latest_time(input->begin()->second.front().time);
    I3MCPESeriesMapPtr output(new I3MCPESeriesMap(*input));
    for (I3MCPESeriesMap::iterator map_iter = output->begin(); 
         map_iter != output->end(); map_iter++){ 

      if (!presorted_) { 
        std::sort(map_iter->second.begin(), map_iter->second.end(), compare); 
      }

      if(map_iter->second.front().time < earliest_time){ 
        earliest_time = map_iter->second.front().time;
      }

      if(map_iter->second.back().time > latest_time){ 
        latest_time = map_iter->second.back().time;
      }
    }

    if(latest_time - earliest_time > maxdt_){
      // need to remove anything later than maxdt_
      // otherwise there's nothing to do
    
      for (I3MCPESeriesMap::iterator map_iter = output->begin();
           map_iter != output->end(); map_iter++){

        // if the latest PE is before maxdt_, the whole series is
        if(map_iter->second.back().time - earliest_time < maxdt_)
          continue; // go to the next DOM
        
        // if the earliest PE is after maxdt_, the whole series is
        if(map_iter->second.front().time - earliest_time > maxdt_){
          map_iter->second.clear();
          continue; // go to the next DOM
        }

        // at this point maxdt_ is within the [front, back], so
        // should be safe to pop off the back until all of the PEs
        // are less than maxdt_
        while(map_iter->second.back().time - earliest_time > maxdt_)
          map_iter->second.pop_back();
      }        
    }

    // In case we are overwriting object
    if (outputResponse_ == inputResponse_) {
      frame->Put("old"+inputResponse_,input);
      frame->Delete(inputResponse_);
    }
    frame->Put(outputResponse_, output);
    PushFrame(frame,"OutBox");
}      

I3_MODULE(I3RemoveLargeDT);


