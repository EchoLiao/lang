//
//  _4_ControlFun_2ViewController.h
//  04-ControlFun-2
//
//  Created by 睢常明 on 12-6-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

//#define kSwitchesSegmentIndex    0
#define kShowSegmentIndex		0

@interface _4_ControlFun_2ViewController : UIViewController
		<UIActionSheetDelegate> {
	UITextField	*nameField;
	UITextField *numberField;
	UILabel     *sliderLabel;
	UISwitch	*leftSwitch;
    UISwitch	*rightSwitch;
	UIView		*switchView;
	UIButton	*doSomethingButton;
}


@property (nonatomic, retain) IBOutlet UITextField *nameField;
@property (nonatomic, retain) IBOutlet UITextField *numberField;
@property (nonatomic, retain) IBOutlet UILabel *sliderLabel;
@property (nonatomic, retain) IBOutlet UISwitch *leftSwitch;
@property (nonatomic, retain) IBOutlet UISwitch *rightSwitch;
@property (nonatomic, retain) IBOutlet UIView	*switchView;
@property (nonatomic, retain) IBOutlet UIButton	*doSomethingButton;

- (IBAction)textFieldDoneEditing:(id)sender;
- (IBAction)backgroundTap:(id)sender;
- (IBAction)sliderChanged:(id)sender;
- (IBAction)switchChanged:(id)sender;
//- (IBAction)buttonPressed;
- (IBAction)toggleShowHide:(id)sender;
-(IBAction)doSomething:(id)sender;

@end

