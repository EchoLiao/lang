//
//  Singleton.m
//  Singleton
//
//  Created by Carlo Chung on 6/10/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import "Singleton.h"

/*
The Keyword super

Sometimes (quite often, in Cocoa programming) you want to override an inherited method but still
access the overridden functionality. To do so, you’ll use the keyword super. Like self, the keyword
super is something you send a message to. But its meaning has nothing to do with “this instance”
or any other instance. The keyword super is class-based, and it means: “Start the search for
messages I receive in the superclass of this class” (where “this class” is the class where the
keyword super appears).
*/


@implementation Singleton


static Singleton *sharedSingleton_ = nil;

- (void) operation
{
  // do something
  NSLog(@"Singleton");
}

+ (void) createSharedInstance:(Singleton **)singleton
{
    if ( *singleton == nil ) {
        *singleton = [NSAllocateObject([self class], 0, NULL) init];
    }
}

+ (Singleton *) sharedInstance
{
//  if (sharedSingleton_ == nil)
//  {
//    // 为了使得该类可以子类化必须使用 NSAllocateObject(), 而不能使用 [super allocWithZone:NULL];
//    // 因为若子类化了, 子类调用该方法时, super 指向的是该类! 而 allocWithZone: 等已被重载, 所以...
//    sharedSingleton_ = [NSAllocateObject([self class], 0, NULL) init];
//  }
//  
//  return sharedSingleton_;
    
    [Singleton createSharedInstance:&sharedSingleton_];
    return sharedSingleton_;
}


// 重载, 以保证返回的是同一个实例.
// retain 也已被重载.
+ (id) allocWithZone:(NSZone *)zone
{
  return [[self sharedInstance] retain];
}


// 重载, 以保证返回的是同一个实例.
- (id) copyWithZone:(NSZone*)zone
{
  return self;
}

// 重载, 以保证返回的是同一个实例.
- (id) retain
{
  return self;
}

// 重载, 以保证返回的是同一个实例.
- (NSUInteger) retainCount
{
  return NSUIntegerMax; // denotes an object that cannot be released
}

// 重载, 以保证实例在整个程序生命期间存在
- (oneway void) release
{
  // do nothing
}

// 重载, 以保证返回的是同一个实例.
- (id) autorelease
{
  return self;
}

@end
