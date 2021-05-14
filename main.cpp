#include <vector>
#include <iostream>
#include "grid.h"

using namespace std;

string get(Object o) {
	return o.color() + " " + to_string(o.price());
}

int main() {

	vector<vector<Object>> grid;

	string colors[] = { "red", "green", "blue", "cyan", "magenta", "yellow" };

	for (int i = 0; i < 6; ++i) {
		grid.push_back(vector<Object>());
		for (int j = 0; j < 7; ++j) {
			grid[i].push_back(Object(colors[(i + j) % 6], rand() % 100, rand() % 100));
		}
	}

	for (int i = 0; i < grid.size(); ++i) {
		for (int j = 0; j < grid[i].size(); ++j) {
			cout << grid[i][j].color() << " " << grid[i][j].price() << "\t";
		}
		cout << endl;
	}


	Grid g = Grid(grid);

	// Some test runs
	cout << get(g.get_by_color_lowest_price("cyan")) << endl;

	// Not accurate in euclidean space since
	// The closest is computed in manhattan distance for simplicity
	cout << get(g.get_closest_color("blue", 0, 0)) << endl;

}