#include "Net.h"
using namespace std;
Net::Net(const std::string& model_path, unsigned int batch_size) :
    module(torch::jit::load(model_path)),
    batch_size(batch_size)
{
    module.eval();
    module.to(at::kCUDA);
    predictThread = thread([this] {
        while (running) {
            infer();
        }
    });
}

future<Net::ActProbs_Value> Net::predict(Tensor& data)
{
    promise<ActProbs_Value> promise;
    auto ret = promise.get_future();
    {
        lock_guard<mutex> lock(this->lock);
        tasks.emplace(make_pair(data, std::move(promise)));
    }
    cv.notify_all();
    return ret;
}

Net::~Net()
{
    running = false;
    predictThread.join();
}

void Net::infer()
{
    // get inputs
    vector<Tensor> dataStack;
    vector<promise<ActProbs_Value>> promises;
    bool timeout = false;
    while (dataStack.size() < batch_size && !timeout) {
        // pop task
        {
            unique_lock<mutex> lock(this->lock);
            if (cv.wait_for(lock, 1ms,
                [this] { return tasks.size() > 0; })) {
                auto task = std::move(tasks.front());
                dataStack.emplace_back(std::move(task.first));
                promises.emplace_back(std::move(task.second));
                this->tasks.pop();
            }
            else
                timeout = true;
        }
    }

    // inputs empty
    if (dataStack.size() == 0)
        return;

    // infer
    std::vector<torch::jit::IValue> inputs{torch::cat(dataStack,0).to(at::kCUDA)};
    /*std::cout << inputs << std::endl;*/
    auto result = module.forward(inputs).toTuple();
    torch::Tensor p_batch = result->elements()[0].toTensor().to(at::kCPU);
    torch::Tensor v_batch = result->elements()[1].toTensor().to(at::kCPU);
    // set promise value
    for (unsigned int i = 0; i < promises.size(); i++) {
        torch::Tensor p = p_batch[i];
        torch::Tensor v = v_batch[i];
        std::vector<float> prob(p.data_ptr<float>(), p.data_ptr<float>() + p.numel());
        float value = v.item<float>();
        ActProbs_Value temp{std::move(prob), std::move(value) };
        promises[i].set_value(std::move(temp));
    }
}
