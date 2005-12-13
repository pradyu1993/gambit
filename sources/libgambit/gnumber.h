//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// General number class
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#ifndef GNUMBER_H
#define GNUMBER_H

#include "base.h"
#include "rational.h"

typedef enum { precDOUBLE, precRATIONAL } gPrecision;

class gbtNumber  {
protected:
  gPrecision rep;
  union {
    gbtRational *rval;
    double dval;
  };

public:
  // CONSTRUCTORS, DESTRUCTOR, CONSTRUCTIVE OPERATORS
  gbtNumber(void);
  gbtNumber(float);
  gbtNumber(double);
  gbtNumber(int n);
  gbtNumber(long n);
  gbtNumber(const gbtInteger &y);
  gbtNumber(const gbtRational &y);
  gbtNumber(const gbtNumber &y);
  ~gbtNumber();

  gbtNumber &operator=(const gbtNumber &y);

  // OPERATOR OVERLOADING
  friend bool operator==(const gbtNumber &x, const gbtNumber &y);
  friend bool operator!=(const gbtNumber &x, const gbtNumber &y);
  friend bool operator< (const gbtNumber &x, const gbtNumber &y);
  friend bool operator<=(const gbtNumber &x, const gbtNumber &y);
  friend bool operator> (const gbtNumber &x, const gbtNumber &y);
  friend bool operator>=(const gbtNumber &x, const gbtNumber &y);

  friend gbtNumber operator+(const gbtNumber &x, const gbtNumber &y);
  friend gbtNumber operator-(const gbtNumber &x, const gbtNumber &y);
  friend gbtNumber operator*(const gbtNumber &x, const gbtNumber &y);
  friend gbtNumber operator/(const gbtNumber &x, const gbtNumber &y);
  friend gbtNumber operator-(const gbtNumber &x);

  gbtNumber &operator+=(const gbtNumber &y);
  gbtNumber &operator-=(const gbtNumber &y);
  gbtNumber &operator*=(const gbtNumber &y);
  gbtNumber &operator/=(const gbtNumber &y);

  friend std::ostream &operator<<(std::ostream &s, const gbtNumber &y);
  friend std::istream &operator>>(std::istream &s, gbtNumber &y);

  // MISCELLANEOUS MATHEMATICAL FUNCTIONS
  friend gbtNumber pow(const gbtNumber&,long);

  // PRECISION-RELATED FUNCTIONS AND CASTS
  operator double() const;
  operator gbtRational() const;
  gPrecision Precision(void) const { return rep; }
  bool IsInteger(void) const;
};

std::ostream &operator<<(std::ostream &, const gbtNumber &);

std::string ToText(const gbtNumber &);
std::string ToText(const gbtNumber &p_number, int p_precision);
gbtNumber ToNumber(const std::string &);    

void gEpsilon(gbtNumber &v, int i=8);

inline gbtNumber abs(const gbtNumber &a)
{
  if (a > gbtNumber(0))  return a;  else return -a;
}

#endif  // GNUMBER_H
