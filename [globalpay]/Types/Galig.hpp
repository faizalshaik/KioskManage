/**
* Galig functions - Galig.hpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include <windows.h>
#include <string>

namespace globalpay {
	class galig
	{
	public:
		static const std::wstring mn(const WCHAR ch)
		{
			if (ch == TEXT('А')) return TEXT("A");
			if (ch == TEXT('а')) return TEXT("a");
			if (ch == TEXT('Б')) return TEXT("B");
			if (ch == TEXT('б')) return TEXT("b");
			if (ch == TEXT('В')) return TEXT("V");
			if (ch == TEXT('в')) return TEXT("v");
			if (ch == TEXT('Г')) return TEXT("G");
			if (ch == TEXT('г')) return TEXT("g");
			if (ch == TEXT('Д')) return TEXT("D");
			if (ch == TEXT('д')) return TEXT("d");
			if (ch == TEXT('Е')) return TEXT("Ye");
			if (ch == TEXT('е')) return TEXT("ye");
			if (ch == TEXT('Ё')) return TEXT("Yo");
			if (ch == TEXT('ё')) return TEXT("yo");
			if (ch == TEXT('Ж')) return TEXT("J");
			if (ch == TEXT('ж')) return TEXT("j");
			if (ch == TEXT('З')) return TEXT("Z");
			if (ch == TEXT('з')) return TEXT("z");
			if (ch == TEXT('И')) return TEXT("I");
			if (ch == TEXT('и')) return TEXT("i");
			if (ch == TEXT('Й')) return TEXT("I");
			if (ch == TEXT('й')) return TEXT("i");
			if (ch == TEXT('К')) return TEXT("K");
			if (ch == TEXT('к')) return TEXT("k");
			if (ch == TEXT('Л')) return TEXT("L");
			if (ch == TEXT('л')) return TEXT("l");
			if (ch == TEXT('М')) return TEXT("M");
			if (ch == TEXT('м')) return TEXT("m");
			if (ch == TEXT('Н')) return TEXT("N");
			if (ch == TEXT('н')) return TEXT("n");
			if (ch == TEXT('О')) return TEXT("O");
			if (ch == TEXT('о')) return TEXT("o");
			if (ch == TEXT('Ө')) return TEXT("U");
			if (ch == TEXT('ө')) return TEXT("u");
			if (ch == TEXT('П')) return TEXT("P");
			if (ch == TEXT('п')) return TEXT("p");
			if (ch == TEXT('Р')) return TEXT("R");
			if (ch == TEXT('р')) return TEXT("r");
			if (ch == TEXT('С')) return TEXT("S");
			if (ch == TEXT('с')) return TEXT("s");
			if (ch == TEXT('Т')) return TEXT("T");
			if (ch == TEXT('т')) return TEXT("t");
			if (ch == TEXT('У')) return TEXT("U");
			if (ch == TEXT('у')) return TEXT("u");
			if (ch == TEXT('Ү')) return TEXT("U");
			if (ch == TEXT('ү')) return TEXT("u");
			if (ch == TEXT('Ф')) return TEXT("Ph");
			if (ch == TEXT('ф')) return TEXT("ph");
			if (ch == TEXT('Х')) return TEXT("Kh");
			if (ch == TEXT('х')) return TEXT("kh");
			if (ch == TEXT('Ц')) return TEXT("Ts");
			if (ch == TEXT('ц')) return TEXT("ts");
			if (ch == TEXT('Ч')) return TEXT("Ch");
			if (ch == TEXT('ч')) return TEXT("ch");
			if (ch == TEXT('Ш')) return TEXT("Sh");
			if (ch == TEXT('ш')) return TEXT("sh");
			if (ch == TEXT('Щ')) return TEXT("Shs");
			if (ch == TEXT('щ')) return TEXT("shs");
			if (ch == TEXT('Ъ')) return TEXT("I");
			if (ch == TEXT('ъ')) return TEXT("i");
			if (ch == TEXT('Ы')) return TEXT("I");
			if (ch == TEXT('ы')) return TEXT("i");
			if (ch == TEXT('Ь')) return TEXT("I");
			if (ch == TEXT('ь')) return TEXT("i");
			if (ch == TEXT('Э')) return TEXT("E");
			if (ch == TEXT('э')) return TEXT("e");
			if (ch == TEXT('Ю')) return TEXT("Yu");
			if (ch == TEXT('ю')) return TEXT("yu");
			if (ch == TEXT('Я')) return TEXT("Ya");
			if (ch == TEXT('я')) return TEXT("ya");

			std::wstring txt = TEXT(" ");
			txt[0] = ch;
			return txt;
		}

		static const std::wstring mntolatin(std::wstring Mongol)
		{
			std::wstring Latin;
			for (UINT i = 0; i < Mongol.length(); i++)
				Latin += galig::mn(Mongol[i]);

			return Latin;
		}
	};
}