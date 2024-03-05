#pragma once
#include<iostream>
#include<bitset>
#include<vector>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<windows.h>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <map>
#include <queue>
#include <chrono>

using namespace System;

class OP {
public:
	int freq;
	char ch;
	std::string codw;
};
class chunks {
public:
	std::string b8;
	char d8;
	int i8;
};
class Shanno{
public : 
	int actual;
	OP pi[127];
	Shanno() {}

chunks cnks[1000000];

void shan(int s, int e)
{
	if (s < e)
	{
		int sum_1 = 0, sum_2 = 0;
		int mind = 10000;
		int t = 0;
		int diff = 0;
		int pt;
		int i, j, k, cmp;

		for (cmp = 0; cmp < (e - s); cmp++)
		{

			for (k = s; k <= s + cmp; k++)
			{
				sum_1 = sum_1 + pi[k].freq;
			}
			for (j = s + cmp + 1; j <= e; j++)
			{
				sum_2 = sum_2 + pi[j].freq;
			}
			diff = abs(sum_2 - sum_1);
			if (diff <= mind)
			{
				mind = diff;
				pt = cmp + s;
			}
			sum_1 = 0;
			sum_2 = 0;
		}
		for (i = s; i <= pt; i++)
		{
			pi[i].codw = pi[i].codw + "1";
		}
		
		for (i = pt + 1; i <= e; i++)
		{
			pi[i].codw = pi[i].codw + "0";
		}
		int tmpp = pt;
		shan(tmpp + 1, e);
		shan(s, tmpp);
	}
}

void rdfile(std::string input)
{
	std::ifstream inFile;
	char ch;
	
	inFile.open(input);
	if (!inFile)
	{
		
		exit(0);
	}
	for (int i = 0; i < 127; i++)
	{
		
		pi[i].ch=char(i);
		pi[i].freq= 0;

	}
	ch = inFile.get();
	
	while (ch != EOF)
	{
		pi[int(ch)].freq++;
		ch = inFile.get();
	}

	int n;

	n = 127;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (pi[j].freq > pi[j + 1].freq)
			{
				OP tmp;
				tmp = pi[j];
				pi[j] = pi[j + 1];
				pi[j + 1] = tmp;
			}
		}
	}
	int h;
	for (int i = 0; i < n; i++)
	{
		if (pi[i].freq != 0)
		{
			h = i;
			break;
		}
	}
	std::ofstream outfile;
	outfile.open("headkeliye.txt");
	outfile << h<<" "<<actual;

	shan(h, n - 1);

	std::ofstream codfile;//codeword file
	codfile.open("codfile.txt");

	for (int i = h; i < n; i++)
	{
		codfile << pi[i].ch<< pi[i].freq<< pi[i].codw << std::endl;
	}
	codfile.close();
	wtfile(h, input);
	dcp();
	fina();
	las(h);
}

void wtfile(int h, std::string input)
{
	std::ifstream inFile;
	std::ofstream outfile;
	outfile.open("1_Codeword.txt");
	
	char ch;
	
	inFile.open(input);
	if (!inFile)
	{
		exit(0);
	}

	ch = inFile.get();
	int i;
	while (ch != EOF)
	{
		i = h;
		while (pi[i].ch != ch)
		{
			i++;
		}
		outfile << pi[i].codw;
		ch = inFile.get();
	}
	outfile.close();
}

int binTOdec(std::string binaryString)
{
	int value = 0;
	int indexCounter = 0;
	for (int i = binaryString.length() - 1; i >= 0; i--)
	{
		if (binaryString[i] == '1') {
			value += pow(2, indexCounter);
		}
		indexCounter++;
	}
	return value;
}


void dcp()
{
	
	std::ifstream inFile;
	std::ofstream outfile;
	outfile.open("2_compressed.xipf");
	char ch;

	inFile.open("1_codeword.txt");
	if (!inFile)
	{
		exit(0);
	}
	ch = inFile.get();
	int i = 0, lmt = 0;
	while (ch != EOF)
	{
		cnks[i].b8 = cnks[i].b8 + ch;
		while (cnks[i].b8.length() != 8)
		{
			ch = inFile.get();
			if (ch == EOF)
			{
				lmt++;
				ch = '0';
			}
			cnks[i].b8 = cnks[i].b8 + ch;
		}
		cnks[i].i8 = binTOdec(cnks[i].b8);
		cnks[i].d8 = char(cnks[i].i8);
		outfile << cnks[i].d8;
		ch = inFile.get();
		i++;
	}
	actual = (i * 8) - lmt;
	outfile.close();
}



void fina()
{
	std::ifstream inFile;   // input file
	std::ofstream outfile;	//output file

	outfile.open("3_BinaryOfCompressed.txt");
	
	char ch;
	inFile.open("2_compressed.xipf");
	if (!inFile)
	{
		exit(0);
	}
	ch = inFile.get();
	int i = 0;
	while (ch != EOF)
	{
		while (ch != EOF)
		{
			std::bitset<8> bin_x(ch);
			outfile << bin_x;
			ch = inFile.get();
		}

		ch = inFile.get();
	}

	outfile.close();
}

void las(int h)
{
	std::ifstream inFile;
	std::ofstream outfile;

	//inFile.open("3_BinaryOfCompressed.txt");
	inFile.open("1_Codeword.txt");
	outfile.open("4_Finalizedfile.txt");
	if (!inFile)
	{
		exit(0);
	}
	char ch;
	ch = inFile.get();
	std::string str = "";
	for (int i = 0; i < actual; i++)
	{

		str = str + ch;
		for (int k = 120; k < 127; k++)
		{
			if (pi[k].codw == str)
			{
				outfile << pi[k].ch;
				str = "";
			}
		}
		ch = inFile.get();
	}
	outfile.close();
}
};


void CompressShanno(System::String^ managedString,int choice)
{
    msclr::interop::marshal_context context;
    std::string input = context.marshal_as<std::string>(managedString);
	

	Shanno *shanno=new Shanno();

	if (choice == 1) {
		shanno->rdfile(input);
	}
	else {
		
	}
	return;
	//read file -> input file
	
	
	

}