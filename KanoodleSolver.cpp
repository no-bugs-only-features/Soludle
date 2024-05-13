/**
 * @file KanoodleSolver.cpp
 * @author Dylan Wright (no-bugs-only-features)
 * @brief 
 * @version 1.0
 */

#include "KanoodleSolver.h"

#include <iomanip>
#include <iostream>
#include <sys/time.h>

/**
 * @brief Construct a new Kanoodle Solver:: Kanoodle Solver object
 * 
 * The 12 Noodles are initialized and a call is made to fill_problem_matrix_() and
 * create_dll_() to fill in the information needed for the solver. The sparse matrix
 * should contain fixed positions of the noodles on the board.
 * 
 * @param sparse_matrix Fixed position of noodles on the board.
 */
KanoodleSolver::KanoodleSolver(std::vector<std::vector<int>> sparse_matrix) {

	Noodle A(0, 
		{
			{0, 1},
			{0, 1},
			{1, 1}
		}
	);

	Noodle B(1, 
		{
			{0, 1},
			{1, 1},
			{1, 1}
		}
	);

	Noodle C(2, 
		{
			{0, 1},
			{0, 1},
			{0, 1},
			{1, 1}
		}
	);

	Noodle D(3, 
		{
			{0, 1},
			{0, 1},
			{1, 1},
			{0, 1}
		}
	);

	Noodle E(4, 
		{
			{0, 1},
			{0, 1},
			{1, 1},
			{1, 0}
		}
	);

	Noodle F(5, 
		{
			{0, 1},
			{1, 1}
		}
	);

	Noodle G(6, 
		{
			{0, 0, 1},
			{0, 0, 1},
			{1, 1, 1}
		}
	);

	Noodle H(7, 
		{
			{0, 0, 1},
			{0, 1, 1},
			{1, 1, 0}
		}
	);

	Noodle I(8, 
		{
			{1, 0, 1},
			{1, 1, 1}
		}
	);

	Noodle J(9, 
		{
			{1},
			{1},
			{1},
			{1}
		}
	);

	Noodle K(10, 
		{
			{1, 1},
			{1, 1}
		}
	);

	Noodle L(11,
		{
			{0, 1, 0},
			{1, 1, 1},
			{0, 1, 0}
		}
	);

	this->noodles_ = {A, B, C, D, E, F, G, H, I, J, K, L};

	this->fill_problem_matrix_(sparse_matrix);

	this->create_dll_();

	this->num_solutions_ = 0;
}

/**
 * @brief 
 * 
 * @param sparse_matrix 
 */
void KanoodleSolver::fill_problem_matrix_(std::vector<std::vector<int>> sparse_matrix) {
	for (int i = 0; i < (int) sparse_matrix.size(); i++) {
		std::vector<int> row(12+55, 0);
		int noodle_id = sparse_matrix[i][0];
		row[noodle_id] = 1;
		this->noodles_.erase(this->noodles_.begin()+noodle_id-i);
		for (int j = 1; j < (int) sparse_matrix[i].size(); j++) {
			int index = sparse_matrix[i][j];
			row[index] = 1;
		}
		this->problem_matrix_.push_back(row);
	}

	for (Noodle n : this->noodles_) {
		for (std::vector<int> row : n.get_rows()) {
			this->problem_matrix_.push_back(row);
		}
	}
}

void KanoodleSolver::create_dll_() {
	int num_rows = (int) this->problem_matrix_.size();
	int num_cols = (int) this->problem_matrix_[0].size();

	// Create the circular doubly linked list. One extra row for column headers.
	this->dll_ = std::vector<std::vector<struct Node>>(num_rows+1, std::vector<struct Node>(num_cols));

	// Create column headers.
	for (int c = 0; c < num_cols; c++) {

		this->dll_[0][c].R = &this->dll_[0][(c+1) % num_cols];
		this->dll_[0][c].L = &this->dll_[0][(c-1) >= 0 ? c-1 : num_cols-1];
		// Up and down links will need to be updated later.
		this->dll_[0][c].U = &this->dll_[0][c];
		this->dll_[0][c].D = &this->dll_[0][c];
		// Column link points to itself.
		this->dll_[0][c].C = &this->dll_[0][c];
		this->dll_[0][c].count_ = 0;
		this->dll_[0][c].x = -1;
		this->dll_[0][c].y = c;
	}


	// Fill in the rest of the matrix.
	for (int r = 1; r < num_rows+1; r++) {
		for (int c = 0; c < num_cols; c++) {

			// If this entry is set in the problem matrix
			if (this->problem_matrix_[r-1][c]) {

				// Set the row id
				this->dll_[r][c].x = r-1;
				this->dll_[r][c].y = c;

				// Set its column pointer.
				this->dll_[r][c].C = &this->dll_[0][c];

				// Increment the column header's count.
				this->dll_[0][c].count_++;

				// Set the right link.
				int i = c;
				do { i = (i+1) % num_cols; } while (!this->problem_matrix_[r-1][i] && i != c);
				this->dll_[r][c].R = &this->dll_[r][i];


				// Set the left link.
				i = c;
				do { i = i-1 >= 0 ? i-1 : num_cols-1; } while (!this->problem_matrix_[r-1][i] && i != c);
				this->dll_[r][c].L = &this->dll_[r][i];

				// Set the up link.
				i = r;
				do { i = i-1; } while (i != 0 && !this->problem_matrix_[i-1][c]);
				this->dll_[r][c].U = &this->dll_[i][c];
				// If the up node is the column header, update the column headers down link.
				if (i == 0)
					this->dll_[0][c].D = &this->dll_[r][c];


				// Set the down link.
				i = r;
				do { i = (i+1) % (num_rows+1); } while (i != 0 && !this->problem_matrix_[i-1][c]);
				this->dll_[r][c].D = &this->dll_[i][c];
				// If the down node is the column header, update the column headers up link.
				if (i == 0)
					this->dll_[0][c].U = &this->dll_[r][c];

			}
		}

	}

	// Set the values for H.
	this->H_ = new struct Node;
	this->H_->R = &this->dll_[0][0];
	this->dll_[0][0].L = this->H_;
	this->H_->L = &this->dll_[0][num_cols-1];
	this->dll_[0][num_cols-1].R = this->H_;
}

void KanoodleSolver::cover_(struct Node* n) {

	struct Node* c = n->C;

	c->R->L = c->L;
	c->L->R = c->R;

	for (struct Node* i = c->D; i != c; i = i->D) {
		for (struct Node* j = i->R; j != i; j = j->R) {
			j->D->U = j->U;
			j->U->D = j->D;
			j->C->count_--;
		}
	}
}

void KanoodleSolver::uncover_(struct Node* n) {

	struct Node* c = n->C;

	for (struct Node* i = c->U; i != c; i = i->U) {
		for (struct Node* j = i->L; j != i; j = j->L) {
			j->C->count_++;
			j->D->U = j;
			j->U->D = j;
		}
	}

	c->R->L = c;
	c->L->R = c;
}

struct KanoodleSolver::Node* KanoodleSolver::get_min_col_() {
	struct Node* c = this->H_->R;
	for (struct Node* j = c; j != this->H_; j = j->R) {
		if (j->count_ < c->count_)
			c = j;
	}
	return c;
}

/**
 * @brief 
 * 
 * @param k The current depth of the search.
 */
void KanoodleSolver::search_(int k) {
	if (this->H_->R == this->H_) {
		this->print_solutions_();
		return;
	}

	struct Node* c = this->get_min_col_();

	this->cover_(c);

	for (struct Node* r = c->D; r != c; r = r->D) {
		this->solutions_.push_back(r->x);

		for (struct Node* j = r->R; j != r; j = j->R) {
			this->cover_(j);
		}

		this->search_(k+1);

		c = r->C;
		for (struct Node* j = r->L; j != r; j = j->L) {
			this->uncover_(j);
		}

		this->solutions_.pop_back();

	}

	this->uncover_(c);
}

void KanoodleSolver::print_solutions_() {
	for (int row_id : this->solutions_) {
		for (int c = 0; c < (int) this->problem_matrix_[row_id].size(); c++) {
			if (this->problem_matrix_[row_id][c] == 1)
				std::cout << c << ' ';
		}
		std::cout << '\n';
	}
	this->num_solutions_++;
}

void KanoodleSolver::solve() {

	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	this->search_(0);

	clock_gettime(CLOCK_MONOTONIC, &end);

	double time_taken;
	time_taken = (end.tv_sec - start.tv_sec) * 1e9;
	time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

	if (this->num_solutions_ == 0)
		std::clog << "No solutions found in" << std::fixed << time_taken << std::setprecision(9) << "s.\n";
	else 
		std::clog << "Found " << this->num_solutions_ << " solutions in " << std::fixed << time_taken << std::setprecision(9) << "s.\n";
}

void KanoodleSolver::print_dll() {
	for (std::vector<struct Node> row : this->dll_) {
		for (struct Node n : row) {
			if (n.C)
				std::cout << 'X';
			else 
				std::cout << '-';
		}
		std::cout << '\n';
	}
}

