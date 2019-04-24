#include <iostream>
#include <thread>
#include <condition_variable>

const int max_count = 500000;
bool ping_time = true;
std::condition_variable cond;
std::mutex m;

void Ping()
{
    for(int i = 0; i < max_count; i++)
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return ping_time;});
        std::cout << "ping" << "\n";
        ping_time = false;
        cond.notify_one();
    }
}

void Pong()
{
    for(int i = 0; i < max_count; i++)
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return (!ping_time);});
        std::cout << "pong" << "\n";
        ping_time = true;
        cond.notify_one();
    }
}

int main()
{
    std::thread stream1(Ping);
    std::thread stream2(Pong);
    stream1.join();
    stream2.join();
    return 0;
}
