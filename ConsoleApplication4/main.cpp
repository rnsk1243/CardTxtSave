#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"ReadHandler.h"
#include"WriteHandler.h"
#include<stdio.h>

int func2(char* souce, string& searchCardNum, bool& isNewCard, int& cardAmount);
void func4(int num, char* chResult);
const int BufSize = 1024;
//수정할유저가 있는 위치까지 커서 이동해야할 크기 반환(text이름, 해당 유저가 기록된 줄 수, 유저가 기록된 줄을 저장할 공간)
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
	// 해당 아이디 라인을 '|'을 기준으로 자름
	vector<string> tempVec = ReadHandlerStatic->Parse(targetLine, '|');

	vector<string>::iterator iterBegin = tempVec.begin();
	vector<string>::iterator iterEnd = tempVec.end();
	// 맨 앞 id 부분을 더함  
	offset += (strlen((*iterBegin).c_str()) + 1/*'|'때문에 1을 더함*/);
	++iterBegin; // 아이디 부분 넘김
	for (; iterBegin != iterEnd; ++iterBegin) // 카드 부분
	{
		// (카드번호/카드갯수)을 '/'을 기준으로 자름
		vector<string> tempVec2 = ReadHandlerStatic->Parse((*iterBegin), '/');
		vector<string>::iterator iterBegin2 = tempVec2.begin(); // 카드 번호 부분
		// 카드 번호와 찾으려는 카드 일치 여부 확인
		int compareResult = (*iterBegin2).compare(searchCardNum);
		if (0 == compareResult)
		{	// 일치하면 offset리턴(가지고 있던 카드임)
			isNewCard = false;
			++iterBegin2; // 카드 갯수 부분
			cardAmount = atoi((*iterBegin2).c_str());
			return offset;
		}
		// 일치 하지 않으면 "카드번호/카드갯수|" <-이 만큼을 offset에 더함
		offset += (strlen((*iterBegin).c_str()) + 1);
	}
	// 신규 카드
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
	//{// 새로운 카드
	//	output.seekp(-7, ios::cur); // "xxxxxx" 부분 만큼 뒤로(수정될 부분)
	//	string nextSpace = "xxxxxx"; // 다음 새로운 카드 공간 확보용
	//	result += nextSpace;
	//	const char* doWrite = result.c_str();
	//	output.write(doWrite, strlen(doWrite));
	//}
	//else
	//{// 새로운 카드가 아님
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
	cardAmount++; // 카드 갯수 증가
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
	//	cout << "파일을 열기 실패" << endl;
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
	//	//	cout << "같다" << endl;
	//		input.seekg(1, ios::cur);
	//		count++;
	//	}
	//	else
	//	{
	//	//	cout << "다르다" << endl;
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
	cout << "결과 = " << result.c_str() << endl;*/

	char* userName = "jbc";
	int cardNum = 12;
	WriteCard("test.txt", userName, cardNum);
}