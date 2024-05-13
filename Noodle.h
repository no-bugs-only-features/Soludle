/**
 * @file Noodle.h
 * @author Dylan Wright (no-bugs-only-features)
 * @brief 
 * @version 1.0
 */
#ifndef NOODLE_H
#define NOODLE_H

#include <set>
#include <vector>

using Shape = std::vector<std::vector<int>>;

class Noodle {
	private:

		int id_;
		Shape shape_;
		int rows_;
		int cols_;

		void update_(Shape shape);
		std::vector<int> flatten_(Shape shape);

		Shape reflect_();
		Shape rotate_();

		std::set<Shape> reflections_();
		std::set<Shape> rotations_();
		std::set<Shape> permutations_();

		Shape fit_();

	public:
		Noodle(int id, Shape shape);

		Noodle() = delete;

		inline int get_id() { return id_; }

		Shape get_rows();
};

#endif