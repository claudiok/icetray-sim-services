import sys
from icecube import icetray

try:
    icetray.load('libBadDomList') #, False)
except RuntimeError, err:
    sys.stderr.write("ERROR: Could not load libBadDomList (%s)." % err)

del icetray
del sys
