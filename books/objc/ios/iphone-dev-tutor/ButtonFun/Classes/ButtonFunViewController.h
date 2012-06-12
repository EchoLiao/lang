//
//  ButtonFunViewController.h
//  ButtonFun
//
//  Created by 睢常明 on 12-6-10.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ButtonFunViewController : UIViewController {
	UILabel	*statusText;	
}

@property (nonatomic, retain) IBOutlet UILabel *statusText;
- (IBAction)buttonPressed:(id)sender;
@end

