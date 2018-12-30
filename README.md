#  libitunesdb2
A successor to [libitunesdb](http://libitunesdb.sourceforge.net/) using C++ and supporting the latest database versions

## Compiling
The compilation is already all set up in the Xcode project, but you can compile manually with:
```
gcc -c -o libitunesdb2.o libitunesdb2/libitunesdb2.cpp
ar rcs libitunesdb2.a libitunesdb2.o
gcc -o libitunesdb2-test libitunesdb2-test/main.cpp -L. -litunesdb2
# If not on macOS, skip this
gcc -o itunesdb2plist itunesdb2plist/main.mm -L. -litunesdb2
```

## Usage
1. Create a new `iTunesDBReader` object, using the file name to open as a constructor
2. Read the album list (a vector of `album_entry`'s) with `reader.getAlbums()`
3. Read the track list (a vector of `track_entry`'s) with `reader.getTracks()`

Closing the file is not necessary afterwards: the file is automatically closed on destruction. See `libitunesdb2.hpp` for more info on types.

## Tools
libitunesdb2-test allows you to inspect iTunesDB files. You can list albums, tracks, or get all info on a specific song.  
`Usage: libitunesdb2-test <a|t|i> <iTunesDB> [album/song id]` (song id is required for `i`.)  

itunesdb2plist converts iTunesDB files to property lists for easier reading. By default it writes to ./iTunesDB.plist.  
`Usage: itunesdb2plist <iTunesDB> [output.plist]`

#### Legal
iTunes and iPod are trademarks of Apple Inc., registered in the U.S. and other countries.
