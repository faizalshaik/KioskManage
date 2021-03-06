#pragma once

#include "types/galig.hpp"

#include "base.hpp"

namespace transdep
{
	class Aimag : public BaseName
	{
	public:
		Aimag() : BaseName() { }

		Aimag(const UINT id, const std::wstring name)
		{
			ID = id;

			switch (ID)
			{
			case 1:	Name = TEXT("Архангай"); break;
			case 2:	Name = TEXT("Баян-Өлгий"); break;
			case 3:	Name = TEXT("Баянхонгор"); break;
			case 4:	Name = TEXT("Булган"); break;
			case 5:	Name = TEXT("Говь-Алтай"); break;
			case 6:	Name = TEXT("Дорноговь"); break;
			case 7:	Name = TEXT("Дорнод"); break;
			case 8:	Name = TEXT("Дундговь"); break;
			case 9:	Name = TEXT("Завхан"); break;
			case 10: Name = TEXT("Өвөрхангай"); break;
			case 11: Name = TEXT("Өмнөговь"); break;
			case 12: Name = TEXT("Сүхбаатар"); break;
			case 13: Name = TEXT("Сэлэнгэ"); break;
			case 14: Name = TEXT("Төв"); break;
			case 15: Name = TEXT("Увс"); break;
			case 16: Name = TEXT("Ховд"); break;
			case 17: Name = TEXT("Хөвсгөл"); break;
			case 18: Name = TEXT("Хэнтий"); break;
			case 19: Name = TEXT("Дархан-Уул"); break;
			case 20: Name = TEXT("Орхон"); break;
			case 21: Name = TEXT("Говьсүмбэр"); break;
			default: Name = name;
			}
		}

		static bool IsMglAimag(const UINT id)
		{
			return (id <= 21);
		}

		std::wstring getClean()
		{
			if (cfg.IsMongolian())
				return Name;

			return galig::mntolatin(Name);
		}
	};
}
