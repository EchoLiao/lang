//
//  _8_SectionsAppDelegate.h
//  08-Sections
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class _8_SectionsViewController;

@interface _8_SectionsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    _8_SectionsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet _8_SectionsViewController *viewController;

@end

