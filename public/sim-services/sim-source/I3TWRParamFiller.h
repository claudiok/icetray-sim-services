#ifndef TWRPARAMFILLER_H
#define TWRPARAMFILLER_H
/**
 * class: I3TWRParamFiller
 *
 * Version $Id: $
 *
 * date: $Date: $
 *
 * @author wikstrom
 *
 * (c) 2006 IceCube Collaboration
 */

#include "icetray/OMKey.h"
#include "simclasses//I3MCTWRParams.h"

typedef std::map<OMKey,I3MCTWRParams> I3MCTWRParamsMap;
I3_POINTER_TYPEDEFS(I3MCTWRParamsMap);

namespace I3TWRParamFiller 
{

  bool IsOptical(int);
  void FillElecConstants(std::string,std::string,I3MCTWRParamsMapPtr);
  void FillStopDelay(std::string,I3MCTWRParamsMapPtr);
  void FillDMADDThreshold(std::string,I3MCTWRParamsMapPtr);
  void FillCableDelay(std::string,std::string,I3MCTWRParamsMapPtr);
  void FillRelativeSensitivity(std::string,I3MCTWRParamsMapPtr);

};

inline bool I3TWRParamFiller::IsOptical(int wf_type){
  //these waveform types correspond to optical channels
  return ((wf_type==8) || 
	  (wf_type==10) || 
	  (wf_type==12) || 
	  (wf_type==13) );
}

#endif


