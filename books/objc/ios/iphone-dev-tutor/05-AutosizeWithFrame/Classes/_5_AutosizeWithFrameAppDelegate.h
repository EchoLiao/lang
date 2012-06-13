//
//  _5_AutosizeWithFrameAppDelegate.h
//  05-AutosizeWithFrame
//
//  Created by nuoerlz on 12-6-13.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _5_AutosizeWithFrameViewController;

@interface _5_AutosizeWithFrameAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _5_AutosizeWithFrameViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _5_AutosizeWithFrameViewController *viewController;

@end

