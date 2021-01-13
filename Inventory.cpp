#include "Inventory.h"

void Inventory::drawInventory(BUFFER &buffer)
{
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
    
    selected_item_idx = 0;
    selected_item_idx_y = h_start;
    selected_item_idx_x = w_start + spacing;

    //Inventory is limited to 3 items
	for (int i = h_start, inv_cnt = 0; i < h_start + inv_height - spacing, inv_cnt < inventory.size(); i++, inv_cnt++) {
        std::string desc = (inv_cnt == 0) ? "*" : " ";
        desc += inventory[inv_cnt]->toString();

        int start_offset = w_start + spacing;
		for (int j = start_offset, k = 0; j < w_start + inv_width - spacing && k < desc.length(); j++, k++) {
            buffer[i][j] = desc[k];
		}
	}

    if(inv_height < 16)
        h_start = inv_height - 1;
    else
        h_start = h_start + inv_height - 2;

    
    //Last line ╚══Drop(x)══Use(u)═══╝
    std::string usedu = "Drop(x)---Use(u)";
    int cnt = 0;
    for (int j = w_start; j < w_start + inv_width; j++) {
        if(j == w_start)
            buffer[h_start][j] = '+';
        else if(j == w_start + inv_width - 1)
            buffer[h_start][j] = '+';
        else if(j > w_start + spacing && cnt < usedu.length()) {
            buffer[h_start][j] = usedu[cnt];
            cnt++;
        } else 
            buffer[h_start][j] = '-';
    }
    

	return;
}

void Inventory::dropFromInventory()
{
    if(inventory.size() > 0 && inventory.size() > selected_item_idx)
        inventory.erase(inventory.begin() + selected_item_idx);
}

void Inventory::changeInventorySelection(int p, BUFFER &buffer)
{
    buffer[selected_item_idx_y][selected_item_idx_x] = ' ';

    if(inventory.empty())
        return;

    if(p > 0 && selected_item_idx < inventory.size()-1){
        selected_item_idx++;
        selected_item_idx_y++;
    }
    else if(p < 0 && selected_item_idx > 0){
        selected_item_idx--;
        selected_item_idx_y--;
    }

    buffer[selected_item_idx_y][selected_item_idx_x] = '*';

    return;
}

std::shared_ptr<Item> Inventory::getSelectedItem()
{
    if(inventory.size() > 0)
        return inventory[selected_item_idx];

    return std::shared_ptr<Item>(nullptr);
}

int Inventory::addToInventory(std::shared_ptr<Item> item) {

    if(inventory.size() >= limit)
        return -1;

    inventory.push_back(item);
    
    return 0;
}