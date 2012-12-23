//
//  OpenGLView.h
//  GLFun
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


@interface OpenGLView : UIView {
@protected
	EAGLContext     *context;
	GLuint          viewRenderbuffer;
	GLuint          viewFramebuffer;
	GLint           backingWidth;
	GLint           backingHeight;
    
}

- (BOOL)initOpenGL;
- (BOOL)unInitOpenGL;
- (void)render;

@end
