//
//  _8_CellsViewController.h
//  08-Cells
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#define kNameValueTag     1
#define kColorValueTag    2

@interface _8_CellsViewController : UIViewController 
		<UITableViewDataSource, UITableViewDelegate>
{
    NSArray    *computers;
}

@property (nonatomic, retain) NSArray *computers;


@end

