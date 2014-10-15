/**
 * Copyright (c) 2014
 * Ben Jones <bjpjones@mit.edu>
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
 */

#include <string>
#include <vector>

#include "icetray/I3ConditionalModule.h"
#include <phys-services/I3RandomService.h>
#include "simclasses/I3MCPE.h"

/**
 * Randomly downsample MCPEs from one collection to another
 */
class I3DownsampleMCPE : public I3ConditionalModule
{
public:
    I3DownsampleMCPE(const I3Context& ctx);
    virtual ~I3DownsampleMCPE();

    virtual void Configure();
    virtual void DAQ(I3FramePtr frame);
    virtual void Finish();

private:
  std::string inputName_;
  std::string outputName_;
  double      sampleFrac_;
  I3RandomServicePtr rng_;


private:
    SET_LOGGER("I3DownsampleMCPE");
};


I3DownsampleMCPE::I3DownsampleMCPE(const I3Context& ctx) :
I3ConditionalModule(ctx),
outputName_("DownsampledMCPEs")
{
    AddParameter("InputName",
        "Name of the MCPE set to read in and downsample"
        "InputNameDefault"); 

    AddParameter("OutputName",
	"Name of the output MCPE series",
        "OutputNameDefault"); 

    AddParameter("SampleFrac",
	"Fraction of MCPEs to accept",
        0.95); 

    AddParameter("RandomService",
            "A random number generating service (derived from I3RandomService)."
		 " The default is to get it from the context.",
		 rng_);

    AddOutBox("OutBox");
}

I3DownsampleMCPE::~I3DownsampleMCPE()
{

}


void I3DownsampleMCPE::Configure()
{
    GetParameter("InputName", inputName_); 
    GetParameter("OutputName", outputName_); 
    GetParameter("SampleFrac", sampleFrac_); 
    GetParameter("RandomService", rng_);
    if (!rng_) rng_ = context_.Get<I3RandomServicePtr>();
    if (!rng_)
      log_fatal("No random service available. Please add one.");
    
    if((sampleFrac_<0)||(sampleFrac_>1))
      log_fatal("Sample fraction is not a number between 0 and 1.");
	
}

void I3DownsampleMCPE::DAQ(I3FramePtr frame)
{

    I3MCPESeriesMapPtr output(new I3MCPESeriesMap);
    I3MCPESeriesMapConstPtr input = frame->Get<I3MCPESeriesMapConstPtr>(inputName_);
    
    if(!input)
      {
	log_warn("Frame is missing input MCPE series. cannot downsample");
      }
    for (I3MCPESeriesMap::const_iterator map_iter = input->begin();
	 map_iter != input->end(); map_iter++)
        {
            const OMKey& omkey = map_iter->first; 
            const I3MCPESeries &pe_series = map_iter->second;

            for (I3MCPESeries::const_iterator series_iter = pe_series.begin();
                 series_iter != pe_series.end(); ++series_iter)
            {
	      if( rng_->Uniform(1) < sampleFrac_ )
		(*output)[omkey].push_back(*series_iter);
            }
        }
    
    
    frame->Put(outputName_, output);
    PushFrame(frame,"OutBox");
}      

void I3DownsampleMCPE::Finish()
{

}


I3_MODULE(I3DownsampleMCPE);