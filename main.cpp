#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdio>

using namespace std;

struct line{
    int classifier; // 1 or 2 (first number)
    vector<double> data;    // vector of rest of data in line
};
struct set{ vector<line> lines; };
clock_t start;

set importData(string filename){    /* import IEEE standard input to set object */
    set mainSet; line l; ifstream file; string temp; double num;
    file.open(filename.c_str());
    if(file.is_open()){
        while(getline(file, temp)){
            l.data.clear();
            stringstream ss;
            ss << temp;
            ss >> num;
            l.classifier = num;
            while(ss >> num){ l.data.push_back(num); }
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
double getDistance(line L1, line L2, vector<int>featureNumbers){    /* use distance formula: Σ((x_1 - x_2)^2) */
    double temp = 0, s = 0;
    for(int i = 0; i < featureNumbers.size(); i++){
        temp = pow(L1.data.at(featureNumbers.at(i)) - L2.data.at(featureNumbers.at(i)), 2);
        s += temp;
    }
    return sqrt(s);
}
double kFoldFast(set s, vector<int> features, int & bestNumWrong){
    double numCorrect = 0, numIncorrect = 0, closestFeature = 99999; line closest;
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
        else numIncorrect++;
        if(numIncorrect > bestNumWrong) return 0;
        closestFeature = 99999;
    }
    bestNumWrong = numInstances(s) - numCorrect;
    return numCorrect/numInstances(s) * 100;    // percentage
}
double kFold(set s, vector<int> features){  /* MachineLearning001.ppt slide 34 */
    double numCorrect = 0, closestFeature = 99999; line closest;
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
}
string printSubset(vector<int> v){
    string r = "{";
    for(int j = v.size() - 1; j >= 0; j--){
        r += to_string(v.at(j) + 1);
        if(j != 0) r += ",";
    }
    return r + "}";
}
void callClock(int i){
    if(i == 1) start = clock();
    else cout << "Your algorithm took " << (clock() - start)/(double) CLOCKS_PER_SEC << " seconds.\n";
}
void forwardSelection(set s, int typeKfold){
    vector<int> featureList; double bestAccuracy = 0.01, prevAccuracy = 0, tempFeature = 0, accuracy; int bestNumWrong = 999;
    callClock(1);
    cout << "Beginning search.\n";
    while(prevAccuracy < bestAccuracy){
        prevAccuracy = bestAccuracy;
        for(int i = 0; i < s.lines.at(0).data.size(); i++){
            for(int j = 0; j < featureList.size(); j++)
            if(featureList.at(j) == i) goto skip;   // skip already used features
            featureList.push_back(i);
            if(typeKfold == 1) accuracy = kFold(s, featureList);
            else               accuracy = kFoldFast(s, featureList, bestNumWrong);
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
    callClock(2);
}
void backwardElimination(set s){
    vector<int> featureList, tempFeatureList; double bestAccuracy = 0.01, prevAccuracy = 0, tempFeature = 0, accuracy;
    callClock(1);
    cout << "Beginning search.\n";
    for(int i = 0; i < s.lines.at(0).data.size(); i++)
        featureList.push_back(i);
    while(prevAccuracy < bestAccuracy){
        prevAccuracy = bestAccuracy;
        for(int i = 0; i < featureList.size(); i++){
            tempFeatureList = featureList;
            tempFeatureList.erase(tempFeatureList.begin() + i);
            accuracy = kFold(s, tempFeatureList);
            cout << "\tUsing feature(s) " << printSubset(tempFeatureList) << " accuracy is " << accuracy << "%\n";   
            if(accuracy > bestAccuracy){
                bestAccuracy = accuracy;
                tempFeature = i;
            }
        }
        if(prevAccuracy == bestAccuracy) break;
        featureList.erase(featureList.begin() + tempFeature);
        cout << "Feature set " << printSubset(featureList) << " was best, accuracy is " << bestAccuracy << "%\n";
    }
    cout << "Finished search! The best feature subset is " << printSubset(featureList) << ", which has an accuracy of " << bestAccuracy << "%\n";
    callClock(2);
}
int main(){
    string filename; int sel = 0; set mainData; vector<int> t;
    cout << "Welcome to Ryan Yuzuki's Feature Selection Algorithm.\nType the name of the file you wish to test: ";
    cin >> filename;
    mainData = importData(filename);
    for(int i = 0; i < numFeatures(mainData); i++){ t.push_back(i); }
    cout << "This dataset has " << numFeatures(mainData) << " features (not including the class attribute), with " << numInstances(mainData) << " instances.\n";
    cout << "Running nearest neighbor with all " << numFeatures(mainData) << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << kFold(mainData, t) << "%\n";
    algorithmSelect: cout << "Type the number of the algorithm you want to run:\n1) Forward Selection\n2) Backward Elimination\n3) Forward Selection with Fast k-Fold evaluation\n";
    cin >> sel;
    if(sel==1){ forwardSelection(mainData, 1); }
    else if(sel==2){ backwardElimination(mainData); }
    else if(sel==3){ forwardSelection(mainData, 2); }  // forwardSelection with kFoldFast instead of kFold 
    else{goto algorithmSelect;}
}