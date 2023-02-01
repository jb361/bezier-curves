/*!
    @file Colour.h @author Joel Barrett @date 12/09/09 @brief Colour definitions.
*/

#ifndef FRAMEWORK_RENDERING_COLOUR_H
#define FRAMEWORK_RENDERING_COLOUR_H

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Rendering
    {
        /*!
            Define some common colours for simplicity.
        */
        struct Colour
        {
            static const float black[3];
            static const float white[3];
            static const float red[3];
            static const float green[3];
            static const float blue[3];
            static const float cyan[3];
            static const float yellow[3];
            static const float magenta[3];
        };
    }
}

#endif // FRAMEWORK_RENDERING_COLOUR_H
