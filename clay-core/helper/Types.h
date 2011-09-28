/**************************************************************************\
*
*  This file is part of the C++ Modular Development Framework.
*  Copyright (C) 2009 by Christoph Bastuck.  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  ("GPL") version 2 as published by the Free Software Foundation.
*  See the file LICENSE.GPL at the root directory of this source
*  distribution for additional information about the GNU GPL.
*
*  For using this framework with software that can not be combined with the GNU
*  GPL please contact mail@cbastuck about acquiring a commercial license.
*
*  See http://www.cbastuck.de/ for more information.
*
\**************************************************************************/

#ifndef TYPES_H_
#define TYPES_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleIOBase.h>

namespace CLAY
{
  //forward declaration
  template<class T> class TypedModuleOutput;
  template<class T> class TypedModuleInput;
  template<class T> class ModuleOutput;
  template<class T> class ModuleInput;

  namespace HELPER
  {   

    //----------------------------------
    template<char a=0x00, char b=0x00, char c=0x00, char d=0x00>
    class IntegerEncoding
    {
    public:
      #define ENCODE(a,b,c,d)((((unsigned int)d) << 24) | (((unsigned int)c) << 16) | (((unsigned int)b) << 8) | ((unsigned int)a))

      enum{ value = ENCODE(a,b,c,d)};

      static int encode(const std::string& sStr)
      {
        return ENCODE(sStr[0], sStr[1], sStr[2], sStr[3]);
      }
    };

    //----------------------------------
    class IntegerDecoding
    {
    public:
      static tString toString(const int* p)
      {
        return tString(reinterpret_cast<const char*>(p), sizeof(int));
      }

      static tString toString(const unsigned int* p)
      {
        return tString(reinterpret_cast<const char*>(p), sizeof(unsigned int));
      }
    };

    //----------------------------------
    //    type checking layer of CLAY
    //----------------------------------

    //----------------------------------
    template<class T>
    bool isOfType(ModuleInputBase* pInput)
    {
      if(dynamic_cast<TypedModuleInput<T>*>(pInput))
      {
       return true;
      }
      return false;
    }

    //----------------------------------
    template<class T>
    bool isOfType(ModuleOutputBase* pOutput)
    {
      if(dynamic_cast<TypedModuleOutput<T>*>(pOutput))
      {
        return true;
      }
      return false;
    }
    
    
    template<class T>
    inline T readBigEndian(unsigned char* ptr)
    {
        T v=ptr[0];
        for(unsigned int i=1, n=sizeof(T); i<n; ++i)
            v=v*256+ptr[i];
        return v;
    }

    template<class T>
    inline T readLittleEndian(unsigned char* ptr)
    {
        unsigned int n=sizeof(T);
        T v=ptr[n-1];
        for(unsigned int i=2; i<=n; ++i)
            v=v*256+ptr[n-i];
        return v;
    }

  }
}

#endif
