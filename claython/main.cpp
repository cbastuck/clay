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

#include <claython/PythonHost.h>

#include <clay-core/base/Module.h>
#include <clay-core/nugget/IClayNugget.h>

#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(claython)
{
  //expose enums
  enum_<CLAY::Module::ProcessResultCode>("ProcessResultCode")
    .value("Process_OK", CLAY::Module::Process_OK)
    .value("Process_ERROR", CLAY::Module::Process_ERROR)
    .value("ProcessNotImplemented_ERROR", CLAY::Module::ProcessNotImplemented_ERROR)
    .value("Process_NotPossible", CLAY::Module::Process_NotPossible);

  enum_<CLAY::Module::ConnectResultCode>("ConnectResultCode")
    .value("Connect_OK", CLAY::Module::Connect_OK)
    .value("Connect_ERROR", CLAY::Module::Connect_ERROR)
    .value("NoSuitableConnection_ERROR", CLAY::Module::NoSuitableConnection_ERROR);

  //expose module class
  bool (CLAY::Module::*initModuleWithString)(const CLAY::tString&) = &CLAY::Module::init;
  CLAY::Module::ConnectResultCode (CLAY::Module::*connectFrom)(CLAY::Module*) = &CLAY::Module::connectFrom;
  class_<CLAY::Module, boost::noncopyable>("Module", no_init)
    .def("init", initModuleWithString)
    .def("process", &CLAY::Module::process)
    .def("connectFrom", connectFrom)
    ; 


  //expose IClayNugget class
  class_<CLAY::NUGGET::IClayNugget, boost::noncopyable>("IClayNugget", no_init);

  //expose Host class
  bool (CLAY::Host::*loadProjectWithString)(const CLAY::tString&) = &CLAY::Host::loadProject;
  class_<CLAY::PythonHost>("Host")
    .def("init",   &CLAY::Host::init)
    .def("deInit", &CLAY::Host::deInit)
    .def("loadProject", loadProjectWithString)
    .def("loadNugget", &CLAY::Host::loadNugget, return_value_policy<reference_existing_object>())
    .def("createModule", &CLAY::PythonHost::createModule, return_value_policy<reference_existing_object>())
    ; 
}