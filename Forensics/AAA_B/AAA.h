#include <string>
using namespace std;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int double_word;
struct MBR_ENTRY
{
	byte contents[16];
	bool isActive();
	byte start_head();
	byte start_cylinder();
	byte start_sector();
	string type();
	byte end_head();
	byte end_cylinder();
	byte end_sector();
	double_word offset();
	double_word size();
};

struct VBR
{
	byte contents[40];

	string OEM_NAME();
	word sector_size();
	byte cluster_size();
	word reserved_sectors();
	byte FATs();
	word root_size();
	word fs_size16();
	byte media_type();
	word FAT_size16();
	word sectors_per_track();
	word head_count();
	double_word offset();
	double_word fs_size32();
	double_word FAT_size32();
};