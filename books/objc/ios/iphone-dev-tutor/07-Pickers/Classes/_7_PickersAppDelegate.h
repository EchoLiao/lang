//
//  _7_PickersAppDelegate.h
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PickersViewController;


@interface _7_PickersAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
	IBOutlet UITabBarController *rootController; // 使用标签控制类
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) UITabBarController *rootController;

@end

