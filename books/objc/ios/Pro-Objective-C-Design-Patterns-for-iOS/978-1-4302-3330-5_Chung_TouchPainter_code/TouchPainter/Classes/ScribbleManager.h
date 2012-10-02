//
//  ScribbleManager.h
//  TouchPainter
//
//  Created by Carlo Chung on 9/20/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Scribble.h"
#import "ScribbleThumbnailViewImageProxy.h"

// MARK-Facade: ScribbleManager 充当"外观". [(P118)]
@interface ScribbleManager : NSObject 
{
	
}

- (void) saveScribble:(Scribble *)scribble thumbnail:(UIImage *)image;
- (NSInteger) numberOfScribbles;
- (Scribble *) scribbleAtIndex:(NSInteger)index;
- (UIView *) scribbleThumbnailViewAtIndex:(NSInteger)index;

@end
