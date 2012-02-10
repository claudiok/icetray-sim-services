class SCBaseModule :
    def __init__(self):
        self.registry = dict()

    # returns True if all is well
    def check( self ):

        all_is_well = True
        for k,c in self.registry.iteritems() :
            all_is_well = all_is_well and c.is_within_tolerance()
        return all_is_well

    def register( self, c_str ):
        self.registry[ c_str ] = self.__dict__[ c_str ] 

    def reset_registry( self ):
        for k,c in self.registry.iteritems() :
            c.reset()
        
    # what to do if there's a failure
    def fail( self ) :
        for k,c in self.registry.iteritems() :
            if not c.is_within_tolerance() :
                print "FAIL : ",c.failure_msg

    def __getstate__( self ) :
       return self.registry

    def __setstate__( self, state ) :
        self.registry = state
        self.__dict__.update( state )
