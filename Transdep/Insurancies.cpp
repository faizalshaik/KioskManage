#include "insurancies.h"

using namespace transdep;

Insurance::Insurance()
	: BaseName()
{
}

Insurance::Insurance(const UINT end_i_com_id, const std::wstring end_i_com_name, const std::wstring end_i_com_reg)
	: BaseName(end_i_com_id, end_i_com_name)
{
	Register = end_i_com_reg;
}