//
//
//

#import "MySingletonChild.h"


@implementation MySingletonChild

static MySingletonChild *sSharedMySingletonChild = nil;


+ (MySingletonChild *) sharedInstance
{
    [self createSharedInstance:&sSharedMySingletonChild];
    return sSharedMySingletonChild;
}


- (id) init
{
  return self;
}

- (void) operation
{
  // do something
  NSLog(@"MySingletonChild");
}

@end
