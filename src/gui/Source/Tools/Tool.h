#import <Cocoa/Cocoa.h>
#import "RbItem.h"
@class AnalysisView;
@class Inlet;
@class InOutlet;
@class Outlet;
@class ToolLoop;
@class YRKSpinningProgressIndicator;


@interface Tool : RbItem <NSCoding> {

    AnalysisView*                            analysisView;
    NSMutableArray*                          inlets;
    NSMutableArray*                          outlets;
	int                                      flagCount;
	BOOL                                     touchOnRevival;
    BOOL                                     isVisited;
    BOOL                                     isCurrentlyExecuting;
    BOOL                                     isLoop;
    BOOL                                     isDirty;
    YRKSpinningProgressIndicator*            progressIndicator;
    NSString*                                workspaceName;
    
    NSMutableArray*                          loopMembership;
    int                                      executeOrder;
}

@property (nonatomic,strong) AnalysisView*   analysisView;
@property (nonatomic)        int             flagCount;
@property (nonatomic)        BOOL            touchOnRevival;
@property (nonatomic)        BOOL            isCurrentlyExecuting;
@property (nonatomic)        BOOL            isLoop;
@property (nonatomic)        BOOL            isDirty;
@property (nonatomic)        BOOL            isVisited;
@property (nonatomic,strong) NSString*       workspaceName;
@property (nonatomic,strong) NSMutableArray* loopMembership;
@property (nonatomic)        int             executeOrder;

- (void)addInletOfColor:(NSColor*)c;
- (void)addOutletOfColor:(NSColor*)c;
- (void)addToolToLoop:(ToolLoop*)loop;
- (BOOL)areAnyParentsDirty;
- (BOOL)checkForExecute:(NSMutableDictionary*)errors;
- (BOOL)checkForWarning:(NSMutableDictionary*)warnings;
- (NSColor*)colorOfInletIndexed:(int)idx;
- (NSColor*)colorOfOutletIndexed:(int)idx;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (InOutlet*)findInOutletWithColor:(NSColor*)c;
- (NSMutableArray*)getChildrenTools;
- (NSMutableArray*)getParentTools;
- (Tool*)getParentToolOfInlet:(Inlet*)inlt;
- (Tool*)getParentToolOfInletIndexed:(int)idx;
- (BOOL)hasParents;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName;
- (Inlet*)inletIndexed:(int)idx;
- (void)initializeImage;
- (BOOL)isFullyConnected;
- (BOOL)isOnLoop;
- (BOOL)isSomeParentVisited;
- (void)lockView;
- (int)numInlets;
- (int)numInletsOfColor:(NSColor*)c;
- (int)numOutlets;
- (int)numOutletsOfColor:(NSColor*)c;
- (void)orderLoops;
- (NSPoint)originForControlWindow:(NSWindow*)win;
- (Outlet*)outletIndexed:(int)idx;
- (BOOL)performToolTask;
- (void)prepareForExecution;
- (void)removeAllConnections;
- (void)removeAllConnectionsToInlets;
- (void)removeAllConnectionsToOutlets;
- (void)removeAllInletsAndOutlets;
- (void)removeAllInlets;
- (void)removeAllOutlets;
- (void)removeFilesFromTemporaryDirectory;
- (void)removeInletOfColor:(NSColor*)c;
- (void)removeToolFromLoop:(ToolLoop*)loop;
- (void)removeAllLoops;
- (BOOL)resolveStateOnWindowOK;
- (NSMutableAttributedString*)sendTip;
- (void)setImageWithSize:(NSSize)s;
- (void)setInletLocations;
- (void)setOutletLocations;
- (void)setStatusMessage:(NSString*)msg;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (void)startProgressIndicator;
- (void)stopProgressIndicator;
- (NSString*)toolName;
- (void)unlockView;
- (void)updateChildrenTools;
- (void)updateDisplay;
- (void)updateForChangeInParent;

@end
