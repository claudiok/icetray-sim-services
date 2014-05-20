import numpy
from I3Tray import I3Units

def bins(nbins, min, max) :
    db = float(max-min)/float(nbins)
    return numpy.arange(min,max,db)

# Units of s^-1
def event_weight(frame) :

    if "GaisserH3aWeight" in frame :
        return frame["GaisserH3aWeight"].value

    if "CorsikaWeightMap" in frame :
        weightmap = frame["CorsikaWeightMap"]
        timescale = weightmap["TimeScale"] / I3Units.second
        return weightmap["Weight"] * weightmap["DiplopiaWeight"] / timescale
    
    if "I3MCWeightDict" in frame : 
        weightmap = frame["I3MCWeightDict"]
        datalivetime = frame["DataLiveTime"].value / I3Units.second
        return datalivetime * weightmap["OneWeight"] / weightmap["NEvents"]

    if "DataLiveTime" in frame :
        datalivetime = frame["DataLiveTime"].value / I3Units.second
        return 1./ datalivetime

    # unweighted
    return 1.
