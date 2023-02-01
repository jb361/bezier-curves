/*
    milkshape.h
    
        Loads and renders a Milkshape3D model.
    
    Author:	Brett Porter
    Email: brettporter@yahoo.com
    Website: http://www.geocities.com/brettporter/
    Copyright (C)2000, Brett Porter. All Rights Reserved.
    
    This file may be used only as long as this copyright notice remains intact.
*/

#ifndef FRAMEWORK_OPENGL_MS3DMODEL_H
#define FRAMEWORK_OPENGL_MS3DMODEL_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Model.h"

namespace Framework
{
    namespace OpenGL
    {
        class MS3DModel : public Model
        {
        public:
            // load model data into the private variables
            bool LoadModelData(const char *filename);
        };
    }
}

#endif // FRAMEWORK_OPENGL_MS3DMODEL_H
