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


-(void)loadView {
    [[UIApplication sharedApplication] setStatusBarHidden:YES
                                            withAnimation: UIStatusBarAnimationSlide];
    UIImageView *contentView = [[UIImageView alloc]
                                initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    UIImage *image = [UIImage imageNamed:@"iphone.png"];
    [contentView setImage:image];
    
    self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    [self.view addSubview:contentView];
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
