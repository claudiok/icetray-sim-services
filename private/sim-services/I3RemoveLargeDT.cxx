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

/**
 * Combines several I3MCPEHitSeriesMaps into one.
 */
class I3RemoveLargeDT: public I3Module
{
public:
    I3RemoveLargeDT(const I3Context& ctx);
    virtual ~I3RemoveLargeDT();

    virtual void Configure();
    virtual void DAQ(I3FramePtr frame);
    virtual void Finish();

private:
    std::string inputResponse_;
    std::string outputResponse_;
    double maxdt_;

    static bool compare(const I3MCPE& lhs, const I3MCPE& rhs)
    {
        if(lhs.time < rhs.time) return true;
        return false;
    }

    static bool compare_hitpairs(const std::pair<OMKey,double>& lhs, const std::pair<OMKey,double>& rhs)
    {
        if(lhs.second < rhs.second) return true;
        return false;
    }



    SET_LOGGER("I3RemoveLargeDT");
};

I3RemoveLargeDT::I3RemoveLargeDT(const I3Context& ctx) :
I3Module(ctx),
inputResponse_("I3MCPESeriesMap"),
outputResponse_("CleanedI3MCPESeriesMap"),
maxdt_(100*I3Units::ms)
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

    AddOutBox("OutBox");
}

I3RemoveLargeDT::~I3RemoveLargeDT()
{

}

void I3RemoveLargeDT::Configure()
{
    GetParameter("MaxDeltaT", maxdt_); 
    GetParameter("InputResponse", inputResponse_); 
    GetParameter("OutputResponse", outputResponse_); 
}

void I3RemoveLargeDT::DAQ(I3FramePtr frame)
{
    I3MCPESeriesMapPtr output(new I3MCPESeriesMap);
    I3MCPESeriesMapPtr tmp(new I3MCPESeriesMap);
    std::vector< std::pair<OMKey,double> > hitpairs;

    I3MCPESeriesMapConstPtr input = frame->Get<I3MCPESeriesMapConstPtr>(inputResponse_);
    if(!input)
    {
            log_fatal("Frame is missing input response");
    }

    // First we copy the map to tmp in order to make sure the hits are time-sorted 
    for (I3MCPESeriesMap::const_iterator map_iter = input->begin();
             map_iter != input->end(); map_iter++)
    {
            const OMKey& omkey = map_iter->first; 
            const I3MCPESeries &pe_series = map_iter->second;

            for (I3MCPESeries::const_iterator series_iter = pe_series.begin();
                 series_iter != pe_series.end(); ++series_iter)
            {
                // this will insert an empty vector automatically in case there is none
                (*tmp)[omkey].push_back(*series_iter);
            }

    }

    // Make sure the resultant reponses are in time order
    for (I3MCPESeriesMap::iterator map_iter = tmp->begin();
         map_iter != tmp->end(); map_iter++)
    {
        I3MCPESeries::iterator beginning = map_iter->second.begin();
        I3MCPESeries::iterator ending = map_iter->second.end();
        std::sort(beginning,ending,compare);
    }

    for (I3MCPESeriesMap::const_iterator map_iter = tmp->begin();
	 map_iter != tmp->end(); map_iter++)
        {
            const OMKey& omkey = map_iter->first; 
            const I3MCPESeries &pe_series = map_iter->second;

            // get initial hit for OM and store it in map for later comparison
            double tt = pe_series.begin()->time;
            hitpairs.push_back( std::make_pair(omkey,tt) );

            for (I3MCPESeries::const_iterator series_iter = pe_series.begin();
                 series_iter != pe_series.end(); ++series_iter)
            {

	      if(series_iter->time - tt > maxdt_) 
		break; // all remaining hits happen too late
	      (*output)[omkey].push_back(*series_iter);
	      tt = series_iter->time;
            }
        }
    std::vector< std::pair<OMKey,double> >::iterator pairiter;
    std::vector< std::pair<OMKey,double> >::iterator firstpair = hitpairs.begin();
    std::vector< std::pair<OMKey,double> >::iterator lastpair  = hitpairs.end();
    std::sort(firstpair,lastpair,compare_hitpairs);

    double ttt = hitpairs.begin()->second;
    for (pairiter = hitpairs.begin(); 
		    pairiter != hitpairs.end(); pairiter++)
    {
         const OMKey& omkey = pairiter->first;
         const double t = pairiter->second;
         if ( t-ttt > maxdt_ ) {
		 (*output)[omkey] = I3MCPESeries();
	 }
	 else {
            ttt = t;
	 }
    }
 
    // In case we are ovewritting object
    if (outputResponse_ == inputResponse_) {
      frame->Put("old"+inputResponse_,input);
      frame->Delete(inputResponse_);
    }
    frame->Put(outputResponse_, output);
    PushFrame(frame,"OutBox");
}      

void I3RemoveLargeDT::Finish()
{

}


I3_MODULE(I3RemoveLargeDT);


