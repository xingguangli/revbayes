#import "ValueReal.h"



@implementation ValueReal

- (Value*)clone {
    
    return [[ValueReal alloc] initWithValue:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [super encodeWithCoder:aCoder];
	[aCoder encodeObject:value forKey:@"value"];
}

- (id)init {

    self = [self initWithNumber:0.0];
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        value = [aDecoder decodeObjectForKey:@"value"];
		}
	return self;
}

- (id)initWithNumber:(float)v {

    if ( (self = [super init]) ) 
		{
        value = [[NSNumber alloc] initWithFloat:v];
		}
    return self;
}

- (id)initWithValue:(Value*)v {
    
    if ( (self = [super init]) ) 
		{
        float x = [(ValueReal*)v value];
        value = [[NSNumber alloc] initWithFloat:x];
		}
    return self;
}

- (void)setValue:(float)x {

    value = [NSNumber numberWithFloat:x];
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = [NSString stringWithFormat:@"%f", [value floatValue]];
    return strRep;
}

- (float)value {

    return [value floatValue];
}

@end
