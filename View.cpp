#include "View.h"

enum fields
{
    MONSTER = 'M',
    POTION = 'H',
    ARMOR = 'A',
    WEAPON = 'W'
};

int View::checkFieldAndPerfAction(){
    char c = buffer[center_y][center_x];
    unsigned seed = time(0);
    srand(seed);
    switch(c)
    {
        case MONSTER:
            break;
        case POTION: {
            int hp = rand() % 100;
            std::string hpn = "Lesser Health Potion";
            if(hp > 80)
                hpn = "Greater Health Potion";
            else if(hp > 50)
                hpn = "Great Health Potion";

            std::shared_ptr<Consumable> cons(new Consumable(hpn, hp));
            
            shrdPlayer->addToInventory(std::static_pointer_cast<Item>(cons));

            //If Item was consumed we need to update the main map here.
            shrdMap->updateMap(x_cord, y_cord, '_');

        }
    }
    return 0;
}

int View::gameLogic(gcode &code) {
    switch(code)
    {
        case gcode::DF_KEY_INVENTORY: {
            if(!inv_open) {
                inv_open = true;
                stats_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                shrdPlayer->drawInventory(buffer);
            } else {
                //Closing the inventory
                inv_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                buffer[center_y][center_x] = 'P';
            }
            break;
        }
        case gcode::DF_KEY_DROP: {
            if(inv_open){
                shrdPlayer->dropFromInventory();
                shrdPlayer->drawInventory(buffer);
            }
            break;
        }
        case gcode::DF_KEY_USE: {
            if(inv_open) {
                shrdPlayer->useSelectedItemFromInventory();
                shrdPlayer->drawInventory(buffer);
            }
            break;
        }
        case gcode::DF_KEY_STATS: {
            if(!stats_open) {
                stats_open = true;
                inv_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                shrdPlayer->drawCharacterStats(buffer);
            } else {
                stats_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                buffer[center_y][center_x] = 'P';
            }
            break;
        }
        case gcode::DF_KEY_UP: {
            if(inv_open && !stats_open){
                shrdPlayer->changeInventorySelection(buffer, -1);
                y_cord = prev_y;        //Forbid moving while inventory is open
            } else if (stats_open) {
                y_cord = prev_y;
            } else {
                y_cord--;
                if (y_cord < 0)
                    y_cord = 0;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                checkFieldAndPerfAction();

                buffer[center_y][center_x] = 'P';
                prev_y = y_cord;
            }
            break;
        }
        case gcode::DF_KEY_DOWN: {
            if(inv_open && !stats_open){
                shrdPlayer->changeInventorySelection(buffer, 1);
                y_cord = prev_y;        //Forbid moving while inventory is open
            } else if(stats_open) {
                y_cord = prev_y;
            } else {
                y_cord++;
                if(y_cord > limit_y)
                    y_cord = limit_y;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                checkFieldAndPerfAction();

                buffer[center_y][center_x] = 'P';
                prev_y = y_cord;
            }
            break;                
        }
        case gcode::DF_KEY_LEFT: {
            if(inv_open || stats_open){
                x_cord = prev_x;
            } else {
                x_cord--;
                if(x_cord < 0)
                    x_cord = 0;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                checkFieldAndPerfAction();
                buffer[center_y][center_x] = 'P';
                prev_x = x_cord;
            }
            break;
        }
        case gcode::DF_KEY_RIGHT: {
            if(inv_open || stats_open){
                x_cord = prev_x;
            } else {
                x_cord++;
                if(x_cord > limit_x)
                    x_cord = limit_x;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);
                checkFieldAndPerfAction();
                buffer[center_y][center_x] = 'P';
                prev_x = x_cord;
            }
            break;                
        }
    }

    shrdMap->draw_map(buffer); //Its not neccessary to redraw everytime
    code = gcode::DF_KEY_NONE;

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    return 0;
}

int View::init() {
    buffer.reserve(height);
    std::vector<char> dd(width);

    for(int i = 0; i < height; i++)
        buffer.push_back(dd);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            buffer[i][j] = '-';

    //shrdMap->getMapAroundPlayer(x_cord, y_cord, width, height, buffer);

    drawMap(); 

    return 0;
}

int View::drawMap()
{
	//int height = buffer.size();
	//int width = buffer[0].size();

	int bsize = (width + 0) * height;
	char buf[bsize];
	int start = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			char c = buffer[i][j];
			if(c == 'X')
				bsize++;
			start += sprintf(buf + start, "%c", c);
		}
		//start += sprintf(buf + start, "\n");
	}

	//start += sprintf(buf + start, "\n");
	
    CLEAR;
    GOTOXY(0, 0);
	printf("%s", buf);
	fflush(stdout);
	return 0;
}