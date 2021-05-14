#pragma once

#include <string>
#include <map>
#include <unordered_map>

class Object {
private:
	std::string& color_;
	int size_;
	int price_;

public:
	Object(std::string& color, int size, int price);
	std::string& color();
	int size();
	int price();
};

class Grid {
private:
	std::vector<std::vector<Object>> grid_;

	typedef std::pair<int, int> int_pair;

	// Efficient indexing of price and coordinates
	// unordered_map - O(1) to check whether color exists
	// map - sorted keys - O(1) to retrieve lowest price
	// map provides a convenient API - upper_bound and lower_bound
	// Useful for range calculation
	std::unordered_map<std::string, std::map<int, int_pair>> color_index_;

public:
	Grid(const std::vector<std::vector<Object>>& grid);

	const Object& get(int row, int col);

	const Object& get_by_offset(int row_offset, int col_offset);

	const Object& get_closest_color(const std::string& color, int row_offset, int col_offset);

	const Object& get_by_color_price(const std::string& color, int price);

	const Object& get_by_color_lowest_price(const std::string& color);

	const Object& get_by_color_price_range(const std::string& color, int min_price, int max_price);
};
