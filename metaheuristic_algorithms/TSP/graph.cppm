module;

#include <vector>
#include <cmath>
#include <iostream>

export module graph;

using namespace std;	

export class Graph {
private:
	vector<vector<int>> distances;
	vector<pair<double, double>> cities;
public:
	Graph(const vector<pair<double, double>>& their_cities){
		for (int i = 0; i< cities.size(); i++) {
			for(int j = 0; j<cities.size(); j++){
				distances[i][j] = sqrt(pow((cities[i].first - cities[j].first), 2) + pow((cities[i].second, cities[j].second),2));
			}
		}
		cities = their_cities;
	}
	
	void draw() {
		for(const vector<int> row : distances) {
			for(const int distance : row) {
				cout << distance;
			}
			cout << endl;
		}
	}
};
