// Copyright 2020 NXP Semiconductors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef _DEWARP_H_
#define _DEWARP_H_

#define DEWARP_VERT_FILE "dewarp.vert"
#define DEWARP_FRAG_FILE "dewarp.frag"
#define IMAGE_EXAMPLE 	 "example.jpg"

EGLDisplay egldisplay;
EGLSurface eglsurface;
EGLConfig eglconfig;
EGLContext eglcontext;
EGLNativeWindowType eglNativeWindow;
EGLNativeDisplayType eglNativeDisplayType;

EGLint s_configAttribs[] =  { EGL_RED_SIZE,  5,
                              EGL_GREEN_SIZE,  6,
                              EGL_BLUE_SIZE,  5,
                              EGL_ALPHA_SIZE,  0,
                              EGL_SAMPLES,  0,
                              EGL_NONE
                            };
EGLint numconfigs;

GLuint locVertices = 0;
GLuint locTransformMat = 0;
GLuint locTexcoord = 0;
GLuint locSampler = 0;
GLuint locMouse = 0;
GLuint locResolution = 0;
GLuint vertShaderNum  = 0;
GLuint pixelShaderNum = 0;
GLuint programHandle  = 0;

GLuint hVertexShader;
GLuint hFragmentShader;

GLuint texture[1];
GLuint texture2D[1];

const GLfloat vertices[][2] = {	{ -1.0f, -1.0f},
				{  1.0f, -1.0f},
				{ -1.0f, 1.0f},
				{  1.0f,  1.0f}
			      };

const GLfloat texcoords[][2] = { { 0.0f, 1.0f},
				 { 1.0f, 1.0f},
				 { 0.0f, 0.0f},
				 { 1.0f, 0.0f}
			       };

GLfloat transformMatrix[16] = {	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			      };

#endif /* _DEWARP_H_DEFINED */
