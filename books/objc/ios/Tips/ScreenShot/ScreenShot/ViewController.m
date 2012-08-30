//
//  ViewController.m
//  ScreenShot
//
//  Created by erlz nuo on 12-8-30.
//  Copyright (c) 2012年 erlz nuo. All rights reserved.
//
//
// http://disanji.net/2010/12/18/ios-clip-and-display-part-of-pictures/
//


#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

/*
    CGContextRotateCTM(currentContext, M_PI);
 
 (1) 理解为旋转物体, 坐标系不变.
                 y
                 ^
                 |
                 |
                  
                 --------
                 |      |
                 |      |
                 |*     |
            src  |***   |
                 |*     |
                 |      |
                 |      |
         -------AA-------   -->x
         |      |
         |      |
         |     *|
         |   ***| dst
         |     *|
         |      |
         |      |
         --------
 
--------------------------------------------------------
 
 (2) 理解为旋转坐标系, 物体不变.
 
                 y
                 ^
                 |
                 |
                  
                 --------
                 |      |
                 |      |
                 |*     |
            src  |***   |
                 |*     |
                 |      |
                 |      |
      x'<------- A-------   -->x  (src)
    (dst)        |
                 |
                 |
                 |
                 |
                 V
                 y'
*/



/*
    CGContextRotateCTM();
    CGContextTranslateCTM();
    CGContextScaleCTM();
    ... 
 这些命令的书写顺序产生的效果与OpenGL是一样的!
*/
-(void)loadView 
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES
                                            withAnimation: UIStatusBarAnimationSlide];
    UIImage *image = [UIImage imageNamed:@"iphone.png"];
    CGRect rect = CGRectMake(0, 0, 210, 280);//创建矩形框
    
    UIGraphicsBeginImageContext(rect.size);//根据size大小创建一个基于位图的图形上下文
    CGContextRef currentContext = UIGraphicsGetCurrentContext();//获取当前quartz 2d绘图环境
    CGContextClipToRect(currentContext, rect);//设置当前绘图环境到矩形框
    CGContextTranslateCTM(currentContext, rect.size.width, 0.0);
    CGContextScaleCTM(currentContext, -1.0, 1.0);
    CGContextRotateCTM(currentContext, M_PI);
    CGContextTranslateCTM(currentContext, -rect.size.width, -rect.size.height);
    
    CGContextDrawImage(currentContext, rect, image.CGImage);//绘图
    // [image drawInRect:rect];
    UIImage *cropped = UIGraphicsGetImageFromCurrentImageContext();//获得图片
    UIGraphicsEndImageContext();//从当前堆栈中删除quartz 2d绘图环境
    
    UIImageView *contentView = [[UIImageView alloc] initWithFrame:rect];
    contentView.image = cropped;
    
    self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    [self.view addSubview:contentView];
    
    [cropped release];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

@end
