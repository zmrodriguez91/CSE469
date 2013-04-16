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
	part_type[0x04] = "DOS 16-bit FAT ( < 32MB )";
	part_type[0x05] = "Extended Partition";
	part_type[0x06] = "DOS 16-bit FAT ( > 32MB )";
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
	temp = (temp | (((unsigned int) bytes[8]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[9]) << 8)) & 0x0000FFFF;
	temp = (temp | (((unsigned int) bytes[10]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((unsigned int) bytes[11]) << 24)) & 0xFFFFFFFF;
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

//VBR Entry
struct VBR
{
	char bytes[40];
	string OEM_name();
	unsigned int sector_size();
	unsigned int cluster_size();
	unsigned int reserved_sectors();
	unsigned int fat_count();
	unsigned int max_files();
	unsigned int fs_size_sectors16();
	unsigned int media_type();
	unsigned int FAT_size_sectors();
	unsigned int sectors_per_track();
	unsigned int head_count();
	unsigned int offset_sectors();
	unsigned int fs_size_sectors32();
	unsigned int FAT_size_sectors32();
};
string VBR::OEM_name(){
	string temp = "";
	for (int i = 3; i < 11; i++)
	{
		temp = temp + bytes[i];
	}
	return temp;
}
unsigned int VBR::sector_size()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[11]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[11]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::cluster_size()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[13]) << 0)) & 0x000000FF;
	temp = temp & 0x000000FF;
	return temp;
}
unsigned int VBR::reserved_sectors()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[14]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[15]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::fat_count()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[16]) << 0)) & 0x000000FF;
	return temp;
}
unsigned int VBR::max_files()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[17]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[18]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::fs_size_sectors16()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[19]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[20]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::media_type()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[21]) << 0)) & 0x000000FF;
	return temp;
}
unsigned int VBR::FAT_size_sectors()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[22]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[23]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::sectors_per_track()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[24]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[25]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::head_count()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[26]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[27]) << 8)) & 0x0000FFFF;
	return temp;
}
unsigned int VBR::offset_sectors()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[28]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[29]) << 8)) & 0x0000FFFF;
	temp = (temp | (((unsigned int) bytes[30]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((unsigned int) bytes[31]) << 24)) & 0xFFFFFFFF;
	return temp;
}
unsigned int VBR::fs_size_sectors32()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[32]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[33]) << 8)) & 0x0000FFFF;
	temp = (temp | (((unsigned int) bytes[34]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((unsigned int) bytes[35]) << 24)) & 0xFFFFFFFF;
	return temp;
}
unsigned int VBR::FAT_size_sectors32()
{
	unsigned int temp = 0x00000000;
	temp = (temp | (((unsigned int) bytes[36]) << 0)) & 0x000000FF;
	temp = (temp | (((unsigned int) bytes[37]) << 8)) & 0x0000FFFF;
	temp = (temp | (((unsigned int) bytes[38]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((unsigned int) bytes[39]) << 24)) & 0xFFFFFFFF;
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
		cout << "\t(" << hex << (0x000000FF & (unsigned int) MBR_CONTENTS[i].bytes[4]) << dec << ") ";
		cout << MBR_CONTENTS[i].type() <<",\t" << MBR_CONTENTS[i].mbr_offset() << ",\t" << MBR_CONTENTS[i].size();
		cout << endl;
	}

	cout << endl << "VBR Analysis:" << endl;
	for (int i = 0; i < 4; i++){
		if (
			MBR_CONTENTS[i].type() == part_type[0x01] ||
			MBR_CONTENTS[i].type() == part_type[0x04] ||
			MBR_CONTENTS[i].type() == part_type[0x06] ||
			MBR_CONTENTS[i].type() == part_type[0x0B] ||
			MBR_CONTENTS[i].type() == part_type[0x0C] ||
			MBR_CONTENTS[i].type() == part_type[0x1B] ||
			MBR_CONTENTS[i].type() == part_type[0x86])
		{
			input_file.seekg((MBR_CONTENTS[i].mbr_offset()) * 512);			
			VBR vbr;
			for (int j = 0; j < 40; j++) 
			{
				vbr.bytes[j] = input_file.get();
				//cout << hex << (0x000000FF & ((unsigned int) vbr.bytes[j])) << dec << " ";
			}
			
			cout << "Partition " << i << " ("<< MBR_CONTENTS[i].type() << "):" << endl;
			cout << "\tReserved Area:" << endl;
			cout << "\t\tStart Sector:\t0" << endl;
			cout << "\t\tEnding Sector:\t" << (vbr.reserved_sectors() - 1) << endl;
			cout << "\t\tSize:\t" << vbr.reserved_sectors() << " sectors" << endl;
			cout << "\tSectors per cluster: " << vbr.cluster_size() << " sectors" << endl;
			cout << "\tFat area:" << endl;
			cout << "\t\tStart Sector:\t" << vbr.reserved_sectors() << endl;
			cout << "\t\tEnding Sector:\t" <<(vbr.reserved_sectors() -1 + vbr.fat_count()*((vbr.FAT_size_sectors() == 0)? vbr.FAT_size_sectors32() : vbr.FAT_size_sectors()) )<< endl;
			cout << "\t# of FATs: " << vbr.fat_count() << endl;
			cout << "\tThe size of each FAT: " << ((vbr.FAT_size_sectors() == 0)? vbr.FAT_size_sectors32() : vbr.FAT_size_sectors()) << " sectors" << endl;			
		}
	}
	//Close file
	input_file.close();
	return 0;
}