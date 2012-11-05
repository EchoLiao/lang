//
//  CustomTabBar.m
//  07-Pickers
//
//  Created by erlz nuo on 12-11-5.
//
//

#import <QuartzCore/QuartzCore.h>
#import "CustomTabBar.h"

@implementation CustomTabBar

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}

- (void)awakeFromNib 
{
    [super awakeFromNib];
    _image = [UIImage imageNamed:@"Title"];
    self.tintColor = [UIColor colorWithRed:0.0 / 255.0
                                     green:50.0 / 255.0
                                      blue:149.0 / 255.0
                                     alpha:1.0];
    
}

- (void)drawRect:(CGRect)rect
{
    [_image drawInRect:rect];
    
    // draw shadow
    self.layer.masksToBounds = NO;
    self.layer.shadowOffset = CGSizeMake(0, -3);
    self.layer.shadowOpacity = 0.6;
    self.layer.shadowPath = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;
    
    
    UIDeviceOrientation curOri = [UIApplication sharedApplication].statusBarOrientation;
    if ( UIInterfaceOrientationIsPortrait(curOri) ) {
        self.selectionIndicatorImage = [UIImage imageNamed:@"tab_bar_select"];
    } else {
        self.selectionIndicatorImage = [UIImage imageNamed:@"tab_bar_select_lan"];
    }
}


@end
