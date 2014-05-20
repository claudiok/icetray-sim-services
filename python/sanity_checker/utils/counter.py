class Counter :
    def __init__( self, tolerance = 100 ):
        self.failure_msg = "i can't tolerate this anymore"
        self._counter = 0
        self._tolerance = tolerance

    def assert_true( self, cond ):
        self._counter = 0 if cond else self._counter + 1

    def check(self):
        return self._counter <= self._tolerance 

    def __getstate__( self ):

        state = { "failure_msg" : self.failure_msg , \
                  "_counter" : self._counter , \
                  "_tolerance" :  self._tolerance }
        return state
