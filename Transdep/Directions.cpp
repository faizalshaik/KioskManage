#include "directions.h"

using namespace transdep;

Direction::Direction()
	: BaseName()
{
}

Direction::Direction(const UINT direction_id, const std::wstring direction_name, const bool direction_UX_eseh)
	: BaseName(direction_id, direction_name)
{
	isUX = direction_UX_eseh;
}

Directions::Directions()
{
	raw = new std::vector<RawDirection>();
}

Directions::~Directions()
{
	if (raw != NULL)
		delete raw;
}

bool Directions::IsEmpty()
{
	return raw->empty();
}

void Directions::Reset()
{
	raw->clear();
}

void Directions::ParseXML(const pugi::xml_document* doc)
{
	if (!doc)
		return;

	pugi::xml_node element =
		doc->child(TEXT("DataTable"))
			.child(TEXT("diffgr:diffgram"))
			.child(TEXT("DocumentElement"))
			.first_child();

	while (element != NULL) {
		RawDirection rd;
		RawData::SetInt(rd.id, element.child_value(TEXT("id")));

		RawData::SetInt(rd.direction_id, element.child_value(TEXT("direction_id")));
		RawData::SetBool(rd.direction_UX_eseh, element.child_value(TEXT("direction_UX_eseh")));
		rd.direction_name = element.child_value(TEXT("direction_name"));

		RawData::SetInt(rd.stop_id, element.child_value(TEXT("stop_id")));
		rd.stop_name = element.child_value(TEXT("stop_name"));
		RawData::SetInt(rd.stop_sequence, element.child_value(TEXT("stop_sequence")));

		RawData::SetInt(rd.distance, element.child_value(TEXT("distance")));

		RawData::SetTimeTransdep(rd.big_move_time, element.child_value(TEXT("big_move_time")));
		RawData::SetTimeTransdep(rd.big_stop_time, element.child_value(TEXT("big_stop_time")));
		RawData::SetTimeTransdep(rd.mid_move_time, element.child_value(TEXT("mid_move_time")));
		RawData::SetTimeTransdep(rd.mid_stop_time, element.child_value(TEXT("mid_stop_time")));
		RawData::SetTimeTransdep(rd.lit_move_time, element.child_value(TEXT("lit_move_time")));
		RawData::SetTimeTransdep(rd.lit_stop_time, element.child_value(TEXT("lit_stop_time")));
		RawData::SetTimeTransdep(rd.sit_move_time, element.child_value(TEXT("sit_move_time")));
		RawData::SetTimeTransdep(rd.sit_stop_time, element.child_value(TEXT("sit_stop_time")));

		RawData::SetBool(rd.is_motion, element.child_value(TEXT("is_motion")));
		RawData::SetBool(rd.is_rtrn, element.child_value(TEXT("is_rtrn")));

		RawData::SetInt(rd.aimag_id, element.child_value(TEXT("aimag_id")));
		RawData::SetDouble(rd.lat, element.child_value(TEXT("lat")));
		RawData::SetDouble(rd.lon, element.child_value(TEXT("lon")));

		raw->push_back(rd);
		element = element.next_sibling();
	}
}