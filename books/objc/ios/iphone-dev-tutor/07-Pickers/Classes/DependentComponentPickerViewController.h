//
//  DependentComponentPickerViewController.h
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define kStateComponent 0
#define kZipComponent 1

@interface DependentComponentPickerViewController : UIViewController
			<UIPickerViewDelegate, UIPickerViewDataSource> {
				
	IBOutlet	UIPickerView *picker;
	
	NSDictionary *stateZips;
	NSArray	*states;
	NSArray *zips;
}

@property (retain, nonatomic) UIPickerView *picker;
@property (retain, nonatomic) NSDictionary *stateZips;
@property (retain, nonatomic) NSArray *states;
@property (retain, nonatomic) NSArray *zips;

- (IBAction)butonPressed:(id)sender;

@end
