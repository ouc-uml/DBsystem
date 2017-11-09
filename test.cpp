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
    //table t1(s);
    //t1.delete_talbe();
    //*
    
    table t(s);
    t.add_column(s1,'s');
    t.add_column(s2,'d');
    t.add_item();
    t.add_item(2,"faqwff",33);
    t.add_item();
    
    t.edit_item(1,2,"qwe",1);
    t.edit_item(3,2,"qwer",2);
    t.delete_item(2);
    
    t.show_all();
    
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
