#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "grid.h"

// Implementation of Object interface
Object::Object(std::string& color, int size, int price) 
	: 
	color_(color), size_(size), price_(price) {}

// Getters
std::string& Object::color() { return color_; }
int Object::size() { return size_;  }
int Object::price() { return price_; }


// Implementation of Grid interface
// Assuming the grid objects is passed as input to constructor
Grid::Grid(const std::vector<std::vector<Object>>& grid) : grid_(grid) {
	int rows = grid.size();
	int cols = grid[0].size();

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {

			std::string& color = grid_[row][col].color();
			int price = grid_[row][col].price();
			if (color_index_.find(color) == color_index_.end()) {
				color_index_[color] = std::map<int, int_pair>({ { price, {row, col} } });
			}
			else {
				color_index_[color].insert({ price, {row, col} });
			}
		}
	}
}

// 2
// Assuming row indexing starts from 1
const Object& Grid::get(int row, int col) {
	if (row > grid_.size() || row <= 0) {
		throw std::out_of_range("Row index out of range");
	}
	else if (col > grid_[0].size() || col <= 0) {
		throw std::out_of_range("Column index out of range");
	}
	return grid_[row - 1][col - 1];
}

// 3
// Assuming row indexing starts from 1
const Object& Grid::get_by_offset(int row_offset, int col_offset) {
	int rows = grid_.size();
	int cols = grid_[0].size();

	if (row_offset > rows || row_offset < 0) {
		throw std::out_of_range("Row offset out of range");
	}
	else if (col_offset > cols || col_offset < 0) {
		throw std::out_of_range("Column offset out of range");
	}

	return grid_[rows - row_offset - 1][cols - col_offset - 1];
}

// 4
// For simplicity, using manhattan distance instead of euclidean distance
// Though euclidean distance gives the more accurate distance between bins
// Assuming row indexing starts from 1
const Object& Grid::get_closest_color(
	const std::string& color,
	int row_offset,
	int col_offset) {

	int rows = grid_.size();
	int cols = grid_[0].size();

	if (row_offset > rows || row_offset < 0) {
		throw std::out_of_range("Row offset out of range");
	}
	if (col_offset > cols || col_offset < 0) {
		throw std::out_of_range("Column offset out of range");
	}

	if (color_index_.find(color) == color_index_.end()) {
		throw std::invalid_argument("Color not found");
	}

	int row = rows - row_offset - 1;
	int col = cols - col_offset - 1;

	if (grid_[row][col].color() == color) {
		return grid_[row][col];
	}

	// Spiral search
	// Guaranteed to return since color is present in one of the bins
	for (int dist = 1; dist < std::max(rows, cols); ++dist) {
		for (int i = 0; i < dist + 1; i++) {
			int x1 = row - dist + i;
			int y1 = col - i;

			if (x1 < rows && y1 < cols && grid_[x1][y1].color() == color) {
				return grid_[x1][y1];
			}

			int x2 = row + dist - i;
			int y2 = col + i;

			if (x2 < rows && y2 < cols && grid_[x2][y2].color() == color) {
				return grid_[x2][y2];
			}
		}

		for (int i = 1; i < dist; i++) {
			int x1 = row - i;
			int y1 = col + dist - i;

			if (x1 < rows && y1 < cols && grid_[x1][y1].color() == color) {
				return grid_[x1][y1];
			}

			int x2 = row + i;
			int y2 = col - dist + i;

			if (x2 < rows && y2 < cols && grid_[x2][y2].color() == color) {
				return grid_[x2][y2];
			}
		}
	}
}

// 5
const Object& Grid::get_by_color_price(const std::string& color, int price) {
	if (color_index_.find(color) == color_index_.end()) {
		throw std::invalid_argument("Color not found");
	}

	auto& price_map = color_index_[color];
	if (price_map.find(price) == price_map.end()) {
		throw std::invalid_argument("Price not found");
	}

	int row = price_map[price].first;
	int col = price_map[price].second;
	return grid_[row][col];
}

// 6
const Object& Grid::get_by_color_lowest_price(const std::string& color) {
	if (color_index_.find(color) == color_index_.end()) {
		throw std::invalid_argument("Color not found");
	}

	// std::map is ordered by keys
	// Lowest price is stored first, thereforce map.begin()
	// Returns the grid coordinates of object with lowest price
	auto& grid_index = color_index_[color].begin()->second;

	int row = grid_index.first;
	int col = grid_index.second;
	return grid_[row][col];
}

// 7
const Object& Grid::get_by_color_price_range( const std::string& color, int min_price, int max_price) {
	if (min_price < 0) {
		throw std::invalid_argument("Invalid price range");
	}

	if (color_index_.find(color) == color_index_.end()) {
		throw std::invalid_argument("Color not found");
	}

	auto& price_map = color_index_[color];

	// Assuming inclusive i.e. in range [min_price, max_price]
	auto it_lower = price_map.lower_bound(min_price);
	auto it_upper = price_map.upper_bound(max_price);

	int row, col;
	if (it_lower == price_map.end() && it_upper == price_map.end()) {
		throw std::invalid_argument("No object found in the price range");
	}
	else if (it_lower == price_map.end()) {
		row = (it_lower->second).first;
		col = (it_lower->second).second;
	}
	else {
		row = (it_upper->second).first;
		col = (it_upper->second).second;
	}
#if 0
	if (it_lower != price_map.end()) {
		row = (it_lower->second).first;
		col = (it_lower->second).second;
	}
	else if (it_upper != price_map.end) {
		row = (it_upper->second).first;
		col = (it_upper->second).second;
	}
	else {
		throw std::invalid_argument("No object found in the price range");
	}
#endif
	return grid_[row][col];
}
