/*!
    @file Texture.cpp @author Unknown @date 01/01/12 @brief OpenGL texture functions.
*/

#include "Texture.h"

namespace Framework
{
    namespace OpenGL
    {
        AUX_RGBImageRec *LoadBitmap(char *name)
        {
            // Handle to the file
            FILE *File = NULL;

            // Make sure a filename was given
            if (!name) {
                return NULL;
            }
            File = fopen(name,"r"); // Opens the file with "read" access
            // just checks it actually exists
            if (File)
            {
                fclose(File); // close the file we opened
                return auxDIBImageLoad(name);
            }
            return NULL;
        }

        bool CreateGLTexture(char *name, GLuint & TexID)
        {
            AUX_RGBImageRec *Texture[1]; // Creates storage space for the texture Width hieght info etc, and the data itself .
            memset(Texture,0,sizeof(void *)*1);	// Makes sure the pointer is set to null.

            if (Texture[0]=LoadBitmap(name))	// Load in the bitmap. Checking the pointer is returned
            {
                // We are going to generate 1 texture and associate it with the Gluint ID we passes into this function.
                glGenTextures(1, &TexID);
                glBindTexture(GL_TEXTURE_2D, TexID); // binds the texture to a 2d type and to the TexID 

                // This huge line tells OpenGl to create a texture image using the file we loaded.
                // For full parameter listing check the openGL documentation.
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Linear Filtering
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Linear Filtering
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture[0]->sizeX, Texture[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture[0]->data);

                // Free up the memory that stored the texture before openGL converted it
                free(Texture[0]->data);
                free(Texture[0]);
                return true;
            }
            return false;
        }
    }
}
