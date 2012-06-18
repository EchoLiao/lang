//
//  DataPickerViewController.h
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DataPickerViewController : UIViewController {
	IBOutlet	UIDatePicker	*datePicker;
}

@property (nonatomic, retain) UIDatePicker *datePicker;
-(IBAction)buttonPressed:(id)sender;

@end
