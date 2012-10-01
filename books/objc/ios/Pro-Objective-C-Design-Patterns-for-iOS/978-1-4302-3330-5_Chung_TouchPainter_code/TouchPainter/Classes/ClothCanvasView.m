//
//  ClothCanvasView.m
//  TouchPainter
//
//  Created by Carlo Chung on 10/16/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import "ClothCanvasView.h"


@implementation ClothCanvasView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
    {
        UIImage *backgroundImage = [UIImage imageNamed:@"cloth"];
        UIImageView *backgroundView = [[[UIImageView alloc]
                                        initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
        backgroundView.image = backgroundImage;
        backgroundView.contentMode = UIViewContentModeScaleToFill;
        [self addSubview:backgroundView];
    }
    
    return self;
}

// implementation for other behaviors

@end
