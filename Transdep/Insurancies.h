#pragma once

#include "../config.hpp"

#include "base.hpp"

namespace transdep
{
	class Insurance : public BaseName
	{
	public:
		Insurance();
		Insurance(const UINT, const std::wstring, const std::wstring);

	public:
		std::wstring Register;
	};
}