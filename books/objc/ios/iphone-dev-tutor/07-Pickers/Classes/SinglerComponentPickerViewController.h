//
//  SinglerComponentPickerViewController.h
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SinglerComponentPickerViewController : UIViewController
			<UIPickerViewDelegate, UIPickerViewDataSource> {
	IBOutlet	UIPickerView *singlePicker;
	NSArray	*pickerData;
}

@property (nonatomic, retain) UIPickerView *singlePicker;
@property (nonatomic, retain) NSArray *pickerData;
- (IBAction)buttonPressed:(id)sender;

@end
