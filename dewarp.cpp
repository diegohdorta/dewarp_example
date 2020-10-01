// Copyright 2020 NXP Semiconductors
// SPDX-License-Identifier: BSD-3-Clause

#include <iostream>

#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include <IL/il.h>

#include "FSL/fsl_egl.h"
#include "FSL/fslutil.h"

#include "dewarp.hpp"

void start(void);
void load_image_textures(void);
void render(float w, float h);

void load_shaders(const char *vShaderFName, const char *pShaderFName);
void compile_shader(const char *FName, GLuint ShaderNum);

int main(void)
{
    EGLint egl_width = 0;
    EGLint egl_height = 0;
    
    ilInit();    
    start();

    while (1) {  
        eglQuerySurface(egldisplay, eglsurface, EGL_WIDTH, &egl_width);
        
        eglQuerySurface(egldisplay, eglsurface,	EGL_HEIGHT, &egl_height);
        
        render(egl_width, egl_height);
        
        eglSwapBuffers(egldisplay, eglsurface);
    }    
    return EXIT_SUCCESS;
}

void start(void)
{
    std::string dewarp_vert = DEWARP_VERT_FILE;  
    std::string dewarp_frag = DEWARP_FRAG_FILE;

    eglNativeDisplayType = fsl_getNativeDisplay();
    
    egldisplay = eglGetDisplay(eglNativeDisplayType);
    
    eglInitialize(egldisplay, NULL, NULL);
    
    assert(eglGetError() == EGL_SUCCESS);
    
    eglBindAPI(EGL_OPENGL_ES_API);

    eglChooseConfig(egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
    
    assert(numconfigs == 1);
    
    assert(eglGetError() == EGL_SUCCESS);
    
    eglNativeWindow = fsl_createwindow(egldisplay,
    				       eglNativeDisplayType); 
    
    assert(eglNativeWindow); 

    eglsurface = eglCreateWindowSurface(egldisplay,
    					eglconfig,
    					eglNativeWindow,
    					NULL);

    assert(eglGetError() == EGL_SUCCESS);
    
    EGLint ContextAttribList[] = {EGL_CONTEXT_CLIENT_VERSION,
    				  2,
    				  EGL_NONE };

    eglcontext = eglCreateContext(egldisplay,
    				  eglconfig,
    				  EGL_NO_CONTEXT,
    				  ContextAttribList);
    				  
    assert(eglGetError() == EGL_SUCCESS);
    
    eglMakeCurrent(egldisplay, eglsurface, eglsurface, eglcontext);
    		   
    assert(eglGetError() == EGL_SUCCESS);
    
    load_shaders(dewarp_vert.c_str(), dewarp_frag.c_str());   
    
    load_image_textures();

    locVertices = glGetAttribLocation(programHandle, "vPosition");    				      
    locTexcoord = glGetAttribLocation(programHandle, "my_Texcoor");
    locTransformMat = glGetUniformLocation(programHandle, "u_TransMatrix");    					   
    locSampler = glGetUniformLocation(programHandle, "tex");   
    locMouse = glGetUniformLocation(programHandle, "iMouse");    				    
    locResolution= glGetUniformLocation(programHandle, "iResolution"); 
    
    glEnableVertexAttribArray(locVertices);
    
    glEnableVertexAttribArray(locTexcoord);

    glVertexAttribPointer(locVertices,
    			  2,
    			  GL_FLOAT,
    			  GL_FALSE,
    			  0,
    			  &vertices[0][0]);
    			  
    glVertexAttribPointer(locTexcoord,
    			  2,
    			  GL_FLOAT,
    			  GL_FALSE,
    			  0,
    			  &texcoords[0][0]);
    			  
    glUniformMatrix4fv(locTransformMat,
    		       1,
    		       GL_FALSE,
    		       transformMatrix);
    		       
    glUniform1i(locSampler, 0);

    glUniform2f(locResolution, 1024.0, 1024.0);
    
    glUniform2f(locMouse, -512.0,384.0);
}

void load_image_textures(void)
{
    std::string image_file = IMAGE_EXAMPLE;

    ILboolean result = ilLoadImage(image_file.c_str());
    
    if (result == true) {
        std::cout << "Image loaded successfully: " << image_file << std::endl;
    }
    else {
        ILenum error = ilGetError();
        std::cout << "\nImage failed to load." <<
                     "\nError:\t" << error <<
                     "\nMessage:\t" << ilGetString(error) << std::endl;
        exit(EXIT_FAILURE);
    }

    const unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
    const unsigned int height = ilGetInteger(IL_IMAGE_HEIGHT);

    const int image_format = ilGetInteger(IL_IMAGE_FORMAT);
    const int image_type = ilGetInteger(IL_IMAGE_TYPE);
    const int image_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
    
    
    std::cout << "\nImage Width:\t" << width <<
                 "\nImage Height:\t" << height <<
                 "\nImage Format:\t" << image_format <<
                 "\nImage Type:\t" <<  image_type <<
                 "\nImage Size:\t" << image_size << std::endl;
    
    ILubyte *bytes = ilGetData();

    glGenTextures(1, texture2D);

    glBindTexture(GL_TEXTURE_2D, *texture2D);

    glTexImage2D(GL_TEXTURE_2D,
    		 0,
    		 GL_RGB,
    		 width,
    		 height,
    		 0,
    		 GL_RGB,
    		 GL_UNSIGNED_BYTE,
    		 bytes);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    std::cout << "\nTexture loaded and created successfully!" << std::endl;
}

void render(float w, float h)
{
    static float mousex = -0.0;
    static int up = 1;
    
    glViewport(0, 0, 1024, 1024);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programHandle);    
    glBindTexture(GL_TEXTURE_2D, *texture2D);
    glUniform1i(locSampler, 0);    
    glUniform2f(locMouse, mousex, 0.0);
    
    glVertexAttribPointer(locVertices,
    			  2,
    			  GL_FLOAT,
    			  GL_FALSE,
    			  0,
    			  &vertices[0][0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
    if (up) {
        mousex += 1.0; 
        if (mousex >= 1024) {
            up = 0;
        }
    }
    else {
        mousex -= 1.0;
        if (mousex <= 0) {
            up = 1;
        }
    }
}

/* shaders */

void load_shaders(const char *vShaderFName, const char *pShaderFName)
{
    vertShaderNum = glCreateShader(GL_VERTEX_SHADER);
    pixelShaderNum = glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(vShaderFName, vertShaderNum);
    compile_shader(pShaderFName, pixelShaderNum);

    programHandle = glCreateProgram();

    glAttachShader(programHandle, vertShaderNum);
    glAttachShader(programHandle, pixelShaderNum);
    glLinkProgram(programHandle);

    GLint linked = 0;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint errorBufSize, errorLength;
        glGetShaderiv(programHandle, GL_INFO_LOG_LENGTH, &errorBufSize);

        char * log = (char*)malloc(errorBufSize * sizeof (char) + 1);
        if (!log) {
            glGetProgramInfoLog(programHandle, errorBufSize, &errorLength, log);
            log[errorBufSize + 1] = '\0';
            std::cout << log << std::endl;

            free(log);
        }
        std::cout << "Error: linking program." << std::endl;
        exit(EXIT_FAILURE);
    }
    glUseProgram(programHandle);
}

void compile_shader(const char *FName, GLuint ShaderNum)
{
    FILE * fptr = NULL;
    fptr = fopen(FName, "rb");

    if (fptr == NULL) {
        std::cout << "Error: can not open file: " <<  FName << std::endl;
        exit(EXIT_FAILURE);
    }

    int length;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    fseek(fptr, 0 ,SEEK_SET);

    char *shaderSource = (char*)malloc(sizeof(char) * length);
    if (shaderSource == NULL) {
        std::cout << "Error: out of memory." << std::endl;
        exit(EXIT_FAILURE);
    }

    ssize_t r = fread(shaderSource, length, 1, fptr);
    if (r == -1) {
        exit(EXIT_FAILURE);
    }

    glShaderSource(ShaderNum, 1, (char**)&shaderSource, &length);
    glCompileShader(ShaderNum);

    free(shaderSource);
    fclose(fptr);

    GLint compiled = 0;
    glGetShaderiv(ShaderNum, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled) {
        GLint errorBufSize, errorLength;
        glGetShaderiv(ShaderNum, GL_INFO_LOG_LENGTH, &errorBufSize);

        char *log = (char*)malloc(errorBufSize * sizeof(char) + 1);
        if (!log) {
            glGetShaderInfoLog(ShaderNum, errorBufSize, &errorLength, log);
            log[errorBufSize + 1] = '\0';
            free(log);
        }
        std::cout << "Error: compiling shader: " << FName << std::endl;
        exit(EXIT_FAILURE);
    }
}
