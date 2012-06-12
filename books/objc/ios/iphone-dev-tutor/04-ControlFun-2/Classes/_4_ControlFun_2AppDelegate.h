//
//  _4_ControlFun_2AppDelegate.h
//  04-ControlFun-2
//
//  Created by 睢常明 on 12-6-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _4_ControlFun_2ViewController;

@interface _4_ControlFun_2AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _4_ControlFun_2ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _4_ControlFun_2ViewController *viewController;

@end

