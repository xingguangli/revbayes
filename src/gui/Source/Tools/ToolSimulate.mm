#define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0

#import "GuiTree.h"
#import "InOutlet.h"
#import "Node.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolSimulate.h"
#import "WindowControllerSimulate.h"
#import "WindowControllerSimulateQuery.h"

#include <iostream>
#include "DiscreteTaxonData.h"
#include "DistributionExponential.h"
#include "DistributionGamma.h"
#include "DistributionUniform.h"
#include "DnaState.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbStatisticsHelper.h"



@implementation ToolSimulate

@synthesize simTree;
@synthesize treeLength;
@synthesize alpha;
@synthesize rAC;
@synthesize rAG;
@synthesize rAT;
@synthesize rCG;
@synthesize rCT;
@synthesize rGT;
@synthesize piA;
@synthesize piC;
@synthesize piG;
@synthesize piT;
@synthesize sequenceLength;

- (void)awakeFromNib {

}

- (char)charState:(int)x {

    if (x == 0)
        return 'A';
    else if (x == 1)
        return 'C';
    else if (x == 2)
        return 'G';
    else if (x == 3)
        return 'T';
    return 'N';
}

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeQueryPanel {

    [NSApp stopModal];
	[queryWindow close];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[super encodeWithCoder:aCoder];
    [aCoder encodeDouble:treeLength  forKey:@"treeLength"];
    [aCoder encodeDouble:alpha       forKey:@"alpha"];
    [aCoder encodeDouble:rAC         forKey:@"rAC"];
    [aCoder encodeDouble:rAG         forKey:@"rAG"];
    [aCoder encodeDouble:rAT         forKey:@"rAT"];
    [aCoder encodeDouble:rCG         forKey:@"rCG"];
    [aCoder encodeDouble:rCT         forKey:@"rCT"];
    [aCoder encodeDouble:rGT         forKey:@"rGT"];
    [aCoder encodeDouble:piA         forKey:@"piA"];
    [aCoder encodeDouble:piC         forKey:@"piC"];
    [aCoder encodeDouble:piG         forKey:@"piG"];
    [aCoder encodeDouble:piT         forKey:@"piT"];
    [aCoder encodeInt:sequenceLength forKey:@"sequenceLength"];
    [aCoder encodeObject:simTree     forKey:@"simTree"];
}

- (BOOL)execute {

    [self simulate];

    return [super execute];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize the inlet/outlet information
		//[self addInletOfColor:[NSColor blueColor]];
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize the model tree
        simTree = nil;
        
        // set parameters
        treeLength     = 5.0;
        alpha          = 0.5;
        rAC            = 1.0;
        rAG            = 1.0;
        rAT            = 1.0;
        rCG            = 1.0;
        rCT            = 1.0;
        rGT            = 1.0;
        piA            = 0.25;
        piC            = 0.25;
        piG            = 0.25;
        piT            = 0.25;
        sequenceLength = 100;

		// initialize the control window
		controlWindow = [[WindowControllerSimulate alloc] initWithTool:self];
        queryWindow = nil;
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

        treeLength     = [aDecoder decodeDoubleForKey:@"treeLength"];
        alpha          = [aDecoder decodeDoubleForKey:@"alpha"];
        rAC            = [aDecoder decodeDoubleForKey:@"rAC"];
        rAG            = [aDecoder decodeDoubleForKey:@"rAG"];
        rAT            = [aDecoder decodeDoubleForKey:@"rAT"];
        rCG            = [aDecoder decodeDoubleForKey:@"rCG"];
        rCT            = [aDecoder decodeDoubleForKey:@"rCT"];
        rGT            = [aDecoder decodeDoubleForKey:@"rGT"];
        piA            = [aDecoder decodeDoubleForKey:@"piA"];
        piC            = [aDecoder decodeDoubleForKey:@"piC"];
        piG            = [aDecoder decodeDoubleForKey:@"piG"];
        piT            = [aDecoder decodeDoubleForKey:@"piT"];
        sequenceLength = [aDecoder decodeIntForKey:@"sequenceLength"];
        simTree        = [aDecoder decodeObjectForKey:@"simTree"];

		// initialize the control window
		controlWindow  = [[WindowControllerSimulate alloc] initWithTool:self];
        queryWindow    = nil;

        [simTree setCoordinates:NO];
        [controlWindow setMyTree:simTree];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Simulate25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Simulate50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Simulate75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Simulate100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Simulate125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Simulate150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Simulate200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Simulate400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)makeTreeOfSize:(int)nt {

    simTree = [[GuiTree alloc] initWithTipSize:nt];
    [simTree initializeDownPassSequence];
    [simTree setCoordinates:NO];
    if (controlWindow != nil)
        [controlWindow setMyTree:simTree];
}

- (void)prepareForExecution {

}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Data Simulation Tool ";
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)showControlPanel {

    if (queryWindow != nil)
        {
        queryWindow = nil;
        }
    
    if (queryWindow == nil)
        {
        queryWindow = [[WindowControllerSimulateQuery alloc] initWithTool:self];
        NSPoint p = [self originForControlWindow:[queryWindow window]];
        [[queryWindow window] setFrameOrigin:p];
        [queryWindow showWindow:self];    
        [[queryWindow window] makeKeyAndOrderFront:nil];
        [NSApp runModalForWindow:[queryWindow window]];
        }
    
    if ( [queryWindow closedWithCancel] == YES )
        {
        queryWindow = nil;
        }
    else
        {
        if (simTree == nil)
            [self makeTreeOfSize:[queryWindow numTaxa]];
        else if (simTree != nil && [queryWindow whichTreeSourceMethod] == TREE_FROM_SIM)
            [self makeTreeOfSize:[queryWindow numTaxa]];
        NSPoint p = [self originForControlWindow:[controlWindow window]];
        [[controlWindow window] setFrameOrigin:p];
        [controlWindow showWindow:self];    
        [[controlWindow window] makeKeyAndOrderFront:nil];
        [NSApp runModalForWindow:[controlWindow window]];
        
        queryWindow = nil;
        }
}

- (void)simulate {

    NSLog(@"simTree = %@", simTree);
    if (simTree == nil)
        return;
    
    // get a pointer to the random number generator
    RevBayesCore::RandomNumberGenerator* rng = RevBayesCore::GLOBAL_RNG;
    
    // initialize some local parameters
    double r[6], f[4];
    r[0] = rAC;
    r[1] = rAG;
    r[2] = rAT;
    r[3] = rCG;
    r[4] = rCT;
    r[5] = rGT;
    f[0] = piA;
    f[1] = piC;
    f[2] = piG;
    f[3] = piT;
    
    // set up scaled rate matrix
    double q[4][4];
    for (int i=0, k=0; i<4; i++)
        {
        for (int j=i+1; j<4; j++)
            {
            q[i][j] = r[k] * f[j];
            q[j][i] = r[k] * f[i];
            k++;
            }
        }
    double aveRate = 0.0;
    for (int i=0; i<4; i++)
        {
        double sum = 0.0;
        for (int j=0; j<4; j++)
            {
            if (i != j)
                sum += q[i][j];
            }
        q[i][i] = -sum;
        aveRate += f[i] * sum;
        }
    double scaleFactor = 1.0 / aveRate;
    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++)
            q[i][j] *= scaleFactor;
            
    // set up a matrix to hold the data at the nodes of the tree
    int numNodes = [simTree numberOfNodes];
    int** m = new int*[numNodes];
    m[0] = new int[numNodes * sequenceLength];
    for (int i=1; i<numNodes; i++)
        m[i] = m[i-1] + sequenceLength;
    for (int i=0; i<numNodes; i++)
        for (int j=0; j<sequenceLength; j++)
            m[i][j] = 0;
            
    // calculate the sum of the branch lengths
    double branchLengthSum = 0.0;
    for (int n=0; n<[simTree numberOfNodes]; n++)
        {
        Node* p = [simTree downPassNodeIndexed:n];
        if ( [simTree isRoot:p] == NO )
            branchLengthSum += ([p y] - [[p ancestor] y]);
        }
            
    // simulate the sequences
    [simTree setInitializedDownPass:NO];
    [simTree initializeDownPassSequence];
    //[simTree print];
    for (int c=0; c<sequenceLength; c++)
        {
        double siteRate = RevBayesCore::RbStatistics::Gamma::rv(alpha, alpha, *rng);
        for (int n=[simTree numberOfNodes]-1; n>=0; n--)
            {
            Node* p = [simTree downPassNodeIndexed:n];
            if ( [simTree isRoot:p] == YES )
                {
                double u = RevBayesCore::RbStatistics::Uniform::rv(*rng);
                double sum = 0.0;
                for (int i=0; i<4; i++)
                    {
                    sum += f[i];
                    if (u < sum)
                        {
                        [p setState:i];
                        break;
                        }
                    }
                }
            else
                {
                double v = (([p y] - [[p ancestor] y]) * treeLength * siteRate) / branchLengthSum;
                double curT = 0.0;
                int curState = [[p ancestor] state];
                if (v < 0.00001)
                    {
                    // very short (zero) length branch
                    [p setState:curState];
                    }
                else
                    {
                    // worth simulating along the branch
                    while (curT < v)
                        {
                        double rate = -q[curState][curState];
                        curT += RevBayesCore::RbStatistics::Exponential::rv(rate, *rng);
                        if (curT < v)
                            {
                            double u = RevBayesCore::RbStatistics::Uniform::rv(*rng);
                            double sum = 0.0;
                            for (int j=0; j<4; j++)
                                {
                                if (curState != j)
                                    {
                                    sum += q[curState][j] / rate;
                                    if (u < sum)
                                        {
                                        curState = j;
                                        break;
                                        }
                                    }
                                }
                            }
                        [p setState:curState];
                        }
                    }
                }
                
            m[[p index]][c] = [p state];
            }
        }
    
#   if 0
    for (int i=0; i<numNodes; i++)
        {
        std::cout << i << " -- ";
        for (int j=0; j<sequenceLength; j++)
            {
            std::cout << m[i][j];
            }
        std::cout << std::endl;
        }
#   endif

    // create the character matrix (homology established)
    RbData* dm = [[RbData alloc] init];
    [dm setNumTaxa:[simTree numberOfTaxa]];
    [dm setNumCharacters:sequenceLength];
    [dm setDataType:DNA];
    [dm setIsHomologyEstablished:YES];
    [dm setName:@"Simulated Data Matrix"];
    [dm setStateLabels:@"ACGT"];
    for (int n=0; n<[simTree numberOfTaxa]; n++)
        {
        RbTaxonData* td = [[RbTaxonData alloc] init];
        [td setDataType:DNA];

        for (int j=0; j<sequenceLength; j++)
            {
            RbDataCell* c = [[RbDataCell alloc] init];
            [c setIsDiscrete:YES];
            [c setRow:n];
            [c setColumn:j];
            [c setDataType:DNA];
            [c setNumStates:4];
            [c setIsAmbig:NO];
            [c setIsGapState:NO];
            unsigned charIdx = m[n][j];
            [c setDiscreteStateTo:charIdx];
            [td addObservation:c];
            }
        [dm addTaxonName:[NSString stringWithFormat:@"Taxon_%d", n+1]];
        [td setTaxonName:[NSString stringWithFormat:@"Taxon_%d", n+1]];
        [dm addTaxonData:td];
        }

    //[dm print];

    if ([self numDataMatrices] > 0)
        [self removeAllDataMatrices];
    [self addMatrix:dm];
    if ( [dataMatrices count] > 0 )
        {
        [self makeDataInspector];
        }

    delete [] m[0];
    delete [] m;
}

- (NSString*)toolName {

    return @"Data Simulation";
}

- (void)updateForChangeInParent {

}

@end
