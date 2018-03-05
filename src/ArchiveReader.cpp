#include "ArchiveReader.h"



ArchiveReader::ArchiveReader()
{
}


ArchiveReader::~ArchiveReader()
{
}

bool ArchiveReader::loadArchivedFiles(std::string file_path)
{
	offset_cache.clear();
	archive_path = file_path;
	int file_type_pos = archive_path.rfind(".");
	std::string file_type(archive_path.begin() + file_type_pos + 1, archive_path.end());
	std::transform(file_type.begin(), file_type.end(), file_type.begin(), ::tolower);
	if (file_type == "png" || file_type == "jpg" || file_type == "bmp") {
		page_num_total = 1;
		isSingleImg = true;

		return true;
	}

	//cbr cbz
	isSingleImg = false;
	char *path = &archive_path[0];
	stream = ar_open_file(path);
	if (stream == NULL) return false;
	ar = ar_open_any_archive(stream, strrchr(path, '.'));
	if (!ar) return false;
	int count = 0;
	while (ar_parse_entry(ar)) {
		int offset = ar_entry_get_offset(ar);
		offset_cache.push_back(offset);
		count++;
	}
	page_num_total = count;
	return true;
}

bool ArchiveReader::loadOneImage(int num, cv::Mat & a_image)
{
	if (num > page_num_total || num < 1) return false;
	if (isSingleImg == true) {
		a_image = cv::imread(archive_path, cv::IMREAD_COLOR);
		return true;
	}


	if(!ar_parse_entry_at(ar, offset_cache[num-1])) return false;
	int size = ar_entry_get_size(ar);
	if (size <= 0) {
		const char* zero_file = ar_entry_get_name(ar);
		std::cout << zero_file << std::endl;
		return false;
	}
	//check buffer size 
	read_buffer.resize(size);
	unsigned char *buffer = &read_buffer[0];
	if(!ar_entry_uncompress(ar, buffer, size)) return false;

	a_image = cv::imdecode(read_buffer, cv::IMREAD_COLOR);
	
	
	return true;
}

int ArchiveReader::getPageNumTotal()
{
	return page_num_total;
}

ar_archive * ArchiveReader::ar_open_any_archive(ar_stream * stream, const char * fileext)
{
	ar_archive *ar = ar_open_rar_archive(stream);
	if (!ar)
		ar = ar_open_zip_archive(stream, fileext && (strcmp(fileext, ".xps") == 0 || strcmp(fileext, ".epub") == 0));
	if (!ar)
		ar = ar_open_7z_archive(stream);
	if (!ar)
		ar = ar_open_tar_archive(stream);
	return ar;
}
