//
//  _8_SectionsViewController.h
//  08-Sections
//
//  Created by nuoerlz on 12-6-18.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface _8_SectionsViewController : UIViewController
		<UITableViewDataSource, UITableViewDelegate>
{
    NSDictionary *names;
    NSArray     *keys;
}

@property (nonatomic, retain) NSDictionary *names;
@property (nonatomic, retain) NSArray *keys;

@end

