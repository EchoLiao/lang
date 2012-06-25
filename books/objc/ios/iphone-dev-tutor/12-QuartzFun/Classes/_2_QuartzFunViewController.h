//
//  _2_QuartzFunViewController.h
//  12-QuartzFun
//
//  Created by nuoerlz on 12-6-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface _2_QuartzFunViewController : UIViewController {
    UISegmentedControl *colorControl;
}

@property (nonatomic, retain) IBOutlet UISegmentedControl *colorControl;

- (IBAction)changeColor:(id)sender;
- (IBAction)changeShape:(id)sender;

@end

