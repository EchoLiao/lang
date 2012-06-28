//
//  UIColor-Random.m
//  12-QuartzFun
//
//  Created by nuoerlz on 12-6-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "UIColor-Random.h"


@implementation UIColor(Random)

+(UIColor *)randomColor
{
    static BOOL seeded = NO;
    if (!seeded) {
        seeded = YES;
        srandom(time(NULL));
    }
    CGFloat red =  (CGFloat)random()/(CGFloat)RAND_MAX;
    CGFloat blue = (CGFloat)random()/(CGFloat)RAND_MAX;
    CGFloat green = (CGFloat)random()/(CGFloat)RAND_MAX;
    return [UIColor colorWithRed:red green:green blue:blue alpha:1.0f];
}

@end