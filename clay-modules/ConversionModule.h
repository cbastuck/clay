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

#ifndef ConversionModule_h_
#define ConversionModule_h_

#include <clay-core/base/ModuleHelper.h>

namespace CLAY
{

  template<class CONV>
  class ConversionModule : public ModuleHelper
  {
  private:
    typedef ModuleHelper tBase;

    typedef typename CONV::tConvSrc tConvSrc;
    typedef typename CONV::tConvDst tConvDst;

  public:
    ConversionModule(const tString& sRuntimeId)
      : tBase(sRuntimeId),
        m_pModuleInput(NULL),
        m_pModuleOutput(NULL)
    {

    }

    virtual const char* getModuleURI() const
    {
      return "http://claymodules.org/Conversion";
    }

    virtual bool init(XERCES::DOMNode* pNode)
    {
      return tBase::init(pNode);
    }

    virtual void deInit()
    {
      tBase::deInit();
    }

    virtual void registerModuleInputs()
    {
      m_pModuleInput = new ModuleInput<tConvSrc>();
      registerInput(m_pModuleInput, "conv-in");
    }

    virtual void registerModuleOutputs()
    {
      m_pModuleOutput = new ModuleOutput<tConvDst>();
      registerOutput(m_pModuleOutput, "conv-out");
    }

    virtual Module::ProcessResultCode process()
    {
      const tConvSrc* pIn  = m_pModuleInput->getData();
      tConvDst& aOut = m_pModuleOutput->getData();

      if(m_aConverter.convert(*pIn, aOut))
      {
        return Module::Process_OK;
      }
      return Module::Process_ERROR;
    }

  private:
    ModuleInput<tConvSrc>*  m_pModuleInput;
    ModuleOutput<tConvDst>* m_pModuleOutput;
    CONV m_aConverter;
  };
}

#endif

