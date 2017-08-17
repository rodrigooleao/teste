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
#include <cstdlib>

using namespace std;

int dimension = 9125;
int n_clusters;
int max_iter = 100;
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

vector<string> Parse( string str , char token){
    vector<string> result;
    string r;

    int ini = 0, fim = 0;

    while( fim < str.size() ){
        if( str[fim] == token ){
            r = str.substr( ini , (fim - ini));
            
            cout<<r<<endl;
            result.push_back( r );

            while( str[fim] == token)fim++;
            ini = fim;
        }
        fim++;
    }

    r = str.substr( ini , (fim - ini));
    cout<<r<<endl;
    result.push_back( r );

    return result;
}

class Movie{
public:
  string nome;
  int id;
};

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

  double euclideanDistance( Point p){
    double sum = 0.0;

    for( int i = 0 ; i < this->dimension ; i++){
      sum += (( this->values[i] - p.values[i]) * ( this->values[i] - p.values[i]));
    }

    sum = sqrt( sum );

    return sum;
  }

  // double cosineDistance( Point p){
    
  // }
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
      //cout <<x<<endl;
      clusters[i].center =  this->points[x];
      
    }
  }

  int findNearestCenter( int i){
    double min = this->points[i].euclideanDistance( clusters[0].center);
    int cluster = 0;

    for( int i = 0 ; i < n_clusters ; i++){
      double dist = this->points[i].euclideanDistance( clusters[i].center );
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
    bool change = true;

    while( iter < max_iter && change ){

        //cout<<"Realocando pontos..."<<endl;
        change = false;
        cont = 0;

        for( int i = 0 ; i < this->points.size() ; i++){
          int old_cluster = points[i].cluster;
          int new_cluster = 0;
          double min;

          //achando o cluster mais proximo

          for( int j = 0 ; j < n_clusters ; j++){
            double dist = points[i].euclideanDistance( this->clusters[j].center );
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
        // cout<<cont<<" pontos realocados"<<endl;
        //recalculando os centroides
        // cout<<"Recalculando centroides..."<<endl;
        for( int i = 0 ; i < this->clusters.size() ; i++){
          clusters[i].UpdateCenter();
          
        }
        
        iter++;
        if( iter%1000 == 0 )
          cout<<iter<<endl; 
    }
  }

  double calculateRMSE(){

    double result = 0.0 , sum = 0.0;
    vector<pair<int,int> >::iterator it;
    vector<Cluster> :: iterator itc;
    vector<pair<int,int> > predictions;

    for( itc = this->clusters.begin() ; itc != this->clusters.end() ; itc++){

        
        Point predicted = itc->center;

        for( int i = 0 ; i < itc->points.size() ; i++){
            for( int j = 0 ; j < dimension * 0.2 ; j++){
              predictions.push_back( make_pair( itc->points[i].values[j]   , predicted.values[j]));
            }
        }
    }
    
    for( it = predictions.begin() ; it != predictions.end() ; it++){
      sum += (( it->first - it->second) * ( it->first - it->second)); 
    }

    sum = sum / (predictions.size());

    result = sqrt( sum );

    return result;
  }

  void getRecomendations( int n , vector<Movie> moviesData){

    vector<string> recomendations;
    Point activeUser = this->points[n];
    Cluster activeCluster = this->clusters[ activeUser.cluster];

    vector<int> ::iterator it;
    int cont = 0;
    for( int i  = 0 ; i <  activeCluster.center.values.size() ; i++){
      if( activeCluster.center.values[i] > 3 ){
        cont++;
        if( cont <= 10)
          recomendations.push_back( moviesData[i+1].nome);
      } 
    }

    cout <<"********** Filmes Recomendados ********"<<endl;

    for( int i = 0 ; i < recomendations.size() ; i++){
      cout<<recomendations[i]<<endl;
    }
  }

  void getPersonalRecomendations( vector<Movie> moviesData){
    int x , nota;
    vector<int> chosen;
    vector<int> vls(dimension , 0);
    
    for( int i = 0 ; i < 10 ; i++){
      do{
         x = rand()%dimension;
      }while( find( chosen.begin() , chosen.end() , x) != chosen.end());

      chosen.push_back(x);
      cout<<"Qual nota voce daria para o filme:  " <<moviesData[x].nome<<" (0 caso nao tenha assistido)"<<endl;
      cin >> nota;

      vls[x] = nota;
    }

    Point p( dimension , vls );

    int closest = 0;
    double min;

    //achando o cluster mais proximo

    for( int j = 0 ; j < n_clusters ; j++){
      double dist = p.euclideanDistance( this->clusters[j].center );
      if( j == 0 || dist < min){
        min = dist;
        closest = j;
      }
    }

    cout<<"cl : " <<closest<<endl;

    //Cluster closestCluster = this->clusters[closest];

    //cout<<"cl : " <<closestCluster.center.values.size()<<endl;

    // for( int i = 0 ; i < closestCluster.center.values.size() ; i++){
    //   if( closestCluster.center.values[i] > 3){
    //     cout<<moviesData[i].nome<<endl;
    //   }
    // }
    // cout<<endl;

    
    for( int i = 0 ; i < n_clusters ; i++){
      Cluster clt = this->clusters[i];
      int cont = 0;
  
      for( int j = 0 ; j < clt.center.values.size() ; j++){
        if( clt.center.values[j] > 0){
          cont++;
        }
      }

      cout<<"Cluster: "<<i<<" - Valores: "<<cont<<" - Pontos: "<<this->clusters[i].points.size()<<endl;
    }


  }

};

int main( int argc, char* argv[]){

  srand( time( NULL));


  vector<Point> pts;
  string str;
  vector<string> vstr;
  int x;

  vector<Movie> moviesData;
  ifstream arqMovie( "datasets/movie.data");
  ifstream arqData(argv[1]);

  //Lendo o dataset com as infos do filmes;
  if( arqMovie.is_open()){
    while( getline (arqMovie,str)){

      Movie novo;

      novo.id = atoi( str.c_str() );
      
      string nome;
      getline (arqMovie,novo.nome);

      moviesData.push_back( novo );
    }
    
    arqMovie.close();
  }
  //Lendo os dados dos Usuários
  //cin>>n_elements>>dimension>>n_clusters;

  // n_elements = atoi( argv[1]);
  // dimension = atoi( argv[2]);
  // n_clusters = atoi( argv[3]);

  getline( arqData , str);

  vstr = Parse( str , ' ');

  n_elements = atoi( vstr[0].c_str());
  dimension = atoi( vstr[1].c_str());
  n_clusters = atoi( vstr[2].c_str());

  

  for( int i = 0 ; i < n_elements ; i++){
    
    vector<int> vls;

    getline( arqData , str);
    for( int j = 0 ; j < str.size() ; j+=2){
      vls.push_back(str[j] - 48);
    }
   
    Point np( dimension , vls );
    pts.push_back( np );
    
  }

  

  Kmeans kmm( pts , n_clusters);
  kmm.run();

  double result = kmm.calculateRMSE();

  cout <<"RMSE: "<<result<<endl; 

  while( true ){

    cout <<"Qual seu ID?"<<endl<<endl<<"-> ";
    int x;

    cin >>x;
    if( x == -1)break;
    kmm.getRecomendations( x , moviesData );
  }

  kmm.getRecomendations( 148 , moviesData );
  kmm.getPersonalRecomendations( moviesData );
  
  return 0;
}
