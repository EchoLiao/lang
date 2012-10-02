//
//  ConsoleEmulator.h
//  Bridge
//
//  Created by Carlo Chung on 11/26/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ConsoleCommands.h"


// MARK-Bridge: ConsoleEmulator 充当"Implementor". [(P107)]
@interface ConsoleEmulator : NSObject
{

}

- (void) loadInstructionsForCommand:(ConsoleCommand) command;
- (void) executeInstructions;

// other behaviors and properties.

@end
