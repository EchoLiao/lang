//
//  MarkEnumerator+Private.h
//  TouchPainter
//
//  Created by Carlo Chung on 1/6/11.
//  Copyright 2011 Carlo Chung. All rights reserved.
//

#import <Foundation/Foundation.h>


// 内部接口, 其实现在 MarkEnumerator.m 中进行. 该文件只对内部开放, Client 看不到. 
@interface MarkEnumerator ()

- (id) initWithMark:(id <Mark>)mark;
- (void) traverseAndBuildStackWithMark:(id <Mark>)mark;

@end
