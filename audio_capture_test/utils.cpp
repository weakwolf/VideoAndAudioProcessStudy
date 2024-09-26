#include "utils.h"

#include "pch.h"

#include <string>

namespace convert
{
	CStringW UTF8ToUnicode16(const std::string& szInput)
	{
		if (0 == szInput.size())
			return _T("");

		// 确定要转换的字符数
		int nCount = MultiByteToWideChar(CP_UTF8, 0, szInput.c_str(), -1, nullptr, 0);
		WCHAR* pBuf = new WCHAR[nCount];
		memset(pBuf, 0, nCount * sizeof(WCHAR));
		// autf8字符转为unicode16字符
		MultiByteToWideChar(CP_UTF8, 0, szInput.c_str(), -1, pBuf, nCount);

		CStringW szTemp(pBuf);
		delete[] pBuf;
		pBuf = nullptr;

		return szTemp;
	}

	std::string Unicode16ToUtf8(CStringW& szInput)
	{
		if (0 == szInput.GetLength())
			return "";

		// 确定要转换的字符数
		int nCount = WideCharToMultiByte(CP_UTF8, 0, szInput.GetBuffer(), -1, nullptr, 0, NULL, NULL);
		char* pBuf = new char[nCount];
		memset(pBuf, 0, nCount * sizeof(char));
		// unicode16字符转utf8字符
		WideCharToMultiByte(CP_UTF8, 0, szInput.GetBuffer(), -1, pBuf, nCount, NULL, NULL);

		std::string szTemp(pBuf);
		delete[] pBuf;
		pBuf = nullptr;

		return szTemp;
	}
}

