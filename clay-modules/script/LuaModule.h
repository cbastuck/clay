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

#ifndef LUAMODULE_H_
#define LUAMODULE_H_

#include <clay-core/base/ModuleHelper.h>

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace CLAY
{
  namespace MODULE
  {

    class LuaModule : public ModuleHelper<LuaModule,
                                          HELPER::IntegerEncoding<'L','U','A','-'>::value,
                                          HELPER::IntegerEncoding<'S','C','R','P'>::value >
    {
    public:   
      typedef ModuleHelper<LuaModule, 
                           HELPER::IntegerEncoding<'L','U','A','-'>::value,
                           HELPER::IntegerEncoding<'S','C','R','P'>::value > tBase;

      static int luaRegisterInputs (lua_State* pState);
      static int luaRegisterOutputs(lua_State* pState);
      static LuaModule* popThisPointer(lua_State* pState);

      LuaModule(const tString& sId);
      virtual bool init(XERCES::DOMNode* pNode);

      virtual bool save(XERCES::DOMElement* pNode);
      virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

      virtual void registerModuleOutputs();
      virtual void registerModuleInputs ();

      virtual ProcessResultCode process();

      void setProcessFunctionFilename(const tString& sFilename);
      void setProcessFunction(const tString& sBuffer);
      void setRegisterInputsScript(const tString& sBuffer);
      void setRegisterOutputsScript(const tString& sBuffer);

      //----- inline methods
      const tString& getProcessFunction()      { return m_sProcessFunction; }
      const tString& getRegisterInputsScript() { return m_sRegisterInputsScript; }
      const tString& getRegisterOutputsScript(){ return m_sRegisterOutputsScript; }

    protected:
      lua_State* createLuaState();
      void pushThisPointer(lua_State* pState);

      void onProcessFunctionModified();
      void onRegisterInputsScriptModified();
      void onRegisterOutputsScriptModified();

    private:
      tString m_sRegisterInputsScript;
      tString m_sRegisterOutputsScript;
      tString m_sProcessFunction;
    };
  }
}

#endif 
