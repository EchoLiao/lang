//
//  OpenGLView.h
//
//  Created by nuoerlz on 12-6-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>


@interface OpenGLView : UIView
{
@protected
	EAGLContext     *context;
	GLuint          viewRenderbuffer;
	GLuint          viewFramebuffer;
	GLint           backingWidth;
	GLint           backingHeight;
	GLfloat         backingDatum;
}

- (BOOL)initOpenGL;
- (BOOL)unInitOpenGL;

- (void)render;
- (void)renderRectInIosCoorWithX1:(float)x1
                           withY1:(float)y1
                           withX2:(float)x2
                           withY2:(float)y2;


- (void)OVDisplayClear;
- (void)OVDisplayShow;

- (void)OVDisplaySetColorWithR:(float)r withG:(float)g withB:(float)b withA:(float)a;

- (void)OVDisplayDrawRectWithX1:(float)x1 withY1:(float)y1 withX2:(float)x2 withY2:(float)y2;
- (void)OVDisplayFillRectWithX1:(float)x1 withY1:(float)y1 withX2:(float)x2 withY2:(float)y2;
- (void)OVDisplayDrawRectWithX:(float)x withY:(float)y withW:(float)w withH:(float)h;
- (void)OVDisplayFillRectWithX:(float)x withY:(float)y withW:(float)w withH:(float)h;


@end
