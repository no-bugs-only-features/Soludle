/**
 * @file Noodle.cpp
 * @author Dylan Wright (no-bugs-only-features)
 * @brief 
 * @version 1.0
 */
#include "Noodle.h"

#include <utility>
#include <iostream>

/**
 * @brief Construct a new Noodle:: Noodle object
 * 
 * @param id The noodles id.
 * @param shape The beginning shape of the noodle.
 */
Noodle::Noodle(int id, Shape shape) : id_(id), shape_(shape), rows_(shape.size()), cols_(shape[0].size()) {}

/**
 * @brief Update the shape of the noodle to the given shape.
 * 
 * This function is used to update the shape of the noodle when it is rotated
 * and reflected.
 * 
 * @param shape The shape to update to.
 */
void Noodle::update_(Shape shape) {
	this->shape_ = shape;
	this->rows_ = this->shape_.size();
	this->cols_ = this->shape_[0].size();
}

/**
 * @brief Flatten a 2-D shape into a 1-D array.
 * 
 * @param shape The 2-D shape to flatten.
 * @return std::vector<int>  The flattened shape.
 */
std::vector<int> Noodle::flatten_(Shape shape) {
	std::vector<int> res;

	for (std::vector<int> row : shape) {
		for (int i : row) {
			res.push_back(i);
		}
	}

	return res;
}

/**
 * @brief Reflect the noodles current shape.
 * 
 * @return Shape The result of the reflection
 */
Shape Noodle::reflect_() {
	Shape res(this->rows_, std::vector<int>(this->cols_, 0));

	for (int i = 0; i < this->rows_; i++) {
		for (int j = 0; j < this->cols_; j++) {
			res[i][this->cols_-j-1] = this->shape_[i][j];
		}
	}

	return res;
}

/**
 * @brief Rotate the noodles current shape clockwise 90 degrees.
 * 
 * @return Shape The result of the rotation.
 */
Shape Noodle::rotate_() {
	Shape res(this->cols_, std::vector<int>(this->rows_, 0));

	for (int i = 0; i < this->rows_; i++) {
		for (int j = 0; j < this->cols_; j++) {
			res[j][this->rows_-i-1] = this->shape_[i][j];
		}
	}

	return res;
}

/**
 * @brief Get all possible reflections of the noodle.
 * 
 * @return std::set<Shape> A set of the unique reflections of the noodle.
 */
std::set<Shape> Noodle::reflections_() {
	std::set<Shape> res;
	std::pair<std::set<Shape>::iterator, bool> ret;

	res.insert(this->shape_);
	res.insert(this->reflect_());

	return res;
}

/**
 * @brief Get all possible rotations of the noodle.
 * 
 * @return std::set<Shape> A set of the unique rotations for the noodle.
 */
std::set<Shape> Noodle::rotations_() {
	std::set<Shape> res;
	std::pair<std::set<Shape>::iterator, bool> ret;

	res.insert(this->shape_);

	do {
		this->update_(this->rotate_());
		ret = res.insert(this->shape_);
	} while (ret.second);

	return res;
}

/**
 * @brief Get all possible permutations of the noodle.
 * 
 * Get all possible permutations (all rotations for each reflection)
 * of the noodle.
 * 
 * @return std::set<Shape> A set of the unique permutations of the noodle.
 */
std::set<Shape> Noodle::permutations_() {
	std::set<Shape> res;

	for (Shape refl : this->reflections_()) {
		this->update_(refl);
		for (Shape rot : this->rotations_()) {
			res.insert(rot);
		}
	}

	return res;
}

/**
 * @brief Get possible board positions.
 * 
 * Finds and returns all possible board positions of the current shape without
 * rotation or reflection. Each row in the returned shape is a flattened board
 * with the noodle's position.
 * 
 * @return Shape A 2-D array of possible board positions.
 */
Shape Noodle::fit_() {
	Shape res;

	for (int i = 0; i <= 5-(this->rows_); i++) {
		for (int j = 0; j <= 11-(this->cols_); j++) {
			Shape board(5, std::vector<int>(11, 0));
			for (int x = 0; x < this->rows_; x++) {
				for (int y = 0; y < this->cols_; y++) {
					board[i+x][j+y] = this->shape_[x][y];
				}
			}
			res.push_back(this->flatten_(board));
		}
	}

	return res;
}

/**
 * @brief Get all rows for this noodle to be used in the KanoodleSolver.
 * 
 * Calls fit_() on each possible noodle permutation, combines them, and
 * appends 12 columns representing the 12 possible noodle ids to the beginning
 * of each row.
 * 
 * @return Shape Rows for the current noodle for the solver's problem matrix.
 */
Shape Noodle::get_rows() {
	Shape res;

	for (Shape perm : this->permutations_()) {
		this->update_(perm);
		Shape rows = this->fit_();
		for (std::vector<int> entry : rows) {
			std::vector<int> row(12, 0);
			row[this->id_] = 1;
			row.insert(row.end(), entry.begin(), entry.end());
			res.push_back(row);
		}
	}

	return res;
}