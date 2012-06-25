//
//  QuartzFunView.h
//  12-QuartzFun
//
//  Created by nuoerlz on 12-6-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Constants.h"


@interface QuartzFunView : UIView {
    CGPoint        firstTouch;
    CGPoint        lastTouch;
    UIColor        *currentColor;
    ShapeType      shapeType;
    UIImage        *drawImage;
    BOOL           useRandomColor;
    
    CGRect         redrawRect;
}

@property CGPoint firstTouch;
@property CGPoint lastTouch;
@property (nonatomic, retain) UIColor *currentColor;
@property ShapeType shapeType;
@property (nonatomic, retain) UIImage *drawImage;
@property BOOL useRandomColor;
@property CGRect redrawRect;

@property (readonly) CGRect currentRect; // 动态属性

@end


