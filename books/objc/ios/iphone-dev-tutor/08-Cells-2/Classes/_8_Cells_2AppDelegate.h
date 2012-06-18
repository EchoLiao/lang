//
//  _8_Cells_2AppDelegate.h
//  08-Cells-2
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _8_Cells_2ViewController;

@interface _8_Cells_2AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _8_Cells_2ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _8_Cells_2ViewController *viewController;

@end

