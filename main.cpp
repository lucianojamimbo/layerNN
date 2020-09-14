#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "vectio.h"
#include "math.h"
#include "mnist.h"
#include "activfuncs.h"
#include "lossfuncs.h"
#include "baselayer.h"
#include "sigmoidlayer.h"
#include "misclayers.h"
using namespace std;

int main(){
  //load MNIST dataset:
  cout << "Loading MNIST data" << endl;
  vector<vector<float>> imgs = loadimages();
  vector<int> labels = loadlabels();
  vector<vector<float>> testimgs = loadtestimages();
  vector<int> testlabels = loadtestlabels();
  cout << "Data loaded" << endl;
  cout << "Normalising data" << endl;
  for (auto& i : imgs){
    for (auto& i2 : i){
      i2 = i2/255;
    }
  }
  for (auto& i : testimgs){
    for (auto& i2 : i){
      i2 = i2/255;
    }
  }
  cout << "Data normalised" << endl;


  //network hyperparameters:
  float eta = 1;

  //create vector of pointers to class objects to define the network:
  vector<baselayer*> lyrs;
  inputlayer* l0 = new inputlayer(2);
  sigmoidlayer* l1 = new sigmoidlayer(2,5);
  sigmoidlayer* l2 = new sigmoidlayer(5,2);
  sigmoidlayer* l3 = new sigmoidlayer(2,10);
  sigmoidlayer* l4 = new sigmoidlayer(10,2);
  sigmoidoutputlayer* outputlayer = new sigmoidoutputlayer(2,2);
  lyrs.push_back (l1);
  lyrs.push_back (l2);
  lyrs.push_back (l3);
  lyrs.push_back (l4);
  lyrs.push_back (outputlayer);

  //init network params:
  for (auto& l : lyrs){
    l->paraminit(-1, 1);
  }

  //dummy gradient descent test:
  vector<float> inp = {0.8, 0.9};
  vector<float> desiredout = {0, 0};

  l0->a = inp;

  for (int loops = 0; loops < 100; loops++){
    //run forwards pass:
    for (int l = 1; l < lyrs.size(); l++){
      lyrs[l]->feedforwards(lyrs[l-1]->a);
    }
    //run backprop pass:
    outputlayer->backprop(desiredout);
    for (int l = lyrs.size()-2; l > -1; l--){
        lyrs[l]->backprop(lyrs[l+1]->d, lyrs[l+1]->tpw);
      }
    //calc nablas for all layers:
    for (int l = 1; l < lyrs.size(); l++){
      lyrs[l]->calcnablas(lyrs[l-1]->a);
    }
    //update all layer params:
    for (auto& l : lyrs){
      l->updateparams(eta);
    }
    //print out network activation values:
    printV(outputlayer->a);
  }


  return 0;
}
