/*
    Sabre Fighter Plane Simulator 
    Copyright (c) 1997 Dan Hammer
    Portions Donated By Antti Barck

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/*************************************************
 *           Sabre Fighter Plane Simulator       *
 * File   : gimesh.cpp                           *
 * Date   : Oct, 2000                            *
 * Author : Dan Hammer                           *
 * Mesh object
 *************************************************/
#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <math.h>
#include "defs.h"
#include "grafix.h"
#include "vmath.h"
#include "clip.h"

#define giISABASE    0
#define giISAVERTEX  1
#define giISMATERIAL 2
#define giISAFACE    3
#define giISAMESH    4
#define giISARRAY    5
#define giISALIST    6

typedef float giREAL;


class giObject
{
public:
	virtual int IsA(void)
	{
		return giISABASE;
	}
protected:
	giObject *next;
};

class giObjectArray : public giObject
{
  giObject **objs;       // pointers to objects
  int     size;         // size of array (max) 
  int     n;            // how many assigned so far
  int     owns;         // do we delete objects on Destroy()
public:
  void Create(int size, int owns = 1);
  void Destroy();
  void Flush();

  giObjectArray()
  {
    n = 0;
    size = 0;
    owns = 0;
    objs = NULL;
  }

  int IsA() const
  {
    return giISARRAY;
  }

  giObjectArray(int size, int owns = 1)
  {
    n = 0;
    objs = NULL;
    Create(size,owns);
  }

  ~giObjectArray()
  {
    Destroy();
  }

  giObject * operator[](int idx) const;

  int Count() const
  {
    return (n);
  }

  int Size() const
  {
    return (size);
  }

  int Add(giObject *obj);
  int Append(giObject *obj);

  int Owns() const
  {
    return (owns);
  }

  void SetOwnership(int owns)
  {
	this->owns = owns;
  }

  int Remove(int idx);
};

class giVertex : public giObject
{
public:
	giVertex(int)
	{
		memset(coords,0,sizeof(coords));
	}
	giVertex(const giVertex &v)
	{
		Copy(v);
	}
	const giVertex & operator =(const giVertex &v)
	{
		Copy(v);
		return *this;
	}
	virtual int IsA(void) 
	{
		return giISAVERTEX;
	}
	giREAL x(void)
	{
		return coords[0];
	}
	giREAL y(void)
	{
		return coords[1];
	}
	giREAL z(void)
	{
		return coords[2];
	}
	giREAL u(void)
	{
		return coords[3];
	}
	giREAL v(void)
	{
		return coords[4];
	}
	giREAL w(void)
	{
		return coords[5];
	}
protected:
	void Copy(const giVertex &v)
	{
		memcpy(coords,v.coords,sizeof(coords));
	}
	giREAL coords[6];
};

class giMaterial : public giObject
{
public:
	
protected:

};

class giFace : public giObject
{
public:
	giFace()
	{
		_v0 = NULL;
		_v1 = NULL;
		_v2 = NULL;
	}
	giFace(giVertex *v0, giVertex *v1, giVertex *v2, giMaterial *material)
	{
		_v0 = v0;
		_v1 = v1;
		_v2 = v2;
		this->material = material;
	}
	giFace(const giFace &f)
	{
		Copy(f);
	}
	giVertex *v0(void)
	{
		return _v0;
	}
	giVertex *v1(void)
	{
		return _v1;
	}
	giVertex *v2(void)
	{
		return _v2;
	}
	const giFace & operator =(const giFace &f)
	{
		Copy(f);
		return *this;
	}
protected:
	giVertex *_v0;
	giVertex *_v1;
	giVertex *_v2;
	giMaterial *material;

	void Copy(const giFace &f)
	{
		_v0 = f._v0;
		_v1 = f._v1;
		_v2 = f._v2;
		material = f.material;
	}
};

class giMesh : public giObject
{

};



void giObjectArray::Create(int size, int owns)
{
	if (objs)
		Destroy();
	this->size = size;
	this->owns = owns;
	objs = new giObject *[size];
	for (int i=0;i<size;i++)
		objs[i] = NULL;
	n = 0;
}

void giObjectArray::Destroy()
{
	if (objs)
	{
		if (owns)
		{
			for (int i=0;i<n;i++)
			{
				if (objs[i])
					delete objs[i];
			}
		}
		delete [] objs;
		objs = NULL;
	}
	n = 0;
}

void giObjectArray::Flush()
{
	if (objs)
	{
		for (int i=0;i<size;i++)
		{
			if (objs[i] != NULL)
			{
				if (owns)
					delete objs[i];
				objs[i] = NULL;
			}
		}
	}
	n = 0;
}

int giObjectArray::Add(giObject *obj)
{
int result = -1;
	/*
	 * First check for room in any
	 * previously assigned slots
	 */
	for (int i=0;i<n;i++)
	{
		if (objs[i] == NULL)
		{
			result = i;
			objs[i] = obj;
			break;
		}
	}
	/*
	* if no room, add to end
	*/
	if (result == -1)
		result = Append(obj);
	return result;
}

int giObjectArray::Append(giObject *obj)
{
 int result = -1;
	if (n < size)
	{
		result = n;
		objs[n++] = obj;
	}
  return result;
}

int giObjectArray::Remove(int idx)
{
	int result = -1;
	if (idx >= 0 && idx < n)
	{
		if (objs[idx] != NULL)
		{
			if (owns)
				delete objs[idx];
			objs[idx] = NULL;
			result = idx;
		}
	}
	return result;
}

giObject *giObjectArray::operator [](int idx) const
{
	if (idx >= 0 && idx < n)
		return (objs[idx]);
	else
		return (NULL);
}
