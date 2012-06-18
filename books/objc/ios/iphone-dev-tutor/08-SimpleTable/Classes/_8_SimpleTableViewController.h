//
//  _8_SimpleTableViewController.h
//  08-SimpleTable
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface _8_SimpleTableViewController : UIViewController 
		<UITableViewDelegate, UITableViewDataSource>         
{
    NSArray *listData;
}

@property (nonatomic, retain) NSArray *listData;

@end

