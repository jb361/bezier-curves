/*!
    @file Constants.h @author Joel Barrett @date 27/11/09 @brief Mathematical constants.
*/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        /*!
            A struct of common mathematical constants. Template specialisation is 
            employed to allow float or double precision.
        */
        template < typename T = float >
        struct Const
        {
            static const T PI;
            static const T TWO_PI;
            static const T HALF_PI;
            static const T INV_PI;
            
            static const T E;
            static const T LN2;
            static const T LN10;
            static const T EPSILON;
            
            static const T TO_DEG;
            static const T TO_HALF_DEG;
            static const T TO_RAD;
            static const T TO_HALF_RAD;
        };
    }
}

#include "..\source\Constants.inl"

#endif // CONSTANTS_H_
