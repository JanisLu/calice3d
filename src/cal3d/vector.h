//****************************************************************************//
// vector.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_VECTOR_H
#define CAL_VECTOR_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalQuaternion;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The vector class.
  *****************************************************************************/

class CAL3D_API CalVector
{
// member variables
public:
  float x;
  float y;
  float z;

// constructors/destructor
public:
  CalVector();
  CalVector(const CalVector& v);
  CalVector(float vx, float vy, float vz);
  ~CalVector();

// member functions
public:
  inline float& operator[](unsigned int i);
  inline const float& operator[](unsigned int i) const;
  inline void operator=(const CalVector& v);
  inline void operator+=(const CalVector& v);
  inline void operator-=(const CalVector& v);
  inline void operator*=(const float d);
  inline void operator*=(const CalQuaternion& q);
  inline void operator/=(const float d);
  friend inline CAL3D_API CalVector operator+(const CalVector& v, const CalVector& u);
  friend inline CAL3D_API CalVector operator-(const CalVector& v, const CalVector& u);
  friend inline CAL3D_API CalVector operator*(const CalVector& v, const float d);
  friend inline CAL3D_API CalVector operator*(const float d, const CalVector& v);
  friend inline CAL3D_API CalVector operator/(const CalVector& v, const float d);
  friend inline CAL3D_API float operator*(const CalVector& v, const CalVector& u);
  friend inline CAL3D_API CalVector operator%(const CalVector& v, const CalVector& u);
  inline void blend(float d, const CalVector& v);
  inline void clear();
  inline float length();
  inline float normalize();
  inline void set(float vx, float vy, float vz);
};

#endif

//****************************************************************************//
