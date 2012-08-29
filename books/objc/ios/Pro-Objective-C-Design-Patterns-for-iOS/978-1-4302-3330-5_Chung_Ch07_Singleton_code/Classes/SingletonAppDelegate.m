//
//  SingletonAppDelegate.m
//  Singleton
//
//  Created by Carlo Chung on 6/10/10.
//  Copyright Carlo Chung 2010. All rights reserved.
//

#import "SingletonAppDelegate.h"
#import "MySingleton.h"
#import "MySingleton2.h"
#import "MySingletonChild.h"



@implementation SingletonAppDelegate

@synthesize window;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
  
    Singleton *s_root = [Singleton sharedInstance];
    assert(s_root != nil);
    [s_root operation];
    
    Singleton *s = [MySingleton sharedInstance];
    assert(s != nil);
    Singleton *s2 = [MySingleton2 sharedInstance];
    assert(s2 != nil);
    Singleton *s_child = [MySingletonChild sharedInstance];
    assert(s_child != nil);
    assert(s_root != s && s_root != s2 && s != s2 && s2 != s_child);
    [s operation];
    [s2 operation];
    [s_child operation];
    
    Singleton *s_copy = [s copy];
    assert(s_copy == s);
    [s_copy operation];
    
    Singleton *s_alloc = [MySingleton alloc];
    assert(s_alloc == s);
    [s_alloc operation];
    
    Singleton *s_retain = [s retain];
    assert(s_retain == s);
    [s_retain operation];
    
    [s release];
    [s release];
    [s release];
    [s release];
    [s release];
    [s release];
    [s release];
    assert(s != nil);
    [s operation];
    
  [window makeKeyAndVisible];
  
  return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
  /*
   Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
   Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
   */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
  /*
   Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
   If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
   */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
  /*
   Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
   */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
  /*
   Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
   */
}


- (void)applicationWillTerminate:(UIApplication *)application {
  /*
   Called when the application is about to terminate.
   See also applicationDidEnterBackground:.
   */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
  /*
   Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
   */
}


- (void)dealloc {
  [window release];
  [super dealloc];
}


@end
