#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#ifndef _DB_MAP_H_
#include "storage-system/db_manage.h"
#endif
#include <stdlib.h>

struct item{
    char nam[32];
	db_map l;///本体, 列名 对应 值；
	db_struct *db;

	item(const char name0[]){///new
        db_struct *_db=new db_struct(1);
        db=_db;
        char s1[32];
        strcpy(s1,name0);
        strcat(s1,"_l");
		if(db->exists_map(s1)){
			//printf("loading item %s ...\n",name0);
			l=db->get_map(s1);
			//printf("Item %s load done.\n",name0);
		}
		else{
			printf("creating item %s ...\n",name0);
			l=db->create_map(s1,'s','s');
			printf("Item %s create done.\n",name0);
			//printf("%s\n",l.name);
		}
		strcpy(nam,name0);
	}

    bool add_val(unsigned char k[],unsigned n){
        if(l.exists(k)){
            printf("Val exist!\n");
            return 0;
        }
        l.add(k,n);
        return 1;
    }
    bool add_val(unsigned char k[],unsigned char s[]){
        if(l.exists(k)){
            printf("Val exist!\n");
            return 0;
        }
        l.add(k,s);
        return 1;
    }

    bool delete_val(unsigned char k[]){
        if(!l.exists(k)){
            printf("Not exist!\n");
            return 0;
        }
        l.drop(k);
        return 1;
    }

	bool modify_val(unsigned char k[],unsigned i){
        if(!l.exists(k)){
            printf("Not exist!\n");
            return 0;
        }
		else {
            l.update(k,i);
		}
        return 1;
	}
	bool modify_val(unsigned char k[],unsigned char s[]){
        if(!l.exists(k)){
            printf("Not exist!\n");
            return 0;
        }
		else {
            l.update(k,s);
        }
        return 1;
	}

	bool get_val(unsigned char k[],unsigned int *i){
        if(!l.exists(k)){
            printf("Not exist!\n");
            return 0;
        }
		else {
			l.get_by_key(k, i);
		}
		return 1;
	}
	bool get_val(unsigned char k[],unsigned char s[]){
        if(!l.exists(k)){
            printf("Not exist!\n");
            return 0;
        }
		else {
			l.get_by_key(k, s);
		}
		return 1;
	}

    void clear_all(){
        int num=l.get_size();
        unsigned char s[num][32];
        l.get_all_key(s);
        for(int i=0;i<num;i++)
            l.drop(s[i]);
    }
    void delete_all(){
        char s[32];
        memset(s,0,sizeof(s));
        strcpy(s,nam);
        strcat(s,"_l");
        if(db->exists_map(s)){
        	db->delete_map(s);
        	//printf("item %s delete done.\n",s);
        }
    }

    ~item(){
        delete db;
    }
};

#endif // ITEM_H_INCLUDED
