//
//  _5_SwapAppDelegate.h
//  05-Swap
//
//  Created by nuoerlz on 12-6-13.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _5_SwapViewController;

@interface _5_SwapAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _5_SwapViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _5_SwapViewController *viewController;

@end

