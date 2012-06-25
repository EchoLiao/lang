//
//  _2_GLFunAppDelegate.h
//  12-GLFun
//
//  Created by nuoerlz on 12-6-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _2_GLFunViewController;

@interface _2_GLFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _2_GLFunViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _2_GLFunViewController *viewController;

@end

