#include "View.h"

enum fields
{
    MONSTER = 'M',
    POTION = 'H',
    ARMOR = 'A',
    WEAPON = 'W'
};


void View::printMessage(int x, int y, std::string s) {
    for(int i = 0; i < s.length() && i < buffer[y].size(); i++)
        buffer[y][x+i] = s[i]; 
    return;
}

int View::initiateFight(std::shared_ptr<Monster> m) {
    //Fight takes turns. Firstly player attacks the monster
    //then the monster fights back. This cycle is repeated
    //until one of the contestans dies.

    bool 
        alive_p = true,
        alive_m = true;

    while (alive_p && alive_m) {
        int dmg = shrdPlayer->getAttack() - m->getDefense();
        if(dmg < 0) dmg = 1;
        
        alive_m = m->reduceHealth(dmg);

        if(!alive_m)
            break;

        dmg = m->getAttack() - shrdPlayer->getDefense();
        if(dmg < 0) dmg = 1;
        
        alive_p = shrdPlayer->reduceHealth(dmg);
    }

    return (alive_p) ? 0 : 1;
}

int View::checkFieldAndPerfAction(){
    char c = buffer[center_y][center_x];
    switch(c)
    {
        case MONSTER: {
            std::shared_ptr<Monster> monster = Monster::generateMonster();
            if(initiateFight(monster)) {
                //Player has died
                printMessage(center_x, center_y, "You have DIED!");
            } else {
                shrdMap->updateMap(x_cord, y_cord, 'x');
                //We need to drop the items that monster had
                int x = x_cord,
                    y = y_cord, 
                    w, a, c;

                monster->itemsInInventory(w, a, c);
                for(int i = 0; i < w; i++)
                    shrdMap->updateMap(x - i, y, 'W');
                
                y++;
                for(int i = 0; i < a; i++)
                    shrdMap->updateMap(x - i, y, 'A');

                y++;
                for(int i = 0; i < c; i++)
                    shrdMap->updateMap(x - i, y, 'C');                

            }

            break;
        }
        case POTION: {
            std::shared_ptr<Consumable> cons = Consumable::generateConsumable();
            
            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(cons)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, '_'); //If Item was consumed we need to update the main map here.

            break;
        }
        case ARMOR: {
            std::shared_ptr<Armor> armor = Armor::generateArmor();

            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(armor)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, '_'); //If Item was consumed we need to update the main map here.

            break;
        }
        case WEAPON: {
            std::shared_ptr<Weapon> weapon = Weapon::generateWeapon();

            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(weapon)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, '_'); //If Item was consumed we need to update the main map here.         
            break;
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