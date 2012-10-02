//
//  StrokeColorCommand.m
//  TouchPainter
//
//  Created by Carlo Chung on 11/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import "SetStrokeColorCommand.h"
#import "CoordinatingController.h"
#import "CanvasViewController.h"

@implementation SetStrokeColorCommand

#ifdef Use_Delegate_Implement_Adapter
@synthesize delegate=delegate_;
#else
@synthesize postColorUpdateProvider=postColorUpdateProvider_;
@synthesize RGBValuesProvider=RGBValuesProvider_;
#endif


- (void) execute
{
  CGFloat redValue = 0.0;
  CGFloat greenValue = 0.0;
  CGFloat blueValue = 0.0;
  
  // Retrieve RGB values from a delegate or a block 
  
#ifdef Use_Delegate_Implement_Adapter
  // Delegation (object adapter) approach:
  [delegate_ command:self didRequestColorComponentsForRed:&redValue
                                                    green:&greenValue
                                                     blue:&blueValue];
#else
  // Block approach:
  if (RGBValuesProvider_ != nil)
  {
    RGBValuesProvider_(&redValue, &greenValue, &blueValue);
  }
#endif
  
  // Create a color object based on the RGB values
  UIColor *color = [UIColor colorWithRed:redValue
                                   green:greenValue
                                    blue:blueValue
                                   alpha:1.0];
  
  // Assign it to the current canvasViewController
  CoordinatingController *coordinator = [CoordinatingController sharedInstance];
  CanvasViewController *controller = [coordinator canvasViewController];
  [controller setStrokeColor:color];
  
  // Forward a post update message
  
#ifdef Use_Delegate_Implement_Adapter
    // Delegation approach:
  [delegate_ command:self didFinishColorUpdateWithColor:color];
#else
  // Block approach:
  if (postColorUpdateProvider_ != nil)
  {
    postColorUpdateProvider_(color);
  }
#endif
}

- (void) dealloc
{
#ifndef Use_Delegate_Implement_Adapter
  [RGBValuesProvider_ release];
  [postColorUpdateProvider_ release];
#endif
  [super dealloc];
}

@end
