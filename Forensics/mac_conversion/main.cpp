#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

void usage(){
	cout << "This is how you do it!";
	exit(EXIT_FAILURE);
	return;
}

 int parseHex(string str){
	int data = 0;
	if (str[0] != '0') usage();
	if (str[1] != 'x') usage();
	for (int i = 2; i < str.length(); i ++)
	{
		data *= 16;
		if (str[i] >= 'A' && str[i] <= 'F')
		{
			data += 10 + ( int)(str[i] - 'A');
		}
		else if (str[i] >='a' && str[i] <= 'f')
		{
			data += 10 + ( int)(str[i] - 'a');
		}
		else if (str[i] >='0' && str[i] <= '9')
		{
			data += ( int)(str[i] - '0');
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

	int data = 0x00000000;
	if (filename_string != "")
	{

	}
	else if (hex_string != "")
	{
		data = parseHex(hex_string);
		data = ((data & 0xFF00FF00) >> 8)|((data & 0x00FF00FF) << 8);
	}
	cout << hex << data << dec << endl;

	switch (operation){
		case Time:
			int Hour, Minute, Second;
			Hour = (data & 0x0000F800) >> 11;
			Minute = (data & 0x000007F0) >> 5;
			Second = (data & 0x0000001F) >> 0;
			cout << "Time: " << Hour << ":" << Minute << ":" << Second << " ";
			cout << ((Hour > 12)? "PM" : "AM") << endl;
			break;
		case Date:
			int Year, Month, Day;
			Year = (data & 0x0000FE00) >> 9;
			Month = (data & 0X000001E0) >> 5;
			Day = (data & 0x0000001F) >> 0;
			cout << "Date: ";
			switch (Month){
				case 1:
					cout << "January";
					break;
				case 2:
					cout << "February";
					break;
				case 3:
					cout << "March";
					break;
				case 4:
					cout << "April";
					break;
				case 5:
					cout << "May";
					break;
				case 6:
					cout << "June";
					break;
				case 7:
					cout << "July";
					break;
				case 8:
					cout << "August";
					break;
				case 9:
					cout << "September";
					break;
				case 10:
					cout << "October";
					break;
				case 11:
					cout << "November";
					break;
				case 12:
					cout << "December";
					break;
				default:
					cout << "Invalid Date!" << endl;
					exit(EXIT_FAILURE);
					break;
			}
			cout << " " << Day << ", " << Year << endl;
			break;
		default:
			usage();
	}
	
	return 0;
}