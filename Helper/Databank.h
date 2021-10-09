#pragma once

#include "globalpay.hpp"

#include "[vendor]/simplejson/json.h"

#include "internet/internet.h"
#include "types/string.h"

#include "../config.hpp"

using namespace globalpay;

namespace helper {
	typedef enum
	{
		dbeNoResponse,
		dbeInvalid,
		dbeTechnical,
		dbeBank
	} DatabankResponseError;

	class DatabankResponse
	{
	public:
		DatabankResponse();
		~DatabankResponse();

		void         Parse(LPCTSTR);
		bool         IsLoaded();
		bool         IsParsed();
		bool         IsTechnical();
		bool         IsBank();
		DatabankResponseError
			         GetLevel();
		std::wstring GetCode();
		std::wstring GetMsg();
		std::wstring AsString(LPCTSTR);
		bool         AsBool(LPCTSTR);

	protected:
		JSONValue   *raw;
		JSONObject   root;
		JSONObject   response;		
		
		bool         status;
		std::wstring code;
		std::wstring msg;

		DatabankResponseError
			         errorLevel;
	};

	class Databank
	{
	public:
		static std::wstring DoSaleTransaction(
			LPCTSTR db_ref_no,
			LPCTSTR amount,
			int     vatps_bill_type = 3);

		static std::wstring ConfirmSaleCompleted(
			LPCTSTR db_ref_no,
			LPCTSTR trace_no,
			bool    is_vatps = false,
			LPCTSTR ddtd = NULL,
			LPCTSTR vatps_bill_amount = NULL,
			LPCTSTR vatps_lottery = NULL,
			LPCTSTR vatps_data = NULL);
		
		static std::wstring VoidTransaction(
			LPCTSTR db_ref_no,
			LPCTSTR trace_no);

		static std::wstring Request(
			LPCTSTR       szServiceName,
			LPCTSTR       szServiceParams,
			LPCTSTR       szHost = TEXT("https://www.metakiosk.mn"),
			INTERNET_PORT nServerPort = 443,
			LPCTSTR       szHeader = TEXT("Content-Type: application/json; charset=utf-8"));
	};
}