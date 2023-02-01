/*!
    @file Bitmap.h @author J.M.Doyle @date 01/01/12 @brief AUX bitmap replacement.
*/

#ifndef FRAMEWORK_OPENGL_BITMAP_H
#define FRAMEWORK_OPENGL_BITMAP_H

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace OpenGL
    {
        class AUX_RGBImageRec
        {
        public:
            AUX_RGBImageRec(): NoErrors(false), data(NULL){};
            AUX_RGBImageRec(const char *FileName);
            ~AUX_RGBImageRec();

            //! load specified bmp in a format suitable for opengl
            bool loadFile(const char *FileName);

            friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);

        private:
            //! opengl uses rgb format
            void convertBGRtoRGB();

        public:
            byte *data;
            DWORD sizeX, sizeY;

            bool NoErrors;
        };
    }
}

#endif // FRAMEWORK_OPENGL_BITMAP_H
