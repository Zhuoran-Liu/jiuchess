#pragma once
#include <torch/script.h>
#include<mutex>
#include <string>
#include <future>
#include <memory>
#include <queue>
#include <vector>
#include<utility>

using std::string;
using std::pair;
using std::vector;
using std::future;
using std::promise;
using std::unique_ptr;
using std::shared_ptr;
using std::mutex;
using std::condition_variable;
using std::queue;
using std::thread;
using torch::Tensor;
using torch::jit::script::Module;
class Net
{
public:
	using ActProbs_Value = pair<vector<float>, float>;
	Net(const std::string& model_path, unsigned int batch_size);
	future<ActProbs_Value> predict(Tensor& state_tensor);
	void setBatchSize(unsigned int batch_size) { this->batch_size = batch_size; };
	~Net();
private:
	using task_type = pair<Tensor, promise<ActProbs_Value>>;
	void infer();  // infer
	thread predictThread;  //�������Ԥ���߳�
	bool running = true;             //�߳���������
	queue<task_type> tasks;  //�߳������
	mutex lock;              //�߳���
	condition_variable cv;   //�������������
	Module module;
	unsigned int batch_size;
};

