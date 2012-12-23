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
//    [self.renderView render];
//    [self.renderView renderRectInIosCoorWithX1:0 withY1:0 withX2:300 withY2:460];

    [self.renderView OVDisplayClear];
    [self.renderView OVDisplaySetColorWithR:1.0 withG:0 withB:0 withA:0];
    [self.renderView OVDisplayDrawRectWithX1:1 withY1:1 withX2:300 withY2:460];
//    [self.renderView OVDisplayFillRectWithX1:0 withY1:0 withX2:300 withY2:460];
    [self.renderView OVDisplayShow];
}

- (void)viewDidUnload
{
    [super viewDidUnload];

    [self.renderView unInitOpenGL];
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
