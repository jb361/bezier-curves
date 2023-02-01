/*
	model.h

		Abstract base class for a model. The specific extended class will render the given model.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#ifndef FRAMEWORK_OPENGL_MODEL_H
#define FRAMEWORK_OPENGL_MODEL_H

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace OpenGL
    {
        class Model
        {
        public:
            struct Mesh
            {
                int m_materialIndex;
                int m_numTriangles;
                int *m_pTriangleIndices;
            };

            struct Material
            {
                float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
                float m_shininess;
                int m_texture;
                char *m_pTextureFilename;
            };

            struct Triangle
            {
                float m_vertexNormals[3][3];
                float m_s[3], m_t[3];
                int m_vertexIndices[3];
            };

            struct Vertex
            {
                char m_boneID;
                float m_location[3];
            };

        public:
            Model();
            virtual ~Model();

            // load the model data into the private variables
            virtual bool LoadModelData(const char *filename) = 0;

            // draw the model
            void Draw();

            // called if OpenGL context was lost and we need to reload textures, etc
            void ReloadTextures();

        protected:
            // Meshes used
            int m_numMeshes;
            Mesh *m_pMeshes;

            // Materials used
            int m_numMaterials;
            Material *m_pMaterials;

            // Triangles used
            int m_numTriangles;
            Triangle *m_pTriangles;

            // Vertices used
            int m_numVertices;
            Vertex *m_pVertices;
        };
    }
}

#endif // FRAMEWORK_OPENGL_MODEL_H
