//#include "stdafx.h"
#include "answer.h"




void write_txt(string totxt)
{
	string txt_name = totxt;
	ofstream out(txt_name);

	auto beg = map_path.begin();
	while (beg!= map_path.end())
	{
		out << "(" << beg->first;
		auto beg2 = beg->second.begin();
		while (beg2 != beg->second.end())
		{
			out << "," << *beg2++;
		}
		out << ")" << endl;
		beg++;

	}
	
}


