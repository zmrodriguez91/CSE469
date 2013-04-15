#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

void usage(){
	cout << "This is how you do it!";
	exit(EXIT_FAILURE);
	return;
}

long parseHex(string str){
	long data = 0;
	if (str[0] != '0') usage();
	if (str[1] != 'x') usage();
	for (int i = 2; i < str.length(); i ++)
	{
		data *= 16;
		if (str[i] >= 'A' && str[i] <= 'F')
		{
			data += 10 + (long)(str[i] - 'A');
		}
		else if (str[i] >='a' && str[i] <= 'f')
		{
			data += 10 + (long)(str[i] - 'a');
		}
		else if (str[i] >='0' && str[i] <= '9')
		{
			data += (long)(str[i] - '0');
		}
		else usage();
	}
	return data;
}

enum Time_Date {Invalid, Time, Date};
string filename_string = "";
string hex_string = "";
Time_Date operation = Invalid;

int main (int argc, char** argv){
	cout << argc << "\n";
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-T")
		{	
			if (operation == Invalid) operation = Time;
			else usage();
		}
		if (arg == "-D")
		{
			if (operation == Invalid) operation = Date;
			else usage();			
		}
		if (arg == "-f")
		{
			filename_string = argv[i + 1];
		}
		if (arg == "-h")
		{
			hex_string = argv[i + 1];
		}		
	}

	long data = 0;
	if (filename_string != "")
	{

	}
	else if (hex_string != "")
	{
		data = parseHex(hex_string);
	}
	cout << data << endl;

	switch (operation){
		case Time:
			cout << "TIME";
			break;
		case Date:
			cout << "DATE";
			break;
		default:
			usage();
	}
	
	return 0;
}