#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <locale>
#include <codecvt>

#include <stdio.h>

//Needed for testing.
#include <chrono>
#include <thread>

#include <sstream>

enum {
	HEIGHT_ERROR = -1,
	WIDTH_ERROR = -2,
	DATA_ERROR = -3,
	BAD_JSON = -4
};


/**
* Note: If the console window size changes, we could change the 
* border size accordingly. 
*/
#define FULL_BORDER 400
#define BORDER (FULL_BORDER/2)
#define CLEAR printf("\033[H\033[J")
#define GOTOXY(x,y) printf("\033[%d;%dH", (y), (x))

typedef std::wstring data_t;
typedef std::vector<data_t> VSBUFF;
typedef std::vector<VSBUFF> BUFFER;

class Map
{
private:
	int width;
	int height;



	/**
	* Map is actually bigger by 200 characters on each side.
	* This will enable very fast retrieving of the "view" player
	* is looking at.
	*/
	VSBUFF map;

public:
	Map(int w, int h) {
		width = w + FULL_BORDER;
		height = h + FULL_BORDER;
		map.resize(width * height);
	}

	Map() {
		width = 0;
		height = 0;
		map.clear();
	}

	~Map() {
		map.clear();
	}

	/**
	* Loads the configuration file in JSON format of the map view. Expected
	* value : data pairs are
	* 
	* width   ->   width of the map
	* height  ->   height of the map
	* data    ->   Array of characters representing specific things on the map.
	*              Map is expected to be a single array.
	*/
	int loadMap(std::string fileName);


	/**
	* Returns the area around the player given the players coordinates. Player
	* is always centered on the screen. If the player is near a maps bounds 
	* then buffer will be populated with blank's or "space" characters. 
	* 
	* x        ->    is the players X coordinate on the map, not on the screen
	* y        ->    is the players Y coordinate on the map, not on the screen
	* center_x ->	 is the center x of the console window
	* center_y ->	 is the center y of the console window
	* buffer   ->    is a reference to the vector matrix in which we should
	*                put the screen data
	*/
	int getMapAroundPlayer(int x, int y, int bufw, int bufh, BUFFER& buffer);

	/**
	 * Updates the map at the designated coordinates. 
	 * 
	 * x      ->    is the players X coordinate on the map, not on the screen
	 * y      ->    is the players Y coordinate on the map, not on the screen
	 * c      ->    character to place at coordinates
	*/
	int updateMap(int x, int y, char c);

	//Testing code
	int self_check();
	int draw_map(BUFFER& buffer);

	int getWidth() { return width; }
	int getHeight() { return height; }

private:
	//UNIT TESTS BELOW

	int test_load_bad_file1();
	int test_load_bad_file2();
	int test_load_bad_file3();
	int test_load_bad_file4();
	
	int test_load_good_file();

	int test_get_map();
};

