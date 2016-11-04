#import "InOutlet.h"
#import "RevBayes.h"
#import "Tool.h"



@implementation InOutlet

@synthesize position;
@synthesize toolColor;
@synthesize toolOwner;

- (BOOL)amInlet {

    return NO;
}

- (BOOL)amOutlet {

    return NO;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodePoint:position   forKey:@"position"];
	[aCoder encodeObject:toolColor forKey:@"toolColor"];
	[aCoder encodeObject:toolOwner forKey:@"toolOwner"];
}

- (NSPoint)getDrawingPositionForToolWithRect:(NSRect)tr andBoundedRect:(NSRect)b {

    NSRect r = [self rectForToolWithRect:tr];
    NSPoint p = r.origin;
    float spacer = r.size.width * 0.2;
    
    if (position.x < 0.01)
        {
        // left side
        p.x += r.size.width + spacer;
        p.y += (r.size.height - b.size.height) * 0.5;
        }
    else if (position.x > 0.99)
        {
        // right side
        p.x -= (b.size.width + spacer);
        p.y += (r.size.height - b.size.height) * 0.5;
        }
    else if (position.y < 0.01)
        {
        // bottom side
        p.x += (r.size.width - b.size.width) * 0.5;
        p.y += r.size.height + spacer;
        }
    else if (position.y > 0.99)
        {
        // top side
        p.x += (r.size.width - b.size.width) * 0.5;
        p.y -= (b.size.height + spacer);
        }
    else
        {
        NSLog(@"Cannot figure out which edge of the tool the inlet is on");
        }
    return p;
}

- (NSRect)getTextRectForToolWithRect:(NSRect)r {

    NSRect inOutRect = [self rectForToolWithRect:r];
    
    if (position.x < 0.01)
        {
        // left side
        inOutRect.origin.x += inOutRect.size.width;
        }
    else if (position.x > 0.99)
        {
        // right side
        inOutRect.origin.x -= inOutRect.size.width;
        }
    else if (position.y < 0.01)
        {
        // bottom side
        inOutRect.origin.y += inOutRect.size.height;
        //float d = (inOutRect.size.width - b.size.width) * 0.5;
        //inOutRect.origin.x += d;
        }
    else if (position.y > 0.99)
        {
        // top side
        inOutRect.origin.y -= inOutRect.size.height;
        //float d = (inOutRect.size.width - b.size.width) * 0.5;
        //inOutRect.origin.x += d;
        }
    else
        {
        NSLog(@"Cannot figure out which edge of the tool the inlet is on");
        }
    return inOutRect;
}

- (id)init {

	return [self initWithTool:nil];
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
		toolColor = [aDecoder decodeObjectForKey:@"toolColor"];
		toolOwner = [aDecoder decodeObjectForKey:@"toolOwner"];
		position  = [aDecoder decodePointForKey:@"position"];
		}
	return self;
}

- (id)initWithTool:(Tool*)t {

    if ( (self = [super init]) ) 
		{
		toolOwner = t;
        position = NSMakePoint(1.0, 0.5);
		}
    return self;
}

- (BOOL)isOnLeftEdge {

    if ( position.x < 0.00001)
        return YES;
    return NO;
}

- (BOOL)isOnRightEdge {

    if ( position.x > 0.99999)
        return YES;
    return NO;
}

- (BOOL)isOnUpperEdge {

    if ( position.y > 0.99999)
        return YES;
    return NO;
}

- (BOOL)isOnLowerEdge {

    if ( position.y < 0.00001)
        return YES;
    return NO;
}

- (int)numberOfConnections {

    return 0;
}

- (NSPoint)offsetPosition {

    NSPoint myPoint = position;
    if ( position.x < 0.000001 || position.x > 0.999999 )
        myPoint.y += 0.04;
    else if ( position.y < 0.000001 || position.y > 0.999999 )
        myPoint.x += 0.04;
    return myPoint;
}

- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3 {
    
}

- (NSPoint)pointForToolWithRect:(NSRect)r {

#   if 1
    NSPoint myPoint = NSMakePoint( r.origin.x + r.size.width * [self offsetPosition].x, r.origin.y + r.size.height * [self offsetPosition].y );
    return myPoint;
#   else
    NSPoint myPoint = NSMakePoint( r.origin.x + r.size.width * position.x, r.origin.y + r.size.height * position.y );
    return myPoint;
#   endif
}

- (NSRect)rectForToolWithRect:(NSRect)r {

    // the rect that will be returned, containing the inlet/outlet's position
    NSRect myRect;

    // calculate the size of the rectangle's sides
    float mySideLength = r.size.width * 0.2;
    
    // we assume that the coordinate system or r is bottom-left
    // first, set the origin of the rectangle
    myRect.origin.x = r.origin.x + r.size.width * position.x;
    myRect.origin.y = r.origin.y + r.size.height * position.y;
    if (position.x < 0.01)
        myRect.origin.x -= mySideLength * BURY_FRACTION;
    else
        myRect.origin.x -= mySideLength * (1.0 - BURY_FRACTION);
    if (position.y < 0.01)
        myRect.origin.y -= mySideLength * BURY_FRACTION;
    else
        myRect.origin.y -= mySideLength * (1.0 - BURY_FRACTION);
    
    // then, set the size of the rectangle
    myRect.size.width = mySideLength;
    myRect.size.height = mySideLength;
    
    return myRect;
}

@end
