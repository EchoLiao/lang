//
//  Shader.vsh
//  OpenGL-ES-Hello
//
//  Created by nuoerlz on 12-6-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

uniform float translate;

void main()
{
    gl_Position = position;
//    gl_Position.y += sin(translate) / 2.0;
//    gl_Position.x += cos(translate) / 2.0;

    colorVarying = color;
}
