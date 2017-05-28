#include "ReadHandler.h"
#include"WriteHandler.h"
CReadHandler::CReadHandler()
{
}

CReadHandler * CReadHandler::getInstance()
{
	if (nullptr == ReadHandlerStatic)
	{
		ReadHandlerStatic = new CReadHandler();
	}
	return ReadHandlerStatic;
}


CReadHandler::~CReadHandler()
{
}

int CReadHandler::Search(const char * textFileName, bool isFullMatch, int count, ...)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return -1;
	}
	va_list Marker;
	va_start(Marker, count);

	vector<string> searchTarget;
	for (int i = 0; i < count; i++)
	{
		searchTarget.push_back(va_arg(Marker, string));
	}

	int sameAmount = 0;
	int line = 0;
	while (!inFile.eof())
	{
		line++;
		sameAmount = 0;
		char temp[256];
		inFile.getline(temp, 256);
		string tempString = temp;
		vector<string> tempVec = Parse(tempString, '|');

		if (isFullMatch)
		{
			if (searchTarget.size() != tempVec.size())
			{
				cout << "갯수가 일치하지 않습니다." << endl;
				return -1;
			}
		}

		for (int i = 0; i < count; i++)
		{
			if (count > (int)tempVec.size()) // tempVec 범위 벗어나는것 방지
				break;
			int compareResult = searchTarget[i].compare(tempVec[i]);
			if (0 == compareResult)
			{
				sameAmount++;
			}
			else
			{
				break;
			}
		}
		if (sameAmount == count)
		{
			break;
		}
	}
	va_end(Marker);
	inFile.close();
	if (sameAmount == count)
	{
		return line;
	}
	return -1;
}

vector<string> CReadHandler::Parse(const string & str, const char & ch)
{
	string next;
	vector<string> result;

	for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == ch)
		{
			if (!next.empty())
			{
				result.push_back(next);
				next.clear();
			}
		}
		else
		{
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}
