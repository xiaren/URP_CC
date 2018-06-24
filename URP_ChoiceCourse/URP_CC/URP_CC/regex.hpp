#pragma once

#include "init.h"



inline bool my_regex_search(const string pattern,const string& src);
inline int my_sregex_iterator(const string pattern, const string& src, vector<string>& v_str);





/**********************定义部分***********************/




inline bool my_regex_search(const string pattern, const string& src)
{
	regex re(pattern);
	bool ret = regex_search(src, re);
	if (ret)
		return TRUE;
	return FALSE;
}

inline int my_sregex_iterator(const string pattern,const string& src,vector<string>& v_str)
{
	regex re(pattern);
	auto begin = sregex_iterator(src.begin(), src.end(), re);
	auto   end = sregex_iterator();

	cout << "Found :" << distance(begin, end) << endl;

	for (sregex_iterator i = begin; i != end; ++i)
	{
		smatch match = *i;
		v_str.push_back(match.str());
	}

	return 0;
}