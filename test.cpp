#include "table.h"
#include<cstdio>
#include<iostream>

using namespace std;

int main(){
    load_configuration();
    char s[32]="test";
    unsigned char s1[32]="column1";
    unsigned char s2[32]="column2";
    unsigned char s3[32]="column3";
    unsigned char s4[32]="column4";
    unsigned char s5[32]="column5";
    //table t1(s);
    //t1.delete_talbe();
    //*
    
    table t(s);
    t.add_column(s1,'s',"www");
    t.add_column(s2,'d',777);
    t.add_column(s3,'s');
    t.add_column(s3,'s');
    t.add_column(s4,'s');
    t.add_item();
    t.add_item(2,"faqwff",33);
    //*
    vector<string> v;
    string s_add="qqqqq";
    v.push_back(s_add);
    s_add ="123";
    v.push_back(s_add);
    s_add ="12340";
    v.push_back(s_add);//*/
    t.add_item(v);
    
    t.edit_item(1,2,"qwe",1);
    t.edit_item(4,2,"qwer",2);
    t.edit_item(3,1,v);
    //t.delete_item(2);
    
    vector<string> v_s=t.get_column_s(s1);
    vector<unsigned int> v_i=t.get_column_i(s2);
    printf("%7s %7s\n",s1,s2);
    for(int i=0;i<v_i.size();i++){
    	printf("%7s %7d\n",v_s[i].c_str(),v_i[i]);
    }
    
    /*
    t.set_index(s1);
    unsigned char qw[32]="qw";
    vector<unsigned int> v= t.find_by_index(s1,qw);
    printf("No. which values qw: ");
    for(int i=0;i<v.size();i++){
    	cout<<v[i]<<"  ";
    }
    cout<<endl;
    /*
    unsigned int keys[1000]; 
    int i=t.get_all_no(keys);
    for(int j=0;j<i;j++){
    	cout<<"key: "<<keys[j]<<endl;
    }//*/
    t.show_all();
    printf("\nsuccess\n");
    return 0;
}
