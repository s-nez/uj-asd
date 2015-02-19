#ifndef MUSEUM_H
#define MUSEUM_H

#include <algorithm>
#include <sstream>
#include "structures.h"
#include "colors.h"


// Uwaga! Publiczne metody indeksują od 1, a prywatne od 0
class Museum {
		using coord = std::pair<std::size_t, std::size_t>;
	public:
		Museum (std::size_t, std::size_t);
		~Museum();
		int& room (std::size_t, std::size_t);
		// Ścieżka od wejścia do wyjścia z przejściem przez salę o współrzędnych podanych w argumentach
		PathData tour (std::size_t, std::size_t);
		// Ścieżka bezpośrednia od wejścia do wyjścia
		PathData entry_to_exit ();
		bool graphics = false;
	private:
		coord entry, exit; // Współrzędne wejścia i wyjścia
		int** rooms; // Tablica z kosztami sal
		RoomData** aux_array; // Tablica pomocnicza do alg. Dijsktry
		bool** aux_path; // Tablica do sptawdzania powtarzających się sal
		std::size_t height, width; // Rozmiar muzeum

		int& room (coord);
		RoomData& aux (coord); // Dostęp do tablicy pomocniczej
		void reset_aux(); // Przywraca tablicę pomocniczą do początkowego stanu
		void reset_path(); // Czyści tablicę powtórzeń sal
		inline bool& part_of_path(coord); // Sprawdza, czy sala jest częścią ścieżki
		inline bool room_exists (coord); // Sprawdza, czy sala istnieje
		void relax (coord); // Procedura do alg. Dijkstry
		// Określa w którym kierunku jest drugi argument względem pierwszego
		char move_direction (coord, coord);
		void show_aux_dist(); // Wyświetla odległości z tablicy pomocniczej
		void show_path(); // Wyświetla przejście po oryginalnej tablicy
		// Oblicza najtańsze przejście miedzy dwoma punktami algorytmem Dijkstry
		PathData cheapest_path (coord, coord);
};

int& Museum::room (coord target)
{
	return rooms[target.first][target.second];
}

RoomData& Museum::aux (coord target)
{
	return aux_array[target.first][target.second];
}

void Museum::reset_aux()
{
	for (std::size_t h = 0; h != height; ++h)
		for (std::size_t w = 0; w != width; ++w)
			aux_array[h][w] = RoomData();
}

void Museum::reset_path()
{
	for (std::size_t h = 0; h != height; ++h)
		for (std::size_t w = 0; w != width; ++w)
			aux_path[h][w] = false;
}

inline bool& Museum::part_of_path(coord room_loc)
{
	return aux_path[room_loc.first][room_loc.second];
}

inline bool Museum::room_exists (coord room_to_check)
{
	return (room_to_check.first >= 0
	        && room_to_check.first < height
	        && room_to_check.second >= 0
	        && room_to_check.second < width);
}

void Museum::relax (coord target)
{
	coord neighbours[4] = {
		coord (target.first - 1, target.second), // north
		coord (target.first + 1, target.second), // south
		coord (target.first, target.second + 1), // west
		coord (target.first, target.second - 1)  // east
	};

	for (int it = 0; it != 4; ++it) {
		if (room_exists (neighbours[it])) {
			int tmp_dist = aux (target).dist + room (neighbours[it]);
			if (aux (neighbours[it]).dist > tmp_dist) {
				aux (neighbours[it]).dist = tmp_dist;
				aux (neighbours[it]).prev = target;
			}
		}
	}
}

char Museum::move_direction (coord from, coord to)
{
	if (from.first == to.first)
		return from.second < to.second ? 'E' : 'W';
	else
		return from.first < to.first ? 'S' : 'N';
}

void Museum::show_aux_dist()
{
	if (!graphics) return;
	for (std::size_t h = 0; h != height; ++h) {
		for (std::size_t w = 0; w != width; ++w) {
			std::cout << aux (coord (h, w)).color;
			if (aux_array[h][w].dist == std::numeric_limits<int>::max())
				std::cout << "INF ";
			else
				std::cout << aux_array[h][w].dist << " ";
			std::cout << Color::Reset;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Museum::show_path()
{
	if (!graphics) return;
	std::cout << "\n";
	for (std::size_t h = 0; h != height; ++h) {
		for (std::size_t w = 0; w != width; ++w) {
			std::cout << aux (coord (h, w)).color;
			std::cout << rooms[h][w] << " ";
			std::cout << Color::Reset;
		}
		std::cout << '\n';
	}
}

PathData Museum::cheapest_path (coord start, coord finish)
{
	reset_aux();
	Heap queue (aux_array, height, width);
	aux (start).dist = 0;
	for (std::size_t h = 0; h != height; ++h)
		for (std::size_t w = 0; w != width; ++w)
			queue.insert (coord (h, w));
	coord current;
	do {
		current = queue.pop();
		relax (current);
		aux (current).visited = true;
		queue.heapify();
	} while (!queue.empty() && current != finish);
	PathData result;
	result.price = aux(finish).dist;
	for (coord it = finish; it != start; it = aux (it).prev) {
		aux (it).color = Color::Green;
		result.directions.push_back (move_direction (aux (it).prev, it));
		//result.price += room (it);
		if (!part_of_path(it)) {
			++result.room_count;
			part_of_path(it) = true;
		}
	}
	aux (start).color = Color::Blue;
	aux (finish).color = Color::Red;
	//show_aux_dist();
	show_path();
	std::reverse (result.directions.begin(), result.directions.end());
	return result;
}

Museum::Museum (std::size_t n_height, std::size_t n_width)
	: height (n_height), width (n_width)
{
	entry.first = n_height - 1;
	entry.second = 0;
	exit.first = 0;
	exit.second = n_width - 1;
	rooms = new int* [height];
	aux_array = new RoomData*[height];
	aux_path = new bool*[height];
	for (std::size_t it = 0; it != height; ++it) {
		rooms[it] = new int[width];
		aux_array[it] = new RoomData[width];
		aux_path[it] = new bool[width];
	}
}

Museum::~Museum()
{
	for (std::size_t it = 0; it != height; ++it) {
		delete[] rooms[it];
		delete[] aux_array[it];
		delete[] aux_path[it];
	}
	delete[] rooms;
	delete[] aux_array;
	delete[] aux_path;
}

int& Museum::room (std::size_t hpos, std::size_t wpos)
{
	if (hpos < 1 || wpos < 1 || hpos > height || wpos > width) {
		std::stringstream message;
		message << "No such room (" << hpos << ", " << wpos << ")";
		throw message.str();
	}
	return rooms[hpos - 1][wpos - 1];
}

PathData Museum::tour (std::size_t target_hpos, std::size_t target_wpos)
{
	reset_path();
	--target_hpos;
	--target_wpos;
	PathData to_dest = cheapest_path (entry, coord (target_hpos, target_wpos));
	to_dest.price += room (entry);
	++to_dest.room_count;
	PathData to_exit = cheapest_path (coord (target_hpos, target_wpos), exit);
	return to_dest + to_exit;
}

PathData Museum::entry_to_exit ()
{
	reset_path();
	PathData result = cheapest_path (entry, exit);
	result.price += room (entry);
	++result.room_count;
	return result;
}

#endif
