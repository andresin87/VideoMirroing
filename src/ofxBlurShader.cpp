/*
 *  ofxBlurShader.cpp
 *
 *  Created by Rick Companje on 25-10-10.
 *  updated on 15-07-11 for of007
 *  Based on the blurShader example by Theo
 *
 */

#include "ofxBlurShader.h"

void ofxBlurShader::setup(float w, float h) {
    
    fbo1.allocate(w,h);
    fbo2.allocate(w,h);
    
    string vertexShader =
    "void main() {\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
    gl_Position = ftransform();\
    }";
    
    //what the blur shader does: it takes blurAmnt as input and takes for each 'pixel' / fragment a
    //weighted average of itself and 8 horizontal neighbours (which neighbours depends on the blurAmnt
    
    string fragmentShaderHorizontal =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float amt;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color;\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(amt * -4.0, 0.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(amt * -3.0, 0.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(amt * -2.0, 0.0));\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(amt * -1.0, 0.0));\
    color += 5.0 * texture2DRect(src_tex_unit0, st + vec2(amt, 0));\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(amt * 1.0, 0.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(amt * 2.0, 0.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(amt * 3.0, 0.0));\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(amt * 4.0, 0.0));\
    color /= 25.0;\
    gl_FragColor = color;\
    }";
    
    // same but now for vertical neighbours
    
    string fragmentShaderVertical =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float amt;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color;\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * 4.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * 3.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * 2.0));\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * 1.0));\
    color += 5.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt) );\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * -1.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * -2.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * -3.0));\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, amt * -4.0));\
    color /= 25.0;\
    gl_FragColor = color;\
    }";
    
    
    vShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderVertical);
    vShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    vShader.linkProgram();
    
    hShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderHorizontal);
    hShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    hShader.linkProgram();
}

void ofxBlurShader::begin(int amount, int iterations) {
    this->amount = amount;
    this->iterations = iterations;
    
    //start 'recording' the user's drawing commands to the fbo
    fbo1.begin();
}

void ofxBlurShader::end() {
    fbo1.end();
    
    for (int i=0; i<iterations; i++) {
        
        //apply horizontal blur to fbo1 and put the result in fbo2
        fbo2.begin();
        hShader.begin();
        hShader.setUniform1f("amt", amount);
        fbo1.draw(0,0);
        hShader.end();
        fbo2.end();
        
        //apply vertical blur to fbo2 and put this back in fbo1
        fbo1.begin();
        vShader.begin();
        vShader.setUniform1f("amt", amount);
        fbo2.draw(0,0);
        hShader.end();
        fbo1.end();
    }
    
    fbo1.draw(0,0);
}
