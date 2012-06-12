//
//  _5_Autosize_2AppDelegate.h
//  05-Autosize-2
//
//  Created by nuoerlz on 12-6-12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _5_Autosize_2ViewController;

@interface _5_Autosize_2AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _5_Autosize_2ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _5_Autosize_2ViewController *viewController;

@end

