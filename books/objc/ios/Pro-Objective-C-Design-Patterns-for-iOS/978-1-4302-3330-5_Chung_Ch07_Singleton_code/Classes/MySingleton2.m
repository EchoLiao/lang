//
//  MySingleton2.m
//  Singleton
//
//

#import "MySingleton2.h"


@implementation MySingleton2

static MySingleton2 *sSharedMySingleton2 = nil;


+ (MySingleton2 *) sharedInstance
{
    [super createSharedInstance:&sSharedMySingleton2];
    return sSharedMySingleton2;
}


- (id) init
{
  
  return self; 
}

- (void) operation
{
  // do something
  NSLog(@"MySingleton2");
}

@end
