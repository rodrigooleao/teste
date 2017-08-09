#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

int dimension = 9125;
int n_clusters;
int max_iter = 10000000;
int total_points;
int n_elements = 671;
vector<pair<int, int> > dataPoints[671];
vector<int> movieList;

void readDataSet(){
  
  pair<int, int> novo;
  int a , b;
  int pos = 0;
  while( cin >>a>>b ){
    if( a == -1 && b == -1){
      pos++;
      continue;
    }
    else{
      novo = make_pair(a , b);
      dataPoints[pos].push_back(novo);
     // cout <<novo.first<<" "<<novo.second<<endl;
    }
  }
  
}

int searchRating( int user , int movie){
  vector<pair<int, int> > :: iterator it;

  for( it = dataPoints[user].begin() ; it != dataPoints[user].end() ; it++ ){
    if( it->first == movie ){
      return it->second;
    }
    else if( it->first > movie){
      return 0;
    }
  } 

  return 0;
}

class Point{
public:
  int id;
  int cluster;
  int dimension;
  vector<int> values;

  Point(){

  }
  Point( int dimension, vector<int> pts){
    this->dimension = dimension;
    this->cluster = -1;

    this->values = pts;
  }

  bool Equal( Point p){
    if( this->dimension != p.dimension )return false;

    for( int i = 0 ; i < p.dimension ; i++){
      if( p.values[i] != this->values[i])return false;
    }

    return true;

  }

  double Distance( Point p){
    double sum = 0.0;

    for( int i = 0 ; i < this->dimension ; i++){
      sum += (( this->values[i] - p.values[i]) * ( this->values[i] - p.values[i]));
    }

    sum = sqrt( sum );

    return sum;
  }
};

class Cluster{
public:
  vector<Point> points;
  Point center;

  Cluster(){

  }

  void removePoint( Point p){

    for( vector<Point>::iterator it = this->points.begin() ; it != this->points.end() ; it++){
      if( p.Equal( *it )){
        this->points.erase( it );
        break;
      }
    }
  }

  void addPoint( Point p){
    this->points.push_back( p );
  }

  void UpdateCenter(){
    unordered_map<int, int> mapa;
    unordered_map<int, int> :: iterator it;

    for( int i = 0 ; i < dimension ; i++){

      mapa.clear();

      for( int j = 0 ; j < this->points.size() ; j++){
        int x = this->points[j].values[i];

        it = mapa.find(x);

        if( it == mapa.end()){
          mapa[x] = 1;
        }
        else{
          mapa[x]++;
        }
      }

      int max = 0;
      int result = 0;
      bool equal = false;
      for( it = mapa.begin() ; it != mapa.end() ; it++ ){
        if( it->second > max ){
          max = it->second;
          result = it->first;
          equal = false;
        }
        else if( it->second == max ){
          equal = true;
        }
      }
      
      if( equal )
        result = true;

      this->center.values[i] = result;
    }
  }
};

class Kmeans{
public:
  int n_clusters;
  vector<Point> points;
  vector<Cluster> clusters;

  Kmeans( vector<Point> points , int n_clusters){
    this->points = points;
    this->n_clusters = n_clusters;
  }

  void CreateClusters(){

    vector<int> chosen;
    Cluster cl;

    for( int i = 0 ; i < n_clusters ; i++){
      this->clusters.push_back( cl );
    }

    int x = -1;
    for( int i = 0 ; i < n_clusters ; i++){
      
      do{
         x = rand()%this->points.size();
      }while( find( chosen.begin() , chosen.end() , x) != chosen.end());

      
      chosen.push_back( x );
      cout <<x<<endl;
      clusters[i].center =  this->points[x];
      
    }
  }

  int findNearestCenter( int i){
    double min = this->points[i].Distance( clusters[0].center);
    int cluster = 0;

    for( int i = 0 ; i < n_clusters ; i++){
      double dist = this->points[i].Distance( clusters[i].center );
      if( dist < min ){
        min = dist;
        cluster = i;
      }
    }

    return cluster;
  }

  void run(){
    cout<<"RUN START"<<endl;
    this->CreateClusters();
    
    int iter = 0;
    int cont;

    while( 1 ){

        cout<<"Realocando pontos..."<<endl;
        bool change = false;
        cont = 0;

        for( int i = 0 ; i < this->points.size() ; i++){
          int old_cluster = points[i].cluster;
          int new_cluster = 0;
          double min;

          //achando o cluster mais proximo

          for( int j = 0 ; j < n_clusters ; j++){
            double dist = points[i].Distance( this->clusters[j].center );
            if( j == 0 || dist < min){
              min = dist;
              new_cluster = j;
            }
          }

         
          //se continuar no mesmo cluster, nao faz nada
          //Se muda, exclui do anterior adiciona no novo, aciona a flag de mudança
          if( old_cluster != new_cluster){
            change = true;
            cont++;
            if( old_cluster != -1)
              clusters[old_cluster].removePoint( this->points[i] );

            clusters[new_cluster].addPoint( this->points[i] );
            this->points[i].cluster = new_cluster;
            
          }
          
        }
        cout<<cont<<" pontos realocados"<<endl;
        //recalculando os centroides
        cout<<"Recalculando centroides..."<<endl;
        for( int i = 0 ; i < this->clusters.size() ; i++){
          clusters[i].UpdateCenter();
          
        }
        
        iter++;
        cout<<iter<<endl;
        if( iter > max_iter || !change )break;  
    }
  }


};


int main(){

  srand( time( NULL));
  vector<Point> pts;
  char num[20];
  string str;
  int x;

 // ifstream myfile( "output.txt");

  // if( myfile.is_open()){
  //   while( getline (myfile,str)){

  //     int x = atoi( str.c_str() );
  //     movieList.push_back( x );
  //   }
    
  //   myfile.close();
  // }
  // pts = buildPoints();

  cin>>n_elements>>dimension>>n_clusters;

  for( int i = 0 ; i < n_elements ; i++){
    
    vector<int> vls;
    for( int j = 0 ; j < dimension ; j++){
      cin >>x;
      vls.push_back(x);
    }

    Point np( dimension , vls );
    pts.push_back( np );
    
  }

  Kmeans kmm( pts , n_clusters);
  kmm.run();

  vector<Cluster> :: iterator it;

  for( it = kmm.clusters.begin() ; it != kmm.clusters.end() ; it++){

      vector<int> real;
      vector<int> predict;

      for( int i = 0 ; i < it->points.size() * 0.8 ; i++){
          predict.push_back( kmm.)
      }
  }

  
  return 0;
}
