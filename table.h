#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <vector>
#include <string>
#include <functional>
#include "item.h"

unsigned int Hash(char *str){//BKDR字符串哈希
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str){
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}


// 操作函数囊括 增 查 改 删 管理

struct table{
    char name[k_len];
    char name_items[k_len];
    char name_items_empty[k_len];
    char name_column[k_len];
    char name__column[k_len];
    char name_index[k_len];
    db_map items;///元组编号 对应 元组名字
    db_map column;///属性 对应 属性类型
    db_map _column;///属性编号 对应 属性
    db_map index;///具有索引的属性列 对应 索引名
    db_list items_empty;///暂空元组编号列
    db_struct *db;
    int c,r;///列数,元组数
    //int c_d;//被删除的元组数


    table(const char s[]){///建立空表，若已存在则读入
        strcpy(name,s);
        strcpy(name_items,s);
        strcpy(name_items_empty,s);
        strcpy(name_column,s);
        strcpy(name__column,s);
        strcpy(name_index,s);

        strcat(name_items,"_items");
        strcat(name_items_empty,"_items_empty");
        strcat(name_column,"_column");
        strcat(name__column,"__column");
        strcat(name_index,"_index");
        db_struct *_db=new db_struct(1);
        db=_db;

        if(db->exists_map((char*)name_items)){
        	printf("loading now...\n");
            items=db->get_map(name_items);
            items_empty=db->get_list(name_items_empty);
            column=db->get_map(name_column);
            _column=db->get_map(name__column);

				/*
                unsigned tmp[items_empty.size];
                items_empty.get_all_value(tmp);
                for(int jj=0;jj<items_empty.size;jj++)
                    printf("%d ",tmp[jj]);
                printf("\n");*/

            index=db->get_map(name_index);
            r=items.get_size();//-items_empty.size;
            c=column.get_size();
            //c_d=_column.get_size();c_d-=c;
        }else{
            printf("creating now...\n");
            items=db->create_map(name_items,'d','s');
            items_empty=db->create_list(name_items_empty,'d');
            column=db->create_map(name_column,'s','d');
            _column=db->create_map(name__column,'d','s');
            index=db->create_map(name_index,'s','s');
            items_empty.push_tail(1);
            c=0;r=0;//c_d=0;
            printf("create done.\n");
        }
    }


    bool add_column(unsigned char s[],char type){///添加名为s的int列。值为空
        if(column.exists(s)){
            printf("column %s exist!\n",(char*)s);
            return 0;
        }
        c++;
        column.add(s,(unsigned int)type);
        _column.add(c,s);
        printf("column %s add done!\n",(char*)s);

        int num=items.get_size();
        if(num==0)return 1;

        unsigned keys[num];
        items.get_all_key(keys);
        unsigned char tmp[v_len];
        unsigned char nul[v_len]="";
        for(int i=0;i<num;i++){
            items.get_by_key(keys[i],tmp);
            item item_tmp((char*)tmp);
            if(type=='d')item_tmp.add_val(s,(unsigned)0);
            else {
            	item_tmp.add_val(s,nul);
            }
        }
        return 1;
    }

    bool add_column(unsigned char s[],char type,unsigned int ini){
    ///添加名为s的int列。值为空
        if(column.exists(s)){
            printf("column %s exist!\n",(char*)s);
            return 0;
        }
        c++;
        column.add(s,(unsigned int)type);
        _column.add(c,s);
        printf("column %s add done!\n",(char*)s);

        int num=items.get_size();
        if(num==0)return 1;

        unsigned keys[num];
        items.get_all_key(keys);
        unsigned char tmp[v_len];
        unsigned char nul[v_len]="";
        for(int i=0;i<num;i++)
        {
            items.get_by_key(keys[i],tmp);
            item item_tmp((char*)tmp);
            if(type=='d')item_tmp.add_val(s,ini);
            else {
            	printf("Un match!\n");
            	item_tmp.add_val(s,nul);
            }
        }
        return 1;
    }

    bool add_column(unsigned char s[],char type,const char ini[]){
    ///添加名为s的int列。值为空
        if(column.exists(s)){
            printf("column %s exist!\n",(char*)s);
            return 0;
        }
        c++;
        column.add(s,(unsigned int)type);
        _column.add(c,s);
        printf("column %s add done!\n",(char*)s);

        int num=items.get_size();
        if(num==0)return 1;

        unsigned keys[num];
        items.get_all_key(keys);
        unsigned char tmp[v_len];
        for(int i=0;i<num;i++){
            items.get_by_key(keys[i],tmp);
            item item_tmp((char*)tmp);
            if(type=='d'){
            	printf("Un match!\n");
            	item_tmp.add_val(s,(unsigned)0);
            } else {
            	item_tmp.add_val(s,(unsigned char*)ini);
            }
        }
        return 1;
    }//*/

    unsigned int add_item(){
        ///获取空编号
        unsigned int num;
        items_empty.get_by_index(0,&num);
        items_empty.pop_head();
        if(items_empty.size==0)items_empty.push_head(num+1);
        ///新建元组
        char s[k_len],s_tem[k_len];
        strcpy(s,name);
        strcat(s,"_");
        sprintf(s_tem, "%d", num);
        //itoa(num,s_tem,10);
        strcat(s,s_tem);
        item item_tem(s);

        //添加值
        unsigned char keys[c][k_len];
        column.get_all_key(keys);
        unsigned int type;
        unsigned char nul[v_len]="";
        for(int i=0;i<c;i++){
        	column.get_by_key(keys[i],&type);
        	if(type=='d'){
        		item_tem.add_val(keys[i],(unsigned int)0);
        		update_index(keys[i],num,(unsigned int)0,(unsigned int)0,1);
        	} else {
        		item_tem.add_val(keys[i],nul);
        		update_index(keys[i],num,nul,nul,1);
        	}
        }

        if(!items.exists(num)){
        	items.add(num,(unsigned char *)s);
        	printf("Item %d add done.\n",num);
        }

        r++;
        return num;
    }

    unsigned int add_item(unsigned int len,...){
        if(len>c){
            printf("There only %d columns in the table!\n",c);
            return 0;
        }
      	va_list args;
    	va_start(args,len);
    	int len_2=len;
        ///获取空编号
        unsigned int num;
        items_empty.get_by_index(0,&num);
        items_empty.pop_head();
        if(items_empty.size==0)items_empty.push_head(num+1);
        ///新建元组
        char s[k_len],s_tem[k_len];
        strcpy(s,name);
        strcat(s,"_");
        sprintf(s_tem, "%d", num);
        //itoa(num,s_tem,10);
        strcat(s,s_tem);
        item item_tem(s);

        //添加值
        unsigned char keys[c][v_len];
        //column.get_all_key(keys);
        unsigned int type;
        for(int i=0;i<c;i++){
        	_column.get_by_key((unsigned)(i+1),keys[i]);
        	column.get_by_key(keys[i],&type);
        	if(i<len_2){//有输入
		    	if(type=='d'){
		    		unsigned int in_i=va_arg(args,unsigned int);
		    		item_tem.add_val(keys[i],in_i);
        			update_index(keys[i],num,(unsigned int)0,in_i,1);
		    	} else {
		    		unsigned char* in_s=va_arg(args,unsigned char*);
		    		item_tem.add_val(keys[i],in_s);
        			update_index(keys[i],num,in_s,in_s,1);
		    	}
        	} else {//没输入
        		if(type=='d'){
		    		item_tem.add_val(keys[i],(unsigned)0);
        			update_index(keys[i],num,(unsigned int)0,(unsigned int)0,1);
        		} else {
        			unsigned char nul[v_len]="";
		    		item_tem.add_val(keys[i],nul);
        			update_index(keys[i],num,nul,nul,1);
		    	}
        	}
        }

        if(!items.exists(num)){
        	items.add(num,(unsigned char *)s);
        	printf("Item %d add done.\n",num);
        }

        r++;
        return num;
    }

    unsigned int add_item(vector<string> v){
    	///获取空编号
        unsigned int num;
        items_empty.get_by_index(0,&num);
        items_empty.pop_head();
        if(items_empty.size==0)items_empty.push_head(num+1);
        ///新建元组
        char s[k_len],s_tem[k_len];
        strcpy(s,name);
        strcat(s,"_");
        sprintf(s_tem, "%d", num);
        strcat(s,s_tem);
        item item_tem(s);

        //添加值
        unsigned char keys[c][v_len];
        //column.get_all_key(keys);
        unsigned int type;
        for(int i=0;i<c;i++){
        	_column.get_by_key((unsigned)(i+1),keys[i]);
        	column.get_by_key(keys[i],&type);
        	if(i<v.size()){
		    	if(type=='d'){
		    		unsigned char s_tmp[v_len];
		    		const char* s_tem=v[i].c_str();
		    		strcpy((char*)s_tmp,s_tem);
		    		unsigned int in_i=0;
		    		int len=strlen(s_tem);
		    		for(int j=0;j<len;j++){
		    			in_i*=10;
		    			in_i+=(s_tmp[j]-'0');
		    		}
		    		item_tem.add_val(keys[i],in_i);
        			update_index(keys[i],num,in_i,in_i,1);
		    	} else {
		    		unsigned char s_tmp[v_len];
		    		const char* s_tem=v[i].c_str();
		    		strcpy((char*)s_tmp,s_tem);
		    		item_tem.add_val(keys[i],s_tmp);
        			update_index(keys[i],num,s_tmp,s_tmp,1);
		    	}
        	} else {
        		if(type=='d'){
		    		unsigned int in_i=0;
		    		item_tem.add_val(keys[i],in_i);
        			update_index(keys[i],num,in_i,in_i,1);
        		} else {
		    		unsigned char in_s[v_len]="";
		    		item_tem.add_val(keys[i],in_s);
        			update_index(keys[i],num,in_s,in_s,1);
        		}
        	}
        }

        if(!items.exists(num)){
        	items.add(num,(unsigned char *)s);
        	printf("Item %d add done.\n",num);
        }

        r++;
        return num;
    }


	vector<string> get_column_s(unsigned char s[]){
		vector<string> v;
		if(!column.exists(s)){
    		printf("There is no column '%s' !\n",s);
    		return v;
    	}//检查存在
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    		printf("Type of column '%s' is unsigned int!\n",s);
    		return v;
    	}//检查类型
	if(r==0){
		printf("There is no item in this table!\n");
		return v;
	}

        unsigned char tmp[v_len];
        unsigned int keys[r];

        items.get_all_key(keys);
        string nul="";
        v.push_back(nul);

        for(int i=0;!i;i++){
        	memset(tmp,0,sizeof(tmp));
        	item item_tmp=get_item(keys[i]);
        	item_tmp.get_val(s,tmp);
        	string ss=(char*)tmp;
        	for(int j=1;j<keys[i]-1;j++)v.push_back(nul);
        	v.push_back(ss);
        }

        for(int i=1;i<r;i++){
        	memset(tmp,0,sizeof(tmp));
        	item item_tmp=get_item(keys[i]);
        	item_tmp.get_val(s,tmp);
        	string ss=(char*)tmp;
        	for(int j=1;j<keys[i]-keys[i-1];j++)v.push_back(nul);
        	v.push_back(ss);
        }

    	return v;
	}

	vector<unsigned int> get_column_i(unsigned char s[]){
		vector<unsigned int> v;
		if(!column.exists(s)){
    		printf("There is no column '%s' !\n",s);
    		return v;
    	}//检查存在
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='s'){
    		printf("Type of column '%s' is string!\n",s);
    		return v;
    	}//检查类型
	if(r==0){
		printf("There is no item in this table!\n");
		return v;
	}

    	unsigned int tmp;
        unsigned int keys[r];

        items.get_all_key(keys);
        v.push_back((unsigned)0);

        for(int i=0;!i;i++){
        	item item_tmp=get_item(keys[i]);
        	item_tmp.get_val(s,&tmp);
        	for(int j=1;j<keys[i]-1;j++)v.push_back((unsigned)0);
        	v.push_back(tmp);
        }

        for(int i=1;i<r;i++){
        	item item_tmp=get_item(keys[i]);
        	item_tmp.get_val(s,&tmp);
        	for(int j=1;j<keys[i]-keys[i-1];j++)v.push_back((unsigned)0);
        	v.push_back(tmp);
        }

    	return v;
	}

    int get_all_no(unsigned int i[]){
		items.get_all_key(i);
		return r;
    }

    int get_all_column(unsigned char i[][k_len]){
    	unsigned char s[v_len];
		for(int j=1;j<=c;j++){
			_column.get_by_key(j,s);
			for(int k=0;k<32;k++){
				i[j-1][k]=s[k];
			}
		}
		return c;
    }

    char get_type(unsigned char i[]){
    	if(!column.exists(i)){
    		printf("There is no column '%s' !\n",i);
    		return 'd';
    	}
    	unsigned int ch;
    	column.get_by_key(i,&ch);
    	return ch;
    }

    item get_item(unsigned int i){
        unsigned char s[v_len];
        if(!items.exists(i)){
        	printf("Error!\n");
        }
        items.get_by_key(i,s);
        item item_tmp((char *)s);
        return item_tmp;
    }

    bool set_index(unsigned char ss[]){
    //为名为ss的列建立索引；
    	if(!column.exists(ss)){
    		printf("Not exist column %s!\n",ss);
    		return 0;
    	}
        unsigned char s[v_len],s2[v_len],s3[v_len];///索引名,临时串,临时list名
        strcpy((char*)s,name);
        strcat((char*)s,(char*)ss);
        unsigned int type;
        column.get_by_key(ss,&type);
        if(index.exists(ss)){
            printf("Index for %s exsit!\n",(char*)ss);
            return 0;
            /*重建
            db_map map_tmp=db->get_map((char*)s);
            int num=map_tmp.get_size();
            unsigned char all_keys[num][k_len];
            map_tmp.get_all_key(all_keys);
            for(int i=0;i<num;i++){/////////////
            	memset(s3,0,sizeof(s3));
            	map_tmp.get_by_key(all_keys[i],s3);
				//cout<<"list name: "<<s3<<endl;
            	db->delete_list((char*)s3);
            }
        	db->delete_map((char*)s);
        	*/
        }
        db_map map_tmp=db->create_map((char*)s,type,'s');///新索引
        if(!index.exists(ss))index.add(ss,s);

        if(type=='d'){
            unsigned int keys[r],val;
            items.get_all_key(keys);

            for(int j=0;j<r;j++){
                memset(s3,0,sizeof(s3));
                item item_tmp=get_item(keys[j]);
                item_tmp.get_val(ss,&val);

                if(map_tmp.exists(val)){
                    map_tmp.get_by_key(val,s3);
                    db_list list_tmp=db->get_list((char*)s3);
                    list_tmp.push_tail(keys[j]);
                } else {
                    ///表名+列名+键
                    strcpy((char*)s3,(char*)s);
                    sprintf((char*)s2,"%d",val);
                    //itoa(val,(char*)s2,10);
                    strcat((char*)s3,(char*)s2);
        			//sprintf((char*)s3, "%d", Hash((char*)s3));
                    db_list list_tmp=db->create_list((char*)s3,'d');
                    list_tmp.push_tail(keys[j]);
                    map_tmp.add(val,s3);
                }
            }
        } else {
            unsigned char val[v_len];
            unsigned int keys_s[r];
            items.get_all_key(keys_s);

            for(int j=0;j<r;j++){///
                memset(s3,0,sizeof(s3));

                item item_tmp=get_item(keys_s[j]);
                item_tmp.get_val(ss,val);

                ///取val的所有长度大于等于2的子串
                for(int k1=0;k1<strlen((char*)val);k1++){
                    for(int k2=k1+1;k2<strlen((char*)val);k2++){
                        memset(s2,0,sizeof(s2));
                        memcpy(s2,val+k1,k2-k1+1);
                        if(map_tmp.exists(s2)){
                            map_tmp.get_by_key(s2,s3);
                            db_list list_tmp=db->get_list((char*)s3);
                            list_tmp.push_tail(keys_s[j]);
                        } else {
                            strcpy((char*)s3,(char*)s);
                            strcat((char*)s3,(char*)s2);
        					sprintf((char*)s3, "%d", Hash((char*)s3));
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

    bool update_index(unsigned char s[],unsigned int i,
    				  unsigned int val1,unsigned int val2,
    				  int mode){//mode:1:add 2: delete 3:edit
    	if(!index.exists(s)){
    	    return 0;
    	}

       	unsigned char ind[v_len];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);
    	unsigned char res[v_len];
		if(mode!=1){
			////暴力删除链表中节点
			map_tmp.get_by_key(val1,res);//读结果链表
			db_list list_tmp=db->get_list((char*)res);
			unsigned int keys[list_tmp.size];
			list_tmp.get_all_value(keys);
			int ii=0;
			while(ii<list_tmp.size){
				if(keys[ii]!=i)ii++;
				else break;
			}
			for(;ii<list_tmp.size-1;ii++){
				list_tmp.update(ii,keys[ii+1]);
			}
			list_tmp.pop_tail();
			if(list_tmp.size==0){//如果list里没东西了就删掉?
				//db->delete_list((char*)res);
			}
		}

    	//如果是删除就返回了
    	if(mode==2) return 1;

    	//加入另一个链表
    	if(map_tmp.exists(val2)){
    		map_tmp.get_by_key(val2,res);//读结果链表
    		db_list list_tmp2=db->get_list((char*)res);
            list_tmp2.push_tail(i);
    	} else {
       		///表名+列名+键
       		unsigned char s_l[k_len],s_n[k_len];
       		strcpy((char*)s_l,name);
       		strcat((char*)s_l,(char*)s);
            sprintf((char*)s_n,"%d",val2);
            strcat((char*)s_l,(char*)s_n);
        	//sprintf((char*)s_l, "%d", Hash((char*)s_l));
            db_list list_tmp2=db->create_list((char*)s_l,'d');
            list_tmp2.push_tail(i);
            map_tmp.add(val2,s_l);
        }

    	return 1;
    }

    bool update_index(unsigned char s[],unsigned int i,
    				  unsigned char val1[],unsigned char val2[],
    				  int mode){//mode:1:add 2: delete 3:edit
    	if(!index.exists(s)){
    	    return 0;
    	}

       	unsigned char ind[v_len];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);
    	unsigned char res[v_len];
    	unsigned char s_tmp[v_len];

    	if(mode!=1){
			for(int k1=0;k1<strlen((char*)val1);k1++){///取val１所有长度大于等于2的子串
				for(int k2=k1+1;k2<strlen((char*)val1);k2++){//暴力删除链表中节点
					memset(s_tmp,0,sizeof(s_tmp));
		            memcpy(s_tmp,val1+k1,k2-k1+1);
					map_tmp.get_by_key(s_tmp,res);//读结果链表
					db_list list_tmp=db->get_list((char*)res);
					unsigned int keys[list_tmp.size];
					list_tmp.get_all_value(keys);
					int ii=0;
					while(ii<list_tmp.size){
						if(keys[ii]!=i)ii++;
						else break;
					}
					for(;ii<list_tmp.size-1;ii++){
						list_tmp.update(ii,keys[ii+1]);
					}
					list_tmp.pop_tail();
					if(list_tmp.size==0){//如果list里没东西了就删掉?
						//db->delete_list((char*)res);
					}
				}
		    }//*/
        }

    	//如果是删除就返回了
    	if(mode==2) return 1;

		for(int k1=0;k1<strlen((char*)val2);k1++){///取val2所有长度大于等于2的子串
			for(int k2=k1+1;k2<strlen((char*)val2);k2++){//加入另一个链表
				memset(s_tmp,0,sizeof(s_tmp));
                memcpy(s_tmp,val2+k1,k2-k1+1);
                if(map_tmp.exists(s_tmp)){
                    map_tmp.get_by_key(s_tmp,res);
                    db_list list_tmp=db->get_list((char*)res);
                    list_tmp.push_tail(i);
                } else {
                	unsigned char s_l[k_len];
       				strcpy((char*)s_l,name);
       				strcat((char*)s_l,(char*)s);
            		strcat((char*)s_l,(char*)s_tmp);
            		sprintf((char*)s_l, "%d", Hash((char*)s_l));

                    db_list list_tmp=db->create_list((char*)s_l,'d');
                    list_tmp.push_tail(i);
                    map_tmp.add(s_tmp,s_l);
               }
			}
    	}

    	return 1;
    }

    vector<unsigned int> find_val(unsigned char s[],unsigned int val){
       if(index.exists(s))
            return find_by_index(s,val);
       else return find_from_all(s,val);
    }

    vector<unsigned int> find_val(unsigned char s[],unsigned int val[]){
       if(index.exists(s)&&strlen((char*)val)>=2)
            return find_by_index(s,val);
       else return find_from_all(s,val);
    }

    vector<unsigned int> find_val_eql(unsigned char s[],unsigned int val[]){
       if(index.exists(s)&&strlen((char*)val)>=2)
            return find_by_index_eql(s,val);
       else return find_from_all_eql(s,val);
    }

    vector<unsigned int> find_by_index_eql(unsigned char s[],unsigned char val[]){
    //在名为s的列上利用索引搜索值包含val(string)的元组
    //返回满足条件的item的编号的vector；
        vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}//校验存在
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("column %s is type of unsigned int!\n",s);
    		return v;
    	}//校验类型
    	if(strlen((char*)val)<2){
    	    printf("Please enter at least 2 chars!\n");
    		return v;
    	}//至少输入两个字符

    	unsigned char ind[v_len];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

		if(!map_tmp.exists(val)){
			printf("There is no item include val:  %s\n",val);
			return v;
		}
    	unsigned char res[v_len];//读结果链表
    	map_tmp.get_by_key(val,res);
    	db_list list_tmp=db->get_list((char*)res);
		//printf("%d\n",list_tmp.self.filenum);

    	unsigned int keys[list_tmp.size];//塞进vector返回
    	list_tmp.get_all_value(keys);
    	for(int j=0;j<list_tmp.size;j++){
            item item_tmp=get_item(keys[j]);
            char sss[v_len];
            item_tmp.get_val(s,(unsigned char*)sss);
            if(strcmp(sss,(char*)val)==0)
                v.push_back(keys[j]);
    	}
		return  v;
    }


    vector<unsigned int> find_by_index(unsigned char s[],unsigned int val){
    //在名为s的列上利用索引搜索值为val(无符号整型)的元组
    //返回满足条件的item的编号的vector；
    	vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='s'){
    	    printf("column %s is type of string!\n",s);
    		return v;
    	}//校验类型

    	unsigned char ind[v_len];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

		if(!map_tmp.exists(val)){
			printf("There is no item include val:  %d\n",val);
			return v;
		}
    	unsigned char res[v_len];//读结果链表
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
    //在名为s的列上利用索引搜索值包含val(string)的元组
    //返回满足条件的item的编号的vector；
        vector<unsigned int> v;
    	if(!index.exists(s)){
    	    printf("Index for %s doesn't exsit!\n",s);
    		return  v;
    	}//校验存在
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("column %s is type of unsigned int!\n",s);
    		return v;
    	}//校验类型
    	if(strlen((char*)val)<2){
    	    printf("Please enter at least 2 chars!\n");
    		return v;
    	}//至少输入两个字符

    	unsigned char ind[v_len];
    	index.get_by_key(s,ind);//读该列索引
    	db_map map_tmp=db->get_map((char*)ind);

		if(!map_tmp.exists(val)){
			printf("There is no item include val:  %s\n",val);
			return v;
		}
    	unsigned char res[v_len];//读结果链表
    	map_tmp.get_by_key(val,res);
    	db_list list_tmp=db->get_list((char*)res);
		//printf("%d\n",list_tmp.self.filenum);

    	unsigned int keys[list_tmp.size];//塞进vector返回
    	list_tmp.get_all_value(keys);
    	for(int j=0;j<list_tmp.size;j++){
    		v.push_back(keys[j]);
    	}
		return  v;
    }

    vector<unsigned int> find_from_all(unsigned char s[],unsigned int val){
        //在名为s的列上搜索值为val(无符号整型)的元组，返回满足条件的item的编号的vector；
    	vector<unsigned int> v;
    	if(!column.exists(s)){
    	    printf("Column %s doesn't exsit!\n",s);
    		return  v;
    	}
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='s'){
    	    printf("column %s is type of string!\n",s);
    		return v;
    	}//校验类型

    	vector<unsigned int> v_tmp = get_column_i(s);
    	for(unsigned int i=1;i<v_tmp.size();i++){
    		if(items.exists(i)){
    			if(v_tmp[i]==val) v.push_back(i);
    		}
    	}

    	return v;
    }

    vector<unsigned int> find_from_all(unsigned char s[],unsigned char val[]){
        //在名为s的列上搜索包含val(string)的元组，返回满足条件的item的编号的vector；
    	vector<unsigned int> v;
    	if(!column.exists(s)){
    	    printf("Column %s doesn't exsit!\n",s);
    		return  v;
    	}
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("column %s is type of unsigned int!\n",s);
    		return v;
    	}//校验类型

    	vector<string> v_tmp = get_column_s(s);
    	for(unsigned int i=1;i<v_tmp.size();i++){
    		if(items.exists(i)){
    			if(v_tmp[i].find((char*)val)!=string::npos) v.push_back(i);
    		}
    	}

    	return v;
    }

    vector<unsigned int> find_from_all_eql(unsigned char s[],unsigned char val[]){
        //在名为s的列上搜索包含val(string)的元组，返回满足条件的item的编号的vector；
    	vector<unsigned int> v;
    	if(!column.exists(s)){
    	    printf("Column %s doesn't exsit!\n",s);
    		return  v;
    	}
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("column %s is type of unsigned int!\n",s);
    		return v;
    	}//校验类型

    	vector<string> v_tmp = get_column_s(s);
    	for(unsigned int i=1;i<v_tmp.size();i++){
    		if(items.exists(i)){
    		    string sss=(char*)val;
    			if(v_tmp[i]==sss) {
                        v.push_back(i);
    			}
    		}
    	}

    	return v;
    }


    bool edit_item(unsigned int i){
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }

        item item_tmp=get_item(i);
        unsigned char keys[c][v_len];
        //column.get_all_key(keys);
        unsigned int type,in_i=0,in_i0=0;
        unsigned char in_s[v_len]="",in_s0[v_len]="";

        for(int j=0;j<c;j++){
        	_column.get_by_key((unsigned)(j+1),keys[j]);
            column.get_by_key(keys[j],&type);
            if(type=='d'){
            	item_tmp.get_val(keys[j],&in_i0);
                printf("Now: %d\nNew for item %d, column %s: ",in_i0,i,keys[j]);
                scanf("%d",&in_i);
                item_tmp.modify_val(keys[j],in_i);///int
        		update_index(keys[j],i,in_i0,in_i,3);
            } else {
            	item_tmp.get_val(keys[j],in_s0);
                printf("Now: %s\nNew for item %d, column %s: ",in_s0,i,keys[j]);
                scanf("%s",in_s);
                item_tmp.modify_val(keys[j],in_s);///string
        		update_index(keys[j],i,in_s0,in_s,3);
            }
        }
        ///index

        return 1;
    }

    bool edit_item(unsigned int i,unsigned int ii,unsigned int len,...){
    	//unsigned char s[]
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }
        if(len+ii-1>c){
            printf("There only %d columns in the table!\n",c);
            return 0;
        }

    	va_list args;
    	va_start(args,len);
        item item_tmp=get_item(i);
        unsigned char keys[c][v_len];
        //column.get_all_key(keys);
        unsigned int type;
        for(int j=0;j<len;j++){
        	_column.get_by_key((unsigned)(j+ii),keys[j]);
            column.get_by_key(keys[j],&type);
            if(type=='d'){
            	unsigned in_i0,in_i=va_arg(args,unsigned int);
            	item_tmp.get_val(keys[j],&in_i0);
                item_tmp.modify_val(keys[j],in_i);///int
        		update_index(keys[j],i,in_i0,in_i,3);
            } else {
            	unsigned char in_s0[v_len];
        		unsigned char* in_s=va_arg(args,unsigned char *);
            	item_tmp.get_val(keys[j],in_s0);
            	//printf("!!!%s\n",in_s);
                item_tmp.modify_val(keys[j],in_s);///string
        		update_index(keys[j],i,in_s0,in_s,3);
            }
        }
        return 1;
    }

    bool edit_item(unsigned int i,unsigned int ii,vector<string> v){
    //修改元组i 从位置 ii 开始的若干个属性
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }

        item item_tem=get_item(i);
    	unsigned char keys[c][v_len];
        //column.get_all_key(keys);
        unsigned int type;

        for(int k=ii-1;k<(c>v.size()+ii-1?v.size()+ii-1:c);k++){
        	_column.get_by_key((unsigned)(k+1),keys[k]);
        	column.get_by_key(keys[k],&type);
        	if(type=='d'){
        		unsigned char s_tmp[v_len];
        		const char* s_tem=v[k].c_str();
        		strcpy((char*)s_tmp,s_tem);
        		unsigned int in_i0,in_i=0;
        		int len=strlen(s_tem);
        		for(int j=0;j<len;j++){
        			in_i*=10;
        			in_i+=(s_tmp[j]-'0');
        		}
            	item_tem.get_val(keys[k],&in_i0);
        		item_tem.modify_val(keys[k],in_i);
        		update_index(keys[k],i,in_i0,in_i,3);
        	} else {
        		unsigned char s_tmp[v_len],in_s0[v_len];
        		const char* s_tem=v[k].c_str();
        		strcpy((char*)s_tmp,s_tem);
        		item_tem.get_val(keys[k],in_s0);
        		item_tem.modify_val(keys[k],s_tmp);
        		update_index(keys[k],i,in_s0,s_tmp,3);
        	}
        }
        return 1;
    }

	bool edit_items(unsigned char s[],vector<unsigned int>v,unsigned int val){
		if(!column.exists(s)){
    	    printf("Column %s doesn't exsit!\n",s);
    		return  0;
    	}//校验存在

    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='s'){
    	    printf("column %s is type of string!\n",s);
    		return 0;
    	}//校验类型

    	unsigned int in_i0;
    	for(int j=0;j<v.size();j++){
    		unsigned int i=v[j];
    		if(!items.exists(i)){
    	    	printf("Item %d doesn't exsit!\n",i);
    			continue;
    		}
    		item item_tmp=get_item(i);
    		item_tmp.get_val(s,&in_i0);
    		item_tmp.modify_val(s,val);
        	update_index(s,i,in_i0,val,3);
    	}

		return 1;
	}

	bool edit_items(unsigned char s[],vector<unsigned>v,unsigned char val[]){
		if(!column.exists(s)){
    	    printf("Column %s doesn't exsit!\n",s);
    		return  0;
    	}//校验存在
    	unsigned int type;
    	column.get_by_key(s,&type);
    	if(type=='d'){
    	    printf("column %s is type of unsigned int!\n",s);
    		return 0;
    	}//校验类型

    	unsigned char in_s0[v_len];
    	for(int j=0;j<v.size();j++){
    		unsigned int i=v[j];
    		if(!items.exists(i)){
    	    	printf("Item %d doesn't exsit!\n",i);
    			continue;
    		}
    		item item_tmp=get_item(i);
    		item_tmp.get_val(s,in_s0);
    		item_tmp.modify_val(s,val);
        	update_index(s,i,in_s0,val,3);
    	}

		return 1;
	}


    bool delete_item(unsigned int i){
        if(!items.exists(i)){
            printf("Item %d doesn't exist!\n",i);
            return 0;
        }

        items_empty.push_head(i);
       	r--;

        item item_tmp=get_item(i);

        ///index
		unsigned int type,num=index.get_size(),in_i0,in_i;
		unsigned char in_s0[v_len],in_s;
		unsigned char keys[num][k_len];
		index.get_all_key(keys);
		for(int i=0;i<num;i++){
			column.get_by_key(keys[i],&type);
			if(type=='d'){
				item_tmp.get_val(keys[i],&in_i0);
				update_index(keys[i],num,in_i0,in_i0,2);
			} else {
				item_tmp.get_val(keys[i],in_s0);
				update_index(keys[i],num,in_s0,in_s0,2);
			}
		}

        item_tmp.delete_all();

        items.drop(i);
        printf("Item %d deleted done.\n",i);
        return 1;
    }

    bool delete_column(unsigned char s[])
    {
		if(!column.exists(s)){
    		printf("There is no column '%s' !\n",s);
    		return 0;
    	}//检查存在

    	unsigned int i;

    	//删除该列索引
    	if(index.exists(s)){
    		unsigned char s1[v_len];
			index.get_by_key(s,s1);
			db_map map_tmp=db->get_map((char*)s1);

			int num2=map_tmp.get_size();
			unsigned char all_lists[num2][k_len];
			index.get_all_key(all_lists);

			unsigned char s2[v_len];
			for(int j=0;j<num2;j++){
				map_tmp.get_by_key(all_lists[j],s2);
				db->delete_list((char*)s2);
			}
    	}

    	//删除元组上该列数据
    	int num=items.get_size();
        unsigned int keys[num];
        items.get_all_key(keys);
    	for(i=0;i<num;i++){
    		item item_tmp=get_item(keys[i]);
    		item_tmp.delete_val(s);
    	}

    	//修改表信息
    	unsigned char ss[v_len];
    	for(i=1;i<=c;i++){
    		_column.get_by_key(i,ss);
    		if(strcmp((char*)s,(char*)ss)==0){
    			break;
    		}
    	}
    	for(;i<c;i++){
    		_column.get_by_key(i+1,ss);
    		_column.update(i,ss);
    	}
    	_column.drop(i);
    	column.drop(s);

    	c--;

    	return 1;
    }

    void delete_talbe(){
    	unsigned int keys[r];
    	items.get_all_key(keys);
    	for(int i=0;i<r;i++){
    		item item_tmp=get_item(keys[i]);
    		item_tmp.delete_all();
    	}
    	db->delete_list(name_items_empty);
    	db->delete_map(name_items);
    	db->delete_map(name_column);
    	db->delete_map(name__column);
		//index
		int num=index.get_size();
		unsigned char all_indexs[num][k_len];
		index.get_all_key(all_indexs);

		unsigned char s[v_len];
		for(int i=0;i<num;i++){
			index.get_by_key(all_indexs[i],s);
			db_map map_tmp=db->get_map((char*)s);

			int num2=map_tmp.get_size();
			unsigned char all_lists[num2][k_len];
			index.get_all_key(all_lists);

			unsigned char s2[v_len];
			for(int j=0;j<num2;j++){
				map_tmp.get_by_key(all_lists[j],s2);
				db->delete_list((char*)s2);
			}
    		db->delete_map((char*)s);
		}
    	db->delete_map(name_index);

    	printf("table %s has been destroyed.\n",name);
    }

    void show_all(){
    	printf("\n%s: \n",name);

        int num=items.get_size();
        unsigned int keys[num];
        unsigned char keys_i[c][v_len];
        unsigned char val[v_len],val_i[v_len];
        unsigned int val_ii,type;
        items.get_all_key(keys);
        //column.get_all_key(keys_i);

        printf("No. ");
        for(int i=0;i<c;i++){
        	_column.get_by_key((unsigned)(i+1),keys_i[i]);
            printf("%7s ",keys_i[i]);
        }
        printf("\n");
        for(int i=0;i<num;i++){
        	printf("%3d",keys[i]);
            items.get_by_key(keys[i],val);
            item item_tmp((char*)val);
            for(int j=0;j<c;j++){
                column.get_by_key(keys_i[j],&type);
                if(type=='d'){
                    item_tmp.l.get_by_key(keys_i[j],&val_ii);
                    printf("%7d ",val_ii);
                } else {
                    item_tmp.l.get_by_key(keys_i[j],val_i);
                    printf("%7s ",val_i);
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

