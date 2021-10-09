#pragma once

#include "base.hpp"

namespace transdep {
	class Stop : public BaseName
	{
	public:
		Stop();
		Stop(const UINT, const std::wstring, const double, const double, const UINT);

		const std::wstring getClean();

	public:
		double Latitude;
		double Longitude;
		UINT   AimagID;
	};

	class EndStop : public Base
	{
	public:
		EndStop();
		EndStop(const UINT, const UINT);
		void AttachTariff(const UINT);
		bool IsBig();

	public:
		std::vector<UINT> tariff_ids;

	protected:
		UINT nType;
	};
}