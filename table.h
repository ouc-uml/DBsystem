#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include <vector>
#include "item.h"

/*索引在修改值时修改*/

struct table{
    char name[32];
    char name_items[32];
    char name_items_empty[32];
    char name_row[32];
    char name_index[32];
    db_map items;///条目编号 对应 条目名字
    db_map row;///属性 对应 属性类型
    db_map index;///具有索引的属性列 对应 索引名
    db_list items_empty;///暂空条目编号列
    db_struct *db;
    int c,r;///条目数与列数

    table(const char s[]){///建立空表，若已存在则读入
        strcpy(name,s);
        strcpy(name_items,s);
        strcpy(name_items_empty,s);
        strcpy(name_row,s);
        strcpy(name_index,s);

        strcat(name_items,"_items");
        strcat(name_items_empty,"_items_empty");
        strcat(name_row,"_row");
        strcat(name_index,"_index");
        db_struct *_db=new db_struct(1);
        db=_db;

        if(db->exists_map((char*)name_items)){
        	printf("loading now...\n");
            items=db->get_map(name_items);
            items_empty=db->get_list(name_items_empty);
            row=db->get_map(name_row);

				/*
                unsigned tmp[items_empty.size];
                items_empty.get_all_value(tmp);
                for(int jj=0;jj<items_empty.size;jj++)
                    printf("%d ",tmp[jj]);
                printf("\n");*/

            index=db->get_map(name_index);
            c=items.get_size();//-items_empty.size;
            r=row.get_size();
            ///load index
        }
        else{
            printf("creating now...\n");
            items=db->create_map(name_items,'d','s');
            items_empty=db->create_list(name_items_empty,'d');
            row=db->create_map(name_row,'s','d');
            index=db->create_map(name_index,'s','s');
            items_empty.push_tail(1);
            c=0;r=0;
            printf("create done.\n");
        }
    }

   bool add_row(unsigned char s[],char type){///添加名为s的int列。值为空
        if(row.exists(s))
        {
            printf("Row %s exist!\n",(char*)s);
            return 0;
        }
        r++;
        row.add(s,(unsigned int)type);
        printf("Row %s add done!\n",(char*)s);

        int num=items.get_size();
        if(num==0)return 1;

        unsigned keys[num];
        items.get_all_key(keys);
        unsigned char tmp[32];
        unsigned char nul[32]="NUL";
        for(int i=0;i<num;i++)
        {
            items.get_by_key(keys[i],tmp);
            item item_tmp((char*)tmp);
            if(type=='d')item_tmp.add_val(s,(unsigned)0);
            else {
            	item_tmp.add_val(s,nul);
            }
        }
        return 1;
    }

    //void delete_row(unsigned char s[]){}

    unsigned int add_item(){
        ///获取空编号
        unsigned int num;
        items_empty.get_by_index(0,&num);
        items_empty.pop_head();
        if(items_empty.size==0)items_empty.push_head(num+1);
        ///新建条目
        char s[32],s_tem[32];
        strcpy(s,name);
        strcat(s,"_");
        sprintf(s_tem, "%d", num);
        //itoa(num,s_tem,10);
        strcat(s,s_tem);
        item item_tem(s);
        unsigned char keys[r][32];
        row.get_all_key(keys);
        unsigned int type;
        unsigned char nul[32]="NUL";
        for(int i=0;i<r;i++){
        	row.get_by_key(keys[i],&type);
        	if(type=='d'){
        		item_tem.add_val(keys[i],(unsigned int)0);
        	}
        	else {
        		item_tem.add_val(keys[i],nul);
        	}
        }
        if(!items.exists(num)){
        	items.add(num,(unsigned char *)s);
        	printf("Item %d add done.\n",num);
        }

        c++;
        return num;
    }

    int get_all_no(unsigned int i[]){
		items.get_all_key(i);
		return c;
    }

    item get_item(unsigned int i){
        unsigned char s[32];
        if(!items.exists(i)){
        	printf("Error!\n");
        }
        items.get_by_key(i,s);
        item item_tmp((char *)s);
        return item_tmp;
    }

    bool edit_item(unsigned int i){
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }
        item item_tmp=get_item(i);
        unsigned char keys[r][32];
        row.get_all_key(keys);
        unsigned int type;
        unsigned in_i=0;
        unsigned char in_s[32]="NUL";
        for(int j=0;j<r;j++){
            row.get_by_key(keys[j],&type);
            if(type=='d'){
                printf("A unsigned integer for item %d, row %s: ",i,keys[j]);
                scanf("%d",&in_i);
                item_tmp.modify_val(keys[j],in_i);///int
            }
            else{
                printf("A string for item %d, row %s: ",i,keys[j]);
                scanf("%s",in_s);
                item_tmp.modify_val(keys[j],in_s);///string
            }
        }
        ///index

        return 1;
    }
    
    bool edit_item_1(unsigned int i){
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }
        item item_tmp=get_item(i);
        unsigned char keys[r][32];
        row.get_all_key(keys);
        unsigned int type;
        unsigned in_i=0;
        unsigned char in_s[32]="NUL";
        for(int j=0;j<r;j++){
            row.get_by_key(keys[j],&type);
            if(type=='d'){
                printf("A unsigned integer: \n");
                scanf("%d",&in_i);
                item_tmp.modify_val(keys[j],in_i);///int
            }
            else{
                printf("A string: \n");
                scanf("%s",in_s);
                item_tmp.modify_val(keys[j],in_s);///string
            }
        }
        
    }

    bool delete_item(unsigned int i){
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }

        items_empty.push_head(i);
       	c--;

        item item_tmp=get_item(i);
        item_tmp.clear_all();
        item_tmp.delete_all();

        ///index

        items.drop(i);
        printf("Item %d deleted done.\n",i);
    }

    bool set_index(unsigned char ss[]){
    //为名为ss的列建立索引；
    	if(!row.exists(ss)){
    		printf("Not exist row %s!\n",ss);
    		return 0;
    	}
        unsigned char s[32],s2[32],s3[32];///索引名,临时串,临时list名
        strcpy((char*)s,name);
        strcat((char*)s,(char*)ss);
        unsigned int type;
        row.get_by_key(ss,&type);
        if(index.exists(ss)){
            printf("Index for %s exsit. Now recreate.\n",(char*)ss);
            for(){/////////////
            	db_list list_tmp=db->get_list((char*)s3);
            }
        	db->delete_map((char*)s);
        }
        db_map map_tmp=db->create_map((char*)s,type,'s');///新索引
        if(!index.exists(ss))index.add(ss,s);

        if(type=='d'){
            unsigned int keys[c],val;
            items.get_all_key(keys);

            for(int j=0;j<c;j++){
                memset(s3,0,sizeof(s3));
                item item_tmp=get_item(keys[j]);
                item_tmp.get_val(ss,&val);

                if(map_tmp.exists(val)){
                    map_tmp.get_by_key(val,s3);
                    db_list list_tmp=db->get_list((char*)s3);
                    list_tmp.push_tail(keys[j]);
                }
                else{
                    ///表名+列名+键
                    strcpy((char*)s3,(char*)s);
                    sprintf((char*)s2,"%d",val);
                    //itoa(val,(char*)s2,10);
                    strcat((char*)s3,(char*)s2);
                    db_list list_tmp=db->create_list((char*)s3,'d');
                    list_tmp.push_tail(keys[j]);
                    map_tmp.add(val,s3);
                }
            }
        }else{
            unsigned char val[32];
            unsigned int keys_s[c];
            items.get_all_key(keys_s);
			
            for(int j=0;j<c;j++){///
                memset(s3,0,sizeof(s3));
                item item_tmp=get_item(keys_s[j]);
                item_tmp.get_val(ss,val);
                ///取val的所有子串
                for(int k1=0;k1<strlen((char*)val);k1++){
                    for(int k2=k1;k2<strlen((char*)val);k2++){
                        memset(s2,0,sizeof(s2));
                        memcpy(s2,val+k1,k2-k1+1);
                        if(map_tmp.exists(s2)){	 
                            map_tmp.get_by_key(s2,s3);
                            db_list list_tmp=db->get_list((char*)s3);
                            list_tmp.push_tail(keys_s[j]);
                        }else{
                            strcpy((char*)s3,(char*)s);
                            strcat((char*)s3,(char*)s2);
                            //printf("s2:%s\n",s2);
                            db_list list_tmp=db->create_list((char*)s3,'d');
                            list_tmp.push_tail(keys_s[j]);
                            map_tmp.add(s2,s3);
                        }
                    }
                }
                ///
            }
        }
        printf("Index set done.\n");
        return 1;
    }

    vector<unsigned int> find_by_index(unsigned char s[],unsigned int val){
    //在名为s的列上搜索值为val(无符号整型)的条目，返回满足条件的item的编号的vector；
    	vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}
    	//校验类型
    	unsigned int type;
    	row.get_by_key(s,&type);
    	if(type=='s'){
    	    printf("Row %s is type of string!\n",s);
    		return v;
    	}
    	unsigned char ind[32];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

    	unsigned char res[32];//读结果链表
    	map_tmp.get_by_key(val,res);
    	db_list list_tmp=db->get_list((char*)res);

    	unsigned int keys[list_tmp.size];//塞进vector返回
    	list_tmp.get_all_value(keys);
    	for(int j=0;j<list_tmp.size;j++){
    		v.push_back(keys[j]);
    	}
		return  v;
    }

    vector<unsigned int> find_by_index(unsigned char s[],unsigned char val[]){
    //在名为s的列上搜索值包含val(string)的条目，返回满足条件的item的编号的vector；
        vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}
    	//校验类型
    	unsigned int type;
    	row.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("Row %s is type of unsigned int!\n",s);
    		return v;
    	}
    	unsigned char ind[32];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

    	unsigned char res[32];//读结果链表
    	map_tmp.get_by_key(val,res);
    	db_list list_tmp=db->get_list((char*)res);

    	unsigned int keys[list_tmp.size];//塞进vector返回
    	list_tmp.get_all_value(keys);
    	for(int j=0;j<list_tmp.size;j++){
    		v.push_back(keys[j]);
    	}
		return  v;
    }


    void delete_talbe(){
    	unsigned int keys[c];
    	items.get_all_key(keys);
    	for(int i=0;i<c;i++){
    		delete_item(keys[i]);
    	}
    	db->delete_list(name_items_empty);
    	db->delete_map(name_items);
    	db->delete_map(name_row);
		//index

    	db->delete_map(name_index);

    	printf("table %s has been destroyed.\n",name);
    }

    void show_all(){
        int len=r;
        int num=items.get_size();
        unsigned char keys[num][32];
        unsigned char keys_i[len][32];
        unsigned char val[32],val_i[32];
        unsigned int val_ii,type;
        items.get_all_key(keys);
        row.get_all_key(keys_i);
        for(int i=0;i<r;i++)
            printf("%s ",keys_i[i]);
        printf("\n");
        for(int i=0;i<num;i++){
            items.get_by_key(keys[i],val);
            item item_tmp((char*)val);
            for(int j=0;j<len;j++){
                row.get_by_key(keys_i[j],&type);
                if(type=='d'){
                    item_tmp.l.get_by_key(keys_i[j],&val_ii);
                    printf("%d ",val_ii);
                }
                else{
                    item_tmp.l.get_by_key(keys_i[j],val_i);
                    printf("%s ",val_i);
                }
            }
            printf("\n");
        }
    }

    ~table(){
        delete db;
    }
};


#endif // TABLE_H_INCLUDED
