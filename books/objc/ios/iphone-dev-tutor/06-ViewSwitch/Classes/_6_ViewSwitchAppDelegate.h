//
//  _6_ViewSwitchAppDelegate.h
//  06-ViewSwitch
//
//  Created by nuoerlz on 12-6-14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
@class SwitchViewController;


@interface _6_ViewSwitchAppDelegate : NSObject <UIApplicationDelegate> {
    IBOutlet UIWindow *window;
	IBOutlet SwitchViewController *switchViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SwitchViewController *switchViewController;

@end

