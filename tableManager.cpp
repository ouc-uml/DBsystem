#include "table.h"
#include<iostream>
#include<string>

int main(){
	load_configuration();
	db_struct db(1);
	std:: string s;
	bool flag = 1;
	
	table t("test");
	
	while(flag){
		std:cout<<">>";
		std::cin>>s;
		if(s=="add"){
				char c;
				//std::cout<<"add: ";
				std::cin>>c;
				if(c=='i'){
					t.add_item();
				}
				else if(c=='c'){
					unsigned char s_column[32];
					//std::cout<<"name: ";
					std::cin>>s_column;
					//std::cout<<"type: ";
					std::cin>>c;
					t.add_column(s_column,c);
				}
				else {
				
				}
		    	t.show_all();
		}else if(s=="edit"){
				unsigned int i,n;
				unsigned char c;
				unsigned char ss[v_len];
				//std::cout<<"NO.: ";
				std::cin>>i>>n;
				item item_tmp=t.get_item(i);
				unsigned char keys[t.c][k_len];
       			int num=t.get_all_column(keys);
				for(int j=0;j<num;j++){
					if(j>=n)break;
           			c=t.get_type(keys[j]);
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
		    	t.show_all();
		}else if(s=="delete"){
				unsigned int i;
				char c;
				//std::cout<<"NO.: ";
				std::cin>>i;
				std::cout<<"Do you want to delete Item No."<<i<<"?[ y/ n]"<<endl;
				std::cin>>c;
				if(c=='y')t.delete_item(i);
		    	t.show_all();
		}else if(s=="show"){
		    	t.show_all();
		}else if(s=="exit"){
				flag=0;
		}
		
	}
	return 0;
}
