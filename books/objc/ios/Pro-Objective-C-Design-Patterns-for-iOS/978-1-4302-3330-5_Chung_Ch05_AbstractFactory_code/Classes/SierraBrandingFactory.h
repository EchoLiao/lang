//
//  SierraBrandingFactory.h
//  AbstractFactory
//
//  Created by Carlo Chung on 11/16/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BrandingFactory.h"


@interface SierraBrandingFactory : BrandingFactory
{
	
}

- (UIView*) brandedView;
- (UIButton*) brandedMainButton;
- (UIToolbar*) brandedToolbar;

@end

