#include <ytl/brainfuck.hpp>

#include <iostream>
#include <string>

struct pyaa_dict
{
	typedef wchar_t char_type;
	static const int max_length = 6;

	static const char_type* increment_pointer()
	{
		return L"�߬";
	}

	static const char_type* decrement_pointer()
	{
		return L"<";
	}

	static const char_type* repeat()
	{
		return L"(^q^)";
	}

	static const char_type* loop()
	{
		return L"�I";
	}

	static const char_type* increment_value()
	{
		return L"�";
	}

	static const char_type* decrement_value()
	{
		return L"-";
	}

	static const char_type* input()
	{
		return L",";
	}

	static const char_type* output()
	{
		return L"������";
	}
};

int main() {
	const std::wstring s
		= L"�߬���������(^q^)<���������߬-�I<������" \
		L"�߬�������(^q^)<�����߬-�I<������ާ������" \
		L"�����޳����ާ��������(^q^)-�I�߬��������" \
		L"(^q^)<�����߬-�I<�������߬�����������" \
		L"(^q^)<������߬-�I<�������߬��������(^q^)<����߬-�I" \
		L"<�����ާ��������------������--------������" \
		L"(^q^)-�I�߬��������(^q^)<�����߬-�I<�������" \
		L"(^q^)-�I����������������";

	const bool b = ytd::brainfuck::parse<pyaa_dict>( s.cbegin(), s.cend() );
	if ( !b )
		std::cout << " : error..." << std::endl;

	{ char c; std::cin >> c; }
}