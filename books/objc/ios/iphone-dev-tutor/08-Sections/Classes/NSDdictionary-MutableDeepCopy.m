//
//  NSDdictionary-MutableDeepCopy.m
//  08-Sections
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "NSDdictionary-MutableDeepCopy.h"


@implementation NSDictionary(DeepMutableCopy)

-(NSMutableDictionary *)mutableDeepCopy
{
    NSMutableDictionary *ret = [[NSMutableDictionary alloc] 
								initWithCapacity:[self count]];
    NSArray *keys = [self allKeys];
    for (id key in keys)
    {
        id oneValue = [self valueForKey:key];
        id oneCopy = nil;
        
        if ([oneValue respondsToSelector:@selector(mutableDeepCopy)])
            oneCopy = [oneValue mutableDeepCopy];
        else if ([oneValue respondsToSelector:@selector(mutableCopy)])
            oneCopy = [oneValue mutableCopy];
        if (oneCopy == nil)
            oneCopy = [oneValue copy];
        [ret setValue:oneCopy forKey:key];
    }
    return ret;
}

@end
