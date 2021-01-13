#include "View.h"



void View::drawFightScreen(std::shared_ptr<Monster> monster, int& p_hp_x_coord, int& m_hp_x_coord, int& hp_y_coord) {
    //Draw border and fill with blanks
	for (int i = 0; i < height; i++) {
		buffer[i][0] = '|';
        buffer[i][width - 1] = '|';
        for(int j = 1; j < width - 1; j++)
            buffer[i][j] = ' ';
	}
    //Draw border
	for (int j = 0; j < width; j++) {
		buffer[0][j] = '=';
        buffer[height - 1][j] = '=';
	}

    int 
        spacing = 2,
        h_start = 2,
        left_width_start = width - 30;

    //Draw player shape and stats
    for(int i = 0; i < Player::shape.size() && h_start < height-1; i++, h_start++) {
        printMessage(spacing, h_start, Player::shape[i]);
    }
    
    if(h_start + 4 >= height) {
        spacing = 30;
        h_start = 2;
    }

    printMessage(spacing, h_start++, "========================");
    printMessage(spacing, h_start++,   "Attack  " + std::to_string(shrdPlayer->getAttack()));
    printMessage(spacing, h_start++, "Defense " + std::to_string(shrdPlayer->getDefense()));

    p_hp_x_coord = spacing;
    hp_y_coord = h_start;
    printMessage(spacing, h_start++, "Health  " + std::to_string(shrdPlayer->getHealth()));

    creature_t shape = monster->getShape();

    h_start = 2;

    //Draw monster shape and stats
    for(int i = 0; i < shape.size() && h_start < height - 1; i++) {
        printMessage(left_width_start, h_start, shape[i]);
        h_start++;
    }

    if(h_start + 4 >= height) {
        left_width_start -= 30;
        h_start = 2;
    }
    
    printMessage(left_width_start, h_start++, "========================");
    printMessage(left_width_start, h_start++, "Attack  " + std::to_string(monster->getAttack()));
    printMessage(left_width_start, h_start++, "Defense " + std::to_string(monster->getDefense()));

    m_hp_x_coord = left_width_start;
    printMessage(left_width_start, h_start++, "Health  " + std::to_string(monster->getHealth()));
    

    drawMap();

    return;
}

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
    
    int 
        p_hp_x = 0,
        hp_y = 0,
        m_hp_x = 0;

    drawFightScreen(m, p_hp_x, m_hp_x, hp_y);

    float 
        p_att = shrdPlayer->getAttack(),
        p_def = shrdPlayer->getDefense(),
        m_att = m->getAttack(),
        m_def = m->getDefense();

    nodelay(stdscr, FALSE);

    while (alive_p && alive_m) {
        
        float dmg = p_att * ( p_att / (p_att + m_def));
        if(dmg <= 0) dmg = 1;
        
        alive_m = m->reduceHealth(dmg);

        printMessage(m_hp_x, hp_y, "Health  " + std::to_string(m->getHealth()) + "  ");
        drawMap();
        if(!alive_m)
            break;

        dmg = m_att * ( m_att / ( m_att + p_def));
        if(dmg <= 0) dmg = 1;
        
        alive_p = shrdPlayer->reduceHealth(dmg);
        printMessage(p_hp_x, hp_y, "Health  " + std::to_string(shrdPlayer->getHealth()) + "  ");
        drawMap();


        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    printMessage(center_x - 12, height - 2, "Press any key to continue");
    drawMap();

    getch();
    nodelay(stdscr, TRUE);

    return (alive_p) ? 0 : 1;
}

int View::checkFieldAndPerfAction(){
    data_t c = buffer[center_y][center_x];
    switch(hash(c))
    {
        case MONSTER: {
            std::shared_ptr<Monster> monster = Monster::generateMonster();
            if(initiateFight(monster)) {
                //Player has died
                printMessage(center_x, center_y, "You have DIED!");
                drawMap();
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                return -1;
            } else {
                //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

                shrdMap->updateMap(x_cord, y_cord, L".");
                //We need to drop the items that monster had
                int x = x_cord,
                    y = y_cord, 
                    w, a, c;

                monster->itemsInInventory(w, a, c);
                for(int i = 0; i < w; i++)
                    shrdMap->updateMap(x - i, y, L"W");
                
                y++;
                for(int i = 0; i < a; i++)
                    shrdMap->updateMap(x - i, y, L"A");

                y++;
                for(int i = 0; i < c; i++)
                    shrdMap->updateMap(x - i, y, L"H");    

                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
            }

            break;
        }
        case POTION: {
            std::shared_ptr<Consumable> cons = Consumable::generateConsumable();
            
            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(cons)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, L" "); //If Item was consumed we need to update the main map here.

            break;
        }
        case ARMOR: {
            std::shared_ptr<Armor> armor = Armor::generateArmor();

            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(armor)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, L" "); //If Item was consumed we need to update the main map here.

            break;
        }
        case WEAPON: {
            std::shared_ptr<Weapon> weapon = Weapon::generateWeapon();

            if( shrdPlayer->addToInventory(std::static_pointer_cast<Item>(weapon)) >= 0)
                shrdMap->updateMap(x_cord, y_cord, L" "); //If Item was consumed we need to update the main map here.         
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
                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                shrdPlayer->drawInventory(buffer);
            } else {
                //Closing the inventory
                inv_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
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
                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                shrdPlayer->drawCharacterStats(buffer);
            } else {
                stats_open = false;
                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
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
                if (y_cord < BORDER)
                    y_cord = BORDER;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                if(checkFieldAndPerfAction() < 0)
                    return -1;

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

                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                if(checkFieldAndPerfAction() < 0)
                    return -1;

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
                if(x_cord < BORDER)
                    x_cord = BORDER;

                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                if(checkFieldAndPerfAction() < 0)
                    return -1;

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

                shrdMap->getMapAroundPlayer(x_cord, y_cord, center_x, center_y, buffer);
                if(checkFieldAndPerfAction() < 0)
                    return -1;

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
    VSBUFF dd(width);

    for(int i = 0; i < height; i++)
        buffer.push_back(dd);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            buffer[i][j] = '-';

    drawMap(); 

    return 0;
}

int View::drawMap()
{	
	std::wstring buf;
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data_t c = buffer[i][j];
			buf.append(c);
		}
	}

	erase();
	move(0,0);
	addwstr(buf.c_str());
	refresh();
    
	return 0;
}