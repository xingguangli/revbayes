#import "ParmVectorIntegerPos.h"
#import "RevBayes.h"



@implementation ParmVectorIntegerPos

- (void)encodeWithCoder:(NSCoder *)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {
    
    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize some variables
        parmType   = PARM_VECTOR_INTEGERPOS;
        domain     = VECTOR_POSITIVE_INTEGER + POSITIVE_INTEGER;
        isVector   = YES;
        dimensions = 2;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		}
	return self;
}

- (NSString*)xibName {
    
	NSString* xn = @"ControlWindowRV";
	return xn;
}

@end
