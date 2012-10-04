//
//  Mark.h
//  Composite
//
//  Created by Carlo Chung on 9/9/10.
//  Copyright 2010 Carlo Chung. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MarkVisitor.h"


// 原型模式, 应用于“复制"操作. 利用 NSCopying 协议. [(P39 P42)]
@protocol Mark <NSObject, NSCopying, NSCoding>
@optional

// ---------------
// MARK-Composite: Mark 充当组合模式中的节点的抽象节点. 其抽象并统一所有节点的方法接口, 因为
//                 为了给 Client 提供统一的接口方法, 而其又是一个抽象类(协议), 所以其直接子
//                 类应该实现其所有方法. [(P157)]
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


#pragma mark - Prototype Pattern Interface
- (id) copy;


#pragma mark - Others Interface
// for the Visitor pattern
// MARK-Visitor:
- (void) acceptMarkVisitor:(id <MarkVisitor>) visitor;

// for the Iterator pattern
- (NSEnumerator *) enumerator;

// for internal iterator implementation
- (void) enumerateMarksUsingBlock:(void (^)(id <Mark> item, BOOL *stop)) block;

// for a bad example
- (void) drawWithContext:(CGContextRef) context;


@end
