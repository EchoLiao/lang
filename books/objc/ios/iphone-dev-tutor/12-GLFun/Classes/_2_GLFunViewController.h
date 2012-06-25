//
//  _2_GLFunViewController.h
//  12-GLFun
//
//  Created by nuoerlz on 12-6-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Constants.h"

@interface _2_GLFunViewController : UIViewController {
    UISegmentedControl *colorControl;
}

@property (nonatomic, retain) IBOutlet UISegmentedControl *colorControl;

- (IBAction)changeColor:(id)sender;
- (IBAction)changeShape:(id)sender;

@end

