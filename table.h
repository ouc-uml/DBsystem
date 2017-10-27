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

        if(db->exists_map(name_items)){
        	printf("loading now...\n");
            items=db->get_map(name_items);
            items_empty=db->get_list(name_items_empty);
            row=db->get_map(name_row);
			/*
                unsigned tmp[items_empty.size];
                items_empty.get_all_value(tmp);
                for(int jj=0;jj<items_empty.size;jj++)
                    printf("%d ",tmp[jj]);
                printf("These is empty numbers.\n");
			*/
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

   bool add_row(unsigned char s[],bool type){///添加名为s的int列。值为空
        if(row.exists(s))
        {
            printf("Row %s exist!\n",(char*)s);
            return 0;
        }
        r++;
        if(type)row.add(s,'d');
        else row.add(s,'s');

        int num=items.get_size();
        if(num==0)return 1;

        unsigned keys[num];
        items.get_all_key(keys);
        unsigned char tmp[32];
        for(int i=0;i<num;i++)
        {
            items.get_by_key(keys[i],tmp);
            item item_tmp((char*)tmp);
            if(type)item_tmp.add_val(s,(unsigned)0);
            else item_tmp.add_val(s,(unsigned char*)"NUL");
        }
        printf("Row %s add done!\n",(char*)s);
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
        for(int i=0;i<r;i++){
        	row.get_by_key(keys[i],&type);
        	if(type){
        		item_tem.add_val(keys[i],(unsigned int)0);
        	}
        	else {
        		item_tem.add_val(keys[i],(unsigned char*)"NUL");
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
            if(type){
                printf("A unsigned integer: \n");
                item_tmp.modify_val(keys[j],in_i);///int
            }
            else{
                printf("A string: \n");
                item_tmp.modify_val(keys[j],in_s);///string
            }
        }
        ///index

        return 1;
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
    //为名为s的列建立索引；
        if(!row.exists(ss)){
        	printf("There exists not row called %s!\n",(char*)ss);
        	return 0;
        }        
        if(index.exists(ss)){
            printf("Index for %s exsit. Now start to update\n",(char*)ss);
        }
        else printf("Now start to set index for %s...\n",ss);
        
        unsigned char s[32],s2[32],s3[32];///索引名,临时串,临时list名
        strcpy((char*)s,name);
        strcat((char*)s,(char*)ss);
        unsigned int type;
        row.get_by_key(ss,&type);
        char cc=type?'d':'s';

        db_map map_tmp=db->create_map((char*)s,cc,'s');///新索引
        index.add(ss,s);

        if(cc=='d'){
            unsigned int keys[c],val;
            items.get_all_key(keys);

            for(int j=0;j<c;j++){
                memset(s3,0,sizeof(s3));
                memset(s2,0,sizeof(s2));
                item item_tmp=get_item(keys[j]);
                item_tmp.get_val(ss,&val);

                if(map_tmp.exists(val)){
                	//printf("test1\n");
                    map_tmp.get_by_key(val,s3);
                    db_list list_tmp=db->get_list((char*)s3);
                    list_tmp.push_tail(keys[j]);
                }
                else{
                	//printf("test2\n");
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
        } else {
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
                            db_list list_tmp=db->create_list((char*)s3,'d');
                            list_tmp.push_tail(keys_s[j]);
                            map_tmp.add(s2,s3);
                        }
                    }
                }
                ///
            }
        }
        printf("Index for %s set done.\n",ss);
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
    	if(!type){
    	    printf("Row %s is type of string!\n",s);
    		return v;
    	}
    	unsigned char ind[32];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

    	unsigned char res[32];//读结果链表
    	if(!map_tmp.exists(val)){
    		printf("There exists not %d for %s!\n",val,s);
    		return v;
    	}
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
    //在名为s的列上搜索值为val(string)的条目，返回满足条件的item的编号的vector；
        vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}
    	//校验类型
    	unsigned int type;
    	row.get_by_key(s,&type);
    	if(type){
    	    printf("Row %s is type of unsigned int!\n",s);
    		return v;
    	}
    	unsigned char ind[32];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

    	unsigned char res[32];//读结果链表
    	if(!map_tmp.exists(val)){
    		printf("There exists not %s for %s!\n",val,s);
    		return v;
    	}
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

    void show_map(){

    	return ;
    }

    ~table(){
        delete db;
    }
};


#endif // TABLE_H_INCLUDED
