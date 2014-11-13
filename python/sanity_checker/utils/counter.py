class Counter :
    def __init__( self, tolerance = 100 ):
        self.failure_msg = "i can't tolerate this anymore"
        self.counter = 0
        self.tolerance = tolerance

    def assert_true( self, cond ):
        self.counter = 0 if cond else self.counter + 1

    def check(self):
        return self.counter <= self.tolerance 

    def __getstate__( self ):

        state = { "failure_msg" : self.failure_msg , \
                  "counter" : self.counter , \
                  "tolerance" :  self.tolerance }
        return state
