//#ifndef MCTS_PARALLEL_H
//#define MCTS_PARALLEL_H
//#include<unordered_map>
//#include<thread>
//#include<atomic>
//#include<vector>
//#include"ThreadPool.h"
//class TreeNode
//{
//public:
//	friend class MCTS;
//	TreeNode(const TreeNode& node);
//	TreeNode(TreeNode* parent);
//	unsigned int select(double c_puct);
//	void expand();
//	void backup(double value);
//	double get_value(double c_puct, unsigned int sum_n_visited_);
//	inline bool get_is_leaf(){ std::lock_guard<std::mutex> lock(expand_lock); return is_leaf; }
//	TreeNode& operator=(const TreeNode& node);
//
//private:
//	TreeNode* parent = nullptr;
//	std::unordered_map<unsigned int, TreeNode*> children;
//	bool is_leaf = true;
//	std::mutex children_lock;
//	std::mutex data_lock;
//	std::mutex expand_lock;
//	std::atomic<int> n_unobserved = 0;
//	std::atomic<unsigned int> n_visited = 0;
//};
//
//
//
//#endif 
////#include<unordered_map>
////#include<string>
////#include<vector>
////#include<thread>
////#include<atomic>
////#include<random>
////#include<ctime>
////#include"goboard.cpp"
////#include"ThreadPool.h"
////#include"Net.h"
////class TreeNode
////{
////public:
////	friend class MCTS;
////	TreeNode(const TreeNode& node);
////	TreeNode(TreeNode* parent, double p_sa);
////
////	TreeNode& operator=(const TreeNode& node);
////
////	unsigned int select(double c_puct, double c_virtual_loss);
////	void expand(const std::vector<double>& action_priors);
////	void backup(double leaf_value);
////
////	double get_value(double c_puct, double c_virtual_loss, unsigned int sum_n_visited);
////	inline bool get_is_leaf() { std::lock_guard<std::mutex> lock(expand_lock); return is_leaf; }
////
////private:
////	TreeNode* parent = nullptr;
////	std::unordered_map<unsigned int, TreeNode*> children;
////	bool is_leaf = true;
////	std::mutex children_lock;
////	std::mutex data_lock;
////	std::mutex expand_lock;
////	std::atomic<int> n_unobserved = 0;
////	std::atomic<unsigned int> n_visited = 0;
////	double q_sa = 0;
////	double p_sa = 0;
////	std::atomic<int> virtual_loss = 0;
////};
////
////class MCTS {
////public:
////	MCTS(Net* net, unsigned int thread_num, unsigned int mcts_branch_num, unsigned int action_size, double c_puct, double c_virtual_loss, double prob = 0.75);
////	vector<double> getActionProbs(Board* goboard, double temp = 1);
////	double getValue() { return -root->q_sa; }
////	void update(unsigned int last_move);
////
////private:
////	void simulate(std::shared_ptr<Board> game);
////	static void tree_deleter(TreeNode* t);
////	std::unique_ptr<TreeNode, decltype(MCTS::tree_deleter)*> root;
////	ThreadPool thread_pool;
////	Net* net;
////	unsigned int mcts_branch_num;
////	double c_puct;
////	double c_virtual_loss;
////	unsigned int action_size;
////	std::default_random_engine gen;
////	std::bernoulli_distribution b_dist;
////
////};
