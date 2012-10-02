//
//  StrokeColorCommand.h
//  TouchPainter
//
//  Created by Carlo Chung on 11/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Command.h"

// #define Use_Delegate_Implement_Adapter // else Use Block to implement



#ifdef Use_Delegate_Implement_Adapter

// 版本一: 使用协义实现
// MARK-Adapter: SetStrokeColorCommandDelegate 充当"目标". [(P94)]
@class SetStrokeColorCommand;
@protocol SetStrokeColorCommandDelegate
- (void) command:(SetStrokeColorCommand *) command
                didRequestColorComponentsForRed:(CGFloat *) red
                                          green:(CGFloat *) green 
                                           blue:(CGFloat *) blue;
- (void) command:(SetStrokeColorCommand *) command
                didFinishColorUpdateWithColor:(UIColor *) color;
@end

#else

// 版本二: 使用块实现
typedef void (^RGBValuesProvider)(CGFloat *red, CGFloat *green, CGFloat *blue);
typedef void (^PostColorUpdateProvider)(UIColor *color);

#endif




// MARK-Adapter: SetStrokeColorCommand 充当"客户端". [(P94)]
@interface SetStrokeColorCommand : Command
{
  @private
#ifdef Use_Delegate_Implement_Adapter
  id <SetStrokeColorCommandDelegate> delegate_;
#else
  id delegate_; // For PaletteViewController.xib
  RGBValuesProvider RGBValuesProvider_;
  PostColorUpdateProvider postColorUpdateProvider_;
#endif
}

#ifdef Use_Delegate_Implement_Adapter
@property (nonatomic, assign) id <SetStrokeColorCommandDelegate> delegate;
#else
@property (nonatomic, copy) RGBValuesProvider RGBValuesProvider;
@property (nonatomic, copy) PostColorUpdateProvider postColorUpdateProvider;
#endif

- (void) execute;

@end


