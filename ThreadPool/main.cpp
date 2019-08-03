#include <vector>
#include <thread>
#include <future>
#include <queue>
#include <iostream>
#include <string>
#include <cmath>

class MThreadPool
{
public:
	MThreadPool(size_t i_num_of_threads = std::thread::hardware_concurrency()) : m_num_of_threads(i_num_of_threads), m_num_of_active_threads(0)
	{
		if (!m_num_of_threads)
			m_num_of_threads = 2;
		for (int i = 0; i < m_num_of_threads; ++i)
			m_threads.emplace_back(&MThreadPool::_Loop, this, i);

	}
	~MThreadPool()
	{
		Done = true;
		m_task_cv.notify_all();
		for (auto& p : m_threads)
			p.join();
	}


	template<typename F, typename... Args>
	auto Do(F&& func, Args&&... args)
	{
		using ret_type = decltype(func(args...));

		auto promise = std::make_shared<std::promise<ret_type>>();
		auto future = promise->get_future();

		auto new_task = [promise, func = std::forward<F>(func), &args...] () mutable {
            promise->set_value(func(std::forward<Args>(args)...));
		};

        {
		std::lock_guard<std::mutex> g(m_task_mutex);
        m_tasks.emplace(std::move(new_task));
        }

        m_task_cv.notify_one();

		return future;
	}

	size_t TaskNumber() const
	{
		return m_tasks.size();
	}

private:
	void _Loop(size_t index)
	{
	    std::string thread_name("Thread ");
	    thread_name += std::to_string(index);
		while (true)
		{
			std::unique_lock<std::mutex> ul(m_task_mutex);
			m_task_cv.wait(ul, [this] {return TaskNumber() > 0 || Done; });

			if (Done)
				break;

			auto cur_task = std::move(m_tasks.front());
			m_tasks.pop();
			ul.unlock();

            _Log(thread_name + " has taken a task");

			m_num_of_active_threads++;
            cur_task();
            m_num_of_active_threads--;

            if(m_num_of_active_threads == 0 && m_tasks.empty())
                _Log("We are done here");
		}

		_Log(thread_name + " is Over");
	}

	void _Log(const std::string& msg)
	{
		std::lock_guard<std::mutex> g(m_log_mutex);
		std::cout << msg << std::endl;
	}

	size_t m_num_of_threads;
	std::vector<std::thread> m_threads;

	std::queue<std::function<void()>> m_tasks;

	std::condition_variable m_task_cv;

	std::mutex m_task_mutex;
	std::mutex m_log_mutex;

	std::atomic<unsigned int> m_num_of_active_threads;

	bool Done = false;
};

int main()
{
	{
		MThreadPool pool;
		int tasks = 30;
		std::vector<double> v(10000000, 0);
		auto f = [] (std::vector<double>& v){
		    double sum = 0;
			for (const auto& i : v)
				sum += std::sqrt(i + 1000);
			return sum;
		};


        std::vector<std::future<double>> futures;
        futures.resize(tasks);
        for (int i = 0; i < tasks; ++i)
            futures[i] = pool.Do(f, v);

        for(auto& f : futures)
            f.get();


        std::this_thread::sleep_for(std::chrono::seconds(5));

        for (int i = 0; i < tasks; ++i)
            futures[i] = pool.Do([] (std::vector<double>& v){
                double sum = 0;
                for (const auto& i : v)
                    sum += std::sqrt(i + 1000);
                return sum;
            }, v);

        for(auto& f : futures)
            f.get();

	}
}
