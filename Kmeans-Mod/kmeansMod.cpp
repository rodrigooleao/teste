#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

int dimension;
int n_clusters;
int max_iter;
int total_points;

class Point{
public:
  int id;
  int cluster;
  int dimension;
  vector<int> values;

  Point();
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
      sum += (( this->values[i] = p.values[i]) * ( this->values[i] = p.values[i]));
    }

    sum = sqrt( sum );

    return sum;
  }
};

class Cluster{
public:
  vector<Point> points;
  Point center;

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
    int x = -1;
    for( int i = 0 ; i < n_clusters ; i++){
      do{
         x = rand()%this->points.size();
      }while( find( chosen.begin() , chosen.end() , x) != chosen.end());

      chosen.push_back( x );
      clusters[i].center =  this->points[i];
    }

  }

  int findNearestCenter( Point p){
    double min = p.Distance( clusters[0].center);
    int cluster = 0;

    for( int i = 0 ; i < n_clusters ; i++){
      double dist = p.Distance( clusters[i].center );
      if( dist < min ){
        min = dist;
        cluster = i;
      }
    }

    return cluster;
  }

  Point getNewCenterByFreq( Cluster c){
    Point newCenter;
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

      newCenter.values[i] = maior;
    }
  }
  void run(){
    int iter = 0;
    while( 1 ){
      
        bool change = false;
        for( int i = 0 ; i < this->points.size() ; i++){
          int old_cluster = points[i].cluster;
          int new_cluster = this->findNearestCenter( points[i]);

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
          clusters[i].center = getNewCenterByFreq( clusters[i]);
        }
        
        if( iter > max_iter || !change )break;
        this->

        
    }
  }


};

int main(){

  srand( time( NULL));





  return 0;
}
