 /**
 * class: I3TWRParamFiller
 *
 * Version $Id: $
 *
 *
 * (c) 2006 IceCube Collaboration
 * @file I3TWRParamFiller.cxx
 * @date $Date: $
 * @author wikstrom
 **/

#include "sim-services/sim-source/I3TWRParamFiller.h"
#include "amanda-core/ChannelIDService/I3TextFileChannelID2OMKey.h"
#include <fstream>

using namespace std;

void I3TWRParamFiller::FillElecConstants(string elec_file, 
					 string cidfile,
					 I3MCTWRParamsMapPtr params_map)
{
  I3TextFileChannelID2OMKey cid2omk(cidfile);

  ifstream ifs;
  ifs.open(elec_file.c_str(),ifstream::in);
  if(!ifs.good()) log_fatal("couldn't open file %s",elec_file.c_str());

  char newblob[256];
  string newblob2;
  int channum;
  double maxamp, thresh, noise, type, relsens, app, apdt;
  ifs >> newblob;
  newblob2 = newblob;
  while(!ifs.eof()){
    while (newblob2.find("....") > newblob2.size()){
      ifs >> newblob;
      newblob2=newblob;
    }
    ifs >> channum;
    ifs >> thresh;
    ifs >> maxamp;
    ifs >> newblob;
    ifs >> noise;
    ifs >> relsens;
    ifs >> app;
    ifs >> apdt;
    ifs >> type;

    OMKey key = cid2omk.GetOMKey(channum);
    I3MCTWRParams& params = (*params_map)[key];

    params.TWR_thresh = static_cast<int>(thresh);
    params.amplitude = maxamp;
    params.noise_rate = noise;
    params.wf_type = static_cast<int>(type);
    params.optical = IsOptical(params.wf_type);
    params.afterpulse_prob = app;
    params.afterpulse_time = apdt;
    params.rel_sens = relsens;
    params.stop_delay = 0;
    params.DMADD_thresh = 0;
    params.cable_delay = 0;

    for (int i = 0; i < 9; i++){
      ifs >> newblob;
    }
  }

  ifs.close();
}

void I3TWRParamFiller::FillStopDelay(string filename, I3MCTWRParamsMapPtr params_map){
  //fill external stop delay from file
  ifstream ifs;
  int string, om, stopd;
  ifs.open(filename.c_str(),ifstream::in);
  if(!ifs.good()) log_fatal("couldn't open file %s",filename.c_str());

  while(!ifs.eof()){
    ifs >> string;
    ifs >> om;
    ifs >> stopd;
    OMKey key(string,om);
    (*params_map)[key].stop_delay = stopd;
  }
  ifs.close();
}

void I3TWRParamFiller::FillDMADDThreshold(string filename, I3MCTWRParamsMapPtr params_map){
  //fill DMADD threshold from file
  ifstream ifs;
  int string,om;
  int thr;
  ifs.open(filename.c_str(),ifstream::in);
  if(!ifs.good()) log_fatal("couldn't open file %s",filename.c_str());

  while(!ifs.eof()){
    ifs >> string;
    ifs >> om;
    ifs >> thr;
    OMKey key(string,om);
    (*params_map)[key].DMADD_thresh = thr;
  }
  ifs.close();
}

void I3TWRParamFiller::FillCableDelay(string filename, 
				      string cidfile,
				      I3MCTWRParamsMapPtr params_map){
  //fill cable delay from file
  I3TextFileChannelID2OMKey cid2omk(cidfile);
  ifstream ifs;
  int channum;
  double cable_delay;
  ifs.open(filename.c_str(),ifstream::in);
  if(!ifs.good()) log_fatal("couldn't open file %s",filename.c_str());

  while(!ifs.eof()){
    ifs >> channum;
    ifs >> cable_delay;
    OMKey key = cid2omk.GetOMKey(channum);
    (*params_map)[key].cable_delay = cable_delay;
  }
  ifs.close();
}

void I3TWRParamFiller::FillRelativeSensitivity(string filename, I3MCTWRParamsMapPtr params_map){
  ifstream ifs;
  int string,om;
  double omeff_fac;
  ifs.open(filename.c_str(),ifstream::in);
  if(!ifs.good()) log_fatal("couldn't open file %s",filename.c_str());

  while(!ifs.eof()){
    ifs >> string;
    ifs >> om;
    ifs >> omeff_fac;
    OMKey key(string,om);
    (*params_map)[key].rel_sens = omeff_fac;
  }
  ifs.close(); 
}

void I3TWRParamFiller::FillTWRCharge(string filename, I3MCTWRParamsMapPtr params_map){
  //Read TWR charge from file and put in amandaCal
  if(!filename.empty()){
    
    ifstream ifs;
    int str,om;
    double charge;
    ifs.open(filename.c_str(),ifstream::in);
    if(!ifs.good()) log_fatal("couldn't open file %s",filename.c_str());
    
    string str_in;
    while(!ifs.eof()){
      getline(ifs, str_in);
      istringstream line_in(str_in.c_str());
      line_in >> str;
      line_in >> om;
      line_in >> charge;
      
      OMKey omk(str,om);
      (*params_map)[omk].peArea = charge;

      //TWRCalibration twrCalib;      
      //twrCalib.peArea = charge;    
      //calibration->twrCal[omk] = twrCalib;
    }
    
  }
}
