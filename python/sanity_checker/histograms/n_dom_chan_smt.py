import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins

from icecube.dataclasses.trigger_hierarchy_recipes import n_triggers

def _frame_op(frame):
    if "I3TriggerHierarchy" in frame \
           and "InIceRawData" in frame :
        th = frame["I3TriggerHierarchy"]
        if n_triggers(th, sourceID = dataclasses.I3Trigger.IN_ICE,
                      typeID = dataclasses.I3Trigger.SIMPLE_MULTIPLICITY) > 0 :
            return len(frame["InIceRawData"])
        else:
            return -1
    return -1
    
_draw_args = { "bins" : bins(200,0,200),
               "label" : "NChan",
               "title" : "N Channels (DOM Launch) - SMT",
               "figname" : "n_dom_chan_smt.png",
               "log" : True }

n_dom_chan_smt_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args)
