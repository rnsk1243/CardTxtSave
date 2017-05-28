#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"ReadHandler.h"
#include"WriteHandler.h"
#include<stdio.h>

int func2(char* souce, string& searchCardNum, bool& isNewCard, int& cardAmount);
void func4(int num, char* chResult);
const int BufSize = 1024;
//������������ �ִ� ��ġ���� Ŀ�� �̵��ؾ��� ũ�� ��ȯ(text�̸�, �ش� ������ ��ϵ� �� ��, ������ ��ϵ� ���� ������ ����)
int BeginToTargetUserCurserMoveSize(char* textName, int targetUserLineNum, char* targetLine)
{
	ifstream input(textName);
	input.seekg(0, ios::beg);
	char checkLine;
	int offset = 0;
	int line = 1;
	while (true)
	{
		input.seekg(1, ios::cur);
		offset++;
		checkLine = (char)input.peek();
		if (checkLine == '\n')
		{
			input.seekg(1, ios::cur);
			line++;
			offset++;
		}
		if (targetUserLineNum == line)
		{
			input.seekg(1, ios::cur);
			offset++;
			break;
		}
	}
	input.getline(targetLine, BufSize);
	input.close();

	return offset;
}

int func2(char* souce, string& searchCardNum, bool& isNewCard, int& cardAmount)
{
	int offset = 0;
	string targetLine = souce;
	// �ش� ���̵� ������ '|'�� �������� �ڸ�
	vector<string> tempVec = ReadHandlerStatic->Parse(targetLine, '|');

	vector<string>::iterator iterBegin = tempVec.begin();
	vector<string>::iterator iterEnd = tempVec.end();
	// �� �� id �κ��� ����  
	offset += (strlen((*iterBegin).c_str()) + 1/*'|'������ 1�� ����*/);
	++iterBegin; // ���̵� �κ� �ѱ�
	for (; iterBegin != iterEnd; ++iterBegin) // ī�� �κ�
	{
		// (ī���ȣ/ī�尹��)�� '/'�� �������� �ڸ�
		vector<string> tempVec2 = ReadHandlerStatic->Parse((*iterBegin), '/');
		vector<string>::iterator iterBegin2 = tempVec2.begin(); // ī�� ��ȣ �κ�
		// ī�� ��ȣ�� ã������ ī�� ��ġ ���� Ȯ��
		int compareResult = (*iterBegin2).compare(searchCardNum);
		if (0 == compareResult)
		{	// ��ġ�ϸ� offset����(������ �ִ� ī����)
			isNewCard = false;
			++iterBegin2; // ī�� ���� �κ�
			cardAmount = atoi((*iterBegin2).c_str());
			return offset;
		}
		// ��ġ ���� ������ "ī���ȣ/ī�尹��|" <-�� ��ŭ�� offset�� ����
		offset += (strlen((*iterBegin).c_str()) + 1);
	}
	// �ű� ī��
	isNewCard = true;
	cardAmount = 0;
	return offset;
}


void func3(char* text, int offset, int cardNum, int cardAmount, bool isNewCard)
{
	char chCardNum[3];
	func4(cardNum, chCardNum);
	char chCardAmount[3];
	func4(cardAmount, chCardAmount);
	 
	string strCardNum(chCardNum);
	string strCardAmount(chCardAmount);
	string result = strCardNum + '/' + strCardAmount + '|';

	ofstream output(text, ios::out | ios::in);

	output.seekp(offset, ios::beg);
	
	const char* doWrite = result.c_str();
	output.write(doWrite, strlen(doWrite));

	//if (isNewCard)
	//{// ���ο� ī��
	//	output.seekp(-7, ios::cur); // "xxxxxx" �κ� ��ŭ �ڷ�(������ �κ�)
	//	string nextSpace = "xxxxxx"; // ���� ���ο� ī�� ���� Ȯ����
	//	result += nextSpace;
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
	//else
	//{// ���ο� ī�尡 �ƴ�
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
	output.close();
}

void func4(int num, char* chResult)
{
	if (10 > num)
	{
		chResult[0] = '0';
		char temp[10];
		_itoa(num, temp, 10);
		chResult[1] = temp[0];
		chResult[2] = '\0';
		cout << chResult << endl;
	}
	else
	{
		_itoa(num, chResult, 10);
	}
}

void WriteCard(char* textName, char* userName, int cardNum)
{
	char targetLine[BufSize];
	char chCardNum[3];
	bool isNewCard = false;
	int cardAmount = 0;
	string strUserName = userName;
	int line = ReadHandlerStatic->Search(textName, false, 1, strUserName);
	int moveSize1 = BeginToTargetUserCurserMoveSize(textName, line, targetLine);

	func4(cardNum, chCardNum);
	string strCardNum = chCardNum;
	int movesize2 = func2(targetLine, strCardNum, isNewCard, cardAmount);

	int moveCurserSize = moveSize1 + movesize2;
	cardAmount++; // ī�� ���� ����
	func3(textName, moveCurserSize, cardNum, cardAmount, isNewCard);

}

void main()
{
//	string s1 = "bbq";
//	cout << ReadHandlerStatic->Search("test.txt", true, 1, s1) << endl;

	//FILE* fp = nullptr;
	//fp = fopen("test.txt", "a");
	//if (nullptr == fp)
	//{
	//	cout << "null file" << endl;
	//}


	//cout << fseek(fp, 100L, SEEK_SET) << endl;
	////rewind(fp);
	//int size = ftell(fp);
	//cout << size << endl;
	//char ch1[10] = "8";
	//char ch2[10] = "|";
	//fprintf_s(fp, "%s%s", ch1, ch2);

	//fclose(fp);

	//ifstream input("test.txt");
	//if (input.fail())
	//{
	//	cout << "������ ���� ����" << endl;
	//	return;
	//}

	//input.seekg(-1, ios::end);
	//cout << "1 " << (char)input.peek() << endl;
	//input.seekg(0, ios::beg);
	//cout << "2 " << (char)input.peek() << endl;
	//input.seekg(7, ios::cur);
	//cout << "3 " << (char)input.peek() << endl;
	////cout << "4 " << (char)input.get() << endl;
	//input.seekg(-3, ios::end);
	//cout << "5 " << (char)input.peek() << endl;
	////cout << input.tellg() << endl;

	//input.close();

	//string s1 = "kfc";
	//cout << ReadHandlerStatic->Search("test.txt", true, 1, s1) << endl;

	//ofstream output("test.txt", ios::out|ios::in);
	//ifstream input("test.txt");
	//input.seekg(0, ios::beg);
	//char s1;
	//int count = 0;
	//while (true)
	//{
	//	input.seekg(1, ios::cur);
	//	s1 = (char)input.peek();
	//	//cout << "s1 = " << s1 << endl;
	//	if (s1 == '\n')
	//	{
	//	//	cout << "����" << endl;
	//		input.seekg(1, ios::cur);
	//		count++;
	//	}
	//	else
	//	{
	//	//	cout << "�ٸ���" << endl;
	//	}

	//	if (count == 2)
	//	{
	//		input.seekg(-1, ios::cur);
	//		s1 = (char)input.peek();
	//		break;
	//	}
	//}
	//cout << "s1 = " << s1 << endl;

	//

	//input.close();
	/*string s1 = "cucu";
	int line = ReadHandlerStatic->Search("test.txt", false, 1, s1);
	cout << "line = " << line << endl;
	func("test.txt", line, "05");*/

	//ofstream output("tt.txt", ios::out | ios::in);
	//output.seekp(0, ios::beg);
	//output.seekp(1, ios::cur);
	//output.seekp(1, ios::cur);
	//output.seekp(1, ios::cur);
	//output.seekp(1, ios::cur);
	//output.seekp(1, ios::cur);
	//output << "ii\0";
	//output.close();
	
	/*int cardNum = 88;
	char chCardNum[3];
	func4(cardNum, chCardNum);

	int cardAmount = 3;
	char chCardAmount[3];
	func4(cardAmount, chCardAmount);

	string strCardNum(chCardNum);
	string strCardAmount(chCardAmount);
	string result = strCardNum + '/' + strCardAmount + '|';
	cout << "��� = " << result.c_str() << endl;*/

	char* userName = "jbc";
	int cardNum = 12;
	WriteCard("test.txt", userName, cardNum);
}