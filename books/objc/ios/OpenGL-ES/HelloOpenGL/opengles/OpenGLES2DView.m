//
//  OpenGLES2DView.m
//  GLFun
//
//  Created by nuoerlz on 12-8-28.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "OpenGLES2DView.h"

#define USE_OPENGL_ES_2




#ifdef USE_OPENGL_ES_2
GLuint  iProgm, pos_loc, color_loc;
#endif


@implementation OpenGLES2DView

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
    glDeleteFramebuffersOES (1, &viewFramebuffer);
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
}

#pragma mark -

- (BOOL)initOpenGL
{
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
    eaglLayer.opaque = YES;
    if ( eaglLayer.drawableProperties ) {
        [eaglLayer.drawableProperties release];
        eaglLayer.drawableProperties = nil;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    }
#ifdef USE_OPENGL_ES_2
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
#else 
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
#endif
    
    if(!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer]) {
        return NO;
    }
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    // NSLog(@"%f, %f", self.frame.size.width, self.frame.size.height);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0.0, 0.0,  0.0, 1.0);
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DITHER);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_SAMPLE_COVERAGE);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glShadeModel(GL_SMOOTH);
    
    
#ifdef USE_OPENGL_ES_2
    setShaders();
#endif
    
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

- (void)render
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1.0, 0.0,  0.0, 1.0);
    
    GLfloat	vertices[] = {
        -0.5,  -0.5, 0.0,
        0.5,  -0.5, 0.0,
        0.0,   0.5, 0.0, };
    GLfloat	colors[] = {
        1.0,   0.0, 0.0, 1.0,
        0.0,   1.0, 0.0, 1.0,
        0.0,   0.0, 1.0, 1.0, };
    
#ifdef USE_OPENGL_ES_2
    float ortho[16] = {
        1, 0,  0, 0,
        0, 1,  0, 0,
        0, 0,  1, 0,
        0, 0,  0, 1
    };
    float zRotation[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(iProgm, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
    GLint modelviewUniform = glGetUniformLocation(iProgm, "Modelview");
    glUniformMatrix4fv(modelviewUniform, 1, 0, &zRotation[0]);
    
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), colors);
    glEnableVertexAttribArray(color_loc);
#else
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
#endif
    
	glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


#pragma mark -

- (id)initWithCoder:(NSCoder*)coder
{
	if ( (self = [super initWithCoder:coder]) ) {
	}
    return self;
}


- (void)dealloc {
	[super dealloc];
}



#pragma mark -
#pragma mark Methos For OpenGL ES 2.0


#ifdef USE_OPENGL_ES_2

static char *vs =
"attribute vec4 ar_pos;\n"
"attribute vec4 ar_color;\n"
"uniform mat4 Projection;\n"
"uniform mat4 Modelview;\n"
"varying vec4 v_color;\n"
"void main()\n"
"{\n"
"   v_color = ar_color;\n"
"	gl_Position = Projection * Modelview * ar_pos;\n"
"}\n";


// precision highp float;
static char *fs =
"precision mediump float;"
"varying vec4 v_color;\n"
"void main()\n"
"{\n"
"vec4 color = vec4(0.5, 0.5, 0.2, 1.0);\n"
"gl_FragColor = v_color * color;\n"
"}\n";



void setShaders() 
{
    GLint iVerSh, iFraSh;
	const char *vv = vs;
	const char *ff = fs;
    GLint compileSuccess;
    
	iVerSh = glCreateShader(GL_VERTEX_SHADER);
	iFraSh = glCreateShader(GL_FRAGMENT_SHADER);
    
    
	glShaderSource(iVerSh, 1, &vv,NULL);
	glCompileShader(iVerSh);
    glGetShaderiv(iVerSh, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(iVerSh, sizeof(messages), 0, &messages[0]);
        printf("------%s", messages);
        assert(0);
    }
    
    
	glShaderSource(iFraSh, 1, &ff,NULL);
	glCompileShader(iFraSh);
    glGetShaderiv(iFraSh, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(iFraSh, sizeof(messages), 0, &messages[0]);
        printf("------%s", messages);
        assert(0);
    }
    
    
	iProgm = glCreateProgram();
	glAttachShader(iProgm,iFraSh);
	glAttachShader(iProgm,iVerSh);
    
	glLinkProgram(iProgm);
    GLint linkSuccess;
    glGetProgramiv(iProgm, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(iProgm, sizeof(messages), 0, &messages[0]);
        printf("------%s", messages);
        assert(0);
    }
    
    
    pos_loc = glGetAttribLocation(iProgm, "ar_pos");
    color_loc = glGetAttribLocation(iProgm, "ar_color");
    
	glUseProgram(iProgm);
}

#endif




@end
