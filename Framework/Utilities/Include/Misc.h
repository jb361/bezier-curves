/*!
    @file Misc.h @author Joel Barrett @date 12/04/09 @brief Miscellaneous utility functions.
*/

#ifndef FRAMEWORK_UTILITIES_MISC_H
#define FRAMEWORK_UTILITIES_MISC_H

#if _MSC_VER > 1000
    #pragma once
#endif

// print message to output window during compile-time
#define chMSG(x) #x
#define chMSG2(x) chMSG(x)
#define MESSAGE(desc) message(__FILE__ "(" chMSG2(__LINE__) ") : message : " #desc)

namespace Framework
{
    namespace Utilities
    {
        // delete pointers and arrays of pointers, or release an object
        template < typename T > inline void Destroy(T*& p) { delete p; p = NULL; }
        template < typename T > inline void DestroyArray(T*& p) { delete[] p; p = NULL; }
        template < typename T > inline void Release(T*& p) { if(p) p->Release(); p = NULL; }
    }
}

#endif // FRAMEWORK_UTILITIES_MISC_H
