//
//  OpenGL_ES_HelloAppDelegate.h
//  OpenGL-ES-Hello
//
//  Created by nuoerlz on 12-6-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class OpenGL_ES_HelloViewController;

@interface OpenGL_ES_HelloAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    OpenGL_ES_HelloViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet OpenGL_ES_HelloViewController *viewController;

@end

