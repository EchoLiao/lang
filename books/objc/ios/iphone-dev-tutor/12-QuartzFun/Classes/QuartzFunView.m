//
//  QuartzFunView.m
//  12-QuartzFun
//
//  Created by nuoerlz on 12-6-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "QuartzFunView.h"
#import "UIColor-Random.h"


@implementation QuartzFunView

@synthesize firstTouch;
@synthesize lastTouch;
@synthesize currentColor;
@synthesize shapeType;
@synthesize drawImage;
@synthesize useRandomColor;
@synthesize redrawRect;

@synthesize currentRect;


- (CGRect)currentRect {
	return CGRectMake (
					   (firstTouch.x > lastTouch.x) ? lastTouch.x : firstTouch.x,
					   (firstTouch.y > lastTouch.y) ? lastTouch.y : firstTouch.y,
					   fabsf(firstTouch.x - lastTouch.x),
					   fabsf(firstTouch.y - lastTouch.y));
}

- (id)initWithCoder:(NSCoder*)coder
{
	// nib文件中的对象实例存储为归档对象。
	// 该视图是从nib中加载的。从nib中加载对象实例时，init:或initWithFrame:都不会调用，而是调用initWithCoder:!!
    if ( ( self = [super initWithCoder:coder] ) ) {
        self.currentColor = [UIColor redColor];
        self.useRandomColor = NO;
        if (drawImage == nil)
            self.drawImage = [UIImage imageNamed:@"iphone.png"];
    }
    return self;
}


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
    }
    return self;
}



- (void)drawRect:(CGRect)rect {
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetLineWidth(context, 2.0);
    CGContextSetStrokeColorWithColor(context, currentColor.CGColor);
    CGContextSetFillColorWithColor(context, currentColor.CGColor);
//    CGRect currentRect = CGRectMake (
//                                     (firstTouch.x > lastTouch.x) ? lastTouch.x : firstTouch.x,
//                                     (firstTouch.y > lastTouch.y) ? lastTouch.y : firstTouch.y,
//                                     fabsf(firstTouch.x - lastTouch.x),
//                                     fabsf(firstTouch.y - lastTouch.y));
    
    switch (shapeType) {
        case kLineShape:
            CGContextMoveToPoint(context, firstTouch.x, firstTouch.y);
            CGContextAddLineToPoint(context, lastTouch.x, lastTouch.y);
            CGContextStrokePath(context);
            break;
        case kRectShape:
            CGContextAddRect(context, currentRect);
            CGContextDrawPath(context, kCGPathFillStroke);
            break;
        case kEllipseShape:
            CGContextAddEllipseInRect(context, currentRect);
            CGContextDrawPath(context, kCGPathFillStroke);
            break;
        case kImageShape: 
        {
            CGFloat horizontalOffset = drawImage.size.width / 2;
            CGFloat verticalOffset = drawImage.size.height / 2;
            CGPoint drawPoint = CGPointMake(lastTouch.x - horizontalOffset,
                                            lastTouch.y - verticalOffset);
            [drawImage drawAtPoint:drawPoint];            
            break;
        }
        default:
            break;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    if (useRandomColor)
        self.currentColor = [UIColor randomColor];
    UITouch *touch = [touches anyObject];
    firstTouch = [touch locationInView:self];
    lastTouch = [touch locationInView:self];
    [self setNeedsDisplay]; // 全屏重画。
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    lastTouch = [touch locationInView:self];
    
    if (shapeType == kImageShape) {
        CGFloat horizontalOffset = drawImage.size.width / 2.0;
        CGFloat verticalOffset = drawImage.size.height / 2.0;
        redrawRect = CGRectUnion(redrawRect, CGRectMake(lastTouch.x - horizontalOffset, 
                                                        lastTouch.y - verticalOffset, 
                                                        drawImage.size.width,
                                                        drawImage.size.height));
    }
    else
        redrawRect = CGRectUnion(redrawRect, self.currentRect); // 求交集
    
    redrawRect = CGRectInset(redrawRect, -2.0, -2.0);
    [self setNeedsDisplayInRect:redrawRect]; // 只重画指定的区域。
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    lastTouch = [touch locationInView:self];
    
    if (shapeType == kImageShape) {
        CGFloat horizontalOffset = drawImage.size.width / 2.0;
        CGFloat verticalOffset = drawImage.size.height / 2.0;
        redrawRect = CGRectUnion(redrawRect, CGRectMake(lastTouch.x - horizontalOffset, 
                                                        lastTouch.y - verticalOffset, 
                                                        drawImage.size.width,
                                                        drawImage.size.height));
    }
    else
        redrawRect = CGRectUnion(redrawRect, self.currentRect);
    redrawRect = CGRectInset(redrawRect, -2.0, -2.0);
    [self setNeedsDisplayInRect:redrawRect];
}

- (void)dealloc {
	[currentColor release];
    [drawImage release];
    [super dealloc];
}


@end
