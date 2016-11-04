#import <Cocoa/Cocoa.h>
#import "RbItem.h"
@class AnalysisView;
@class Inlet;
@class InOutlet;
@class Outlet;
@class YRKSpinningProgressIndicator;


@interface Tool : RbItem <NSCoding> {

    AnalysisView*                      myAnalysisView;
    NSMutableArray*                    inlets;
    NSMutableArray*                    outlets;
	int                                flagCount;
	BOOL                               touchOnRevival;
    BOOL                               isVisited;
    BOOL                               isCurrentlyExecuting;
    BOOL                               isLoop;
    BOOL                               isDirty;
    YRKSpinningProgressIndicator*      progressIndicator;
    NSString*                          workspaceName;
}

@property (nonatomic)        int       flagCount;
@property (nonatomic)        BOOL      touchOnRevival;
@property (nonatomic)        BOOL      isCurrentlyExecuting;
@property (nonatomic)        BOOL      isLoop;
@property (nonatomic)        BOOL      isDirty;
@property (nonatomic)        BOOL      isVisited;
@property (nonatomic,strong) NSString* workspaceName;

- (void)addInletOfColor:(NSColor*)c;
- (void)addOutletOfColor:(NSColor*)c;
- (BOOL)areAnyParentsDirty;
- (NSColor*)colorOfInletIndexed:(int)idx;
- (NSColor*)colorOfOutletIndexed:(int)idx;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (InOutlet*)findInOutletWithColor:(NSColor*)c;
- (Tool*)getParentToolOfInlet:(Inlet*)inlt;
- (Tool*)getParentToolOfInletIndexed:(int)idx;
- (BOOL)hasParents;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName;
- (Inlet*)inletIndexed:(int)idx;
- (void)initializeImage;
- (BOOL)isFullyConnected;
- (BOOL)isSomeParentVisited;
- (int)numInlets;
- (int)numInletsOfColor:(NSColor*)c;
- (int)numOutlets;
- (int)numOutletsOfColor:(NSColor*)c;
- (NSPoint)originForControlWindow:(NSWindow*)win;
- (Outlet*)outletIndexed:(int)idx;
- (BOOL)performToolTask;
- (void)removeAllConnections;
- (void)removeAllConnectionsToInlets;
- (void)removeAllConnectionsToOutlets;
- (void)removeAllInletsAndOutlets;
- (void)removeAllInlets;
- (void)removeAllOutlets;
- (void)removeFilesFromTemporaryDirectory;
- (void)removeInletOfColor:(NSColor*)c;
- (BOOL)resolveStateOnWindowOK;
- (NSMutableAttributedString*)sendTip;
- (void)setAnalysisView:(AnalysisView*)av;
- (void)setImageWithSize:(NSSize)s;
- (void)setInletLocations;
- (void)setOutletLocations;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (void)startProgressIndicator;
- (void)stopProgressIndicator;
- (NSString*)toolName;
- (void)updateForChangeInUpstreamState;
- (void)updateDownstreamTools;

@end
