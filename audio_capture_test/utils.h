#pragma once

#include <string>

namespace io
{
	/**
	 * ��ӡ�����������������ͣ�ǰ���Ǹ�����֧��<<�����
	 */
	void PrintAny()
	{
		return;
	}
	template<typename T,typename... Types>
	void PrintAny(const T& first, const Types&... others)
	{
#ifdef DEBUG
		TRACE(_T("��ʣ%d������\n"), sizeof...(others));
#endif
		TRACE(_T("%s\n"), first);
		PrintAny(others...);
	}
}

namespace convert
{
	/**
	 * utf-8תUnicode16���ؼ�����MultibyteToWideChar��ʹ��
	 *
	 * @see https://learn.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	 */
	CStringW UTF8ToUnicode16(const std::string& szInput);

	/**
	 * Unicode16תutf-8���ؼ�����WideCharToMultiByte��ʹ��
	 *
	 * @see https://learn.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
	 */
	std::string Unicode16ToUtf8(CStringW& szInput);
}