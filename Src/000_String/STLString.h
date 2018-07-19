#pragma once

namespace core
{
	std::string Format(const char* pszFormat, ...);
	std::wstring Format(const wchar_t* pszFormat, ...);
	std::string Trim(const char* pszText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring Trim(const wchar_t* pszText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string TrimLeft(const char* pszText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring TrimLeft(const wchar_t* pszText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string TrimRight(const char* pszText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring TrimRight(const wchar_t* pszText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string& Trim(std::string& strText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring& Trim(std::wstring& strText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string& TrimLeft(std::string& strText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring& TrimLeft(std::wstring& strText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string& TrimRight(std::string& strText, const char* pszWhiteSpace = "\t\n\r ");
	std::wstring& TrimRight(std::wstring& strText, const wchar_t* pszWhiteSpace = L"\t\n\r ");
	std::string MakeLowerCase(const char* pszText);
	std::wstring MakeLowerCase(const wchar_t* pszText);
	std::string MakeUpperCase(const char* pszText);
	std::wstring MakeUpperCase(const wchar_t* pszText);
	std::string MakeLowerCase(std::string strText);
	std::wstring MakeLowerCase(std::wstring strText);
	std::string MakeUpperCase(std::string strText);
	std::wstring MakeUpperCase(std::wstring strText);
	std::string& BecomeLowerCase(std::string& strText);
	std::wstring& BecomeLowerCase(std::wstring& strText);
	std::string& BecomeUpperCase(std::string& strText);
	std::wstring& BecomeUpperCase(std::wstring& strText);
	std::wstring Replace(const wchar_t* pszText, std::wstring strTarget, std::wstring strReplace);
	std::string Replace(const char* pszText, std::string strTarget, std::string strReplace);
	std::string& Replace(std::string& strText, std::string strTarget, std::string strReplace);
	std::wstring& Replace(std::wstring& strText, std::wstring strTarget, std::wstring strReplace);
	std::string Tokenize(const std::string& strText, int& nOffset);
	std::wstring Tokenize(const std::wstring& strText, int& nOffset);
	std::string Tokenize(const std::string& strText, std::string strDelimiter, int& nOffset);
	std::wstring Tokenize(const std::wstring& strText, std::wstring strDelimiter, int& nOffset);
	size_t TokenizeToArray(std::string strContext, std::string strDelimiter, std::vector<std::string>& outTokens, bool bSkipEmpty = false);
	size_t TokenizeToArray(std::wstring strContext, std::wstring strDelimiter, std::vector<std::wstring>& outTokens, bool bSkipEmpty = false);
	void Swap(std::string& strLeft, std::string& strRight);
	void Swap(std::wstring& strLeft, std::wstring& strRight);
	std::string Split(const std::string& strContext, std::string strDelimiter, std::string* pstrBack = NULL);
	std::wstring Split(const std::wstring& strContext, std::wstring strDelimiter, std::wstring* pstrBack = NULL);
	std::string Right(const std::string& strContext, size_t tCount);
	std::wstring Right(const std::wstring& strContext, size_t tCount);
	bool StrCmpWithWildcard(std::string strContext, std::string strPattern);
	bool StrCmpWithWildcard(std::wstring strContext, std::wstring strPattern);
}