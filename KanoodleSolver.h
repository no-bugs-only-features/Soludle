/**
 * @file KanoodleSolver.h
 * @author Dylan Wright (no-bugs-only-features)
 * @brief 
 * @version 1.0
 */
#ifndef KANOODLESOLVER_H
#define KANOODLESOLVER_H

#include "Noodle.h"

class KanoodleSolver {
	private:
		struct Node;

		std::vector<Noodle> noodles_;
		std::vector<std::vector<int>> problem_matrix_;
		std::vector<std::vector<struct Node>> dll_;
		struct Node* H_;
		int num_solutions_;
		std::vector<int> solutions_;

		void fill_problem_matrix_(std::vector<std::vector<int>> sparse_matrix);
		void create_dll_();

		void cover_(struct Node* c);
		void uncover_(struct Node* c);

		struct Node* get_min_col_();

		void search_(int k);
		void print_solutions_();

	public:
		KanoodleSolver(std::vector<std::vector<int>> sparse_matrix);

		KanoodleSolver(KanoodleSolver&) = delete;
		KanoodleSolver() = delete;
	
		void solve();

		void print_dll();
};

struct KanoodleSolver::Node {
	Node* U;
	Node* D;
	Node* L;
	Node* R;
	Node* C;
	int count_;
	int x;
	int y;
};

#endif