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

#ifndef CLAYEXECUTABLE_H_
#define CLAYEXECUTABLE_H_

//CORE
#include <clay-core/base/Module.h>

//STL
#include <deque>
//boost
#include <boost/function.hpp>

namespace CLAY
{
  //---------------------------------------------
  class ClayCall
  {
  public:
    typedef boost::function0<Module::ProcessResultCode> tMethod;

    ClayCall(tMethod aMethod, Module* pInvoker);
    Module::ProcessResultCode execute() const;

  private:
    tMethod m_aMethod;
    Module* m_pInvoker;
  };

  //---------------------------------------------
  class ClayExecutable
  {
  public:
    typedef std::deque<ClayCall>      tCallList;
    typedef tCallList::const_iterator iterator;  

    void appendMethod(ClayCall::tMethod aMethod, Module* pInvoker);

    //inline methods
    iterator begin() const{ return m_aCallList.begin(); }
    iterator end()   const{ return m_aCallList.end(); }

  private:
    tCallList m_aCallList;
  };

}

#endif
