//
//  _5_SwapViewController.h
//  05-Swap
//
//  Created by nuoerlz on 12-6-13.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define degreesToRadian(x) (M_PI * (x) / 180.0)

@interface _5_SwapViewController : UIViewController {

	UIView    *landscape;
    UIView    *portrait;
    
    // Foo
    UIButton *landscapeFooButton;
    UIButton *portraitFooButton;
    
    // Bar
    UIButton *landscapeBarButton;
    UIButton *portraitBarButton;
}

@property (nonatomic, retain) IBOutlet UIView *landscape;
@property (nonatomic, retain) IBOutlet UIView *portrait;
@property (nonatomic, retain) IBOutlet UIButton *landscapeFooButton;
@property (nonatomic, retain) IBOutlet UIButton *portraitFooButton;
@property (nonatomic, retain) IBOutlet UIButton *landscapeBarButton;
@property (nonatomic, retain) IBOutlet UIButton *portraitBarButton;

-(IBAction)buttonPressed:(id)sender;

@end

