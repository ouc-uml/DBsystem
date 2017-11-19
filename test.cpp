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
    t.add_column(s2,'s',"www");
    t.add_column(s4,'d',777);
    t.add_column(s3,'s');
    t.add_column(s3,'s');
    t.add_column(s1,'s');
    t.add_item();
    t.add_item();
    t.add_item();
    t.add_item(2,"faqwff",33);
    t.show_all();
    //*
    vector<string> v;
    string s_add="qqqqq";
    v.push_back(s_add);
    s_add ="123";
    v.push_back(s_add);
    s_add ="12340";
    v.push_back(s_add);//*/
    t.add_item(v);
    
    t.edit_item(1,1,2,"qwe",1);
    t.edit_item(4,1,2,"qwer",2);
    t.edit_item(3,1,v);
    t.show_all();
    //t.delete_column(s2);
    vector<unsigned int> v_i={1,2,4,5};
    t.edit_items(s4,v_i,721);
    t.show_all();
    
    //vector<string> v_s=t.get_column_s(s1);
    //vector<unsigned int> v_i=t.get_column_i(s2);
    
    //*
    //t.set_index(s2);
    unsigned char qw[32]="qw";
    vector<unsigned int> vv= t.find_from_all(s2,qw);
    printf("No. which values qw: ");
    for(int i=0;i<vv.size();i++){
    	cout<<vv[i]<<"  ";
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
