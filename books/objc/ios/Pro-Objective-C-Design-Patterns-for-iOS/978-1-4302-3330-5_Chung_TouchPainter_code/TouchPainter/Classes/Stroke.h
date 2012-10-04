//
//  Stroke.h
//  Composite
//
//  Created by Carlo Chung on 9/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Mark.h"
#import "MarkEnumerator.h"

@protocol MarkVisitor;

@interface Stroke : NSObject <Mark>
{
  @private
  UIColor *color_;
  CGFloat size_;
  NSMutableArray *children_;
}

// ---------------
// MARK-Composite:
#pragma mark - Property Interface
@property (nonatomic, retain) UIColor *color;
@property (nonatomic, assign) CGFloat size;
@property (nonatomic, assign) CGPoint location;
@property (nonatomic, readonly) NSUInteger count;
@property (nonatomic, readonly) id <Mark> lastChild;

- (void) addMark:(id <Mark>) mark;
- (void) removeMark:(id <Mark>) mark;
- (id <Mark>) childMarkAtIndex:(NSUInteger) index;
// ---------------


// for the Prototype pattern
- (id) copyWithZone:(NSZone *)zone;

// for the Visitor pattern
// MARK-Visitor:
- (void) acceptMarkVisitor:(id <MarkVisitor>)visitor;

// MARK-Iterator: 提供的迭代器
// for the Iterator pattern
- (NSEnumerator *) enumerator;

// for internal iterator implementation
- (void) enumerateMarksUsingBlock:(void (^)(id <Mark> item, BOOL *stop)) block;

// for the Memento pattern
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

@end
