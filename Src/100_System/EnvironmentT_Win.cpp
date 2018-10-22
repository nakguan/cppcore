#include "stdafx.h"
#include "Environment.h"
#include "Log.h"
#include "Information.h"
#undef TEXT
#include <Windows.h>
#include <IPTypes.h>
#include <Iphlpapi.h>
#pragma comment(lib, "version.lib")

namespace core
{
	//////////////////////////////////////////////////////////////////////////
#undef LoadLibrary
	HANDLE LoadLibrary(LPCTSTR pszPath)
	{
#ifdef UNICODE
		return ::LoadLibraryW(pszPath);
#else
		return ::LoadLibraryA(pszPath);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetErrorString(ECODE nCode)
	{
		const size_t	tBufSize = 256;
		TCHAR			szBuf[tBufSize] = { 0, };
		if( 0 == ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, nCode, 0, szBuf, tBufSize, NULL) )
		{
			Log_Error("FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ... %u) calling failure, %u", nCode, ::GetLastError());
			return Format(TEXT("Error Code:%u"), nCode);
		}

		return std::tstring(szBuf);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetUserName(void)
	{
		std::tstring strName = TEXT("unknown");

		DWORD dwLength = 0;
		::GetUserNameA(NULL, &dwLength);
		if( ERROR_INSUFFICIENT_BUFFER == GetLastError() && dwLength > 0)
		{
			strName.resize(dwLength - 1);
#ifdef UNICODE
			::GetUserNameW((TCHAR*)strName.c_str(), &dwLength);
#else
			::GetUserNameA((TCHAR*)strName.c_str(), &dwLength);
#endif
			return strName;
		}

		Log_Error("GetUserName calling failure");
		return TEXT("");
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetComputerName(void)
	{
		TCHAR szName[MAX_COMPUTERNAME_LENGTH+1]= {0,};
		DWORD dwCchLen = MAX_COMPUTERNAME_LENGTH+1;

#ifdef UNICODE
		if( FALSE == ::GetComputerNameW(szName, &dwCchLen) )
#else
		if( FALSE == ::GetComputerNameA(szName, &dwCchLen) )
#endif
		{
			Log_Error("GetComputerName calling failure");
			return TEXT("");
		}

		return szName;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GenerateGuid(void)
	{
		GUID		guid		= {0,};
		std::tstring	strRet;

		if( SUCCEEDED(::CoCreateGuid(&guid)) )
		{
			strRet = Format(TEXT("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X")
				, guid.Data1, guid.Data2, guid.Data3
				, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
			, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		}
		else
		{
			Log_Warn("CoCreateGuid failure, generating random value");
			strRet = Format(TEXT("%04x%04x-%04x-%04x-%04x-%04x%04x%04x")
				, ::rand()&0xffff, ::rand()&0xffff						// Generates a 64-bit Hex number
				, ::rand()&0xffff										// Generates a 32-bit Hex number
				, ((::rand() & 0x0fff) | 0x4000)						// Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
				, ::rand() % 0x3fff + 0x8000							// Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
				, ::rand()&0xffff, ::rand()&0xffff, ::rand()&0xffff);	// Generates a 96-bit Hex number
		}

		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
#undef OutputDebugString
	void OutputDebugString(const TCHAR* pszFormat, ...)
	{
		const size_t tBuffSize = 2048;
		TCHAR szBuff[tBuffSize] = { 0, };
		va_list list;
		va_start(list, pszFormat);
		SafeSVPrintf(szBuff, tBuffSize, pszFormat, list);
		va_end(list);
#ifdef UNICODE
		::OutputDebugStringW(szBuff);
#else
		::OutputDebugStringA(szBuff);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformation(ST_TIME_ZONE_INFORMATION* pTimeZone)
	{
		::TIME_ZONE_INFORMATION stTemp = { 0, };
		DWORD dwRet = ::GetTimeZoneInformation(&stTemp);
		if( TIME_ZONE_ID_INVALID == dwRet )
			return TIME_ZONE_ID_INVALID_;

		memset(pTimeZone, 0, sizeof(*pTimeZone));
		pTimeZone->Bias = stTemp.Bias;
		SafeStrCpy(pTimeZone->StandardName, 32, TCSFromWCS(stTemp.StandardName).c_str());
		SafeStrCpy(pTimeZone->DaylightName, 32, TCSFromWCS(stTemp.DaylightName).c_str());
		if( TIME_ZONE_ID_UNKNOWN == dwRet )
			return TIME_ZONE_ID_UNKNOWN_;

		ST_SYSTEMTIME stGMT;
		GetSystemTime(&stGMT);
		MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.StandardDate, pTimeZone->StandardDate);
		MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.DaylightDate, pTimeZone->DaylightDate);
		pTimeZone->StandardBias = stTemp.StandardBias;
		pTimeZone->DaylightBias = stTemp.DaylightBias;
		return (ETIMEZONE)dwRet;
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformation(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATION* pOutTimeZone)
	{
		::TIME_ZONE_INFORMATION stTemp = { 0, };
		DWORD dwRet = ::GetTimeZoneInformation(&stTemp);
		if( TIME_ZONE_ID_INVALID == dwRet )
		{
			Log_Error("GetTimeZoneInformation calling failure");
			return TIME_ZONE_ID_INVALID_;
		}

		if( TIME_ZONE_ID_UNKNOWN == dwRet )
			return TIME_ZONE_ID_UNKNOWN_;

		std::tstring strStandardName = TCSFromWCS(stTemp.StandardName);
		std::tstring strDaylightName = TCSFromWCS(stTemp.DaylightName);
		SafeStrCpy(pOutTimeZone->StandardName, 32, strStandardName.c_str());
		SafeStrCpy(pOutTimeZone->DaylightName, 32, strDaylightName.c_str());
		pOutTimeZone->Bias			 = stTemp.Bias			;
		pOutTimeZone->StandardBias	 = stTemp.StandardBias	;
		pOutTimeZone->DaylightBias	 = stTemp.DaylightBias	;

		if( !MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.StandardDate, pOutTimeZone->StandardDate) )
			memcpy(&pOutTimeZone->StandardDate, &stTemp.StandardDate, sizeof(stTemp.StandardDate));

		if( MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.DaylightDate, pOutTimeZone->DaylightDate) )
			memcpy(&pOutTimeZone->DaylightDate, &stTemp.DaylightDate, sizeof(stTemp.DaylightDate));

		// if window API returned difference year's absolute date, it is failure.
		if( pOutTimeZone->StandardDate.wYear != stGMT.wYear
		||  pOutTimeZone->DaylightDate.wYear != stGMT.wYear )
			return TIME_ZONE_ID_INVALID_;

		QWORD qwTargetTime = UnixTimeFrom(stGMT);
		QWORD qwDaylightTime = UnixTimeFrom(pOutTimeZone->DaylightDate);
		QWORD qwStandardTime = UnixTimeFrom(pOutTimeZone->StandardDate);

		if( qwDaylightTime < qwStandardTime )
		{
			if( qwTargetTime < qwDaylightTime )
				return TIME_ZONE_ID_STANDARD_;
			if( qwTargetTime < qwStandardTime )
				return TIME_ZONE_ID_DAYLIGHT_;
			return TIME_ZONE_ID_STANDARD_;
		}

		if( qwTargetTime < qwStandardTime )
			return TIME_ZONE_ID_DAYLIGHT_;
		if( qwTargetTime < qwDaylightTime )
			return TIME_ZONE_ID_STANDARD_;
		return TIME_ZONE_ID_DAYLIGHT_;
	}
}