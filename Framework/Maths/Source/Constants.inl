/*!
    @file Constants.inl @author Joel Barrett @date 27/11/09 @brief Mathematical constants.
*/

#ifndef CONSTANTS_INL_
#define CONSTANTS_INL_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        // Float Template Specialisations
        
        template <> const float Const<float>::PI = 3.14159f;
        template <> const float Const<float>::TWO_PI = 6.28319f;
        template <> const float Const<float>::HALF_PI = 1.57080f;
        template <> const float Const<float>::INV_PI = 0.318310f;
        template <> const float Const<float>::E = 2.71828f;
        template <> const float Const<float>::LN2 = 0.693147f;
        template <> const float Const<float>::LN10 = 2.30259f;
        template <> const float Const<float>::EPSILON = 1e-06f;
        template <> const float Const<float>::TO_DEG = 57.2958f;
        template <> const float Const<float>::TO_HALF_DEG = 28.6479f;
        template <> const float Const<float>::TO_RAD = 0.0174533f;
        template <> const float Const<float>::TO_HALF_RAD = 0.00872665f;
        
        // Double Template Specialisations
        
        template <> const double Const<double>::PI = 3.14159265358;
        template <> const double Const<double>::TWO_PI = 6.28318530718;
        template <> const double Const<double>::HALF_PI = 1.57079632679;
        template <> const double Const<double>::INV_PI = 0.318309886184;
        template <> const double Const<double>::E = 2.71828182846;
        template <> const double Const<double>::LN2 = 0.693147180560;
        template <> const double Const<double>::LN10 = 2.30258509299;
        template <> const double Const<double>::EPSILON = 1e-12;
        template <> const double Const<double>::TO_DEG = 57.2957795131;
        template <> const double Const<double>::TO_HALF_DEG = 28.6478897565;
        template <> const double Const<double>::TO_RAD = 0.0174532925199;
        template <> const double Const<double>::TO_HALF_RAD = 0.00872664625997;
        
    }
}

#endif // CONSTANTS_INL_
