//
//  OpenGLView.m
//  GLFun
//
//  Created by nuoerlz on 12-8-28.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "OpenGLView.h"


@implementation OpenGLView

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}


#pragma mark -

- (BOOL)createFramebuffer
{
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES,
                                 viewRenderbuffer);

	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"failed, framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}

	return YES;
}

- (void)freeFramebuffer
{
    if ( viewFramebuffer ) {
        glDeleteFramebuffersOES (1, &viewFramebuffer);
        viewFramebuffer = 0;
    }
    if ( viewRenderbuffer ) {
        glDeleteRenderbuffersOES(1, &viewRenderbuffer);
        viewRenderbuffer = 0;
    }
}

- (void)dealloc
{
    assert(context == nil);
	[super dealloc];
}


#pragma mark -

- (BOOL)initOpenGL
{
    NSAutoreleasePool *autoPool = [[NSAutoreleasePool alloc] init];

    CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
    eaglLayer.opaque = YES;
    NSDictionary *dic = [NSDictionary dictionaryWithObjectsAndKeys:
                         [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                         kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    eaglLayer.drawableProperties = dic;

    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    if ( !context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer] ) {
        return NO;
    }

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float asp = (float) backingHeight / backingWidth;
    glOrthof(-0.5, 0.5, -asp, asp, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DITHER);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_SAMPLE_COVERAGE);
    glDisable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);

    [autoPool release];

    return YES;
}

- (BOOL)unInitOpenGL
{
    [self freeFramebuffer];
    if ( context ) {
        [context release];
        context = nil;
    }

    return YES;
}

#pragma mark -


- (void)render
{
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat	vertices[] = {
        -0.5,  -0.5, 0.0,
        0.5,  -0.5, 0.0,
        0.0,   0.5, 0.0, };
    GLfloat	colors[] = {
        1.0,   0.0, 0.0, 1.0,
        0.0,   1.0, 0.0, 1.0,
        0.0,   0.0, 1.0, 1.0, };

    float x1 = -0.5;
    float x2 = -x1;
    float y1 = -(float)backingHeight / backingWidth;
    float y2 = -y1;
    GLfloat	rect[] = {
        x1, y1, 0.0,
        x1, y2, 0.0,
        x2, y1, 0.0,
        x2, y2, 0.0,
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, rect);
    glPushMatrix(); {
        glScalef(0.95, 0.95, 1.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(rect)/sizeof(rect[0])/3);
    } glPopMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0])/3);

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


@end
