//
//  _8_CellsAppDelegate.h
//  08-Cells
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _8_CellsViewController;

@interface _8_CellsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _8_CellsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _8_CellsViewController *viewController;

@end

