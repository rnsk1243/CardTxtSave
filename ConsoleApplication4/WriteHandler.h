#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
#include<stdio.h>
using namespace std;

class CWriteHandler
{
	CWriteHandler();
public:
	static CWriteHandler* getInstance();
	CWriteHandler(const CWriteHandler&) = delete;
	CWriteHandler& operator=(const CWriteHandler&) = delete;
	~CWriteHandler();
	// ���ϸ�, �, ��Ʈ��
	bool write(const char* textFileName, int count, ...);
};
static CWriteHandler* WriteHandlerStatic = CWriteHandler::getInstance();

