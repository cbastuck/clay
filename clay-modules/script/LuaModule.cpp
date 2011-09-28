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

#include "LuaModule.h"

#include <clay-core/helper/Types.h>
#include <clay-core/helper/XMLHelper.h>
#include <clay-core/helper/FileHelper.h>

#define XML_LUA_MODULE_ROOT                   "lua-module"
#define XML_LUA_MODULE_PROCESS_FUNCTION_ATTR  "process-function"
#define XML_LUA_MODULE_REG_IN_SCRIPT_ATTR     "register-inputs-script"
#define XML_LUA_MODULE_REG_OUT_SCRIPT_ATTR    "register-outputs-script"

namespace CLAY
{
  namespace MODULE
  {

    //---------------------------------------------
    //                    statics
    //---------------------------------------------

    //---------------------------------------------luaRegisterInputs
    int LuaModule::luaRegisterInputs(lua_State* pState)
    {
      LuaModule* pModule    = popThisPointer(pState);
      unsigned int uNumArgs = lua_gettop(pState);
      if(uNumArgs == 2)
      {
        if(lua_isstring(pState, 1))
        {
          tString sInputName = lua_tostring(pState, 1); //first parameter must be the input name

          if(lua_isnumber(pState, 2))
          {
            ModuleInput<float>* pInput = new ModuleInput<float>();
            pModule->Module::registerInput(pInput, sInputName);
          }
          //TODO: add other types
        }     
        lua_pushinteger(pState, 0);
      }
      else
      {
        lua_pushinteger(pState, 1);
      }
      
      return 1;
    }

    //---------------------------------------------luaRegisterOutputs
    int LuaModule::luaRegisterOutputs(lua_State* pState)
    {
      LuaModule* pModule    = popThisPointer(pState);
      unsigned int uNumArgs = lua_gettop(pState);
      if(uNumArgs == 2)
      {
        if(lua_isstring(pState, 1))
        {
          tString sOutputName = lua_tostring(pState, 1); //first parameter must be the output name

          if(lua_isnumber(pState, 2))
          {
            lua_Number aValue = lua_tonumber(pState, 2);
            float fValue = static_cast<float>(aValue); //TODO: should we use double (?)
            ModuleOutput<float>* pOutput = new ModuleOutput<float>(fValue);
            pModule->Module::registerOutput(pOutput, sOutputName);
          }
          //TODO: add other types

          lua_pushinteger(pState, 0);
        }     
      }
      else
      {
        lua_pushinteger(pState, 1);
      }
      return 1;
    }

    //---------------------------------------------popThisPointer
    LuaModule* LuaModule::popThisPointer(lua_State* pState)
    {
      lua_getfield(pState, LUA_REGISTRYINDEX, "MF:LuaHost::this"); //restore the 'this' pointer from the lua-registry
      void* pPtr = lua_touserdata(pState, -1);
      lua_pop(pState, 1); //pop the this pointer from stack
      return static_cast<LuaModule*>(pPtr);
    }

    //---------------------------------------------
    //                    non-statics
    //---------------------------------------------

    //---------------------------------------------LuaModule
    LuaModule::LuaModule(const tString& sId)
      : tBase(sId)
    {

    }

    //---------------------------------------------init
    bool LuaModule::init(XERCES::DOMNode* pNode)
    {
      if(tBase::init(pNode))
      {
        return true;
      }
      return false;
    }

    //---------------------------------------------save
    bool LuaModule::save(XERCES::DOMElement* pNode)
    {
      XERCES::DOMElement* pElement = XercesXML::appendNode(pNode, XML_LUA_MODULE_ROOT);

      XercesXML::addBinaryAttribute(pElement, XML_LUA_MODULE_PROCESS_FUNCTION_ATTR, m_sProcessFunction.c_str(), m_sProcessFunction.size());
      XercesXML::addBinaryAttribute(pElement, XML_LUA_MODULE_REG_IN_SCRIPT_ATTR, m_sRegisterInputsScript.c_str(), m_sRegisterInputsScript.size());
      XercesXML::addBinaryAttribute(pElement, XML_LUA_MODULE_REG_OUT_SCRIPT_ATTR, m_sRegisterOutputsScript.c_str(), m_sRegisterOutputsScript.size());

      return tBase::save(pElement);
    }
    
    //---------------------------------------------load
    bool LuaModule::load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
    {
      XERCES::DOMElement* pElement = XercesXML::findChildNode(pNode, XML_LUA_MODULE_ROOT);
      if(pElement)
      { 
        XercesXML::tBinaryBuffer aProcessFunction;
        if(XercesXML::getBinaryAttributeValue(pElement, XML_LUA_MODULE_PROCESS_FUNCTION_ATTR, aProcessFunction))
        {
          m_sProcessFunction.assign(aProcessFunction.begin(), aProcessFunction.end());
          onProcessFunctionModified();
        }

        XercesXML::tBinaryBuffer aRegisterInputsScript;
        if(XercesXML::getBinaryAttributeValue(pElement, XML_LUA_MODULE_REG_IN_SCRIPT_ATTR, aRegisterInputsScript))
        {
          m_sRegisterInputsScript.assign(aRegisterInputsScript.begin(), aRegisterInputsScript.end());
          onRegisterInputsScriptModified();
        }

        XercesXML::tBinaryBuffer aRegisterOutputsScript;
        if(XercesXML::getBinaryAttributeValue(pElement, XML_LUA_MODULE_REG_OUT_SCRIPT_ATTR, aRegisterOutputsScript))
        {
          m_sRegisterOutputsScript.assign(aRegisterOutputsScript.begin(), aRegisterOutputsScript.end());
          onRegisterOutputsScriptModified();
        }
          
        return tBase::load(pElement, pInputConnections, pOutputConnections);
      }
      return false;
    }

    //---------------------------------------------registerModuleOutputs
    void LuaModule::registerModuleOutputs()
    {
      if(!m_sRegisterOutputsScript.empty())
      {
        lua_State* pState  = createLuaState();
        lua_register(pState, "registerOutput", luaRegisterOutputs);

        pushThisPointer(pState);

        int iRes = luaL_loadbuffer(pState, 
                                   m_sRegisterOutputsScript.c_str(), 
                                   m_sRegisterOutputsScript.size(), 
                                   "registerModuleOutputs");
        if(iRes == 0)
        {
          if(lua_pcall(pState, 0, LUA_MULTRET, 0) == 0)
          {
            lua_pushstring(pState, "registerOutputs");

            lua_gettable(pState, LUA_GLOBALSINDEX); //function is located in the global table
            lua_pcall(pState, 0, LUA_MULTRET, 0);
          }
        }

        lua_close(pState);
      }
    }

    //---------------------------------------------registerModuleInputs
    void LuaModule::registerModuleInputs()
    {
      if(!m_sRegisterInputsScript.empty())
      {
        lua_State* pState = createLuaState();

        lua_register(pState, "registerInput",  luaRegisterInputs);

        pushThisPointer(pState);

        int iRes = luaL_loadbuffer(pState, 
                                   m_sRegisterInputsScript.c_str(), 
                                   m_sRegisterInputsScript.size(), 
                                   "registerModuleInputs");
        if(iRes == 0)
        {
          if(lua_pcall(pState, 0, LUA_MULTRET, 0) == 0)
          {
            lua_pushstring(pState, "registerInputs");

            lua_gettable(pState, LUA_GLOBALSINDEX); //function is located in the global table
            lua_pcall(pState, 0, LUA_MULTRET, 0);
          }
        }

        lua_close(pState);
      }
    }

    //---------------------------------------------process
    Module::ProcessResultCode LuaModule::process()
    {
      lua_State* pState = createLuaState();
      lua_settop(pState, 0); //start with a clean stack

      if(luaL_loadbuffer(pState, m_sProcessFunction.c_str(), m_sProcessFunction.size(), "process") == 0)
      {
        if(lua_pcall(pState, 0, LUA_MULTRET, 0) == 0)
        {
          lua_pushstring(pState, "process");
          lua_gettable(pState, LUA_GLOBALSINDEX);

          //-------------------

          //collecting the input arguments
          unsigned int uNumArguments = 0;
          Module::tInputIterator inIt  = beginInput();
          Module::tInputIterator inEnd = endInput();
          for(; inIt!=inEnd; ++inIt)
          {
            ModuleInputBase* pCurrentIn = *inIt;
            if(pCurrentIn->isConnected())
            {
              if(isOfType<float>(pCurrentIn))
              {
                ModuleInput<float>* pIn = static_cast<ModuleInput<float>*>(pCurrentIn);
                lua_Number aVal = *pIn->getData();
                lua_pushnumber(pState, aVal);
                ++uNumArguments;
              }
              //TODO: add other types
            }
          }

          int iRes = lua_pcall(pState, uNumArguments, LUA_MULTRET, 0);
          if(iRes != 0)
          {
            unsigned int uStackSize = lua_gettop(pState);
            while(uStackSize)
            {
              if(lua_isstring(pState, -1))
              {
                CLAY_ERR_OUT<<lua_tostring(pState, -1)<<CLAY_ENDL;
              }
              else if(lua_isnumber(pState, -1))
              {
                CLAY_ERR_OUT<<lua_tonumber(pState, -1)<<CLAY_ENDL;
              }
              else if(lua_isnil(pState, -1))
              {
                CLAY_ERR_OUT<<"NIL"<<CLAY_ENDL;
              }
              else if(lua_islightuserdata(pState, -1))
              {
                CLAY_ERR_OUT<<lua_touserdata(pState, -1)<<CLAY_ENDL;
              }
              lua_pop(pState, -1);
              uStackSize = lua_gettop(pState);
            }
          }
        }
      }

      unsigned int uReturnArgs = lua_gettop(pState);
      if(uReturnArgs == getNumOutputs()) //can only assign output values if this matches
      {
        Module::tOutputIterator outIt  = beginOutput();
        Module::tOutputIterator outEnd = endOutput();
        for(unsigned int i=1; outIt!=outEnd; ++outIt, ++i)
        {
          ModuleOutputBase* pCurrentOut = *outIt;
          if(lua_isnumber(pState, i))
          {
            lua_Number aRes = lua_tonumber(pState, i);
            if(isOfType<float>(pCurrentOut))
            {
              ModuleOutput<float>* pOut = static_cast<ModuleOutput<float>*>(pCurrentOut);
              pOut->setData((float)aRes);
            }
          }
          //TODO: add other types
        }
        return Module::Process_OK;
      }

      return Module::Process_ERROR;
    }

    //---------------------------------------------setProcessFunctionFilename
    void LuaModule::setProcessFunctionFilename(const tString& sFilename)
    {
      FileStream aFs(sFilename);
      if(aFs.open( FileStream::READ_ONLY_ACCESS | FileStream::OPEN_BINARY))
      {
        aFs.getRawBuffer(m_sProcessFunction);
      }

      onProcessFunctionModified();
    }

    //---------------------------------------------setProcessFunction
    void LuaModule::setProcessFunction(const tString& sBuffer)
    { 
      m_sProcessFunction = sBuffer; 
      onProcessFunctionModified();
    }

    //---------------------------------------------setRegisterInputsScript
    void LuaModule::setRegisterInputsScript(const tString& sBuffer)
    {
      m_sRegisterInputsScript = sBuffer;
      unregisterInputs();
      registerModuleInputs();
      onRegisterInputsScriptModified();
    }

    //---------------------------------------------setRegisterOutputsScript
    void LuaModule::setRegisterOutputsScript(const tString& sBuffer)
    {
      m_sRegisterOutputsScript = sBuffer;
      unregisterOutputs();
      registerModuleOutputs();
      onRegisterOutputsScriptModified();
    }

    //---------------------------------------------createLuaState
    lua_State* LuaModule::createLuaState()
    {
      lua_State* pState = lua_open();

      luaopen_base  (pState);
      luaopen_table (pState);
      luaopen_string(pState);
      luaopen_math  (pState);
      luaopen_debug (pState);

      return pState;
    }

    //---------------------------------------------pushThisPointer
    void LuaModule::pushThisPointer(lua_State* pState)
    {
      lua_pushlightuserdata(pState, (void *) this); //store the 'this' pointer in the lua-registry
      lua_setfield(pState, LUA_REGISTRYINDEX, "MF:LuaHost::this");
    }

    //---------------------------------------------onProcessFunctionModified
    void LuaModule::onProcessFunctionModified()
    {
      
    }

    //---------------------------------------------onRegisterInputsScriptModified
    void LuaModule::onRegisterInputsScriptModified()
    {
      registerModuleInputs();
    }

    //---------------------------------------------onRegisterOutputsScriptModified
    void LuaModule::onRegisterOutputsScriptModified()
    {
      registerModuleOutputs();
    }
  }
}