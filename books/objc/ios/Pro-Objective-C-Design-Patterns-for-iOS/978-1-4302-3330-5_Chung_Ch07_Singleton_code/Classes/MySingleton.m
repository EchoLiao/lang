//
//  MySingleton.m
//  Singleton
//
//  Created by Carlo Chung on 2/17/11.
//  Copyright 2011 Carlo Chung. All rights reserved.
//

#import "MySingleton.h"


@implementation MySingleton

static MySingleton *sSharedMySingleton = nil;


+ (MySingleton *) sharedInstance
{
    [super createSharedInstance:&sSharedMySingleton];
    return sSharedMySingleton;
}


- (id) init
{
  return self;
}

- (void) operation
{
  // do something
  NSLog(@"MySingleton");
}

@end
