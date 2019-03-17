#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <omp.h>
#include <mpi.h>
#include <cctype>
#include <functional>
#include <cstring>

using namespace std;

const char sep = ';';
const int K = 10, MASTER = 0, WORKER1 = 1, WORKER2 = 2;

int world_rank, world_size;

typedef pair<int, string> freqArticle;
typedef priority_queue<freqArticle, vector<freqArticle>, greater<freqArticle> > pqueue;

map<string, pqueue> inverseIndex;
map<string, string> idTitle;

void load_inverse_index() {
  ifstream articlesData;
  if(world_rank == 0)  articlesData.open("../data/articles1_semicolon.csv");
  else if (world_rank == 1) articlesData.open("../data/articles2_semicolon.csv");
  else articlesData.open("../data/articles3_semicolon.csv");

  string line;
  vector<string> lines;
  while(getline(articlesData, line)) lines.push_back(line);
  
    #pragma omp parallel
  {
        #pragma omp for
    for (int i = 0; i < lines.size(); i++) {
      istringstream lineReader(lines[i]);

      string id, title, content;
      getline(lineReader, id, sep);
      getline(lineReader, title, sep);
      getline(lineReader, content, sep);

      istringstream words(content);
      map<string, int> wordCount;
      string word;
      while(words >> word)
	wordCount[word]++;

      for(map<string, int>::iterator it = wordCount.begin(); it != wordCount.end(); it++) {
	string word = it->first;
	int count = it->second;
                #pragma omp critical
	{
	  inverseIndex[word].push(freqArticle(count, id));
	  if(inverseIndex[word].size() > K) inverseIndex[word].pop();
	}
      }
    }
  }
}

void load_id_title() {
  ifstream idTitleData;
  idTitleData.open("../data/title_file.csv");
  
  string line;
  vector<string> lines;
  while(getline(idTitleData, line)) lines.push_back(line);
  
  #pragma omp parallel
  {
    #pragma omp for
    for(int i = 0; i < lines.size(); i++) {
      istringstream lineReader(lines[i]);
      
      string id, title;
      getline(lineReader, id, sep);
      getline(lineReader, title, sep);
      
      idTitle[id] = title;
    }
  }
}

void get_k_articles(string word, vector<int> &ids, vector<int> &freqs) {
  ids.clear();
  ids.assign(10, -1);
  freqs.clear();
  freqs.assign(10, -1);
  
  stack<freqArticle> holder;
  pqueue q = inverseIndex[word];
  while(!q.empty()) {
    holder.push(q.top());
    q.pop();
  }
  int next_pos = 0;
  while(!holder.empty()) {
    freqArticle t = holder.top();
    holder.pop();
    int id = stoi(t.second), freq = t.first;
    ids[next_pos] = id;
    freqs[next_pos] = freq;
    next_pos++;
  }
}

void print_article(string &id, int freq) {
  cout << "Id: " << id << " Apariciones: " << freq << endl;
  cout << idTitle[id] << endl;
}

void wait_for_queries() {
  char word[30];
  while(true) {
    memset(word, 0, sizeof(word));    
    if(world_rank == MASTER) {
      printf("Ingrese un termino de busqueda: ");
      scanf("%s", word);
      int i = 0;
      while(word[i] != '\0') {
        word[i] = tolower(word[i]);
        i++;
      }
      MPI_Send(word, 30, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
      MPI_Send(word, 30, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
      
      int* ids_1 = new int[10];
      int* freqs_1 = new int[10];
      int* ids_2 = new int[10];
      int* freqs_2 = new int[10];
      MPI_Status status;
      MPI_Recv(ids_1, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(freqs_1, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(ids_2, 10, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(freqs_2, 10, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
    
      vector<int> ids_0, freqs_0;
      get_k_articles(string(word), ids_0, freqs_0);
      
      vector<pair<int, string> > candidates;
      for(int i = 0; i < 10; i++) {
        if (ids_0[i]!=-1 && freqs_0[i] != -1) 
          candidates.push_back(make_pair(freqs_0[i], to_string(ids_0[i])));
        if(ids_1[i] != -1 && freqs_1[i] != -1)
	  candidates.push_back(make_pair(freqs_1[i], to_string(ids_1[i])));
        if(ids_2[i] != -1 && freqs_2[i] != -1)
	  candidates.push_back(make_pair(freqs_2[i], to_string(ids_2[i])));
      }
      
      sort(candidates.begin(), candidates.end(), greater<pair<int, string> >());
      if (candidates.size()==0) printf("No se encontraron resultados.\n");
      else {
        for(int i = 0; i < min(10, (int) candidates.size()); i++)
	  print_article(candidates[i].second, candidates[i].first);
      }
          
    } else if(world_rank == WORKER1) {
      MPI_Status status;
      MPI_Recv(word, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      
      vector<int> ids, freqs;
      get_k_articles(string(word), ids, freqs);
      
      MPI_Send(&ids[0], 10, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&freqs[0], 10, MPI_INT, 0, 0, MPI_COMM_WORLD);
    
    } else {
      MPI_Status status;
      MPI_Recv(word, 30, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
      
      vector<int> ids, freqs;
      get_k_articles(string(word), ids, freqs);
      
      MPI_Send(&ids[0], 10, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Send(&freqs[0], 10, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  
  load_inverse_index();
  if(world_rank == MASTER) load_id_title();
  wait_for_queries(); 
    
  MPI_Finalize();
}
