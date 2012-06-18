//
//  _8_SimpleTableAppDelegate.h
//  08-SimpleTable
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _8_SimpleTableViewController;

@interface _8_SimpleTableAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _8_SimpleTableViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _8_SimpleTableViewController *viewController;

@end

