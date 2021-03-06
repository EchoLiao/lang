//
//  Dot.h
//  Composite
//
//  Created by Carlo Chung on 9/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Vertex.h"

@protocol MarkVisitor;

@interface Dot : Vertex
{
  @private
  UIColor *color_;
  CGFloat size_;
}

// for the Prototype pattern
- (id) copyWithZone:(NSZone *)zone;

// for the Visitor pattern
// MARK-Visitor:
- (void) acceptMarkVisitor:(id <MarkVisitor>)visitor;

// for the Memento pattern
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@end
