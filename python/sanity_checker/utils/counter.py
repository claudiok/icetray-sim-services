class Counter :
    def __init__( self, tolerance = 100 ):
        self.failure_msg = "i can't tolerate this anymore"

        self._counter = 0
        self._tolerance = tolerance

    def test_condition( self, cond ):
        if cond :
            self._counter += 1
        else:
            self._counter = 0

    def is_within_tolerance(self):
        return self._counter < self._tolerance 




