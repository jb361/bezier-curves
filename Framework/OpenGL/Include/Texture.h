/*!
    @file Texture.h @author Unknown @date 01/01/12 @brief OpenGL texture functions.
*/

#ifndef FRAMEWORK_OPENGL_TEXTURE_H
#define FRAMEWORK_OPENGL_TEXTURE_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Bitmap.h"

namespace Framework
{
    namespace OpenGL
    {
        //! open and load the specified bitmap file
        AUX_RGBImageRec *LoadBitmap(char *name);

        //! create an opengl texture
        bool CreateGLTexture(char *name, GLuint & TexID);
    }
}

#endif // FRAMEWORK_OPENGL_TEXTURE_H
