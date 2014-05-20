from icecube.sim_services.sanity_checker.bases.sanity_checker import SanityChecker
from icecube.sim_services.sanity_checker.utils.counter import Counter


class LaunchChecker( SanityChecker ):
    def __init__(self) :
        tol = 100
        SanityChecker.__init__(self)
        self.i3object_counter = Counter( tolerance = tol)
        self.i3object_counter.failure_msg = "%d frames with no waveforms!"%tol
        self.registry.append(self.i3object_counter)
        self.empty_wav_counter = Counter( tolerance = tol)
        self.empty_wav_counter.failure_msg = "%d frames with empty waveforms!"%tol
        self.registry.append(self.empty_wav_counter)
        self.zero_wav_counter = Counter( tolerance = tol)
        self.zero_wav_counter.failure_msg = "%d frames with waveforms equal to zero!"%tol
        self.registry.append(self.zero_wav_counter)
        self.launches = 'IceTopRawData'

    def check(self, frame):
        import numpy
        if self.launches in frame and frame[self.launches].keys() > 0:
            launches = frame[self.launches]
            non_empty = [l for k,ll in launches.iteritems() for l in ll if l.raw_atwd[0] or l.raw_atwd[1] or l.raw_atwd[2] or l.raw_fadc]
            self.empty_wav_counter.assert_true(len(non_empty)>0)
            zero_launches = [l for l in non_empty if numpy.all(l.raw_atwd[0]==0) and numpy.all(l.raw_atwd[1]==0) and numpy.all(l.raw_atwd[2]==0) and numpy.all(l.raw_fadc==0)]
            self.zero_wav_counter.assert_true(len(zero_launches)==0)
        else:
            self.i3object_counter.assert_true(False)
        return SanityChecker.check(self)

