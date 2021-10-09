/**
* globalpay.hpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>

template <typename T>
inline void SafeDelete(T *&v)
{
	if (v != NULL)
	{
		delete[] v;
		v = NULL;
	}
}

template <typename T>
inline void SafeRelease(T *&p)
{
	if (p != NULL)
	{
		p->Release();
		p = NULL;
	}
}

template <typename T>
inline void SafeDeleteObject(T *&o)
{
	if (o != NULL)
	{
		::DeleteObject(o);
		o = NULL;
	}
}