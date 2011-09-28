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

#include "ClayTypeConversion.h"

#include <clay-modules/ConversionModule.h>

#include "ClayTypeMapping.h"

namespace CLAY
{
  namespace MODEL
  {
    //---------------------------------------------
    //            Simple Value Converters
    //---------------------------------------------
    template<class S, class D>
    class AssignValueConverter
    {
    public:
      typedef S tConvSrc;
      typedef D tConvDst;

      bool convert(const S& src, D& dst)
      {
        dst = static_cast<D>(src);
        return true;
      }
    };

    //---------------------------------------------
    //---------------------------------------------

    //---------------------------------------------ClayTypeConversion
    ClayTypeConversion::ClayTypeConversion()
    {

    }

    //---------------------------------------------~ClayTypeConversion
    ClayTypeConversion::~ClayTypeConversion()
    {

    }

    //---------------------------------------------init
    void ClayTypeConversion::init()
    {
      using namespace ClayTypeMapping;

      m_collTypeMappings[getType<float>()][getType<unsigned int>()]    = ConversionModule<AssignValueConverter<float, unsigned int> >::tDescriptor::create;
      m_collTypeMappings[getType<float>()][getType<int>()]             = ConversionModule<AssignValueConverter<float, int> >::tDescriptor::create;
      m_collTypeMappings[getType<float>()][getType<short>()]           = ConversionModule<AssignValueConverter<float, short> >::tDescriptor::create;
      m_collTypeMappings[getType<float>()][getType<unsigned short>()]  = ConversionModule<AssignValueConverter<float, unsigned short> >::tDescriptor::create;
      m_collTypeMappings[getType<float>()][getType<double>()]          = ConversionModule<AssignValueConverter<float, double> >::tDescriptor::create;

      m_collTypeMappings[getType<double>()][getType<unsigned int>()]   = ConversionModule<AssignValueConverter<double, unsigned int> >::tDescriptor::create;
      m_collTypeMappings[getType<double>()][getType<int>()]            = ConversionModule<AssignValueConverter<double, int> >::tDescriptor::create;
      m_collTypeMappings[getType<double>()][getType<short>()]          = ConversionModule<AssignValueConverter<double, short> >::tDescriptor::create;
      m_collTypeMappings[getType<double>()][getType<unsigned short>()] = ConversionModule<AssignValueConverter<double, unsigned short> >::tDescriptor::create;
      m_collTypeMappings[getType<double>()][getType<float>()]          = ConversionModule<AssignValueConverter<double, float> >::tDescriptor::create;

      m_collTypeMappings[getType<int>()][getType<unsigned int>()]      = ConversionModule<AssignValueConverter<int, unsigned int> >::tDescriptor::create;
      m_collTypeMappings[getType<int>()][getType<double>()]            = ConversionModule<AssignValueConverter<int, double> >::tDescriptor::create;
      m_collTypeMappings[getType<int>()][getType<short>()]             = ConversionModule<AssignValueConverter<int, short> >::tDescriptor::create;
      m_collTypeMappings[getType<int>()][getType<unsigned short>()]    = ConversionModule<AssignValueConverter<int, unsigned short> >::tDescriptor::create;
      m_collTypeMappings[getType<int>()][getType<float>()]             = ConversionModule<AssignValueConverter<int, float> >::tDescriptor::create;

      m_collTypeMappings[getType<unsigned int>()][getType<unsigned int>()]   = ConversionModule<AssignValueConverter<unsigned int, unsigned int> >::tDescriptor::create;
      m_collTypeMappings[getType<unsigned int>()][getType<double>()]         = ConversionModule<AssignValueConverter<unsigned int, double> >::tDescriptor::create;
      m_collTypeMappings[getType<unsigned int>()][getType<short>()]          = ConversionModule<AssignValueConverter<unsigned int, short> >::tDescriptor::create;
      m_collTypeMappings[getType<unsigned int>()][getType<unsigned short>()] = ConversionModule<AssignValueConverter<unsigned int, unsigned short> >::tDescriptor::create;
      m_collTypeMappings[getType<unsigned int>()][getType<float>()]          = ConversionModule<AssignValueConverter<unsigned int, float> >::tDescriptor::create;
    }

    //---------------------------------------------deInit
    void ClayTypeConversion::deInit()
    {
      m_collTypeMappings.clear();
    }

    //---------------------------------------------getConversionModuleFactory
    ClayTypeConversion::tConversionModuleFactory ClayTypeConversion::getConversionModuleFactory(ClayTypes::Type eSrcType, ClayTypes::Type eDstType)
    {
      tTypeMappingMatrix::iterator row = m_collTypeMappings.find(eSrcType);
      if(row != m_collTypeMappings.end())
      {
        tConvertModules::iterator col = row->second.find(eDstType);
        if(col != row->second.end())
        {
          return col->second;
        }
      }
      return NULL;  
    }

    //---------------------------------------------isConversionAvailable
    bool ClayTypeConversion::isConversionAvailable(ClayTypes::Type eSrcType, ClayTypes::Type eDstType)
    {
      return getConversionModuleFactory(eSrcType, eDstType) != NULL;  
    }
  }
}