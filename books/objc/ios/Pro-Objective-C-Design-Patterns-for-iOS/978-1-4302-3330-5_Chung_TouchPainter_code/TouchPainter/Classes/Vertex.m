//
//  Vertex.m
//  Composite
//
//  Created by Carlo Chung on 9/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import "Vertex.h"


@implementation Vertex


- (id) initWithLocation:(CGPoint) aLocation
{
  if (self = [super init])
  {
    [self setLocation:aLocation];
  }
  
  return self;
}


// MARK-Composite:
#pragma mark - Property Interface
@synthesize location=location_;
@dynamic color, size;

// MARK-Composite: 直接子类应该实现其所有方法.
- (void)setColor:(UIColor *)color {}
- (UIColor*)color { return nil; }
- (void)setSize:(CGFloat)size {}
- (CGFloat)size { return 0.0; }
- (id <Mark>)lastChild { return nil; }
- (NSUInteger)count { return 0; }

// MARK-Composite: 直接子类应该实现其所有方法.
- (void) addMark:(id <Mark>) mark {}
- (void) removeMark:(id <Mark>) mark {}
- (id <Mark>) childMarkAtIndex:(NSUInteger) index { return nil; }



#pragma mark - Prototype Pattern Interface
#pragma mark NSCopying method

// it needs to be implemented for memento
- (id)copyWithZone:(NSZone *)zone
{
  Vertex *vertexCopy = [[[self class] allocWithZone:zone] initWithLocation:location_];
  
  return vertexCopy;
}


#pragma mark - Others Interface

- (NSEnumerator *) enumerator { return nil; }

- (void) acceptMarkVisitor:(id <MarkVisitor>)visitor
{
  [visitor visitVertex:self];
}


#pragma mark -
#pragma mark NSCoder methods

- (id)initWithCoder:(NSCoder *)coder
{
  if (self = [super init])
  {
    location_ = [(NSValue *)[coder decodeObjectForKey:@"VertexLocation"] CGPointValue];
  }
  
  return self;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
  [coder encodeObject:[NSValue valueWithCGPoint:location_] forKey:@"VertexLocation"];
}

#pragma mark -
#pragma mark MarkIterator methods

// for internal iterator implementation
- (void) enumerateMarksUsingBlock:(void (^)(id <Mark> item, BOOL *stop)) block {}

#pragma mark -
#pragma mark An Extended Direct-draw Example

// for a direct draw example
- (void) drawWithContext:(CGContextRef)context
{
  CGFloat x = self.location.x;
  CGFloat y = self.location.y;
  
  CGContextAddLineToPoint(context, x, y);
}

@end
