//
//  CabDriver.h
//  Facade
//
//  Created by Carlo Chung on 11/15/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"
#import "Taximeter.h"


// MARK-Facade: CabDriver 充当"Facader"
@interface CabDriver : NSObject 
{

}

- (void) driveToLocation:(CGPoint) x;

@end
