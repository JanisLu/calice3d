//****************************************************************************//
// platform.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_PLATFORM_H
#define CAL_PLATFORM_H

//****************************************************************************//
// Compiler configuration                                                     //
//****************************************************************************//

#if defined(_WIN32) && !defined(__MINGW32__)
#pragma warning(disable : 4251)
#pragma warning(disable : 4786)
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning(disable : 4530)
#endif

//****************************************************************************//
// Dynamic library export setup                                               //
//****************************************************************************//

#if defined(_WIN32) && !defined(__MINGW32__)

#ifdef CAL3D_EXPORTS
#define CAL3D_API __declspec(dllexport)
#else
#define CAL3D_API __declspec(dllimport)
#endif

#else

#define CAL3D_API

#endif

//****************************************************************************//
// Endianness setup                                                           //
//****************************************************************************//

#if defined(__i386__) || defined(_M_IX86) ||\
	defined(__x86_64__) || defined(_M_X64) ||\
    defined(__alpha__) || \
    (defined(__mips__) && defined(MIPSEL)) || \
    defined(__MIPSEL__) || \
    (defined(__arm__) && defined(ARMEL)) || \
    defined(__ARMEL__)

#define CAL3D_LITTLE_ENDIAN

#else

#define CAL3D_BIG_ENDIAN

#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

// debug includes
#include <cassert>

// STL includes
#include <iostream>
#include <fstream>
#include <rdestl/vector.h>
#include <rdestl/sorted_vector.h>
#include <rdestl/pair.h>

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The platform class.
  *****************************************************************************/

class CAL3D_API CalPlatform
{
// constructors/destructor
protected:
  CalPlatform();
  virtual ~CalPlatform();

// member functions	
public:
  static bool readBytes(std::ifstream& file, void *pBuffer, int length);
  static bool readFloat(std::ifstream& file, float& value);
  static bool readInteger(std::ifstream& file, int& value);
  static bool readString(std::ifstream& file, char* strValue, int length);
  static bool writeBytes(std::ofstream& file, const void *pBuffer, int length);
  static bool writeFloat(std::ofstream& file, float value);
  static bool writeInteger(std::ofstream& file, int value);
  static bool writeString(std::ofstream& file, const char* strValue);
};

#endif

//****************************************************************************//
