#include "Player.h"

std::vector<std::string> Player::shape = {
"   \\\\\\|||///",
" .  =======",
"/ \\| O   O |",
"\\ / \\`___'/ ",
" #   _| |_",
"(#) (     )",  
" #\\//|* *|\\\\ ",
" #\\/(  *  )/",   
" #   =====",  
" #   ( U )", 
" #   || ||",
".#---'| |`----.",
"`#----' `-----'"
};

void Player::useSelectedItemFromInventory() {
    std::shared_ptr<Item> item = inventory.getSelectedItem();
    
    if(!item) 
        return;


    inventory.dropFromInventory();
    equipOrConsume(item);

    return;
}

void Player::drawCharacterStats(BUFFER &buffer) {

       int 
        height = buffer.size(),
	    width = buffer[0].size(),
        inv_width =  (width >= 50) ? 50 : width,
        inv_height = (height >= 16) ? 16 : height,
        temp_h_start = height / 2 - inv_height / 2,
        h_start = (temp_h_start > 0) ? temp_h_start : 0,
        temp_w_start = width / 2 - inv_width / 2,
        w_start = (temp_w_start > 0) ? temp_w_start : 0,
        spacing = 2;


    //Clear the buffer in that region
    for (int i = h_start; i < h_start + inv_height; i++) {
        buffer[i][w_start] = '|';
        buffer[i][w_start + inv_width-1] = '|';
		for (int j = w_start + 1; j < w_start + inv_width - 1; j++) {
            buffer[i][j] = ' ';
		}
	}


    //First line ╔════════╗
    for (int j = w_start; j < w_start + inv_width; j++) {
        if(j == w_start)
            buffer[h_start][j] = '+';
        else if(j == w_start + inv_width - 1)
            buffer[h_start][j] = '+';
        else 
            buffer[h_start][j] = '-';
    }

    h_start += spacing;
    
    std::string desc;
    int start_offset = w_start + spacing;


#define PRINT_STAT(description, var){\
    std::stringstream ss;\
    ss << description << var;\
    desc = ss.str();\
    h_start++;\
	for (int j = start_offset, k = 0; j < w_start + inv_width - spacing && k < desc.length(); j++, k++)\
            buffer[h_start][j] = desc[k];\
    }
	

    PRINT_STAT("Health:  ", health)
    PRINT_STAT("Attack:  ", attack)
    PRINT_STAT("Defense: ", defense)
    PRINT_STAT("====================" , "-")

    if(armor)
        PRINT_STAT("Armor:   ", armor->toString())
    else
        PRINT_STAT("Armor:   ", "Not equiped")
    
    if(weapon)
        PRINT_STAT("Weapon:  ", weapon->toString())
    else
        PRINT_STAT("Weapon:  ", "Not equiped")

	
#undef PRINT_STAT

    if(inv_height < 16)
        h_start = inv_height - 1;
    else
        h_start = h_start + inv_height - 2;

    
    //Last line ╚══Drop(x)══Use(u)═══╝
    int cnt = 0;
    for (int j = w_start; j < w_start + inv_width; j++) {
        if(j == w_start)
            buffer[h_start][j] = '+';
        else if(j == w_start + inv_width - 1)
            buffer[h_start][j] = '+';
        else 
            buffer[h_start][j] = '-';
    }
    

    return;
}

int Player::addToInventory(std::shared_ptr<Item> item) {
    return inventory.addToInventory(item);
}
