//
//  libitunesdb2.cpp
//  libitunesdb2
//
//  Created by JackMacWindows on 12/28/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

#ifdef DEBUG
#include <iostream>
#endif
#include "libitunesdb2.hpp"
#include "libitunesdb2Priv.hpp"
#define icmp(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;

int String_GetEncoding(char *string)
{
	unsigned c, i = 0, flags = 0;
	while (string[i] | string[i + 1] | string[i + 2] | string[i + 3])
		flags = (c = string[i++]) ? flags | ((!(flags % 4) &&
											  c > 0x7F) << 3) : flags | 1 | (!(i & 1) << 1)
		| ((string[i] == 0) << 2);
	return (flags & 1) + ((flags & 2) != 0) +
	((flags & 4) != 0) + ((flags & 8) != 0);
}

std::u16string utf8to16(std::string str) {
	return convert.from_bytes(str);
}

std::string utf16to8(std::u16string str) {
	return convert.to_bytes(str);
}

template<typename T>
void debugPrint(T first) {
#ifdef DEBUG
	std::cout << first;
#endif
}

template<typename T, typename ... Args>
void debugPrint(T first, Args... args) {
#ifdef DEBUG
	std::cout << first;
	debugPrint(args...);
#endif
}

iTunesDBReader::iTunesDBReader(std::string path) {
	fp = fopen(path.c_str(), "r");
}

iTunesDBReader::iTunesDBReader(const char * path) {
	fp = fopen(path, "r");
}

iTunesDBReader::~iTunesDBReader() {
	fclose(fp);
}

void iTunesDBReader::rewind() {fseek(fp, 0, SEEK_SET);}

bool iTunesDBReader::skipToSection(const char name[4]) {
	char buf[4];
	bool bStart = ftell(fp) == 0;
	fread(buf, 4, 1, fp);
	if (buf[0] != 'm' || buf[1] != 'h') {
		debugPrint("Tag ", std::string(buf, 4), " not a tag\n");
		fseek(fp, 0, SEEK_SET);
		fread(buf, 4, 1, fp);
		bStart = true;
		debugPrint("Searching from start\n");
	}
	while (!icmp(buf, name) && !feof(fp)) {
		debugPrint("Found section ", std::string(buf, 4), "\n");
		uint32_t size = 0;
		if (icmp(buf, "mhod")) fseek(fp, 4, SEEK_CUR);
		fread(&size, 4, 1, fp);
		debugPrint("Skipping ", size, " bytes\n");
		fseek(fp, size - (4 * (icmp(buf, "mhod") + 2)), SEEK_CUR);
		fread(buf, 4, 1, fp);
	}
	if (feof(fp)) {
		debugPrint("Reached EOF\n");
		if (bStart) {
			fseek(fp, 0, SEEK_SET);
			return skipToSection(name);
		} else return false;
	} else fseek(fp, -4, SEEK_CUR);
	return true;
}

string_object iTunesDBReader::readMHOD() {
	string_object retval;
	if (!skipToSection("mhod")) return retval;
	uint32_t hsize = 0, size = 0, len = 0;
	fseek(fp, 4, SEEK_CUR);
	fread(&hsize, 4, 1, fp);
	fread(&size, 4, 1, fp);
	fread(&retval.type, 4, 1, fp);
	fseek(fp, 8, SEEK_CUR);
	if (retval.type > 16 && retval.type < 200) return retval;
	else if (!(retval.type == 15 || retval.type == 16)) {
		fread(&retval.position, 4, 1, fp);
		fread(&len, 4, 1, fp);
		fseek(fp, 8, SEEK_CUR);
	} else len = size - hsize;
	debugPrint("Reading from ", ftell(fp), "\n");
	char * str = (char*)malloc(len+1);
	fread(str, len, 1, fp);
	int enc = String_GetEncoding(str);
	if (enc == STRING_ENCODING_UTF8) retval.str = std::u16string(convert.from_bytes(str, &str[len]));
	else retval.str = std::u16string((char16_t*)str, len/2);
	return retval;
}

album_list iTunesDBReader::getAlbums() {
	album_list retval;
	if (!skipToSection("mhla")) return retval;
	fseek(fp, 4, SEEK_CUR);
	uint32_t hsize = 0, albums = 0;
	fread(&hsize, 4, 1, fp);
	fread(&albums, 4, 1, fp);
	debugPrint("Found ", albums, " albums\nSkipping ", hsize, " bytes\n");
	fseek(fp, hsize - 12, SEEK_CUR);
	for (uint32_t i = 0; i < albums; i++) {
		char buf[4];
		long start = ftell(fp);
		fread(buf, 4, 1, fp);
		debugPrint("Found section ", std::string(buf, 4), "\n");
		if (!icmp(buf, "mhia")) break;
		album_entry e;
		e.artist = utf8to16("Unknown Artist");
		e.title = utf8to16("Unknown Album");
		uint32_t hsize = 0, size = 0, strings = 0;
		fread(&hsize, 4, 1, fp);
		fread(&size, 4, 1, fp);
		fread(&strings, 4, 1, fp);
		fread(&e.id, 4, 1, fp);
		fseek(fp, hsize - 20, SEEK_CUR);
		debugPrint("Header size: ", hsize, "\nTotal size: ", size, "\nStrings: ", strings, "\nID: ", e.id, "\n");
		for (int i = 0; i < strings; i++) {
			string_object o = readMHOD();
			if (o.type == STRING_TYPE_ALBUM_TITLE) e.title = o.str;
			else if (o.type == STRING_TYPE_ALBUM_ARTIST) e.artist = o.str;
		}
		fseek(fp, start + size, SEEK_SET);
		retval.push_back(e);
	}
	return retval;
}

track_list iTunesDBReader::getTracks() {
	track_list retval;
	if (!skipToSection("mhlt")) return retval;
	fseek(fp, 4, SEEK_CUR);
	uint32_t hsize = 0, tracks = 0;
	fread(&hsize, 4, 1, fp);
	fread(&tracks, 4, 1, fp);
	debugPrint("Found ", tracks, " tracks\nSkipping ", hsize, " bytes\n");
	fseek(fp, hsize - 12, SEEK_CUR);
	for (uint32_t i = 0; i < tracks; i++) {
		char buf[4];
		uint32_t tmp4 = 0;
		//uint16_t tmp2 = 0;
		//uint8_t tmp1 = 0;
		long start = ftell(fp);
		fread(buf, 4, 1, fp);
		debugPrint("Found section ", std::string(buf, 4), "\n");
		if (!icmp(buf, "mhit")) break;
		track_entry e;
		uint32_t hsize = 0, size = 0, strings = 0;
		fread(&hsize, 4, 1, fp);
		fread(&size, 4, 1, fp);
		fread(&strings, 4, 1, fp);
		fread(&e.uid, 4, 1, fp);
		fread(&tmp4, 4, 1, fp);
		e.visible = tmp4;
		//fread(&(((uint8_t*)&e)[5]), 37, 1, fp);
		fread(e.file_type, 4, 1, fp);
		fread(&e.type_id, 2, 1, fp);
		fread(&e.is_compilation, 1, 1, fp);
		fread(&e.stars, 1, 1, fp);
		fread(&e.last_modified, 4, 1, fp);
		fread(&e.file_size, 4, 1, fp);
		fread(&e.length, 4, 1, fp);
		fread(&e.number, 4, 1, fp);
		fread(&e.total_tracks, 4, 1, fp);
		fread(&e.year, 4, 1, fp);
		fread(&e.bitrate, 4, 1, fp);
		fread(&e.sample_rate, 4, 1, fp);
		buf[0] = e.file_type[0];
		buf[1] = e.file_type[1];
		buf[2] = e.file_type[2];
		buf[3] = e.file_type[3];
		e.file_type[0] = buf[3];
		e.file_type[1] = buf[2];
		e.file_type[2] = buf[1];
		e.file_type[3] = buf[0];
		e.stars /= 20;
		e.sample_rate /= 0x10000;
		int32_t stmp4 = 0;
		fread(&stmp4, 4, 1, fp);
		e.volume = stmp4;
		assert(ftell(fp) - start == 68);
		assert(sizeof(e.dbid) == 8);
		fread(&(((uint8_t*)&e)[46+6]), 16, 1, fp);
		fseek(fp, 4, SEEK_CUR);
		fread(&(((uint8_t*)&e)[62+6]), 24, 1, fp);
		fread(&e.dbid, 8, 1, fp);
		fread(&(((uint8_t*)&e)[98+6]), 12, 1, fp);
		fseek(fp, 8, SEEK_CUR);
		fread(&(((uint8_t*)&e)[110+6]), 12, 1, fp);
		fseek(fp, 4, SEEK_CUR);
		fread(&(((uint8_t*)&e)[122+6]), 23, 1, fp);
		fseek(fp, 5, SEEK_CUR);
		fread(&e.pregap, 4, 1, fp);
		fread(&e.sample_count, 8, 1, fp);
		fseek(fp, 4, SEEK_CUR);
		fread(&e.postgap, 4, 1, fp);
		fread(&tmp4, 4, 1, fp);
		e.mp3_encoded = tmp4;
		fread(&(((uint8_t*)&e)[162+6]), 16, 1, fp);
		fseek(fp, start + hsize, SEEK_SET);
		for (int i = 0; i < strings; i++) {
			string_object o = readMHOD();
			e.strings[o.type] = o;
		}
		fseek(fp, start + size, SEEK_SET);
		retval.push_back(e);
	}
	return retval;
}
