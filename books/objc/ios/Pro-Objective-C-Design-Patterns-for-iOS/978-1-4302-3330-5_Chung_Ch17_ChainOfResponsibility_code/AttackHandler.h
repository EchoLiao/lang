//
//  Entity.h
//  ChainOfResponsibility
//
//  Created by Carlo Chung on 11/28/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Attack.h"

// MARK-ChainOfResponsibility:
@interface AttackHandler : NSObject 
{
  @private
  AttackHandler *nextAttackHandler_;
}

@property (nonatomic, retain) AttackHandler *nextAttackHandler;

// Handle
- (void) handleAttack:(Attack *)attack;

@end
