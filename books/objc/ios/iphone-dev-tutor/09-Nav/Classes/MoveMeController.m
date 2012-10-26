//
//  MoveMeController.m
//  Nav
//
//  Created by jeff on 4/22/09.
//  Copyright 2009 Jeff LaMarche. All rights reserved.
//

#import "MoveMeController.h"

@implementation MoveMeController

@synthesize list;


-(IBAction)toggleMove
{
    [self.tableView setEditing:!self.tableView.editing animated:YES];
    
    if (self.tableView.editing)
        [self.navigationItem.rightBarButtonItem setTitle:@"Done"];
    else
        [self.navigationItem.rightBarButtonItem setTitle:@"Move"];
}

-(void)goBack
{
    [self.navigationController popViewControllerAnimated:YES];
}


- (void)dealloc {
    [list release];
    [super dealloc];
}
- (void)viewDidLoad {
    if (list == nil)
    {
        NSMutableArray *array = [[NSMutableArray alloc] initWithObjects:
                                 @"Eeny", @"Meeny", @"Miney", @"Moe", @"Catch", @"A", 
                                 @"Tiger", @"By", @"The", @"Toe", nil];
        self.list = array;
        [array release];        
    }
    
    
    CGRect btnRect = CGRectMake(0, 0, 65, 28);
    
    UIButton * btn = [[UIButton alloc]initWithFrame:btnRect];
    [btn setBackgroundImage:[UIImage imageNamed:@"ok"] forState:UIControlStateNormal];
    [btn setBackgroundImage:[UIImage imageNamed:@"cancel"] forState:UIControlStateHighlighted];
    [btn setBackgroundImage:[UIImage imageNamed:@"back"] forState:UIControlStateSelected];
    btn.showsTouchWhenHighlighted = YES;
    [btn setTitle:@"Test2" forState:UIControlStateNormal];
    [btn addTarget:self action:NSSelectorFromString(@"toggleMove")
              forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *moveButton = [[UIBarButtonItem alloc] initWithCustomView:btn];
    self.navigationItem.rightBarButtonItem = moveButton;
    [btn release];
    [moveButton release];
    
    
//    UIImageView *iconView=[[UIImageView alloc] initWithImage:[UIImage imageNamed:@"back"]];
//    UIBarButtonItem *leftButton = [[UIBarButtonItem alloc] initWithCustomView:iconView];
//    self.navigationItem.leftBarButtonItem = leftButton;
    
    btn = [[UIButton alloc]initWithFrame:btnRect];
    [btn setBackgroundImage:[UIImage imageNamed:@"back"] forState:UIControlStateNormal];
    [btn setBackgroundImage:[UIImage imageNamed:@"cancel"] forState:UIControlStateHighlighted];
    [btn setBackgroundImage:[UIImage imageNamed:@"ok"] forState:UIControlStateSelected];
//    btn.showsTouchWhenHighlighted = YES;
    [btn setTitle:@"Test2" forState:UIControlStateNormal];
    [btn addTarget:self action:NSSelectorFromString(@"goBack")
              forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithCustomView:btn];
    self.navigationItem.leftBarButtonItem = backButton;
    [btn release];
    [backButton release];
    
    [super viewDidLoad];
}


#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView 
 numberOfRowsInSection:(NSInteger)section {
    return [list count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *MoveMeCellIdentifier = @"MoveMeCellIdentifier";
    
    UITableViewCell *cell = [tableView 
                             dequeueReusableCellWithIdentifier:MoveMeCellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:MoveMeCellIdentifier] autorelease];
        cell.showsReorderControl = YES;
        
    }
    NSUInteger row = [indexPath row];
    cell.textLabel.text = [list objectAtIndex:row];
    
    return cell;
}
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView 
           editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath {
    return UITableViewCellEditingStyleNone;
}
- (BOOL)tableView:(UITableView *)tableView 
canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    return YES;
}
- (void)tableView:(UITableView *)tableView 
moveRowAtIndexPath:(NSIndexPath *)fromIndexPath 
      toIndexPath:(NSIndexPath *)toIndexPath {
    NSUInteger fromRow = [fromIndexPath row];
    NSUInteger toRow = [toIndexPath row];
    
    id object = [[list objectAtIndex:fromRow] retain];
    [list removeObjectAtIndex:fromRow];
    [list insertObject:object atIndex:toRow];
    [object release];
}
@end
