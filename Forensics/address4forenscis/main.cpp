#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

typedef enum AddressType {invalid, logical, physical, cluster};
AddressType sourceType = invalid;
AddressType destinationType = invalid;
/* */
long logicalKnown = 0;
long physicalKnown = 0;
long clusterKnown = 0;
long partitionStart = 0;

/* */
long sectorSize = 512;
bool byteAddress = false;

/*  */
long clusterSize = 0;	//Default n sectors per cluster
long reserved = 0;		//Default n reserved sectors
long fatTables = 2;		//Default two FAT tables
long fatLength = 0;		//Default fat length

void usage(){
	cout << "Usage:\n" << "address4forensics -L|-P|-C -b offset [-B [-s bytes]] [-l address] [-p address] [-c address -k sectors -r sectors -t tables -f sectors]";
	return;
}

long parselong(string str) {
	long val = 0;
	for (int i = 0; i < str.length(); i++){
		val *= 10;
		if (str[i] >= '0' && str[i] <= '9'){
			val += (long)(str[i] - '0');
		}
		else
		{
			usage();
			exit(EXIT_FAILURE);
		}
	}	
	return val;
}

int main(int argc, char **argv) {
	for (int i=0; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-L"|| arg == "--logical") {
			destinationType = logical;
			continue;
		}
		if (arg == "-P"|| arg == "--physical") {
			destinationType = physical;
			continue;
		}
		if (arg == "-C"|| arg == "--cluster") {
			destinationType = cluster;
			continue;
		}
		if (arg == "-b") {
			i++;
			arg = argv[i];
			partitionStart = parselong(arg);
			continue;
		}
		if (arg == "-B" || arg == "--byte-address") {
			byteAddress = true;
			continue;
		}
		if (arg == "-s") {
			i++;
			arg = argv[i];
			sectorSize = parselong(arg);
			continue;
		}
		if (arg == "-l") {
			i++;
			arg = argv[i];
			logicalKnown = parselong(arg);
			sourceType = logical;
			continue;
		}
		if (arg == "-p") {
			i++;
			arg = argv[i];
			physicalKnown = parselong(arg);
			sourceType = physical;
			continue;
		}
		if (arg == "-c") {
			i++;
			arg = argv[i];
			clusterKnown = parselong(arg);
			sourceType = cluster;
			continue;
		}
		if (arg == "-k") {
			i++;
			arg = argv[i];
			clusterSize = parselong(arg);
			continue;
		}
		if (arg == "-r") {
			i++;
			arg = argv[i];
			reserved = parselong(arg);
			continue;
		}
		if (arg == "-t") {
			i++;
			arg = argv[i];
			fatTables = parselong(arg);
			continue;
		}
		if (arg == "-f") {
			i++;
			arg = argv[i];
			fatLength = parselong(arg);
			continue;
		}
	}
	
	long address = 0;
	switch (sourceType){
		case invalid:
			usage();
			exit(EXIT_FAILURE);
			break;
		case logical:
			//Convert logical address to physical address
			address = (logicalKnown + partitionStart);			
			break;
		case physical:
			//DO NOTHING
			address = physicalKnown;					
			break;
		case cluster:
			//Convert cluster address to physical address
			address = ((clusterKnown - 2) * clusterSize) + (fatTables * fatLength) + partitionStart + reserved;
			break;
		default:
			cout << "!" << endl;
			break;
	}
	
	switch (destinationType){
		case invalid:
			usage();
			exit(EXIT_FAILURE);
			break;
		case logical:
			address -= partitionStart;
			break;
		case physical:
			//DO NOTHING
			break;
		case cluster:		
			address = (address - partitionStart - (reserved + fatTables * fatLength + 2) / clusterSize);
			break;
		default:
			cout << "!" << endl;
			break;
	}
	
	address *= ((byteAddress)?sectorSize:1);
	
	cout << address << endl;
	return 0;
}
