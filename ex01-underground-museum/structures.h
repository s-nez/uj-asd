#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <limits>
#include <vector>
#include "colors.h"

using coord = std::pair<std::size_t, std::size_t>;

struct PathData {
	int price = 0, room_count = 0;
	std::string directions = "";
};

std::ostream& operator<< (std::ostream& out, const PathData& in)
{
	return out << in.price << ' ' << in.room_count << ' ' << in.directions;
}

PathData operator+ (const PathData& a, const PathData& b)
{
	PathData result;
	result.price = a.price + b.price;
	result.room_count = a.room_count + b.room_count;
	result.directions = a.directions + b.directions;
	return result;
}

struct RoomData {
	coord prev;
	int dist = std::numeric_limits<int>::max();
	bool visited = false;
	std::string color = Color::White;
};

struct Heap {
	std::vector<coord> v;
	RoomData** cmp_data;
	Heap (RoomData** new_data, int height, int width)
	{
		cmp_data = new_data;
	}

	bool compare (coord a, coord b)
	{
		return cmp_data[a.first][a.second].dist < cmp_data[b.first][b.second].dist;
	}

	void insert (const coord& x)
	{
		v.push_back (x);
		upheap (v.size() - 1);
	}

	coord pop()
	{
		coord result = v[0];
		v[0] = v[v.size() - 1];
		v.pop_back();
		downheap (0);
		return result;
	}

	inline bool empty()
	{
		return v.empty();
	}

	inline std::size_t parent (std::size_t child)
	{
		if (child != 0)
			return (child - 1) / 2;
		return 0;
	}

	inline std::size_t left_child (std::size_t parent)
	{
		return (2 * parent) + 1;
	}

	inline std::size_t right_child (std::size_t parent)
	{
		return (2 * parent) + 2;
	}

	void upheap (std::size_t start)
	{
		while (start != 0 && !compare (v[parent (start)], v[start])) {
			std::swap (v[start], v[parent (start)]);
			start = parent (start);
		}
	}

	void downheap (std::size_t start)
	{
		if (start < v.size() / 2 or empty()) return;
		std::size_t tmp;
		if (compare (v[right_child (start)], v[left_child (start)]))
			tmp = right_child (start);
		else
			tmp = left_child (start);

		while (start < v.size() / 2 && !compare (v[start], v[tmp])) {
			std::swap (v[start], v[tmp]);
			start = tmp;
			if (compare (v[right_child (start)], v[left_child (start)]))
				tmp = right_child (start);
			else
				tmp = left_child (start);
		}
	}

	void heapify()
	{
		for (std::size_t it = 0; it != v.size(); ++it)
			upheap (it);
	}
};

#endif
