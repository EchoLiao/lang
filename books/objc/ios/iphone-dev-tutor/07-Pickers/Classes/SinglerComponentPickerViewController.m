    //
//  SinglerComponentPickerViewController.m
//  07-Pickers
//
//  Created by nuoerlz on 12-6-17.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "SinglerComponentPickerViewController.h"


@implementation SinglerComponentPickerViewController

@synthesize singlePicker;
@synthesize pickerData;

- (IBAction)buttonPressed:(id)sender
{
	// 该选择器只有一个组件，所有我们可以知道选的就是缩影是0的组件。
	NSInteger row = [singlePicker selectedRowInComponent:0];
	NSString *selected = [pickerData objectAtIndex:row];
	NSString *title = [[NSString alloc] 
					   initWithFormat:@"You selected %@!", 
					   selected];
	UIAlertView *alert = [[UIAlertView alloc] 
						  initWithTitle:title 
						  message:@"Thank you for choosing." 
						  delegate:nil
						  cancelButtonTitle:@"You're Welcome" 
						  otherButtonTitles:nil];
	[alert show];
	[alert release];
	[title release];
}

- (void)viewDidLoad {
	NSArray *array = [[NSArray alloc] 
					  initWithObjects:@"Luke", @"Leia", @"Han",
					  @"Chewbacca", @"Artoo", @"Threepio", @"Lando", nil];
	self.pickerData = array;
	[array release];
    
    
    
    UIToolbar *toolbar = [[UIToolbar alloc] init];
    toolbar.frame = CGRectMake(0, 0, self.view.frame.size.width, 44);
    NSMutableArray *items = [[NSMutableArray alloc] init];
    [items addObject:[[[UIBarButtonItem alloc] initWithImage:nil style:UIBarButtonItemStyleBordered  target:self action:nil] autorelease]];
    [toolbar setItems:items animated:NO];
    [items release];
    [self.view addSubview:toolbar];
    [toolbar release];

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

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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
	[singlePicker release];
	[pickerData release];
    [super dealloc];
}

#pragma mark -
#pragma mark Picker Data Source Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	// 该选择器有多少个组件？
	return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView
			numberOfRowsInComponent:(NSInteger)component
{
	//该组件有多少行数据？
	return [pickerData count];
}


#pragma mark Picker Delegate Methods

- (NSString *)pickerView:(UIPickerView *)pickerView
			titleForRow:(NSInteger)row
			forComponent:(NSInteger)component
{
	// 返回当前被选中的行的数据
	return [pickerData objectAtIndex:row];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

@end
