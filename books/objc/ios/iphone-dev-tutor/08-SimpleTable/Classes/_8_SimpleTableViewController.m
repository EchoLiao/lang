//
//  _8_SimpleTableViewController.m
//  08-SimpleTable
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "_8_SimpleTableViewController.h"

@implementation _8_SimpleTableViewController

@synthesize listData;

- (void)viewDidLoad {
    NSArray *array = [[NSArray alloc] initWithObjects:@"Sleepy", @"Sneezy",
                      @"Bashful", @"Happy", @"Doc", @"Grumpy", @"Dopey", @"Thorin",
                      @"Dorin", @"Nori", @"Ori", @"Balin", @"Dwalin", @"Fili", @"Kili",
                      @"Oin", @"Gloin", @"Bifur", @"Bofur", @"Bombur", nil];
    self.listData = array;
    [array release];
    [super viewDidLoad];
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
    self.listData = nil;
    [super viewDidUnload];
}



/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
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
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)dealloc {
    [listData release];
    [super dealloc];
}


#pragma mark -
#pragma mark Table View Data Source Methods

- (NSInteger)tableView:(UITableView *)tableView 
 numberOfRowsInSection:(NSInteger)section
{
	// 表的各个分区各有多少行？由于该表只有一个分区所以。。。
    return [self.listData count];
}

// 表通过次方法来请求数据
- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath 
{
    
    static NSString *SimpleTableIdentifier = @"SimpleTableIdentifier";
	
	// 想表索求一个“出列单元”
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:
                             SimpleTableIdentifier];
    if (cell == nil) { // 没有“出列单元”？
#ifdef __IPHONE_3_0
        // Other styles you can try
        // UITableViewCellStyleSubtitle
        // UITableViewCellStyleValue1
        // UITableViewCellStyleValue2
		
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier: SimpleTableIdentifier] autorelease];
#else
        cell = [[[UITableViewCell alloc] initWithFrame::CGRectZero
                                       reuseIdentifier: SimpleTableIdentifier] autorelease];
#endif
        
    }
    
//    UIImage *image = [UIImage imageNamed:@"star.png"];
//    UIImage *image2 = [UIImage imageNamed:@"star2.png"];
//    cell.imageView.image = image;
//    cell.imageView.highlightedImage = image2;
	
    NSUInteger row = [indexPath row];
    cell.textLabel.text = [listData objectAtIndex:row];
//    cell.textLabel.font = [UIFont boldSystemFontOfSize:50];
	
#ifdef __IPHONE_3_0    
    if (row < 7)
        cell.detailTextLabel.text = @"Mr. Disney";
    else
        cell.detailTextLabel.text = @"Mr. Tolkein";
#endif
    return cell;
}


#pragma mark -
#pragma mark Table Delegate Methods 

// 设置每行的缩进
//- (NSInteger)tableView:(UITableView *)tableView 
//indentationLevelForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    NSUInteger row = [indexPath row];
//    return row;
//}

// 相应点击操作，在高亮被点击行前，该方法被调用。
-(NSIndexPath *)tableView:(UITableView *)tableView 
 willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger row = [indexPath row];
    if (row == 0) // 使第一行不能被选中
        return nil;
    
    return indexPath; 
}

// 相应点击操作，在高亮被点击行后，该方法被调用。
- (void)tableView:(UITableView *)tableView 
didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger row = [indexPath row];
    NSString *rowValue = [listData objectAtIndex:row];
    
    NSString *message = [[NSString alloc] initWithFormat:
                         @"You selected %@", rowValue];
    UIAlertView *alert = [[UIAlertView alloc] 
                          initWithTitle:@"Row Selected!"
                          message:message 
                          delegate:nil 
                          cancelButtonTitle:@"Yes I Did" 
                          otherButtonTitles:nil];
    [alert show];
    
    [message release];
    [alert release];
    [tableView deselectRowAtIndexPath:indexPath animated:YES]; // QQQQQ
}

// 设置行高。
//- (CGFloat)tableView:(UITableView *)tableView 
//heightForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return 70;
//}

@end
