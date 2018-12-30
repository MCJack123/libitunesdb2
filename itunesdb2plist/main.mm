//
//  main.m
//  itunesdb2plist
//
//  Created by JackMacWindows on 12/29/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <stdio.h>
#include "../libitunesdb2/libitunesdb2.hpp"

int main(int argc, const char * argv[]) {
	@autoreleasepool {
		if (argc < 2) {
			printf("Usage: %s <iTunesDB> [output.plist]\n", argv[0]);
			return 1;
		}
		iTunesDBReader reader(argv[1]);
		NSMutableDictionary * retval = [NSMutableDictionary dictionary];
		album_list la = reader.getAlbums();
		retval[@"albums"] = [NSMutableArray array];
		for (int i = 0; i < la.size(); i++) {
			NSData* tdata = [[NSData alloc] initWithBytesNoCopy:(void*)la[i].title.c_str()
														 length:la[i].title.size()*2
												   freeWhenDone:NO];
			NSData* adata = [[NSData alloc] initWithBytesNoCopy:(void*)la[i].artist.c_str()
														 length:la[i].artist.size()*2
												   freeWhenDone:NO];
			[retval[@"albums"] addObject:@{
										   @"id": [NSNumber numberWithUnsignedInteger: la[i].id],
										   @"title": [[NSString alloc] initWithData:tdata encoding:NSUTF16LittleEndianStringEncoding],
										   @"artist": [[NSString alloc] initWithData:adata encoding:NSUTF16LittleEndianStringEncoding]
										   }];
		}
		track_list lt = reader.getTracks();
		retval[@"tracks"] = [NSMutableArray array];
		for (int j = 0; j < lt.size(); j++) {
			NSMutableDictionary * d = [NSMutableDictionary dictionary];
			NSData* tdata = [[NSData alloc] initWithBytesNoCopy:(void*)lt[j].file_type
														 length:4
												   freeWhenDone:NO];
			d[@"uid"] = [NSNumber numberWithUnsignedInteger:lt[j].uid];
			d[@"visible"] = [NSNumber numberWithBool:lt[j].visible];
			d[@"file_type"] = [[NSString alloc] initWithData:tdata encoding:NSASCIIStringEncoding];
			d[@"type_id"] = [NSNumber numberWithUnsignedInteger:lt[j].type_id];
			d[@"is_compilation"] = [NSNumber numberWithBool:lt[j].is_compilation];
			d[@"stars"] = [NSNumber numberWithUnsignedInteger:lt[j].stars];
			d[@"last_modified"] = [NSNumber numberWithUnsignedInteger:lt[j].last_modified];
			d[@"file_size"] = [NSNumber numberWithUnsignedInteger:lt[j].file_size];
			d[@"length"] = [NSNumber numberWithUnsignedInteger:lt[j].length];
			d[@"number"] = [NSNumber numberWithUnsignedInteger:lt[j].number];
			d[@"total_tracks"] = [NSNumber numberWithUnsignedInteger:lt[j].total_tracks];
			d[@"year"] = [NSNumber numberWithUnsignedInteger:lt[j].year];
			d[@"bitrate"] = [NSNumber numberWithUnsignedInteger:lt[j].bitrate];
			d[@"sample_rate"] = [NSNumber numberWithUnsignedInteger:lt[j].sample_rate];
			d[@"volume"] = [NSNumber numberWithInteger:lt[j].volume];
			d[@"start"] = [NSNumber numberWithUnsignedInteger:lt[j].start];
			d[@"stop"] = [NSNumber numberWithUnsignedInteger:lt[j].stop];
			d[@"sound_check"] = [NSNumber numberWithUnsignedInteger:lt[j].sound_check];
			d[@"play_count"] = [NSNumber numberWithUnsignedInteger:lt[j].play_count];
			d[@"last_played"] = [NSNumber numberWithUnsignedInteger:lt[j].last_played];
			d[@"disc_number"] = [NSNumber numberWithUnsignedInteger:lt[j].disc_number];
			d[@"total_discs"] = [NSNumber numberWithUnsignedInteger:lt[j].total_discs];
			d[@"drm_user_id"] = [NSNumber numberWithUnsignedInteger:lt[j].drm_user_id];
			d[@"date_added"] = [NSNumber numberWithUnsignedInteger:lt[j].date_added];
			d[@"bookmark_start"] = [NSNumber numberWithUnsignedInteger:lt[j].bookmark_start];
			d[@"dbid"] = [NSNumber numberWithUnsignedInteger:lt[j].dbid];
			d[@"checked"] = [NSNumber numberWithBool:lt[j].checked];
			d[@"last_rating"] = [NSNumber numberWithUnsignedInteger:lt[j].last_rating];
			d[@"bpm"] = [NSNumber numberWithUnsignedInteger:lt[j].bpm];
			d[@"artwork_count"] = [NSNumber numberWithUnsignedInteger:lt[j].artwork_count];
			d[@"unk9"] = [NSNumber numberWithUnsignedInteger:lt[j].unk9];
			d[@"artwork_size"] = [NSNumber numberWithUnsignedInteger:lt[j].artwork_size];
			d[@"release_time"] = [NSNumber numberWithUnsignedInteger:lt[j].release_time];
			d[@"unk14"] = [NSNumber numberWithUnsignedInteger:lt[j].unk14];
			d[@"unk15"] = [NSNumber numberWithUnsignedInteger:lt[j].unk15];
			d[@"skip_count"] = [NSNumber numberWithUnsignedInteger:lt[j].skip_count];
			d[@"last_skipped"] = [NSNumber numberWithUnsignedInteger:lt[j].last_skipped];
			d[@"has_artwork"] = [NSNumber numberWithBool:lt[j].has_artwork];
			d[@"skip_when_shuffling"] = [NSNumber numberWithBool:lt[j].skip_when_shuffling];
			d[@"remember_playback_position"] = [NSNumber numberWithBool:lt[j].remember_playback_position];
			d[@"podcast_flag"] = [NSNumber numberWithUnsignedInteger:lt[j].podcast_flag];
			d[@"dbid2"] = [NSNumber numberWithUnsignedInteger:lt[j].dbid2];
			d[@"mp3_lyrics"] = [NSNumber numberWithBool:lt[j].mp3_lyrics];
			d[@"is_movie"] = [NSNumber numberWithBool:lt[j].is_movie];
			d[@"podcast_unplayed"] = [NSNumber numberWithUnsignedInteger:lt[j].podcast_unplayed];
			d[@"pregap"] = [NSNumber numberWithUnsignedInteger:lt[j].pregap];
			d[@"sample_count"] = [NSNumber numberWithUnsignedInteger:lt[j].sample_count];
			d[@"postgap"] = [NSNumber numberWithUnsignedInteger:lt[j].postgap];
			d[@"mp3_encoded"] = [NSNumber numberWithBool:lt[j].mp3_encoded];
			d[@"media_type"] = [NSNumber numberWithUnsignedInteger:lt[j].media_type];
			d[@"season_number"] = [NSNumber numberWithUnsignedInteger:lt[j].season_number];
			d[@"episode_number"] = [NSNumber numberWithUnsignedInteger:lt[j].episode_number];
			d[@"is_protected"] = [NSNumber numberWithUnsignedInteger:lt[j].is_protected];
			[retval[@"tracks"] addObject:d];
		}
		NSString * output = @"iTunesDB.plist";
		if (argc > 2) output = [NSString stringWithCString:argv[2] encoding:NSASCIIStringEncoding];
		[retval writeToFile:output atomically:YES];
	}
	return 0;
}
