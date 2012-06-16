    //
//  SwitchViewController.m
//  06-ViewSwitch
//
//  Created by nuoerlz on 12-6-14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "SwitchViewController.h"
#import "BlueViewController.h"
#import "YellowViewController.h"


@implementation SwitchViewController

@synthesize blueViewController;
@synthesize yellowViewController;


- (IBAction)switchViews:(id)sender
{
    if (self.yellowViewController.view.superview == nil)
    {
        if (self.yellowViewController == nil)
        {
            YellowViewController *yellowController = 
            [[YellowViewController alloc] initWithNibName:@"YellowView" 
                                                   bundle:nil];
            self.yellowViewController = yellowController;
            [yellowController release];
        }
		[blueViewController.view removeFromSuperview];
        [self.view insertSubview:yellowViewController.view atIndex:0];
    }
    else
    {
        if (self.blueViewController == nil)
        {
            BlueViewController *blueController = 
            [[BlueViewController alloc] initWithNibName:@"BlueView" 
                                                 bundle:nil];
            self.blueViewController = blueController;
            [blueController release];
        }
        [yellowViewController.view removeFromSuperview];
        [self.view insertSubview:blueViewController.view atIndex:0];
    }
}

 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	BlueViewController *blueController = [[BlueViewController alloc] initWithNibName:@"BlueView" bundle:nil];
	self.blueViewController = blueController;
	[self.view insertSubview:blueController.view atIndex:0];
	[blueController release];
	
//    [super viewDidLoad];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
    return YES;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[yellowViewController release];
	[blueViewController release];
    [super dealloc];
}


@end
