#include "sim-services/sim-source/I3MCTWRParamsService.h"
#include "sim-services/sim-source/I3TWRParamFiller.h"
#include "simclasses/I3MCTWRParams.h"

I3_SERVICE_FACTORY(I3MCTWRParamsService);

I3MCTWRParamsService::
I3MCTWRParamsService(const I3Context& context) : 
  I3ServiceFactory(context),
  serviceName_("I3MCTWRParamsMap")
{
  const string I3_BUILD(getenv("I3_BUILD"));
  elecFileName_ = I3_BUILD + "/sim-services/resources/tables/ama.elec.mc2005";
  cidFileName_ = I3_BUILD + "/amanda-core/resources/channel_ids.txt";
  stopDelayFileName_ = I3_BUILD + "/sim-services/resources/tables/stop_delay_mc.dat";
  //  dmaddThrFileName_ = I3_BUILD + "/sim-services/resources/tables/DMADD_thresh_final_2008_noMask.dat";
  dmaddThrFileName_ = I3_BUILD + "/sim-services/resources/tables/DMADD_thresh_2008_noMask_matchMC.dat";
  //  cableDelayFileName_ = I3_BUILD + "/sim-services/resources/tables/cable_delay.dat";
  cableDelayFileName_ = I3_BUILD + "/sim-services/resources/tables/cable_delay_tdy5.dat";
  relSensFileName_ = I3_BUILD + "/sim-services/resources/tables/omeff.dat";
  useTunedRelsens_ = false;

  twrChargeFile_ = I3_BUILD + "/sim-services/resources/tables/charge_calib_2007.dat";

  AddParameter("ElecFileName","Name of elec file",elecFileName_);
  AddParameter("ChannelIDFileName","Name of channel ID file",cidFileName_);
  AddParameter("StopDelayFileName","Name of stop delay file",stopDelayFileName_);
  AddParameter("DMADDThresholdFileName","Name of DMADD threshold file",dmaddThrFileName_);
  AddParameter("CableDelayFileName","Name of cable delay file",cableDelayFileName_);
  AddParameter("RelSensFileName","Name of relative sensitivity file",relSensFileName_);
  AddParameter("ServiceName","Name of service to install",serviceName_);
  AddParameter("UseTunedRelsens","To use sim-data tuned relative sensitivities",useTunedRelsens_);
  AddParameter("TWRChargeFile","Path to TWR charge file",twrChargeFile_);
}

I3MCTWRParamsService::
~I3MCTWRParamsService(){}

void I3MCTWRParamsService::Configure()
{
  GetParameter("ElecFileName",elecFileName_);
  GetParameter("ChannelIDFileName",cidFileName_);
  GetParameter("StopDelayFileName",stopDelayFileName_);
  GetParameter("DMADDThresholdFileName",dmaddThrFileName_);
  GetParameter("CableDelayFileName",cableDelayFileName_);
  GetParameter("RelSensFileName",relSensFileName_);
  GetParameter("ServiceName",serviceName_);
  GetParameter("UseTunedRelsens",useTunedRelsens_);
  GetParameter("TWRChargeFile",twrChargeFile_);

}

bool I3MCTWRParamsService::InstallService(I3Context& services)
{

  if(!twrParams_){
    twrParams_ = 
      shared_ptr<I3MCTWRParamsMap>
      (new I3MCTWRParamsMap());

    I3TWRParamFiller::FillElecConstants(elecFileName_, cidFileName_, twrParams_);
    I3TWRParamFiller::FillStopDelay(stopDelayFileName_, twrParams_);
    I3TWRParamFiller::FillDMADDThreshold(dmaddThrFileName_, twrParams_);
    I3TWRParamFiller::FillCableDelay(cableDelayFileName_, cidFileName_, twrParams_);
    I3TWRParamFiller::FillTWRCharge(twrChargeFile_, twrParams_);
    if(useTunedRelsens_) I3TWRParamFiller::FillRelativeSensitivity(relSensFileName_, twrParams_);

    log_debug("Made new I3MCTWRParams.");
  }

  //Only install a new service if the use has specified a name
  bool good = services.Put<I3MCTWRParamsMap>(serviceName_,twrParams_);
  log_debug("good %d",good);

  return good;	  
}

