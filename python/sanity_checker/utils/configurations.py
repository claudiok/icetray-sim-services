from icecube.icetray import I3Module
from icecube import dataclasses as dc

import sys

from ..modules.mctree_checkers import MCTreeSCModule
from ..modules.mctree_checkers import InIceMCTreeSCModule
from ..modules.mctree_checkers import IceTopMCTreeSCModule

RunConfigurations = dict()
RunConfigurations["corsika"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["numu"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nue"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["nutau"] = [ MCTreeSCModule, InIceMCTreeSCModule ]
RunConfigurations["icetop"] = [ MCTreeSCModule, IceTopMCTreeSCModule ]

from ..histograms.primary_energy import primary_energy_h
from ..histograms.primary_type import primary_type_h
from ..histograms.primary_zenith import primary_zenith_h
from ..histograms.primary_cos_zenith import primary_cos_zenith_h
from ..histograms.secondary_type import secondary_type_h
from ..histograms.secondary_energy import secondary_energy_h
from ..histograms.secondary_zenith import secondary_zenith_h
from ..histograms.secondary_cos_zenith import secondary_cos_zenith_h
from ..histograms.cascade_energy import cascade_energy_h
from ..histograms.dom_start_time import dom_start_time_h
from ..histograms.dom_occup import dom_occup_h
from ..histograms.mcpes import mcpes_h
from ..histograms.pe_occup import pe_occup_h
from ..histograms.n_dom_chan import n_dom_chan_h
from ..histograms.n_dom_chan_smt import n_dom_chan_smt_h
from ..histograms.n_dom_chan_str import n_dom_chan_str_h
from ..histograms.n_dom_chan_gt import n_dom_chan_gt_h
from ..histograms.n_dom_chan_gm import n_dom_chan_gm_h
from ..histograms.n_pmt_chan import n_pmt_chan_h
from ..histograms.n_mcpe_chan import n_mcpe_chan_h
from ..histograms.n_string_ic import n_string_ic_h
from ..histograms.mu_length import mu_length_h
from ..histograms.n_mu import n_mu_h
from ..histograms.n_mcpulses import n_mcpulses_h
from ..histograms.mcpe_time import mcpe_time_h
from ..histograms.trig_source import trig_source_h
from ..histograms.trig_ICtypeid import trig_ICtypeid_h

histograms = list()
histograms.append(primary_energy_h)
histograms.append(primary_type_h)
histograms.append(primary_zenith_h)
histograms.append(primary_cos_zenith_h)
histograms.append(secondary_type_h)
histograms.append(secondary_energy_h)
histograms.append(secondary_zenith_h)
histograms.append(secondary_cos_zenith_h)
histograms.append(cascade_energy_h)
histograms.append(dom_start_time_h)
histograms.append(dom_occup_h)
histograms.append(mcpes_h)
histograms.append(pe_occup_h)
histograms.append(n_dom_chan_h)
histograms.append(n_dom_chan_smt_h)
histograms.append(n_dom_chan_str_h)
histograms.append(n_dom_chan_gt_h)
histograms.append(n_dom_chan_gm_h)
histograms.append(n_pmt_chan_h)
histograms.append(n_mcpe_chan_h)
histograms.append(n_string_ic_h)
histograms.append(mu_length_h)
histograms.append(n_mu_h)
histograms.append(n_mcpulses_h)
histograms.append(mcpe_time_h)
histograms.append(trig_source_h)
histograms.append(trig_ICtypeid_h)

HistogramConfigurations = dict()
HistogramConfigurations["corsika"] = histograms 
HistogramConfigurations["numu"] = histograms 
HistogramConfigurations["nue"] = histograms 
HistogramConfigurations["nutau"] = histograms 
HistogramConfigurations["icetop"] = histograms 
