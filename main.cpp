#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <cmath>

using namespace std;

struct line{
    int classifier; // 1 or 2 (first number)
    vector<double> data;    // vector of rest of data in line
};
struct set{ vector<line> lines; };
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

int numFeatures(set s){
    int i;
    for(i = 0; i < s.lines.at(0).data.size(); i++) {}
    return i;
}
int numInstances(set s){
    int i;
    for(i = 0; i < s.lines.size(); i++) {}
    return i;
}
double getDistance(line L1, line L2, vector<int>featureNumbers){
    /* so theres 2 lines, and a vector of ints. */
    /* return distance between the features of the 2 lines (specified by featureNumbers) */
    /* if featureNumbers.size = 1, just 1 feature (line with 2 points) 1D */
    /* if featureNumbers.size = 2, 2 features (think x y graph) 2D */
    /* if featureNumbers.size = 3, 3 features - 3D graph... etc */
    double temp = 0, s = 0;
    for(int i = 0; i < featureNumbers.size(); i++){
        temp = pow(L1.data.at(featureNumbers.at(i)) - L2.data.at(featureNumbers.at(i)), 2);
        s += temp;
        /* Σ((x_1 - x_2)^2) */
    }
    return sqrt(s);
}
double kFold(set s, vector<int> features){
    /* MachineLearning001.ppt slide 34 */
    double numCorrect = 0, closestFeature = 99999;
    line closest;
    for(int i = 0; i < s.lines.size(); i++){
        line l = s.lines.at(i);   // the one left out
        for(int j = 0; j < i; j++){
            if(getDistance(l, s.lines.at(j), features) < closestFeature){
                closestFeature = getDistance(l, s.lines.at(j), features);
                closest = s.lines.at(j);
            }
        }
        for(int j = i + 1; j < s.lines.size(); j++){
            if(getDistance(l, s.lines.at(j), features) < closestFeature){
                closestFeature = getDistance(l, s.lines.at(j), features);
                closest = s.lines.at(j);
            }
        }
        if(l.classifier == closest.classifier){ numCorrect++; }
        closestFeature = 99999;
    }
    return numCorrect/numInstances(s) * 100;    // percentage
    // return numCorrect;
}
string printSubset(vector<int> v){
    string r = "{";
    for(int j = v.size() - 1; j >= 0; j--){
        r += to_string(v.at(j) + 1);
        if(j != 0) r += ",";
    }
    r += "}";
    return r;
}
void forwardSelection(set s){
    vector<int> featureList;
    double bestAccuracy = 0.01, prevAccuracy = 0, tempFeature = 0, accuracy;
    cout << "Beginning search.\n";
    while(prevAccuracy < bestAccuracy){
        prevAccuracy = bestAccuracy;
        for(int i = 0; i < s.lines.at(0).data.size(); i++){
            for(int j = 0; j < featureList.size(); j++)
            if(featureList.at(j) == i) goto skip;   // skip already used features
            featureList.push_back(i);
            accuracy = kFold(s, featureList);
            cout << "\tUsing feature(s) " << printSubset(featureList) << " accuracy is " << accuracy << "%\n";   
            if(accuracy > bestAccuracy){
                bestAccuracy = accuracy;
                tempFeature = i;
            }         
            featureList.pop_back();
            skip: ;
        }
        if(prevAccuracy == bestAccuracy) break;
        featureList.push_back(tempFeature);
        cout << "Feature set " << printSubset(featureList) << " was best, accuracy is " << bestAccuracy << "%\n";
    }
    cout << "Finished search! The best feature subset is " << printSubset(featureList) << ", which has an accuracy of " << bestAccuracy << "%\n";
}
void backwardElimination(set s){}
void ryanSpecial(set s){}
int main(){
    string filename;
    int sel = 0;
    set mainData;
    vector<int> t;
    cout << "Welcome to Ryan Yuzuki's Feature Selection Algorithm.\nType the name of the file you wish to test: ";
    cin >> filename;
    mainData = importData(filename);
    for(int i = 0; i < numFeatures(mainData); i++){ t.push_back(i); }
    cout << "This dataset has " << numFeatures(mainData) << " features (not including the class attribute), with " << numInstances(mainData) << " instances.\n";
    cout << "Running nearest neighbor with all " << numFeatures(mainData) << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << kFold(mainData, t) << "%\n";
    algorithmSelect:
    cout << "Type the number of the algorithm you want to run:\n1) Forward Selection\n2) Backward Elimination\n3) Ryan's Special Algorithm\n";
    cin >> sel;
    if(sel==1){forwardSelection(mainData);}
    else if(sel==2){backwardElimination(mainData);}
    else if(sel==3){ryanSpecial(mainData);}
    else{goto algorithmSelect;}
}