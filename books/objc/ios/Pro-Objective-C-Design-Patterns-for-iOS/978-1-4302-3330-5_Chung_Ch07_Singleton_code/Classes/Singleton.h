//
//  Singleton.h
//  Singleton
//
//  Created by Carlo Chung on 6/10/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Singleton : NSObject 
{

}


+ (void) createSharedInstance:(Singleton **)singleton;
+ (Singleton *) sharedInstance;

- (void) operation;


@end
