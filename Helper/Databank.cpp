#include "databank.h"

using namespace helper;

DatabankResponse::DatabankResponse()
{
	raw        = NULL;
	errorLevel = dbeNoResponse;
}

DatabankResponse::~DatabankResponse()
{
	SafeDelete(raw);
}

void DatabankResponse::Parse(LPCTSTR message)
{
	errorLevel = dbeNoResponse;

	raw = JSON::Parse(message);	
	if (IsLoaded())
	{
		errorLevel = dbeInvalid;
		code       = TEXT("");
		msg        = TEXT("");

		JSONObject root;
		if (raw->IsObject())
		{
			root = raw->AsObject();
			if (root.find(TEXT("status")) != root.end() && root[TEXT("status")]->IsBool()
				&& root.find(TEXT("response")) != root.end() && root[TEXT("response")]->IsObject())
			{
				status = root[TEXT("status")]->AsBool();

				response = root[TEXT("response")]->AsObject();
				if (status)
				{
					errorLevel = dbeBank;

					if (response.find(TEXT("response_code")) != response.end() && response[TEXT("response_code")]->IsString())
						code = response[TEXT("response_code")]->AsString();

					if (response.find(TEXT("response_msg")) != response.end() && response[TEXT("response_msg")]->IsString())
						msg = response[TEXT("response_msg")]->AsString();
				}
				else
				{
					errorLevel = dbeTechnical;

					if (response.find(TEXT("Exception")) != response.end() && response[TEXT("Exception")]->IsObject())
					{
						JSONObject Exception = response[TEXT("Exception")]->AsObject();
						if (Exception.find(TEXT("ErrorCode")) != Exception.end() && Exception[TEXT("ErrorCode")]->IsString())
							code = Exception[TEXT("ErrorCode")]->AsString();

						if (Exception.find(TEXT("ErrorMessage")) != Exception.end() && Exception[TEXT("ErrorMessage")]->IsString())
							msg = Exception[TEXT("ErrorMessage")]->AsString();
					}
				}
			}
		}
	}
}

bool DatabankResponse::IsLoaded()
{
	return raw != NULL;
}

bool DatabankResponse::IsParsed()
{
	return GetLevel() > dbeInvalid;
}

bool DatabankResponse::IsTechnical()
{
	return GetLevel() == dbeTechnical;
}

bool DatabankResponse::IsBank()
{
//	return IsParsed() && !IsTechnical();
	return GetLevel() == dbeBank;
}

DatabankResponseError DatabankResponse::GetLevel()
{
	return errorLevel;
}

std::wstring DatabankResponse::GetCode()
{
	return code;
}

std::wstring DatabankResponse::GetMsg()
{
	return msg;
}

std::wstring DatabankResponse::AsString(LPCTSTR szKeyVal)
{
	if (response.find(szKeyVal) != response.end() && response[szKeyVal]->IsString())
	{
		return response[szKeyVal]->AsString();
	}
	return TEXT("");
}

bool DatabankResponse::AsBool(LPCTSTR szKeyVal)
{
	if (response.find(szKeyVal) != response.end() && response[szKeyVal]->IsBool())
	{
		return response[szKeyVal]->AsBool();
	}
	return false;
}

std::wstring Databank::DoSaleTransaction(
	LPCTSTR db_ref_no,
	LPCTSTR amount,
	int     vatps_bill_type)
{
	std::wstring params = TEXT("\"db_ref_no\":\"") + std::wstring(db_ref_no) + TEXT("\",");
	params             += TEXT("\"amount\":\"") + std::wstring(amount) + TEXT("\",");
	params             += TEXT("\"vatps_bill_type\":\"") + std::to_wstring(vatps_bill_type) + TEXT("\"");

	return Databank::Request(TEXT("doSaleTransaction"), params.c_str());
}

std::wstring Databank::ConfirmSaleCompleted(
	LPCTSTR db_ref_no,
	LPCTSTR trace_no,
	bool    is_vatps,
	LPCTSTR ddtd,
	LPCTSTR vatps_bill_amount,
	LPCTSTR vatps_lottery,
	LPCTSTR vatps_data)
{
	std::wstring params = TEXT("\"db_ref_no\":\"") + std::wstring(db_ref_no) + TEXT("\",");
	params              += TEXT("\"trace_no\":\"") + std::wstring(trace_no) + TEXT("\",");
	
	std::wstring ebarimt;
	if (ddtd != NULL)
	{
		ebarimt = TEXT("\"ddtd\":\"") + std::wstring(ddtd) + TEXT("\",");
	}
	else
	{
		ebarimt = TEXT("\"ddtd\":\"") + std::wstring(db_ref_no) + TEXT("\",");
	}
	ebarimt += TEXT("\"is_vatps\":\"0\"");

	if (is_vatps)
	{
		if (ddtd != NULL && vatps_bill_amount != NULL && vatps_lottery != NULL)
		{
			ebarimt = TEXT("\"is_vatps\":\"1\",");
			ebarimt += TEXT("\"ddtd\":\"") + std::wstring(ddtd) + TEXT("\",");
			ebarimt += TEXT("\"vatps_bill_amount\":\"") + std::wstring(vatps_bill_amount) + TEXT("\",");
			ebarimt += TEXT("\"vatps_lottery\":\"") + std::wstring(vatps_lottery) + TEXT("\"");
		}
		else if (vatps_data != NULL)
		{
			ebarimt = TEXT("\"is_vatps\":\"1\",");
			ebarimt += TEXT("\"vatps_data\":") + std::wstring(vatps_data);
		}		
	}

	return Databank::Request(TEXT("confirmSaleCompleted"), (params + ebarimt).c_str());
}

std::wstring Databank::VoidTransaction(
	LPCTSTR db_ref_no,
	LPCTSTR trace_no)
{
	std::wstring params = TEXT("\"db_ref_no\":\"") + std::wstring(db_ref_no) + TEXT("\",");
	params             += TEXT("\"trace_no\":\"") + std::wstring(trace_no) + TEXT("\"");

	return Databank::Request(TEXT("doSaleTransaction"), params.c_str());
}

std::wstring Databank::Request(
	LPCTSTR       szServiceName,
	LPCTSTR       szServiceParams,
	LPCTSTR       szHost,
	INTERNET_PORT nServerPort,
	LPCTSTR       szHeader)
{
	std::string data = "{\"service_name\":\"" + string::utf8encode(szServiceName) + "\",";
	data += "\"service_params\":{" + string::utf8encode(szServiceParams) + "}}";

	return CInternet::PostData(szHost, TEXT(""), szHeader, ::wcslen(szHeader), (LPVOID) data.c_str(), data.length(), nServerPort);
}