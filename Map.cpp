#include "Map.h"


int Map::loadMap(std::string fileName)
{
	namespace bpt = boost::property_tree;

	std::string data;
	bpt::ptree root;

	try {
		bpt::read_json(fileName, root); //pt::json_parser::json_parser_error
	}
	catch (bpt::json_parser::json_parser_error) {
		std::cout << "Ill formatted JSON file!" << std::endl;
		return BAD_JSON;
	}

	try {
		height = root.get<int>("height");
	}
	catch (bpt::ptree_bad_path) {
		std::cout << "Height is not specified in maps configuration file!" << std::endl;
		return HEIGHT_ERROR;
	}

	try {
		width = root.get<int>("width");
	}
	catch (bpt::ptree_bad_path) {
		std::cout << "Width is not specified in maps configuration file!" << std::endl;
		return WIDTH_ERROR;
	}

	try {
		data = root.get<std::string>("data");
	}
	catch (bpt::ptree_bad_path) {
		std::cout << "Data is not specified in maps configuration file!" << std::endl;
		return DATA_ERROR;
	}

	//Resize the map to accomodate the data and fill the map with blanks
	map.resize((height + FULL_BORDER) * (width + FULL_BORDER), ' ');

	//Position the data correctly inside the map
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int map_index = (i + BORDER) * (width + FULL_BORDER) + (j + BORDER);
			int data_index = i * width + j;

			map[map_index] = data[data_index];
		}
	}

	return 0;
}

int Map::getMapAroundPlayer(int x, int y, int bufw, int bufh, std::vector<std::vector<char>>& buffer)
{
	int
		center_x = bufw / 2,
		center_y = bufh / 2;

	//The current position of the player on the screen is (center_x, center_y)
	//The current position of the player on the map is (x, y)
	int
		offset_x = x + BORDER,
		offset_y = y + BORDER;


	buffer.clear();

	int map_start_x = offset_x - center_x;
	int map_start_y = offset_y - center_y;

	for (int i = 0; i < bufh; i++) {
		buffer.push_back(std::vector<char>());
		for (int j = 0; j < bufw; j++) {
			int map_index = (map_start_y + i) * (width + FULL_BORDER) + map_start_x;
			buffer[i].push_back(map[map_index + j]);
		}
	}
	
	return 0;
}

int Map::self_check()
{
	test_load_bad_file1();
	test_load_bad_file2();
	test_load_bad_file3();
	test_load_bad_file4();


	test_load_good_file();
	test_get_map();
	
	return 0;
}

int Map::test_load_bad_file1()
{
	std::cout << "Invoking test 1 ... " << std::endl;
	int ret = loadMap("tests/bad_test1.json");
	if (ret == HEIGHT_ERROR)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Fail" << std::endl;

	return 0;
}

int Map::test_load_bad_file2()
{
	std::cout << "Invoking test 2 ... "  << std::endl;
	int ret = loadMap("tests/bad_test2.json");
	if (ret == WIDTH_ERROR)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Fail" << std::endl;

	return 0;
}

int Map::test_load_bad_file3()
{
	std::cout << "Invoking test 3 ... " << std::endl;
	int ret = loadMap("tests/bad_test3.json");
	if (ret == DATA_ERROR)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Fail" << std::endl;
	return 0;
}

int Map::test_load_bad_file4()
{
	std::cout << "Invoking test 4 ... " << std::endl;
	int ret = loadMap("tests/bad_test4.json");
	if (ret == BAD_JSON)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Fail" << std::endl;
	return 0;
}

int Map::test_load_good_file()
{
	std::cout << "Invoking test 5 ... " << std::endl;
	int ret = loadMap("tests/good_test.json");
	if (ret == 0)
		std::cout << "Pass" << std::endl;
	else
		std::cout << "Fail" << std::endl;

	return 0;
}

int Map::draw_map(std::vector<std::vector<char>> &buffer)
{
	int height = buffer.size();
	int width = buffer[0].size();

	int bsize = width * height + height;
	char* buf = new char[bsize];
	char* const last = buf + bsize;
	char* start = buf;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			start += snprintf(start, last-start, "%c", buffer[i][j]);
		}

		if (i < (height - 1))
			start += snprintf(start, last-start, "\n");
	}

	*start = '\0';

	printf("%s", buf);

	delete buf;

	return 0;
}

int Map::test_get_map()
{
	std::chrono::seconds tm(1);
	std::vector<std::vector<char>> buffer;


	getMapAroundPlayer(92, 15, 185, 30, buffer);
	draw_map(buffer);

	std::this_thread::sleep_for(tm);

	//Move right
	getMapAroundPlayer(93, 15, 185, 30, buffer);
	draw_map(buffer);

	std::this_thread::sleep_for(tm);

	//Move right
	getMapAroundPlayer(94, 15, 185, 30, buffer);
	draw_map(buffer);

	std::this_thread::sleep_for(tm);

	//Move up
	getMapAroundPlayer(94, 14, 185, 30, buffer);
	draw_map(buffer);

	std::this_thread::sleep_for(tm);
	std::this_thread::sleep_for(tm);

	std::this_thread::sleep_for(tm);


	return 0;
}


