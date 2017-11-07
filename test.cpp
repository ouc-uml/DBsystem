#include "table.h"
#include<cstdio>
#include<iostream>

using namespace std;

int main(){
    load_configuration();
    char s[32]="test";
    unsigned char s1[32]="row1";
    unsigned char s2[32]="row2";
    unsigned char s3[32]="row3";
    //table t1(s);
    //t1.delete_talbe();
    //*
    
    table t(s);
    t.add_row(s1,'d');
    t.add_row(s2,'s');
    t.add_item();
    t.add_item();
    t.add_item();
    t.add_item();
    t.add_item();
    t.edit_item(1);
    t.edit_item(3);
    //t.delete_item(2);
    
    t.set_index(s2);
    /*
    vector<unsigned int> v= t.find_by_index(s2,'s');
    printf("No. which values %d: ",0);
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
