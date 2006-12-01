/**
 * Definition of I3MCDetectorStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCDetectorStatusService.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCDetectorStatusService.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3MCDETECTORSTATUSSERVICE_H
#define I3MCDETECTORSTATUSSERVICE_H

#include "phys-services/I3DetectorStatusService.h"
#include "phys-services/I3GeometryService.h"
#include "simclasses/I3MCTWRParams.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

class I3Geometry;
I3_POINTER_TYPEDEFS(I3Geometry);
/**
 *
 * @brief This service allows you to fill the detector status data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3MCDetectorStatusService : public I3DetectorStatusService
{
public:

  I3MCDetectorStatusService(I3GeometryServicePtr,I3DetectorStatusServicePtr);

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  void InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus);

  virtual ~I3MCDetectorStatusService(){};

  SET_LOGGER("I3MCDetectorStatusService");

  void SetStartYear(int32_t t){startYear_= t;};
  void SetStartDAQTime(int64_t t){startDAQTime_= t;};
  void SetEndYear(int32_t t){endYear_= t;};
  void SetEndDAQTime(int64_t t){endDAQTime_= t;};

  void SetIceTopLCWindowPre(double p){icetopLCWindowPre_= p;};
  void SetIceTopLCWindowPost(double p){icetopLCWindowPost_= p;};
  void SetIceTopHighGainVoltage(double v){icetopHighGainVoltage_= v;};
  void SetIceTopLowGainVoltage(double v){icetopLowGainVoltage_= v;};
  void SetInIceLCWindowPre(double p){iniceLCWindowPre_= p;};
  void SetInIceLCWindowPost(double p){iniceLCWindowPost_= p;};
  void SetLCSpan(int s){lcSpan_= s;};
  void SetInIceVoltage(double v){iniceVoltage_= v;};
  void SetTriggerMode(int m){triggerMode_= static_cast<I3DOMStatus::TrigMode>(m);};
  void SetLCMode(int m){lcMode_= static_cast<I3DOMStatus::LCMode>(m);};
  void SetStatusATWDa(int o){statusATWDa_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusATWDb(int o){statusATWDb_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusFADCInIce(int o){statusFADC_InIce_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusFADCIceTop(int o){statusFADC_IceTop_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetSPEThreshold(double t){speThreshold_= t;};
  void SetFEPedestal(double t){fePedestal_= t;};
  void SetDACTriggerBias0(int b){dacTriggerBias0_= b;};
  void SetDACTriggerBias1(int b){dacTriggerBias1_= b;};
  void SetDACFADCRef(int r){ dacFADCRef_ = r; };
  void SetNBinsATWD0InIce(int n){ nBinsATWD0_InIce_ = n; };
  void SetNBinsATWD1InIce(int n){ nBinsATWD1_InIce_ = n; };
  void SetNBinsATWD2InIce(int n){ nBinsATWD2_InIce_ = n; };
  void SetNBinsFADCInIce(int n){ nBinsFADC_InIce_ = n; };
  void SetNBinsATWD0IceTop(int n){ nBinsATWD0_IceTop_ = n; };
  void SetNBinsATWD1IceTop(int n){ nBinsATWD1_IceTop_ = n; };
  void SetNBinsATWD2IceTop(int n){ nBinsATWD2_IceTop_ = n; };
  void SetNBinsFADCIceTop(int n){ nBinsFADC_IceTop_ = n; };

  void SetMCTWRParamsMap(I3MCTWRParamsMapPtr p){ twrParamsMap_ = p;};
  void SetTWRBinSize(double b){twrBinSize_ = b; };
  void SetTWRBaseline(unsigned b){twrBaseline_ = b; };

 private:
  I3MCDetectorStatusService();
  I3GeometryServicePtr geo_service_;
  I3DetectorStatusServicePtr old_status_service_;
  I3MCTWRParamsMapPtr twrParamsMap_;

  void SetDOMStatus(I3DetectorStatusPtr, const I3OMGeoMap&);
  void SetAOMStatus(I3DetectorStatusPtr, const I3OMGeoMap&);

  shared_ptr<I3DetectorStatus> status_;
  map<TriggerKey, I3TriggerStatus> triggerStatus_;

  /**
   *Configuration parameters for detector status
   */

  /**
   *Start of the valid time range of the detector status
   */
  int32_t startYear_;
  int64_t startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t endYear_;
  int64_t endDAQTime_;

  /**
   * Icetop local coincidence trigger windows
   */
  double icetopLCWindowPre_;
  double icetopLCWindowPost_;

  /**
   * IceTop PMT voltages for high and low gain
   */
  double icetopHighGainVoltage_;
  double icetopLowGainVoltage_;

  /**
   * InIce local coincidence trigger windows
   */
  double iniceLCWindowPre_;
  double iniceLCWindowPost_;

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  int lcSpan_;

  /**
   * InIce PMT voltage
   */
  double iniceVoltage_;

  I3DOMStatus::TrigMode triggerMode_;
  I3DOMStatus::LCMode lcMode_;
  I3DOMStatus::OnOff statusATWDa_;
  I3DOMStatus::OnOff statusATWDb_;

  I3DOMStatus::OnOff statusFADC_InIce_;
  I3DOMStatus::OnOff statusFADC_IceTop_;

  double speThreshold_;
  double fePedestal_;
  int dacTriggerBias0_;
  int dacTriggerBias1_;
  int dacFADCRef_;

  /**
   *Number of FADC bins
   */
  int nBinsATWD0_InIce_;
  int nBinsATWD1_InIce_;
  int nBinsATWD2_InIce_;
  int nBinsFADC_InIce_;

  int nBinsATWD0_IceTop_;
  int nBinsATWD1_IceTop_;
  int nBinsATWD2_IceTop_;
  int nBinsFADC_IceTop_;

  double twrBinSize_;
  unsigned twrBaseline_;
};

I3_POINTER_TYPEDEFS(I3MCDetectorStatusService);
#endif
