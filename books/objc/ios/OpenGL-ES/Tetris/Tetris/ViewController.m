//
//  ViewController.m
//  Tetris
//
//  Created by erlz nuo on 12-12-23.
//  Copyright (c) 2012年 erlz nuo. All rights reserved.
//

#import <assert.h>
#import "ViewController.h"
#import "OpenGLView.h"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    int ret = [self.renderView initOpenGL];
    assert(ret);

    [[UIApplication sharedApplication] setStatusBarHidden:YES];
     [self.renderView render];
}

- (void)viewDidUnload
{
    [super viewDidUnload];

    [(OpenGLView *)self.view unInitOpenGL];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)dealloc
{
    [_renderView release];
    [super dealloc];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
//    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    return YES;
}


@end
