//
//  DrawScribbleCommand.h
//  TouchPainter
//
//  Created by erlz nuo on 12-10-4.
//  Copyright (c) 2012年 Carlo Chung. All rights reserved.
//

#import "Command.h"
#import "Scribble.h"




@interface DrawScribbleCommand : Command
{
@private
    Scribble *scribble_;
    id<Mark> mark_;
    BOOL shouldAddToPreviousMark_;
}

- (void)execute;
- (void)undo;

@end
