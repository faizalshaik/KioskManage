#pragma once

#include "base.hpp"

using namespace globalpay;

namespace transdep
{
	struct StopSequence
	{
		UINT Stop;
		UINT Sequence;

		StopSequence(UINT, UINT);

		bool operator < (const StopSequence) const;
		bool operator == (const UINT) const;
	};

	class Route
	{
	public:
		std::vector<StopSequence> sequence;

		Route();

		void Reset();
		bool IsEmpty();
		void Extend(StopSequence);
		void Sort();
	};
}