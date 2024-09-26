#pragma once

#include <string>

namespace io
{
	/**
	 * 打印任意数量，任意类型，前提是该类型支持<<运算符
	 */
	void PrintAny()
	{
		return;
	}
	template<typename T,typename... Types>
	void PrintAny(const T& first, const Types&... others)
	{
#ifdef DEBUG
		TRACE(_T("还剩%d个参数\n"), sizeof...(others));
#endif
		TRACE(_T("%s\n"), first);
		PrintAny(others...);
	}
}

namespace convert
{
	/**
	 * utf-8转Unicode16，关键在于MultibyteToWideChar的使用
	 *
	 * @see https://learn.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	 */
	CStringW UTF8ToUnicode16(const std::string& szInput);

	/**
	 * Unicode16转utf-8，关键在于WideCharToMultiByte的使用
	 *
	 * @see https://learn.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
	 */
	std::string Unicode16ToUtf8(CStringW& szInput);
}