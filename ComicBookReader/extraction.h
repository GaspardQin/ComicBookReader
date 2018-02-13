#pragma once
#include "C:\Users\yanis\Documents\GitHub\ComicBookReader\3dparts\include\libarchive\archive.h"

#include "C:\Users\yanis\Documents\GitHub\ComicBookReader\3dparts\include\libarchive\archive_entry.h"
#include<io.h>
#include "C:\Users\yanis\Documents\GitHub\ComicBookReader\include\comic_book_reader_contract.h"

class Extraction: public ArchiveInterface {
public:
	

int r;
long size;

struct archive *a = archive_read_new();
archive_read_support_compression_all(a);


r = archive_read_open_filename(a, filename, 16384);
if (r != ARCHIVE_OK) {
	/* ERROR */
}
r = archive_read_next_header(a, &ae);
if (r != ARCHIVE_OK) {
	/* ERROR */
}

for (;;) {
	size = archive_read_data(a, buff, buffsize);
}

};