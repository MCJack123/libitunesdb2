//
//  main.cpp
//  libitunesdb2-test
//
//  Created by JackMacWindows on 12/29/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "../libitunesdb2/libitunesdb2.hpp"

int main(int argc, const char * argv[]) {
	// insert code here...
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <a|t|i> <iTunesDB> [album/song id]\n";
		return 1;
	}
	iTunesDBReader reader(argv[2]);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	if (argv[1][0] == 'a') {
		album_list l = reader.getAlbums();
		for (album_entry e : l)
			std::cout << "Album ID " << e.id << " named " << convert.to_bytes(e.title) << " by " << convert.to_bytes(e.artist) << "\n";
	} else if (argv[1][0] == 't') {
		track_list l = reader.getTracks();
		std::u16string filter;
		if (argc > 3) {
			uint32_t filter_num = std::stoi(std::string(argv[3]));
			reader.rewind();
			album_list la = reader.getAlbums();
			for (album_entry e : la) if (e.id == filter_num) filter = e.title;
			if (filter == convert.from_bytes("")) {
				std::cerr << "Could not find album ID " << filter_num << "\n";
				return 2;
			}
		}
		std::cout << "Searching for album " << convert.to_bytes(filter) << "\n";
		for (track_entry e : l)
			if (filter == convert.from_bytes("") || e.strings[STRING_TYPE_ALBUM].str == filter)
				std::cout << "Song ID " << e.uid << " named " << convert.to_bytes(e.strings[STRING_TYPE_TITLE].str) << " by " << convert.to_bytes(e.strings[STRING_TYPE_ARTIST].str) << " in album " << convert.to_bytes(e.strings[STRING_TYPE_ALBUM].str) << "\n";
	} else if (argv[1][0] == 'i' && argc > 3) {
		track_list l = reader.getTracks();
		uint32_t filter = std::stoi(std::string(argv[3]));
		for (track_entry e : l)
			if (e.uid == filter) {
printf("ID: %u\n\
Visible: %s\n\
File type: %s\n\
Type ID: %u\n\
Compilation: %s\n\
Rating: %u stars\n\
Last modified: %u\n\
File size: %u\n\
Length: %u:%02u\n\
Song position: %u/%u\n\
Year: %u\n\
Bitrate: %u kbps\n\
Sample rate: %u Hz\n\
Volume offset: %d\n\
Start point: %f seconds\n\
Stop point: %f seconds\n\
Sound check value: %u\n\
Plays: %u\n\
Last played: %u\n\
Disc: %u/%u\n\
DRM User ID: %u\n\
Date added: %u\n\
Bookmark start: %f seconds\n\
Database ID: %llu\n\
Checked: %s\n\
Last rating: %u stars\n\
BPM: %u\n\
Number of artworks: %u\n\
Unknown field 9 (compression): %#06x\n\
Artwork size: %u\n\
Release time: %u\n\
Unknown field 14 (File type/plays): %#010x\n\
Unknown field 15 (DRM): %#010x\n\
Skips: %u\n\
Last skipped: %u\n\
Has artwork: %s\n\
Skip when shuffling: %s\n\
Remember playback position: %s\n\
Podcast flag: %#004x\n\
Has MP3 lyrics: %s\n\
Is a movie: %s\n\
Podcast play flag: %u\n\
Pre-gap: %u\n\
Sample count: %llu\n\
Post-gap: %u\n\
Encoded with MP3 encoder: %s\n\
Media type: %u\n\
Episode (TV shows): S%uE%u\n\
Protection flag: %u\n\
Strings: \n",
	   e.uid,
	   e.visible ? "true" : "false",
	   std::string(e.file_type, 4).c_str(),
	   e.type_id,
	   e.is_compilation ? "true" : "false",
	   e.stars,
	   e.last_modified,
	   e.file_size,
	   (e.length / 1000) / 60, (e.length / 1000) % 60,
	   e.number, e.total_tracks,
	   e.year,
	   e.bitrate,
	   e.sample_rate,
	   e.volume,
	   e.start / 1000.0,
	   e.stop / 1000.0,
	   e.sound_check,
	   e.play_count,
	   e.last_played,
	   e.disc_number, e.total_discs,
	   e.drm_user_id,
	   e.date_added,
	   e.bookmark_start / 1000.0,
	   e.dbid,
	   e.checked ? "true" : "false",
	   e.last_rating,
	   e.bpm,
	   e.artwork_count,
	   e.unk9,
	   e.artwork_size,
	   e.release_time,
	   e.unk14,
	   e.unk15,
	   e.skip_count,
	   e.last_skipped,
	   e.has_artwork ? "true" : "false",
	   e.skip_when_shuffling ? "true" : "false",
	   e.remember_playback_position ? "true" : "false",
	   e.podcast_flag,
	   e.mp3_lyrics ? "true" : "false",
	   e.is_movie ? "true" : "false",
	   e.podcast_unplayed,
	   e.pregap,
	   e.sample_count,
	   e.postgap,
	   e.mp3_encoded ? "true" : "false",
	   e.media_type,
	   e.season_number, e.episode_number,
	   e.is_protected);
				for (auto o : e.strings)
					printf("\tMedia type %u: %s\n", o.first, convert.to_bytes(o.second.str).c_str());
			}
	} else {
		std::cerr << "Usage: " << argv[0] << " <a|t|i> <iTunesDB> [album/song id]\n";
		return 1;
	}
	return 0;
}
