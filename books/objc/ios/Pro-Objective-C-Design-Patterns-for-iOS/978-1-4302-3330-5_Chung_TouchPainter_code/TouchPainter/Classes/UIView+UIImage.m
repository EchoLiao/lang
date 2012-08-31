//
//  UIView+UIImage.m
//  TouchPainter
//
//  Created by Carlo Chung on 10/23/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import "UIView+UIImage.h"
#import <QuartzCore/QuartzCore.h>

@implementation UIView (UIImage)


- (UIImage*)image 
{
  // Create a graphics context with the target size
  // On iOS 4 and later, use UIGraphicsBeginImageContextWithOptions to take the scale into consideration
  // On iOS prior to 4, fall back to use UIGraphicsBeginImageContext
  CGSize imageSize = [self bounds].size;// [[UIScreen mainScreen] bounds].size;
    
  // 根据size大小创建一个基于位图的图形上下文
  if (NULL != UIGraphicsBeginImageContextWithOptions)
    UIGraphicsBeginImageContextWithOptions(imageSize, NO, 0);
  else
    UIGraphicsBeginImageContext(imageSize);
  
  // 获取当前quartz 2d绘图环境
  CGContextRef context = UIGraphicsGetCurrentContext();
  
  // Iterate over every window from back to front
  for (UIWindow *window in [[UIApplication sharedApplication] windows]) 
  {
    if (![window respondsToSelector:@selector(screen)] || [window screen] == [UIScreen mainScreen])
    {
      // -renderInContext: renders in the coordinate space of the layer,
      // so we must first apply the layer's geometry to the graphics context
      CGContextSaveGState(context);
      // Center the context around the window's anchor point
      CGContextTranslateCTM(context, [window center].x, [window center].y);
      // Apply the window's transform about the anchor point
      CGContextConcatCTM(context, [window transform]);
      // Offset by the portion of the bounds left of and above the anchor point
      CGContextTranslateCTM(context,
                            -[window bounds].size.width *[[window layer] anchorPoint].x,
                            -[window bounds].size.height *[[window layer] anchorPoint].y);
      
      // Render the layer hierarchy to the current context
      [[window layer] renderInContext:context];
      
      // Restore the context
      CGContextRestoreGState(context);
    }
  }
  
  // Retrieve the screenshot image
  UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
  // NSData *imageData = UIImageJPEGRepresentation(image, 100);

  
  // 从当前堆栈中删除quartz 2d绘图环境
  UIGraphicsEndImageContext();
  
  return image;
}

@end
