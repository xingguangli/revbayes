#import "DistributionUniformGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionUniformGui

- (DistributionGui*)clone {

    return [[DistributionUniformGui alloc] initWithDist:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
	// more here
}

- (id)init {

	return [self initWithParm:nil];
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{

		}
	return self;
}

- (id)initWithParm:(Parm*)p {

    if ( (self = [super initWithParm:p]) ) 
		{
		[self setDistributionName:@"Uniform"];
		[self setDistributionType:DISTRIBUTION_UNIFORM];
        [self setDistributionDomain:REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* lowerParm = [[Parameter alloc] initWithDist:self];
		[lowerParm setParameterName:@"Lower:"];
		[lowerParm setType:REAL_NUMBER];
		[lowerParm setRefParm:nil];

		Parameter* upperParm = [[Parameter alloc] initWithDist:self];
		[upperParm setParameterName:@"Upper:"];
		[upperParm setType:REAL_NUMBER];
		[upperParm setRefParm:nil];
		
		[parameters addObject:lowerParm];
		[parameters addObject:upperParm];
		}
    return self;
}

@end
