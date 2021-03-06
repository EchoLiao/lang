//
//  _8_SectionsViewController.m
//  08-Sections
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "_8_SectionsViewController.h"
#import "NSDdictionary-MutableDeepCopy.h"


@implementation _8_SectionsViewController

@synthesize names;
@synthesize keys;
@synthesize table;
@synthesize search;
@synthesize allNames;


#pragma mark -
#pragma mark Custom Methods

- (void)resetSearch {
    NSMutableDictionary *allNamesCopy = [self.allNames mutableDeepCopy];
    self.names = allNamesCopy;
    [allNamesCopy release];
    NSMutableArray *keyArray = [[NSMutableArray alloc] init];
    [keyArray addObject:UITableViewIndexSearch]; // QQQQQ (__A__)
    [keyArray addObjectsFromArray:[[self.allNames allKeys] 
                                   sortedArrayUsingSelector:@selector(compare:)]];
    self.keys = keyArray;
    [keyArray release];
}

- (void)handleSearchForTerm:(NSString *)searchTerm
{
    NSMutableArray *sectionsToRemove = [[NSMutableArray alloc] init];
    [self resetSearch];
    
    for (NSString *key in self.keys) {
        NSMutableArray *array = [names valueForKey:key]; // 引用
        NSMutableArray *toRemove = [[NSMutableArray alloc] init];
        for (NSString *name in array) {
            if ([name rangeOfString:searchTerm 
                            options:NSCaseInsensitiveSearch].location == NSNotFound)
                [toRemove addObject:name];
        }
        
        if ([array count] == [toRemove count])
            [sectionsToRemove addObject:key];
        
        [array removeObjectsInArray:toRemove]; // ...
        [toRemove release];
    }
    [self.keys removeObjectsInArray:sectionsToRemove];
    [sectionsToRemove release];
    [table reloadData];
}


- (void)viewDidLoad {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"sortednames"
                                                     ofType:@"plist"];
    NSDictionary *dict = [[NSDictionary alloc] 
                          initWithContentsOfFile:path];
	self.allNames = dict;
    [dict release];
    
    [self resetSearch];
    [table reloadData];
    [table setContentOffset:CGPointMake(0.0, 44.0) animated:NO]; // QQQQQ
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

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	self.names = nil;
    self.keys = nil;
    self.table = nil;
    self.search = nil;
    self.allNames = nil;
}


- (void)dealloc {
	[names release];
    [keys release];
    [table release];
    [search release];
    [allNames release];
    [super dealloc];
}


#pragma mark -
#pragma mark Table View Data Source Methods

// 共多少个分区?
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [keys count];
}

// 各个分区各有多少行？
- (NSInteger)tableView:(UITableView *)tableView 
 numberOfRowsInSection:(NSInteger)section
{
	if ([keys count] == 0)
        return 0;
    
    NSString *key = [keys objectAtIndex:section];
    NSArray *nameSection = [names objectForKey:key];
    return [nameSection count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView 
         cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger section = [indexPath section];
    NSUInteger row = [indexPath row];
    
    NSString *key = [keys objectAtIndex:section];
    NSArray *nameSection = [names objectForKey:key];
    
    static NSString *SectionsTableIdentifier = @"SectionsTableIdentifier";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:
                             SectionsTableIdentifier ];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault 
                                       reuseIdentifier: SectionsTableIdentifier ] autorelease];
    }
    
//    cell.text = [nameSection objectAtIndex:row];
	cell.textLabel.text = [nameSection objectAtIndex:row];
    return cell;
}

- (NSString *)tableView:(UITableView *)tableView 
titleForHeaderInSection:(NSInteger)section
{
    if ([keys count] == 0)
        return nil;
	
    NSString *key = [keys objectAtIndex:section];
    if (key == UITableViewIndexSearch) // (__A__)
        return nil;
    
    return key;
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView
{
    return keys;
}


#pragma mark -
#pragma mark Table View Delegate Methods

- (NSIndexPath *)tableView:(UITableView *)tableView 
  willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [search resignFirstResponder];
    search.text = @"";
    isSearching = NO;
    [tableView reloadData];
    return indexPath;
}

- (NSInteger)tableView:(UITableView *)tableView 
sectionForSectionIndexTitle:(NSString *)title 
               atIndex:(NSInteger)index
{
    NSString *key = [keys objectAtIndex:index];
    if (key == UITableViewIndexSearch) // (__A__)
    {
        [tableView setContentOffset:CGPointZero animated:NO];
        return NSNotFound;
    }
     else return index;
}


#pragma mark -
#pragma mark Search Bar Delegate Methods

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar
{
    NSString *searchTerm = [searchBar text];
    [self handleSearchForTerm:searchTerm];
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)searchBar
{
    isSearching = YES;
    [table reloadData];
}

- (void)searchBar:(UISearchBar *)searchBar 
    textDidChange:(NSString *)searchTerm
{
    if ([searchTerm length] == 0)
    {
        [self resetSearch];
        [table reloadData];
        return;
    }
    [self handleSearchForTerm:searchTerm];
    
}

- (void)searchBarCancelButtonClicked:(UISearchBar *)searchBar
{
    isSearching = NO;
    search.text = @"";
    [self resetSearch];
    [table reloadData];
    [searchBar resignFirstResponder];
}


@end
