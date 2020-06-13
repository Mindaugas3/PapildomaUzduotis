#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>


using namespace std;

static string address = "Mesoamerican pyramids - Wikipedia.txt";
static string text; //pilnas tekstas, 1000+ zodziu
static map<string, vector<int> > FoundAt; //asociatyvus zymejimas
static vector<string> Url;

void import(string address);


void getURLs();
void filter();
void print();
void search();

int main() {
    //deklaracijos

    //pradzia
    import(address);

    getURLs();
    filter();
    for(string h : Url){
        cout << h << endl;
    }
    search();
    cout << endl << endl << endl;
    print();
    system("PAUSE");
    return 0;
}

void import(string address){
    ifstream i(address);
    string line = "";
    while(getline(i, line)){
        text += line;
        text += "\n";
    }
    i.close();
}

void print(){
    cout << text;
    cout << endl << endl;
    cout << "================================================================================================" << endl;
    for(auto it = FoundAt.begin(); it != FoundAt.end(); ++it){
        if(it->first != " " && it->first != ""){
            cout << it->first << " : ";
            cout << " [Aptikta: " << it->second.size() << " ] : " << endl;
            set<int> H(it->second.begin(), it->second.end());
            for(int I : H){
                cout << I << " , ";
            }
            cout << endl;
        }

    }
}

void filter(){
    string del = "0123456789:";
    //ka pasalinam?
    del.push_back('*');
    del.push_back('[');
    del.push_back(']');
    del.push_back('#');
    del.push_back('^');
    del.push_back('/');
    del.push_back(',');
    del.push_back('.');
    del.push_back('\'');
    del.push_back('-');
    del.push_back(';');
    del.push_back('(');
    del.push_back(')');
    del.push_back('+');
    del.push_back('&');

    string result = "";

    //algoritmas
    for(int i = 0; i< text.length(); i++){
        bool isAllowed = true;
        for(char c : del){
            if(text[i] == c){
                isAllowed = false;
            }
        }
        if(isAllowed){
            result += text[i];
        }
    }
    text = result;
}

void getURLs(){
    //funkcijos esme surasti URL
    char start = '<';
    char end = '>';
    char altstart = '"';
    string result = ""; //tekstas po url salinimo
    string addr; //adresas
    bool enclosed = false;
    for(int i = 0; i < text.length(); i++){
        if(text[i] == start || text[i] == altstart){
            enclosed = true;
            addr+= text[i];
            continue;

        }
        if(!enclosed){
            result += text[i];
        } else {
            addr+= text[i];
        }
        if(text[i] == end || text[i] == altstart){
            enclosed = false;
            if(addr.find("http") != string::npos){
                Url.push_back(addr);
            }
            addr = "";
        }

    }
    text = result;
}

void search(){
    istringstream s(text);
    set<string> words;
    int Q = 0;
    string line = "";
    while(getline(s, line)){
        istringstream linestream(line);
        string a;
        while(linestream >> a){

            transform(a.begin(), a.end(), a.begin(), ::tolower);
            pair<_Rb_tree_const_iterator<string>, bool> B = words.insert(a);

            if(!B.second){
                vector<int> fnum;
                fnum.push_back(Q);
                pair<string, vector<int> > p = make_pair(a, fnum);
                FoundAt.insert(p);
            } else {
                for(auto it = FoundAt.begin(); it != FoundAt.end(); ++it){
                    it->second.push_back(Q);
                }
            }
        }
        Q++;
    }

    cout << "Q = " << Q << endl;
}
