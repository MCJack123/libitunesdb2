//
//  libitunesdb2.hpp
//  libitunesdb2
//
//  Created by JackMacWindows on 12/28/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

#ifndef libitunesdb2_
#define libitunesdb2_
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include <map>

// Track file types (track_entry.file_type)
#define TRACK_FILE_TYPE_NONE 0
#define TRACK_FILE_TYPE_MP3 "MP3 "
#define TRACK_FILE_TYPE_AAC "AAC "
#define TRACK_FILE_TYPE_M4A "M4A "
#define TRACK_FILE_TYPE_PROTECTED_AAC "M4P "

// Track file IDs (track_entry.type_id)
#define TRACK_FILE_TYPE_ID_CBR_MP3 0x0100
#define TRACK_FILE_TYPE_ID_VBR_MP3 0x0101
#define TRACK_FILE_TYPE_ID_AAC 0x0000

// Values for unknown track field #9
#define TRACK_UNK9_COMPRESSED 0xffff
#define TRACK_UNK9_UNCOMPRESSED 0x0
#define TRACK_UNK9_AUDIBLE 0x1

// Values for unknown track field #14
#define TRACK_UNK14_MP1_UNPLAYED 0x000000c
#define TRACK_UNK14_MP1_PLAYED 0x100000c
#define TRACK_UNK14_MP2_UNPLAYED 0x0000016
#define TRACK_UNK14_MP2_PLAYED 0x1000016
#define TRACK_UNK14_MP25_UNPLAYED 0x0000020
#define TRACK_UNK14_MP25_PLAYED 0x1000020
#define TRACK_UNK14_AAC 0x1000033
#define TRACK_UNK14_AUDIBLE 0x1000029
#define TRACK_UNK14_WAV 0x0000000

// Values for unknown track field #15
#define TRACK_UNK15_DRM 0x01010100
#define TRACK_UNK15_NO_DRM 0x0

// Track media types
#define TRACK_MEDIA_TYPE_AUDIO_VIDEO 0x00
#define TRACK_MEDIA_TYPE_AUDIO 0x01
#define TRACK_MEDIA_TYPE_VIDEO 0x02
#define TRACK_MEDIA_TYPE_PODCAST 0x04
#define TRACK_MEDIA_TYPE_VIDEO_PODCAST (TRACK_MEDIA_TYPE_VIDEO | TRACK_MEDIA_TYPE_PODCAST)
#define TRACK_MEDIA_TYPE_AUDIO_BOOK 0x08
#define TRACK_MEDIA_TYPE_MUSIC_VIDEO 0x20
#define TRACK_MEDIA_TYPE_TV_SHOW 0x40
#define TRACK_MEDIA_TYPE_TV_SHOW_MUSIC 0x60

// MHOD/string types
#define STRING_TYPE_TITLE 1
#define STRING_TYPE_LOCATION 2
#define STRING_TYPE_ALBUM 3
#define STRING_TYPE_ARTIST 4
#define STRING_TYPE_GENRE 5
#define STRING_TYPE_FILE_TYPE 6
#define STRING_TYPE_EQ_SETTING 7
#define STRING_TYPE_COMMENT 8
#define STRING_TYPE_CATEGORY 9
#define STRING_TYPE_COMPOSER 12
#define STRING_TYPE_GROUPING 13
#define STRING_TYPE_DESCRIPTION_TEXT 14
#define STRING_TYPE_PODCAST_ENCLOSURE_URL 15
#define STRING_TYPE_PODCAST_RSS_URL 16
#define STRING_TYPE_ALBUM_TITLE 200
#define STRING_TYPE_ALBUM_ARTIST 201
#define STRING_TYPE_ALBUM_ARTIST_SORT 202
#define STRING_TYPE_ALBUM_PODCAST_URL 203
#define STRING_TYPE_ALBUM_TV_SHOW 204

/* The classes below are exported */
#pragma GCC visibility push(default)


typedef struct {
	uint32_t id;
	std::u16string title;
	std::u16string artist;
} album_entry;
typedef std::vector<album_entry> album_list;

typedef struct {
	uint32_t type;
	uint32_t position;
	std::u16string str;
} string_object;

typedef struct {
	uint32_t uid;
	bool visible;
	char file_type[4];
	uint16_t type_id;
	bool is_compilation;
	uint8_t stars;
	uint32_t last_modified;
	uint32_t file_size;
	uint32_t length;
	uint32_t number;
	uint32_t total_tracks;
	uint32_t year;
	uint32_t bitrate;
	uint32_t sample_rate;
	int16_t volume;
	uint32_t start;
	uint32_t stop;
	uint32_t sound_check;
	uint32_t play_count;
	uint32_t last_played;
	uint32_t disc_number;
	uint32_t total_discs;
	uint32_t drm_user_id;
	uint32_t date_added;
	uint32_t bookmark_start;
	uint64_t dbid;
	bool checked;
	uint8_t last_rating;
	uint16_t bpm;
	uint16_t artwork_count;
	uint16_t unk9;
	uint32_t artwork_size;
	uint32_t release_time;
	uint32_t unk14;
	uint32_t unk15;
	uint32_t skip_count;
	uint32_t last_skipped;
	bool has_artwork;
	bool skip_when_shuffling;
	bool remember_playback_position;
	uint8_t podcast_flag;
	uint64_t dbid2;
	bool mp3_lyrics;
	bool is_movie;
	uint8_t podcast_unplayed;
	uint32_t pregap;
	uint64_t sample_count;
	uint32_t postgap;
	bool mp3_encoded;
	uint32_t media_type;
	uint32_t season_number;
	uint32_t episode_number;
	uint32_t is_protected;
	std::map<int, string_object> strings;
} track_entry;
typedef std::vector<track_entry> track_list;

class iTunesDBReader {
	FILE * fp;
	bool skipToSection(const char name[4]);
	string_object readMHOD();
public:
	iTunesDBReader(std::string path);
	iTunesDBReader(const char * path);
	~iTunesDBReader();
	void rewind();
	album_list getAlbums();
	track_list getTracks();
};

#pragma GCC visibility pop
#endif
