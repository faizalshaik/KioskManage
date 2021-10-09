#include "tariffs.h"

using namespace globalpay;
using namespace transdep;

Tariffs::Tariffs()
{
	raw = new std::vector<RawTariff>();
}

Tariffs::~Tariffs()
{
	if (raw != NULL)
		delete raw;
}

bool Tariffs::IsEmpty()
{
	return raw->empty();
}

void Tariffs::Reset()
{
	raw->clear();
}

void Tariffs::ParseXML(const pugi::xml_document* doc)
{
	if (!doc)
		return;

	pugi::xml_node element =
		doc->child(TEXT("DataTable"))
			.child(TEXT("diffgr:diffgram"))
			.child(TEXT("DocumentElement"))
			.first_child();

	while (element != NULL)
	{
		RawTariff rtariff;		
		RawData::SetInt(rtariff.id, element.child_value(TEXT("id")));

		RawData::SetInt(rtariff.direction_id, element.child_value(TEXT("direction_id")));
		RawData::SetBool(rtariff.direction_UX_eseh, element.child_value(TEXT("direction_UX_eseh")));
		rtariff.direction_name = element.child_value(TEXT("direction_name"));

		RawData::SetInt(rtariff.start_stop_id, element.child_value(TEXT("start_stop_id")));
		rtariff.start_stop_name = element.child_value(TEXT("start_stop_name"));
		RawData::SetInt(rtariff.end_stop_id, element.child_value(TEXT("end_stop_id")));
		rtariff.end_stop_name = element.child_value(TEXT("end_stop_name"));

		RawData::SetBool(rtariff.is_rtrn, element.child_value(TEXT("is_rtrn")));

		RawData::SetInt(rtariff.start_stop_sequence, element.child_value(TEXT("start_stop_sequence")));
		RawData::SetInt(rtariff.end_stop_sequence, element.child_value(TEXT("end_stop_sequence")));

		RawData::SetInt(rtariff.ticket_type_id, element.child_value(TEXT("ticket_type_id")));

		RawData::SetDouble(rtariff.end_stop_lat, element.child_value(TEXT("end_stop_lat")));
		RawData::SetDouble(rtariff.end_stop_lon, element.child_value(TEXT("end_stop_lon")));
		RawData::SetDouble(rtariff.start_stop_lat, element.child_value(TEXT("start_stop_lat")));
		RawData::SetDouble(rtariff.start_stop_lon, element.child_value(TEXT("start_stop_lon")));

		RawData::SetInt(rtariff.aimag_id, element.child_value(TEXT("aimag_id")));
		RawData::SetInt(rtariff.end_stop_aimag_id, element.child_value(TEXT("end_stop_aimag_id")));

		RawData::SetFloat(rtariff.big_price, element.child_value(TEXT("big_price")));
		RawData::SetFloat(rtariff.big_childprice, element.child_value(TEXT("big_childprice")));
		RawData::SetFloat(rtariff.big_insurance, element.child_value(TEXT("big_insurance")));
		RawData::SetFloat(rtariff.big_childinsurance, element.child_value(TEXT("big_childinsurance")));
		RawData::SetFloat(rtariff.mid_price, element.child_value(TEXT("mid_price")));
		RawData::SetFloat(rtariff.mid_childprice, element.child_value(TEXT("mid_childprice")));
		RawData::SetFloat(rtariff.mid_insurance, element.child_value(TEXT("mid_insurance")));
		RawData::SetFloat(rtariff.mid_childinsurance, element.child_value(TEXT("mid_childinsurance")));
		RawData::SetFloat(rtariff.lit_price, element.child_value(TEXT("lit_price")));
		RawData::SetFloat(rtariff.lit_childprice, element.child_value(TEXT("lit_childprice")));
		RawData::SetFloat(rtariff.lit_insurance, element.child_value(TEXT("lit_insurance")));
		RawData::SetFloat(rtariff.lit_childinsurance, element.child_value(TEXT("lit_childinsurance")));
		RawData::SetFloat(rtariff.sit_price, element.child_value(TEXT("sit_price")));
		RawData::SetFloat(rtariff.sit_childprice, element.child_value(TEXT("sit_childprice")));
		RawData::SetFloat(rtariff.sit_insurance, element.child_value(TEXT("sit_insurance")));
		RawData::SetFloat(rtariff.sit_childinsurance, element.child_value(TEXT("sit_childinsurance")));

		RawData::SetInt(rtariff.end_i_com_id, element.child_value(TEXT("end_i_com_id")));
		rtariff.end_i_com_name = element.child_value(TEXT("end_i_com_name"));
		rtariff.end_i_com_reg = element.child_value(TEXT("end_i_com_reg"));

		RawData::SetInt(rtariff.start_i_com_id, element.child_value(TEXT("start_i_com_id")));
		rtariff.start_i_com_name = element.child_value(TEXT("start_i_com_name"));
		rtariff.start_i_com_reg = element.child_value(TEXT("start_i_com_reg"));

		RawData::SetInt(rtariff.big_price_percent, element.child_value(TEXT("big_price_percent")));
		RawData::SetInt(rtariff.mid_price_percent, element.child_value(TEXT("mid_price_percent")));
		RawData::SetInt(rtariff.lit_price_percent, element.child_value(TEXT("lit_price_percent")));
		RawData::SetInt(rtariff.sit_price_percent, element.child_value(TEXT("sit_price_percent")));
		
		if (rtariff.big_price_percent == 0)
			rtariff.big_price_percent = 8;
		if (rtariff.mid_price_percent == 0)
			rtariff.mid_price_percent = 8;
		if (rtariff.lit_price_percent == 0)
			rtariff.lit_price_percent = 8;
		if (rtariff.sit_price_percent == 0)
			rtariff.sit_price_percent = 4;

		raw->push_back(rtariff);
		element = element.next_sibling();
	}
}

Tariff::Tariff()
	: Base()
{
}

Tariff::Tariff(const UINT id)
	: Base(id)
{
}