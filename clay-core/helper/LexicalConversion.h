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

#ifndef LEXICAL_CONVERSION_H_
#define LEXICAL_CONVERSION_H_

#include <clay-core/arch/Platform.h>
#include <clay-core/base/ClayDefines.h>

#if CLAY_PLATFORM_WIN32
  #include <Windows.h>
#endif 

#include <sstream>

#include <xercesc/util/XMLChar.hpp>

namespace CLAY{ namespace HELPER{   

class LexicalConversion
{
public:
  template<class T> 
  static T toNumber(const std::string& sStr); //inline 

  template<class T>
  static T toNumber(const XMLCh* p); //inline 

  template<class T>
  static std::string toString(T t); //inline 

#if CLAY_PLATFORM_WIN32 && defined(UNICODE)

  static std::string toString(TCHAR* pStr); //inline 
  static std::wstring utf8ToWString(const std::string& aUtf8); //inline 
  static std::string wstringToUtf8(const std::wstring& widestring); //inline 

#endif
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------toNumber
template<class T> 
inline T LexicalConversion::toNumber(const std::string& sStr)
{
  std::stringstream sstream;
  sstream << sStr;
  T t;
  sstream >> t;
  return t;
}

//---------------------------------------------toNumber
template<class T>
inline T LexicalConversion::toNumber(const XMLCh* p)
{
  return toNumber<T>((std::string)X2S(p));
}

//---------------------------------------------toString
template<class T>
inline std::string LexicalConversion::toString(T t)
{
  std::stringstream sstream;
  sstream << t;
  return sstream.str();
}

#if CLAY_PLATFORM_WIN32 && defined(UNICODE)

//---------------------------------------------toString
inline std::string LexicalConversion::toString(TCHAR* pStr)
{
  char rgchTableName[257];
  LPSTR lpszName = rgchTableName;
  int nSize;
  nSize = ::WideCharToMultiByte(CP_ACP,0,  pStr, -1, lpszName, 257, NULL, NULL);
  return lpszName;
}

//---------------------------------------------utf8ToWString
inline std::wstring LexicalConversion::utf8ToWString(const std::string& aUtf8)
{
  //copied from http://www.codeproject.com/KB/string/UtfConverter.aspx?msg=3644783#xx3644783xx
  int widesize = ::MultiByteToWideChar(CP_UTF8, 0, aUtf8.c_str(), -1, NULL, 0);
  if (widesize == ERROR_NO_UNICODE_TRANSLATION || 
      widesize == 0)
  {
    return L"";
  }

  std::vector<wchar_t> resultstring(widesize);
  int convresult = ::MultiByteToWideChar(CP_UTF8, 0, aUtf8.c_str(), -1, &resultstring[0], widesize);
  if (convresult != widesize)
  {
    return L"";
  }
  return std::wstring(&resultstring[0]);
}

//---------------------------------------------wstringToUtf8
inline std::string LexicalConversion::wstringToUtf8(const std::wstring& widestring)
{
  //copied from http://www.codeproject.com/KB/string/UtfConverter.aspx?msg=3644783#xx3644783xx
  int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
  if (utf8size == 0)
  {
    return "";
  }

  std::vector<char> resultstring(utf8size);
  int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

  if (convresult != utf8size)
  {
    return "";
  }

  return std::string(&resultstring[0]);
}

#endif

} }

#endif 
