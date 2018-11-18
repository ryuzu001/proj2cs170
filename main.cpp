#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstdlib>

using namespace std;

struct line{
    int classifier; // 1 or 2 (first number)
    vector<double> data;
};

struct set{
    vector<line> lines;
};

set importData(string filename){
    set mainSet;
    line l;
    ifstream file;
    string temp;
    double num;
    file.open(filename.c_str());
    if(file.is_open()){
        while(getline(file, temp)){
            l.data.clear();
            stringstream ss;
            ss << temp;
            ss >> num;
            l.classifier = num;
            while(ss >> num){
                l.data.push_back(num);
            }
            mainSet.lines.push_back(l);   
        }
    }
    else{
        cout << "Failed to open file\n";
        exit(EXIT_FAILURE);
    }
    file.close();
    return mainSet;
}

void printSet(set s){
    for(int i = 0; i < s.lines.size(); i++){
        cout << s.lines.at(i).classifier << " ";
        for(int j = 0; j < s.lines.at(i).data.size(); j++){
            cout << s.lines.at(i).data.at(j) << " ";
        }
        cout << endl;
    }
}   // test routine print set

int main(){
    string filename;
    int sel;
    set mainData;
    cout << "Welcome to Ryan Yuzuki's Feature Selection Algorithm.\nType the name of the file you wish to test: ";
    cin >> filename;

    mainData = importData(filename);
    // printSet(mainData);

    algorithmSelect:

    cout << "Type the number of the algorithm you want to run:\n1) Forward Selection\n2)Backward Elimination\n3)Ryan's Special Algorithm\n";
    cin >> sel;
    if(sel==1){}
    else if(sel==2){}
    else if(sel==3){}
    else{goto algorithmSelect;}
}