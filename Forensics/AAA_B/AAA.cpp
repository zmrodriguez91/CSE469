#include "AAA.h"
bool MBR_ENTRY::isActive()
{
	return ((contents[0] == 0x80)? true : false);
}
byte MBR_ENTRY::start_head()
{
	return contents[1];
}
byte MBR_ENTRY::start_cylinder()
{
	return contents[2];
}
byte MBR_ENTRY::start_sector()
{
	return contents[3];
}
string MBR_ENTRY::type()
{

	switch (contents[4])
	{
	case 0x01:
		return "DOS 12-bit FAT";
	case 0x04:
		return "DOS 16-bit FAT for partitions smaller than 32 MB";
	case 0x05:
		return "Extended partition";
	case 0x06:
		return "DOS 16-bit FAT for partitions larger than 32 MB";
	case 0x07:
		return "NTFS";
	case 0x08:
		return "AIX bootable partition";
	case 0x09:
		return "AIX data partition";
	case 0x0B:
		return "DOS 32-bit FAT";
	case 0x0C:
		return "DOS 32-bit FAT for interrupt 13 support";
	case 0x17:
		return "Hidden NTFS partition (XP and earlier)";
	case 0x1B:
		return "Hidden FAT32 partition";
	case 0x1E:
		return "Hidden VFAT partition";
	case 0x3C:
		return "Partition Magic recovery partition";
	case 0x66:		
	case 0x67:		
	case 0x68:		
	case 0x69:
		return "Novell Partitions";
	case 0x81:
		return "Linux";
	case 0x82:
		return "Linux swap partition or Solaris partition";
	case 0x83:
		return "Linux native file system (Ext2, Ext3, Reiser, xiafs)";
	case 0x86:
		return "FAT16 volume/stripe set (Windows NT)";
	case 0x87:
		return "HPFS fault-tolerant mirrored partition or NTFS volume/stripe set";
	case 0xA5:
		return "FreeBSD and BSD/386";
	case 0xA6:
		return "OpenBSD";
	case 0xA9:
		return "NetBSD";
	case 0xC7:
		return "Typical of a corrupted NTFS volume/stripe set";
	case 0xEB:
		return "BeOS";
	default:
		return "Unknown";
	}
	
}
byte MBR_ENTRY::end_head()
{
	return contents[5];
}
byte MBR_ENTRY::end_cylinder()
{
	return contents[6];
}
byte MBR_ENTRY::end_sector()
{
	return contents[7];
}
double_word MBR_ENTRY::offset()
{
	double_word temp = 0x00000000;
	temp = (temp | (((double_word) contents[8]) << 0)) & 0x000000FF;
	temp = (temp | (((double_word) contents[9]) << 8)) & 0x0000FFFF;
	temp = (temp | (((double_word) contents[10]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((double_word) contents[11]) << 24)) & 0xFFFFFFFF;
	return temp;	
}
double_word MBR_ENTRY::size()
{
	double_word temp = 0x00000000;
	temp = (temp | (((double_word) contents[12]) << 0)) & 0x000000FF;
	temp = (temp | (((double_word) contents[13]) << 8)) & 0x0000FFFF;
	temp = (temp | (((double_word) contents[14]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((double_word) contents[15]) << 24)) & 0xFFFFFFFF;
	return temp;
}


string VBR::OEM_NAME()
{
	string temp = "";
	for (int i = 3; i < 11; i ++) temp = temp + (char) contents[i];
	return temp;
}
word VBR::sector_size()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[11]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[12]) << 8)) & 0xFFFF;
	return temp;
}
byte VBR::cluster_size()
{
	return contents[13];
}
word VBR::reserved_sectors()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[14]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[15]) << 8)) & 0xFFFF;
	return temp;
}
byte VBR::FATs()
{
	return contents[16];
}
word VBR::root_size()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[17]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[18]) << 8)) & 0xFFFF;
	return temp;
}
word VBR::fs_size16()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[19]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[20]) << 8)) & 0xFFFF;
	return temp;
}
byte VBR::media_type()
{
	return contents[21];
}
word VBR::FAT_size16()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[22]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[23]) << 8)) & 0xFFFF;
	return temp;
}
word VBR::sectors_per_track()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[24]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[25]) << 8)) & 0xFFFF;
	return temp;
}
word VBR::head_count()
{
	word temp = 0x0000;
	temp = (temp | (((word) contents[26]) << 0)) & 0x00FF;
	temp = (temp | (((word) contents[27]) << 8)) & 0xFFFF;
	return temp;
}
double_word VBR::offset()
{
	double_word temp = 0x00000000;
	temp = (temp | (((double_word) contents[28]) << 0)) & 0x000000FF;
	temp = (temp | (((double_word) contents[29]) << 8)) & 0x0000FFFF;
	temp = (temp | (((double_word) contents[30]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((double_word) contents[31]) << 24)) & 0xFFFFFFFF;
	return temp;
}
double_word VBR::fs_size32()
{
	double_word temp = 0x00000000;
	temp = (temp | (((double_word) contents[32]) << 0)) & 0x000000FF;
	temp = (temp | (((double_word) contents[33]) << 8)) & 0x0000FFFF;
	temp = (temp | (((double_word) contents[34]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((double_word) contents[35]) << 24)) & 0xFFFFFFFF;
	return temp;
}
double_word VBR::FAT_size32()
{
	double_word temp = 0x00000000;
	temp = (temp | (((double_word) contents[36]) << 0)) & 0x000000FF;
	temp = (temp | (((double_word) contents[37]) << 8)) & 0x0000FFFF;
	temp = (temp | (((double_word) contents[38]) << 16)) & 0x00FFFFFF;
	temp = (temp | (((double_word) contents[39]) << 24)) & 0xFFFFFFFF;
	return temp;	
}