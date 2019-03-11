#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

const char sep = ';';
const int K = 10;

typedef pair<int, string> freqArticle;
typedef priority_queue<freqArticle, vector<freqArticle>, greater<freqArticle> > pqueue;

map<string, pqueue> inverseIndex;
map<string, string> idTitle;

void printKArticlesfor(string word) {
    if(inverseIndex.count(word) == 0) {
        cout << "No se han encontrado resultados ☹" << endl;
        return;
    }
    stack<freqArticle> holder;
    pqueue q = inverseIndex[word];
    
    while(!q.empty()) {
        holder.push(q.top());
        q.pop();
    }
    while(!holder.empty()) {
        freqArticle t = holder.top();
        holder.pop();
        cout << "Id: " << t.second << " Apariciones: " << t.first << endl;
        cout << idTitle[t.second] << endl;
    }
}
    
int main() {
    ifstream articlesData;
    articlesData.open("data/sample.csv");
    cout << "Preprocesando artículos" << endl;
    string line;
    while(getline(articlesData, line)) {
        istringstream lineReader(line);
        
        string id, title, content;
        getline(lineReader, id, sep);
        getline(lineReader, title, sep);
        getline(lineReader, content, sep);
        
        idTitle[id] = title;

        istringstream words(content);
        map<string, int> wordCount;
        string word;
        while(words >> word)
            wordCount[word]++;
    
        for(auto it = wordCount.begin(); it != wordCount.end(); it++) {
            string word = it->first;
            int count = it->second;
            inverseIndex[word].push(freqArticle(count, id));
            if(inverseIndex[word].size() > K)
                inverseIndex[word].pop();
        }
    }
    cout << "Fin del preprocesado" << endl;
    string word;
    while(true) {
        cout << "Ingrese un termino de busqueda:" << endl;
        cin >> word;
        if(word == "\\q") {
            cout << "Adios :)" << endl;
            break;
        }
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        printKArticlesfor(word);
        cout << endl;
    }
}