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

#ifndef PLATFORMSPECIMPL_H_
#define PLATFORMSPECIMPL_H_

#if CLAY_PLATFORM_WIN32
  #define CLAY_PLATFORM_IMPL_WIN32(x) x
  #define CLAY_PLATFORM_IMPL_APPLE(x)
  #define CLAY_PLATFORM_IMPL_UNIX(x)

  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE(x) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX(x) x
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE(x) x

  #define CLAY_PLATFORM_IMPL_WIN32_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE_DEFAULT(x,d) x

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE(x,y,z) x
  #define CLAY_PLATFORM_PICK_WIN32_UNIX(x,y) x
  #define CLAY_PLATFORM_PICK_WIN32_APPLE(x,y) x

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE_DEFAULT(x,y,z,d) x
  #define CLAY_PLATFORM_PICK_WIN32_UNIX_DEFAULT(x,y,d) x
  #define CLAY_PLATFORM_PICK_WIN32_APPLE_DEFAULT(x,y,d) x

  #define CLAY_PLATFORM_DECL_WIN32_UNIX_APPLE(x,y,z,var) x var;
  #define CLAY_PLATFORM_DECL_WIN32_UNIX(x,y,var) x var;
  #define CLAY_PLATFORM_DECL_WIN32_APPLE(x,y,var) x var;

#elif CLAY_PLATFORM_UNIX
  #define CLAY_PLATFORM_IMPL_WIN32(x)
  #define CLAY_PLATFORM_IMPL_APPLE(x)
  #define CLAY_PLATFORM_IMPL_UNIX(x) x

  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE(x) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX(x) x
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE(x)

  #define CLAY_PLATFORM_IMPL_WIN32_DEFAULT(x,d) d
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE_DEFAULT(x,d)

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE(x,y,z) z
  #define CLAY_PLATFORM_PICK_WIN32_UNIX(x,y) y
  #define CLAY_PLATFORM_PICK_WIN32_APPLE(x,y)

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE_DEFAULT(x,y,z,d) y
  #define CLAY_PLATFORM_PICK_WIN32_UNIX_DEFAULT(x,y,d) y
  #define CLAY_PLATFORM_PICK_WIN32_APPLE_DEFAULT(x,y,d) d

  #define CLAY_PLATFORM_DECL_WIN32_UNIX_APPLE(x,y,z,var) y var;
  #define CLAY_PLATFORM_DECL_WIN32_UNIX(x,y,var) y var;
  #define CLAY_PLATFORM_DECL_WIN32_APPLE(x,y,var)

#elif CLAY_PLATFORM_APPLE
  #define CLAY_PLATFORM_IMPL_WIN32(x)
  #define CLAY_PLATFORM_IMPL_APPLE(x) x
  #define CLAY_PLATFORM_IMPL_UNIX(x)

  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE(x) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX(x)
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE(x) x

  #define CLAY_PLATFORM_IMPL_WIN32_DEFAULT(x,d) d
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_APPLE_DEFAULT(x,d) x
  #define CLAY_PLATFORM_IMPL_WIN32_UNIX_DEFAULT(x,d) d
  #define CLAY_PLATFORM_IMPL_WIN32_APPLE_DEFAULT(x,d) x

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE(x,y,z) z
  #define CLAY_PLATFORM_PICK_WIN32_UNIX(x,y)
  #define CLAY_PLATFORM_PICK_WIN32_APPLE(x,y) y

  #define CLAY_PLATFORM_PICK_WIN32_UNIX_APPLE_DEFAULT(x,y,z,d) z
  #define CLAY_PLATFORM_PICK_WIN32_UNIX_DEFAULT(x,y,d) y
  #define CLAY_PLATFORM_PICK_WIN32_APPLE_DEFAULT(x,y,d) d

  #define CLAY_PLATFORM_DECL_WIN32_UNIX_APPLE(x,y,z,var) z var;
  #define CLAY_PLATFORM_DECL_WIN32_UNIX(x,y,var)
  #define CLAY_PLATFORM_DECL_WIN32_APPLE(x,y,var) y var;
#endif

#endif /* PLATFORMSPECIMPL_H_ */
