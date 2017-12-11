#include<iostream>
#include "table.h"

int main(){
	load_configuration();
	db_struct db(1);
	const char name_all_table[k_len]="all_table";
	db_map m=db.create_map(name_all_table,'s','d');
	return 0;
}
