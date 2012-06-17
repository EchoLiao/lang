//
//  DoubleComponentPickerViewController.h
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


#define kFillingComponent 0
#define kBreadComponent 1

@interface DoubleComponentPickerViewController : UIViewController
			<UIPickerViewDelegate, UIPickerViewDataSource> {
	IBOutlet	UIPickerView *doublePicker;
	NSArray *breadTypes;
	NSArray *fillingTypes;
}

@property(nonatomic, retain) UIPickerView *doublePicker;
@property(nonatomic, retain) NSArray *breadTypes;
@property(nonatomic, retain) NSArray *fillingTypes;

// -(IBAction)buttonPressed; // 错误！！！
-(IBAction)buttonPressed:(id)sender;

@end

