#include "AAA.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

void usage();
void abort();

MBR_ENTRY MBR[4];
string input_filename = "";
string output_filename = "";
fstream ifs;
fstream ofs;
ostream *os;
int main(int argc, char** argv)
{
	os = &cout;
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-f"){
			i++;			
			input_filename = argv[i];
			ifs.open(input_filename, ios::in | ios::binary);
		}
		else if (arg == "-o"){
			i++;			
			output_filename = argv[i];
			ofs.open(output_filename, ios::out);
			os = &ofs;
		}
	}
	if (!ifs.is_open()) usage();	
	//Check MBR signature
	ifs.seekg(0x01FE);
	if (ifs.get() != 0x55) abort();
	if (ifs.get() != 0xAA) abort();

	//Load MBR Entries
	ifs.seekg(0x01BE);
	for (int i = 0; i < 4; i ++)
	{
		for (int j = 0; j < 16; j++)
		{
			MBR[i].contents[j] = ifs.get();
		}
	}
	//Print MBR Entries
	(*os) << "MBR Analysis:" << endl;
	for (int i = 0; i < 4;i++)
	{
		(*os) << "\t(" << hex << (int) MBR[i].contents[4] << dec << ") "<< MBR[i].type() << endl;
		(*os) << "\t\t" << MBR[i].offset() << ",\t";
		(*os) << "\t\t" << MBR[i].size() << endl;
	}
	//Load/Print VBR
	(*os) << "VBR Analysis:" << endl;
	for (int i = 0; i < 4; i++)
	{
		if (
			MBR[i].contents[4] != 0x01 &&
			MBR[i].contents[4] != 0x04 &&
			MBR[i].contents[4] != 0x06 &&
			MBR[i].contents[4] != 0x0B &&
			MBR[i].contents[4] != 0x0C &&
			MBR[i].contents[4] != 0x01 &&
			MBR[i].contents[4] != 0x1B &&
			MBR[i].contents[4] != 0x86 
			) continue;
		VBR vbr;
		//Load VBR
		ifs.seekg(MBR[i].offset() * 512);
		for (int j = 0; j < 40; j++) vbr.contents[j] = ifs.get();
		//Print VBR
		//(*os) << vbr.OEM_NAME() << endl;
		(*os) << "Partition " << i;
		(*os) << " (" << MBR[i].type() << "):" << endl;
		(*os) << "\tReserved Area:" << endl;
		(*os) << "\t\tStart Sector:\t" << 0 << endl;
		(*os) << "\t\tEnd Sector:\t" << (vbr.reserved_sectors() - 1) << endl;
		(*os) << "\t\tSize:\t\t" << vbr.reserved_sectors() << endl;
		(*os) << "\tSectors per cluster:\t" << (int) vbr.cluster_size() << endl;
		int fatSize = ((vbr.FAT_size16() == 0)? vbr.FAT_size32():vbr.FAT_size16());
		(*os) << "\tFAT Area:" << endl;
		(*os) << "\t\tStart Sector:\t" << vbr.reserved_sectors() << endl;
		(*os) << "\t\tEnd Sector:\t" << (vbr.reserved_sectors() + fatSize * vbr.FATs() - 1) << endl;
		(*os) << "\t\t# of FATs:\t" << (int) vbr.FATs() << endl; 
		(*os) << "\tSize of each FAT:\t" << fatSize << endl;

		int cluster2 = MBR[i].offset() + vbr.reserved_sectors() + (vbr.FATs() * fatSize);
		switch(MBR[i].contents[4])
		{
		case 0x01://12
			cluster2 += vbr.root_size() * 12;
			break;
		case 0x04://16
		case 0x06://16
		case 0x86://16
			cluster2 += 32;
			break;
		case 0x0B://32
		case 0x0C://32
		case 0x1B://32
			cluster2 += vbr.root_size() * 32;
			break;
		default:
			break;
		}
		(*os) << "\tCluster 2 start:\t" << cluster2 << endl;
		//(*os) << MBR[i].offset() << " " << vbr.offset() << endl;
	}
	//Clean Up
	(*os) << "Done." << endl;
	ifs.close();
	if (ofs.is_open()) ofs.close();
	return 0;
}
void usage()
{
	(*os) << "This is how you use it!" << endl;
	if (ifs.is_open())ifs.close();
	if (ofs.is_open())ofs.close();
	exit(EXIT_FAILURE);
	return;
}
void abort()
{
	(*os) << "Bad image. Analysis aborted." << endl;
	if (ifs.is_open()) ifs.close();
	if (ofs.is_open()) ofs.close();
	exit(EXIT_FAILURE);
	return;
}