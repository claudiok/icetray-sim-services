#include "sim-services/tweak-sources/I3TweakDOMStatusService.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "interfaces/I3GeometryService.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3_SERVICE_FACTORY(I3TweakDOMStatusService);

I3TweakDOMStatusService::I3TweakDOMStatusService(const I3Context& context) :
  I3ServiceFactory(context),
  oldServiceName_(I3DefaultName<I3DetectorStatusService>::value()),
  geometryServiceName_(I3DefaultName<I3GeometryService>::value()),
  tweakedServiceName_(I3DefaultName<I3TweakDOMStatusService>::value()),
  icetopLCWindowPre_(NAN),
  icetopLCWindowPost_(NAN),
  icetopHighGainVoltage_(NAN),
  icetopLowGainVoltage_(NAN),
  iniceLCWindowPre_(NAN),
  iniceLCWindowPost_(NAN),
  inice_LCSpan_(INT_MIN),
  icetop_LCSpan_(INT_MIN),
  iniceVoltage_(NAN),
  iniceTriggerMode_(INT_MIN),
  icetopHGTriggerMode_(INT_MIN),
  icetopLGTriggerMode_(INT_MIN),
  lcMode_inice_first_(INT_MIN),
  lcMode_inice_bulk_(INT_MIN),
  lcMode_inice_last_(INT_MIN),
  lcMode_icetop_(INT_MIN),
  statusATWDa_(INT_MIN),
  statusATWDb_(INT_MIN),
  statusFADC_InIce_(INT_MIN),
  statusFADC_IceTop_(INT_MIN),
  speThreshold_(NAN),
  fePedestal_(NAN),
  dacTriggerBias0_(INT_MIN),
  dacTriggerBias1_(INT_MIN),
  dacFADCRef_(INT_MIN),
  nBinsATWD0_InIce_(INT_MIN),
  nBinsATWD1_InIce_(INT_MIN),
  nBinsATWD2_InIce_(INT_MIN),
  nBinsFADC_InIce_(INT_MIN),
  nBinsATWD0_IceTop_(INT_MIN),
  nBinsATWD1_IceTop_(INT_MIN),
  nBinsATWD2_IceTop_(INT_MIN),
  nBinsFADC_IceTop_(INT_MIN)
{
  AddParameter("OldServiceName","Name of service to tweak",oldServiceName_);
  AddParameter("GeometryServiceName","Name of geometry service",geometryServiceName_);
  AddParameter("TweakedServiceName","Name of tweaked service",tweakedServiceName_);
  AddParameter("IceTopLCWindowPre","",icetopLCWindowPre_);
  AddParameter("IceTopLCWindowPost","",icetopLCWindowPost_);
  AddParameter("IceTopHighGainVoltage","",icetopHighGainVoltage_);
  AddParameter("IceTopLowGainVoltage","",icetopLowGainVoltage_);
  AddParameter("InIceLCWindowPre","",iniceLCWindowPre_);
  AddParameter("InIceLCWindowPost","",iniceLCWindowPost_);
  AddParameter("InIceLCSpan","",inice_LCSpan_);
  AddParameter("IceTopLCSpan","",icetop_LCSpan_);
  AddParameter("InIceVoltage","",iniceVoltage_);
  AddParameter("InIceTriggerMode","",iniceTriggerMode_);
  AddParameter("IcetopHGTriggerMode","",icetopHGTriggerMode_);
  AddParameter("IcetopLGTriggerMode","",icetopLGTriggerMode_);
  AddParameter("LCModeInIceFirstDOM","",lcMode_inice_first_);
  AddParameter("LCModeInIceBulkDOMs","",lcMode_inice_bulk_);
  AddParameter("LCModeInIceLastDOM","",lcMode_inice_last_);
  AddParameter("LCModeIceTopDOMs","",lcMode_icetop_);
  AddParameter("StatusATWDa","",statusATWDa_);
  AddParameter("StatusATWDb","",statusATWDb_);
  AddParameter("StatusFADCInIce","",statusFADC_InIce_);
  AddParameter("StatusFADCIceTop","",statusFADC_IceTop_);
  AddParameter("SPEThreshold","",speThreshold_);
  AddParameter("FEPedestal","",fePedestal_);
  AddParameter("DACTriggerBias0","",dacTriggerBias0_);
  AddParameter("DACTriggerBias1","",dacTriggerBias1_);
  AddParameter("DACFADCRef","",dacFADCRef_);
  AddParameter("NBinsATWD0InIce","",nBinsATWD0_InIce_);
  AddParameter("NBinsATWD1InIce","",nBinsATWD1_InIce_);
  AddParameter("NBinsATWD2InIce","",nBinsATWD2_InIce_);
  AddParameter("NBinsFADCInIce","",nBinsFADC_InIce_);
  AddParameter("NBinsATWD0IceTop","",nBinsATWD0_IceTop_);
  AddParameter("NBinsATWD1IceTop","",nBinsATWD1_IceTop_);
  AddParameter("NBinsATWD2IceTop","",nBinsATWD2_IceTop_);
  AddParameter("NBinsFADCIceTop","",nBinsFADC_IceTop_);
}


void I3TweakDOMStatusService::Configure()
{  
  GetParameter("OldServiceName",oldServiceName_);
  GetParameter("GeometryServiceName",geometryServiceName_);
  GetParameter("TweakedServiceName",tweakedServiceName_);
  GetParameter("IceTopLCWindowPre",icetopLCWindowPre_);
  GetParameter("IceTopLCWindowPost",icetopLCWindowPost_);
  GetParameter("IceTopHighGainVoltage",icetopHighGainVoltage_);
  GetParameter("IceTopLowGainVoltage",icetopLowGainVoltage_);
  GetParameter("InIceLCWindowPre",iniceLCWindowPre_);
  GetParameter("InIceLCWindowPost",iniceLCWindowPost_);
  GetParameter("InIceLCSpan",inice_LCSpan_);
  GetParameter("IceTopLCSpan",icetop_LCSpan_);
  GetParameter("InIceVoltage",iniceVoltage_);
  GetParameter("InIceTriggerMode",iniceTriggerMode_);
  GetParameter("IcetopHGTriggerMode",icetopHGTriggerMode_);
  GetParameter("IcetopLGTriggerMode",icetopLGTriggerMode_);
  GetParameter("LCModeInIceFirstDOM",lcMode_inice_first_);
  GetParameter("LCModeInIceBulkDOMs",lcMode_inice_bulk_);
  GetParameter("LCModeInIceLastDOM",lcMode_inice_last_);
  GetParameter("LCModeIceTopDOMs",lcMode_icetop_);
  GetParameter("StatusATWDa",statusATWDa_);
  GetParameter("StatusATWDb",statusATWDb_);
  GetParameter("StatusFADCInIce",statusFADC_InIce_);
  GetParameter("StatusFADCIceTop",statusFADC_IceTop_);
  GetParameter("SPEThreshold",speThreshold_);
  GetParameter("FEPedestal",fePedestal_);
  GetParameter("DACTriggerBias0",dacTriggerBias0_);
  GetParameter("DACTriggerBias1",dacTriggerBias1_);
  GetParameter("DACFADCRef",dacFADCRef_);
  GetParameter("NBinsATWD0InIce",nBinsATWD0_InIce_);
  GetParameter("NBinsATWD1InIce",nBinsATWD1_InIce_);
  GetParameter("NBinsATWD2InIce",nBinsATWD2_InIce_);
  GetParameter("NBinsFADCInIce",nBinsFADC_InIce_);
  GetParameter("NBinsATWD0IceTop",nBinsATWD0_IceTop_);
  GetParameter("NBinsATWD1IceTop",nBinsATWD1_IceTop_);
  GetParameter("NBinsATWD2IceTop",nBinsATWD2_IceTop_);
  GetParameter("NBinsFADCIceTop",nBinsFADC_IceTop_);
}

bool I3TweakDOMStatusService::InstallService(I3Context& services)
{
  if(!status_service_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>(oldServiceName_);

    cout<<"  name = "<<geometryServiceName_<<endl;
    I3GeometryServicePtr geo_service = context_.Get<I3GeometryServicePtr>(geometryServiceName_);

    status_service_ = 
      I3TweakDOMStatusPtr
      (new I3TweakDOMStatus(old_status,geo_service));
    log_debug("Made new I3TweakDOMStatusService.");
    //Configure the calibration
    if(!isnan(icetopLCWindowPre_))
      status_service_->SetIceTopLCWindowPre(icetopLCWindowPre_);
    if(!isnan(icetopLCWindowPost_))
      status_service_->SetIceTopLCWindowPost(icetopLCWindowPost_);
    if(!isnan(icetopHighGainVoltage_))
      status_service_->SetIceTopHighGainVoltage(icetopHighGainVoltage_);
    if(!isnan(icetopLowGainVoltage_))
      status_service_->SetIceTopLowGainVoltage(icetopLowGainVoltage_);
    if(!isnan(iniceLCWindowPre_))
      status_service_->SetInIceLCWindowPre(iniceLCWindowPre_);
    if(!isnan(iniceLCWindowPost_))
      status_service_->SetInIceLCWindowPost(iniceLCWindowPost_);
    if(inice_LCSpan_ > INT_MIN)
      status_service_->SetInIceLCSpan(inice_LCSpan_);
    if(icetop_LCSpan_ > INT_MIN)
      status_service_->SetIceTopLCSpan(icetop_LCSpan_);
    if(!isnan(iniceVoltage_))
      status_service_->SetInIceVoltage(iniceVoltage_);
    if(iniceTriggerMode_ > INT_MIN)
      status_service_->SetInIceTriggerMode(static_cast<I3DOMStatus::TrigMode>(iniceTriggerMode_));
    if(icetopHGTriggerMode_ > INT_MIN)
      status_service_->SetIceTopHGTriggerMode(static_cast<I3DOMStatus::TrigMode>(icetopHGTriggerMode_));
    if(icetopLGTriggerMode_ > INT_MIN)
      status_service_->SetIceTopLGTriggerMode(static_cast<I3DOMStatus::TrigMode>(icetopLGTriggerMode_));

    if(lcMode_inice_first_ > INT_MIN)
      status_service_->SetLCModeInIceFirstDOM(static_cast<I3DOMStatus::LCMode>(lcMode_inice_first_));
    if(lcMode_inice_bulk_ > INT_MIN)
      status_service_->SetLCModeInIceBulkDOMs(static_cast<I3DOMStatus::LCMode>(lcMode_inice_bulk_));
    if(lcMode_inice_last_ > INT_MIN)
      status_service_->SetLCModeInIceLastDOM(static_cast<I3DOMStatus::LCMode>(lcMode_inice_last_));
    if(lcMode_icetop_ > INT_MIN)
      status_service_->SetLCModeIceTopDOMs(static_cast<I3DOMStatus::LCMode>(lcMode_icetop_));

    if(statusATWDa_ > INT_MIN)
      status_service_->SetStatusATWDa(static_cast<I3DOMStatus::OnOff>(statusATWDa_));
    if(statusATWDb_ > INT_MIN)
      status_service_->SetStatusATWDb(static_cast<I3DOMStatus::OnOff>(statusATWDb_));
    if(statusFADC_InIce_ > INT_MIN)
      status_service_->SetStatusFADCInIce(static_cast<I3DOMStatus::OnOff>(statusFADC_InIce_));
    if(statusFADC_IceTop_ > INT_MIN)
      status_service_->SetStatusFADCIceTop(static_cast<I3DOMStatus::OnOff>(statusFADC_IceTop_));
    if(!isnan(speThreshold_))
      status_service_->SetSPEThreshold(speThreshold_);
    if(!isnan(fePedestal_))
      status_service_->SetFEPedestal(fePedestal_);
    if(dacTriggerBias0_ > INT_MIN)
      status_service_->SetDACTriggerBias0(dacTriggerBias0_);
    if(dacTriggerBias1_ > INT_MIN)
      status_service_->SetDACTriggerBias1(dacTriggerBias1_);
    if(dacFADCRef_ > INT_MIN)
      status_service_->SetDACFADCRef(dacFADCRef_);
    if(nBinsATWD0_InIce_ > INT_MIN)
      status_service_->SetNBinsATWD0InIce(nBinsATWD0_InIce_);
    if(nBinsATWD1_InIce_ > INT_MIN)
      status_service_->SetNBinsATWD1InIce(nBinsATWD1_InIce_);
    if(nBinsATWD2_InIce_ > INT_MIN)
      status_service_->SetNBinsATWD2InIce(nBinsATWD2_InIce_);
    if(nBinsFADC_InIce_ > INT_MIN)
      status_service_->SetNBinsFADCInIce(nBinsFADC_InIce_);
    if(nBinsATWD0_IceTop_ > INT_MIN)
      status_service_->SetNBinsATWD0IceTop(nBinsATWD0_IceTop_);
    if(nBinsATWD1_IceTop_ > INT_MIN)
      status_service_->SetNBinsATWD1IceTop(nBinsATWD1_IceTop_);
    if(nBinsATWD2_IceTop_ > INT_MIN)
      status_service_->SetNBinsATWD2IceTop(nBinsATWD2_IceTop_);
    if(nBinsFADC_IceTop_ > INT_MIN)
      status_service_->SetNBinsFADCIceTop(nBinsFADC_IceTop_);
  }

  bool good_status = services.Put<I3DetectorStatusService>(tweakedServiceName_,status_service_);
  log_debug("good_status %d",good_status);

  return good_status;
}



