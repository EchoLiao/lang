//
//  ButtonFunAppDelegate.h
//  ButtonFun
//
//  Created by 睢常明 on 12-6-10.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ButtonFunViewController;

@interface ButtonFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    ButtonFunViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ButtonFunViewController *viewController;

@end

