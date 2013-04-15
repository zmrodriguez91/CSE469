#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

//Partition Type Mapping
map<char, string> part_type;
void initMap()
{
	part_type[0x01] = "DOS 12-bit FAT";
	part_type[0x04] = "DOS 16-bit FAT (<32MB)";
	part_type[0x05] = "Extended Partition";
	part_type[0x06] = "DOS 16-bit FAT (>32MB)";
	part_type[0x07] = "NTFS";
	part_type[0x08] = "AIX bootable partition";
	part_type[0x09] = "AIX data partition";
	part_type[0x0B] = "DOS 32-bit FAT";
	part_type[0x0C] = "DOS 32-bit FAT (Interupt 13 Support)";
	part_type[0x17] = "Hidden NTFS (XP and Earlier)";
	part_type[0x1B] = "Hidden FAT32";
	part_type[0x1E] = "Hidden VFAT";
	part_type[0x3C] = "Partition Magic Recovery";
	part_type[0x66] = "Novell Partition";
	part_type[0x67] = "Novell Partition";
	part_type[0x68] = "Novell Partition";
	part_type[0x69] = "Novell Partition";
	part_type[0x81] = "Linux";
	part_type[0x82] = "Linux Swap Partition or Solaris Partition";
	part_type[0x83] = "Linux Native FS (Ext2, Ext3, Resiser, xiafs)";
	part_type[0x86] = "FAT 16 volume/stripe set (Windows NT)";
	part_type[0x87] = "HPFS Fault tolerant mirrored partition or NTFS volume/stripe set";
	part_type[0xA5] = "FreeBSD and BSD/386";
	part_type[0xA6] = "OpenBSD";
	part_type[0xA9] = "NetBSD";
	part_type[0xC7] = "Typical of corrupted NTFS volume/stripe set";
	part_type[0xEB] = "BeOS";






	return;
}

//MBR Entry
struct MBR_ENTRY
{
	char bytes[16];
	bool isActive();
	int begin_head();
	int begin_cylinder();
	int begin_sector();
	string type();
	int end_head();
	int end_cylinder();
	int end_sector();
	unsigned int mbr_offset();
	unsigned int size();
};
bool MBR_ENTRY::isActive()
{
	return ((bytes[0] == 0x80)? true:false);
}
int MBR_ENTRY::begin_head()
{
	return (int) bytes[1];
}
int MBR_ENTRY::begin_cylinder()
{
	return (int) bytes[2];
}
int MBR_ENTRY::begin_sector()
{
	return (int) bytes[3];
}
string MBR_ENTRY::type()
{
	return part_type[bytes[4]];
}
int MBR_ENTRY::end_head()
{
	return (int) bytes[5];
}
int MBR_ENTRY::end_cylinder()
{
	return (int) bytes[6];
}
int MBR_ENTRY::end_sector()
{
	return (int) bytes[7];
}
unsigned int MBR_ENTRY::mbr_offset()
{
	unsigned int temp = 0x00000000;
	temp = temp | ((0x000000FF & (unsigned int)  bytes[8]) << 0);
	temp = temp | ((0x000000FF & (unsigned int)  bytes[9]) << 8);
	temp = temp | ((0x000000FF & (unsigned int) bytes[10]) << 16);
	temp = temp | ((0x000000FF & (unsigned int) bytes[11]) << 24);
	return temp;
}
unsigned int MBR_ENTRY::size()
{
	unsigned int temp = 0x00000000;
	temp = temp | ((0x000000FF & (unsigned int)  bytes[12]) << 0);
	temp = temp | ((0x000000FF & (unsigned int)  bytes[13]) << 8);
	temp = temp | ((0x000000FF & (unsigned int) bytes[14]) << 16);
	temp = temp | ((0x000000FF & (unsigned int) bytes[15]) << 24);
	return temp;
}

//Usage
void usage()
{
	cout << "This is how you use it!" << endl;
	exit(EXIT_FAILURE);
	return;
}

//Globals
string filename = "";
MBR_ENTRY MBR_CONTENTS[4];

//Main
int main(int argc, char** argv)
{

	//Create Mapping
	initMap();
	//Read Inputs
	for (int i = 1 ; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-f")
		{
			filename = argv[i + 1];
			i++;
		}
	}
	//Verify filename was input
	if (filename == "") usage();
	
	//Open file
	fstream input_file;
	input_file.open(filename,ios::binary|ios::in);
	//Verify MBR signature
	char SIG[2];
	input_file.seekg(0x01FE);
	SIG[0] = input_file.get();
	SIG[1] = input_file.get();
	if (SIG[0] != 0x55 && SIG[1] != 0xAA)
	{
		input_file.close();
		cout << "Invalid MBR signature." << endl;
		exit(EXIT_FAILURE);
	}
	//Read MBR Entries
	input_file.seekg(0x01BE);
	cout << "MBR Analysis:" << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			MBR_CONTENTS[i].bytes[j] = input_file.get();
		}
		cout << dec;
		cout << MBR_CONTENTS[i].type() <<",\t" << MBR_CONTENTS[i].mbr_offset() << ",\t" << MBR_CONTENTS[i].size();
		cout << endl;
	}
	//Close file
	input_file.close();
	return 0;
}