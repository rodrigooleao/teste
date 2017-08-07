#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>

using namespace std;

int dimension = 9125;
int n_clusters;
int max_iter;
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

  vector<int> getNewCenterByFreq( Cluster c){
    vector<int> newCenter;
    int cont[5];

    
      
    for( int i = 0 ; i < dimension ; i++){

      for( int k = 0 ; k < 10 ; k++)cont[k] = 0;
      
      for( int j = 0 ; j < c.points.size(); j++){
        cont[ c.points[j].values[i] - 1]++;
      }

      int maior = 0;
      for( int k = 0 ; k < 5 ; k++){
        if( cont[k] > maior){
          maior = k;
        }
      }

      sort( cont , cont+5);

      if( cont[4] == cont[3])maior = 0;

      newCenter.push_back( maior) ;
    }

    return newCenter;
  }
  void run(){
    cout<<"RUN START"<<endl;
    this->CreateClusters();
    
    int iter = 0;

    while( 1 ){
      
        bool change = false;
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

          cout<<i<<" - "<<old_cluster<<" "<<new_cluster<<endl;

          
          //se continuar no mesmo cluster, nao faz nada
          //Se muda, exclui do anterior adiciona no novo, aciona a flag de mudança
          if( old_cluster != new_cluster){
            change = true;
            if( old_cluster != -1)
              clusters[old_cluster].removePoint( this->points[i] );

            clusters[new_cluster].addPoint( this->points[i] );
            this->points[i].cluster = new_cluster;
            
          }

        }
        //recalculando os centroides
        for( int i = 0 ; i < this->clusters.size() ; i++){
          Point p( dimension ,  getNewCenterByFreq( clusters[i]) );
          
          clusters[i].center = p;
          
        }
        
        iter++;
        cout<<iter<<endl;
        if( iter > max_iter || !change )break;  
    }
  }


};

vector<Point> buildPoints(){
    vector<Point> pts; 

    for( int i = 0 ; i < n_elements ; i++){
      
      vector<int> vls;
      for( int j = 0 ; j < dimension ; j++){
        int x = searchRating( i , movieList[j]);
        vls.push_back(x);
      }

      Point np( dimension , vls );
      pts.push_back( np );
      
    }

    return pts;
}

int main(){

  srand( time( NULL));
  vector<Point> pts;
  char num[20];
  string str;
  int x;

  //readDataSet();

  // for( int i = 0 ; i < n_elements ; i++){

  //   vector<pair<int, int> > :: iterator it;

  //    for( it = dataPoints[i].begin() ; it != dataPoints[i].end() ; it++){
  //      cout<<it->first<<","<<it->second<<"|";
  //    }
  //    cout <<endl;
      
  // }
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


  
  


  return 0;
}
