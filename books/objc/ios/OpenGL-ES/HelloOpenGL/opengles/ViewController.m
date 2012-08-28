//
//  ViewController.m
//  opengles
//
//  Created by erlz nuo on 12-8-27.
//  Copyright (c) 2012年 erlz nuo. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLES2DView.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    int ret = [(OpenGLES2DView *)self.view initOpenGL];
    assert(ret);
    
    [(OpenGLES2DView *)self.view render];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    
    [(OpenGLES2DView *)self.view unInitOpenGL];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}



@end
