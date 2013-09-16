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

#include <boost/functional/factory.hpp>

#include <sstream>

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

    template<class S>
    class ValueStringConverter
    {
    public:
      typedef S           tConvSrc;
      typedef std::string tConvDst;

      bool convert(const S& src, std::string& sDst)
      {
        std::stringstream sstream;
        sstream << src;
        sDst = sstream.str();
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

      m_collTypeMappings[getType<float>()][getType<unsigned int>()]    = boost::factory<ConversionModule<AssignValueConverter<float, unsigned int> >* >();
      m_collTypeMappings[getType<float>()][getType<int>()]             = boost::factory<ConversionModule<AssignValueConverter<float, int> >* >();
      m_collTypeMappings[getType<float>()][getType<short>()]           = boost::factory<ConversionModule<AssignValueConverter<float, short> >* >();
      m_collTypeMappings[getType<float>()][getType<unsigned short>()]  = boost::factory<ConversionModule<AssignValueConverter<float, unsigned short> >* >();
      m_collTypeMappings[getType<float>()][getType<double>()]          = boost::factory<ConversionModule<AssignValueConverter<float, double> >* >();

      m_collTypeMappings[getType<double>()][getType<unsigned int>()]   = boost::factory<ConversionModule<AssignValueConverter<double, unsigned int> >* >();
      m_collTypeMappings[getType<double>()][getType<int>()]            = boost::factory<ConversionModule<AssignValueConverter<double, int> >* >();
      m_collTypeMappings[getType<double>()][getType<short>()]          = boost::factory<ConversionModule<AssignValueConverter<double, short> >* >();
      m_collTypeMappings[getType<double>()][getType<unsigned short>()] = boost::factory<ConversionModule<AssignValueConverter<double, unsigned short> >* >();
      m_collTypeMappings[getType<double>()][getType<float>()]          = boost::factory<ConversionModule<AssignValueConverter<double, float> >* >();

      m_collTypeMappings[getType<int>()][getType<unsigned int>()]      = boost::factory<ConversionModule<AssignValueConverter<int, unsigned int> >* >();
      m_collTypeMappings[getType<int>()][getType<double>()]            = boost::factory<ConversionModule<AssignValueConverter<int, double> >* >();
      m_collTypeMappings[getType<int>()][getType<short>()]             = boost::factory<ConversionModule<AssignValueConverter<int, short> >* >();
      m_collTypeMappings[getType<int>()][getType<unsigned short>()]    = boost::factory<ConversionModule<AssignValueConverter<int, unsigned short> >* >();
      m_collTypeMappings[getType<int>()][getType<float>()]             = boost::factory<ConversionModule<AssignValueConverter<int, float> >* >();

      m_collTypeMappings[getType<unsigned int>()][getType<unsigned int>()]   = boost::factory<ConversionModule<AssignValueConverter<unsigned int, unsigned int> >* >();
      m_collTypeMappings[getType<unsigned int>()][getType<double>()]         = boost::factory<ConversionModule<AssignValueConverter<unsigned int, double> >* >();
      m_collTypeMappings[getType<unsigned int>()][getType<short>()]          = boost::factory<ConversionModule<AssignValueConverter<unsigned int, short> >* >();
      m_collTypeMappings[getType<unsigned int>()][getType<unsigned short>()] = boost::factory<ConversionModule<AssignValueConverter<unsigned int, unsigned short> >* >();
      m_collTypeMappings[getType<unsigned int>()][getType<float>()]          = boost::factory<ConversionModule<AssignValueConverter<unsigned int, float> >* >();
      m_collTypeMappings[getType<unsigned int>()][getType<std::string>()]    = boost::factory<ConversionModule<ValueStringConverter<unsigned int> >* >();
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