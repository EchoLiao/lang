//
//  ViewController.m
//  UIApp
//
//  Created by erlz nuo on 13-1-7.
//  Copyright (c) 2013年 erlz nuo. All rights reserved.
//

#import "ViewController.h"

#define kDocsPath [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]


@interface ViewController ()
@end


@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}



#pragma mark -

// iPhone 程序启动互相调用, 打开其他程序
// http://www.cnblogs.com/chen1987lei/archive/2012/04/17/2453834.html

- (IBAction)openBrowser:(id)sender
{
}

- (IBAction)openAnotherApp:(id)sender
{
}


@end
