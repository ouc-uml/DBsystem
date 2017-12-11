#include "table.h"
#include<iostream>

int main(){
	load_configuration();
	
	db_struct db(1);
	const char name_all_table[k_len]="all_table";
	db_map all_table=db.get_map(name_all_table);
	unsigned char all_tables[all_table.get_size()][k_len];
	
	all_table.get_all_key(all_tables);
	for(int i=0;i<all_table.get_size();i++){
		std::cout<<all_tables[i]<<endl;
	}
	
	std:: string s;
	table *t;
	std::cout<<"Choose a table: ";
	std::cin>>s;
	char sss[k_len]="";
	for(int i=0;i<s.size();i++){
		sss[i]=s[i];
	}
	t=new table(sss);
	
	bool flag = 1;	
	while(flag){
		std:cout<<">>";
		std::cin>>s;
		if(s==""){
		} else if(s=="add"){
				char c;
				//std::cout<<"add: ";
				std::cin>>c;
				if(c=='i'){
					t->add_item();
				}
				else if(c=='c'){
					unsigned char s_column[32];
					//std::cout<<"name: ";
					std::cin>>s_column;
					//std::cout<<"type: ";
					std::cin>>c;
					t->add_column(s_column,c);
				}
				else {
				
				}
		    	t->show_all();
		}else if(s=="edit"){
				unsigned int i,n;
				unsigned char c;
				unsigned char ss[v_len];
				//std::cout<<"NO.: ";
				std::cin>>i>>n;
				item item_tmp=t->get_item(i);
				unsigned char keys[t->c][k_len];
       			int num=t->get_all_column(keys);
				for(int j=0;j<num;j++){
					if(j>=n)break;
           			c=t->get_type(keys[j]);
					if(c=='d'){
						unsigned int val;
						std::cin>>ss;
						val=ss[0]-'0';
                		item_tmp.modify_val(keys[j],val);///int
					}else{
						std::cin>>ss;
                		item_tmp.modify_val(keys[j],ss);///string
					}
				}
		    	t->show_all();
		}else if(s=="delete"){
				unsigned int i;
				char c;
				//std::cout<<"NO->: ";
				std::cin>>i;
				std::cout<<"Do you want to delete Item No."<<i<<"?[ y/ n]"<<endl;
				std::cin>>c;
				if(c=='y')t->delete_item(i);
		    	t->show_all();
		}else if(s=="show"){
		    	t->show_all();
		}else if(s=="exit"){
				flag=0;
		}else if(s=="change"){
			delete t;	
			all_table.get_all_key(all_tables);
			for(int i=0;i<all_table.get_size();i++){
				std::cout<<all_tables[i]<<endl;
			}
			std::cout<<"Choose a table: ";
			std::cin>>s;
	
			char ssss[k_len]="";
			for(int i=0;i<s.size();i++){
				ssss[i]=s[i];
			}
			t=new table(ssss);
		}
		
	}
	delete t;
	return 0;
}
