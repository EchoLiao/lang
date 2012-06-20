//
//  _2_QuartzFunAppDelegate.h
//  12-QuartzFun
//
//  Created by nuoerlz on 12-6-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _2_QuartzFunViewController;

@interface _2_QuartzFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _2_QuartzFunViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _2_QuartzFunViewController *viewController;

@end

