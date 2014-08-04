import numpy
from I3Tray import I3Units

def bins(nbins, min, max) :
    db = float(max-min)/float(nbins)
    return numpy.arange(min,max,db)

# Units of s^-1
def event_weight(frame) :

    if "I3MCWeightDict" in frame : 
        weightmap = frame["I3MCWeightDict"]
        datalivetime = frame["DataLiveTime"].value / I3Units.second
        return datalivetime * weightmap["OneWeight"] / weightmap["NEvents"]

    if "GaisserH3aWeight" in frame :
        return frame["GaisserH3aWeight"].value

    # This gives the best results, but I'm still off by factors of 2-3
    # depending on the channel.
    if "Hoerandel5" in frame :
        timescale = 1.
        if "CorsikaWeightMap" in frame :
            weightmap = frame["CorsikaWeightMap"]
            timescale = weightmap["TimeScale"]
        return frame["Hoerandel5"].value

    # This doesn't work for 5-component
    # I'm off by several orders of magnitude
    if "CorsikaWeightMap" in frame :
        weightmap = frame["CorsikaWeightMap"]
        timescale = weightmap["TimeScale"]
        if "Polygonato" in weightmap :
            return weightmap["Weight"] * weightmap["DiplopiaWeight"] * weightmap["Polygonato"] / timescale
        else :
            return weightmap["Weight"] * weightmap["DiplopiaWeight"] / timescale
    
    if "DataLiveTime" in frame :
        datalivetime = frame["DataLiveTime"].value / I3Units.second
        return 1./ datalivetime

    # unweighted
    return 1.
