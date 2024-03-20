#ifndef __CLIENT_TIMER_H__
#define __CLIENT_TIMER_H__

#include <chrono>

using namespace std::chrono;

class ClientTimer {
public:
    duration<double, std::micro> sum; // Total time measured
    duration<double, std::micro> max; // Maximum single interval measured
    duration<double, std::micro> min; // Minimum single interval measured
    int op_count; // Number of operations (intervals) measured

    time_point<std::chrono::high_resolution_clock> start_time; // Start time of the current interval
    duration<double, std::micro> elapsed_time; // Elapsed time of the last measured interval

    ClientTimer();

    void operator=(const ClientTimer &timer) {
        sum = timer.sum;
        max = timer.max;
        min = timer.min;
        op_count = timer.op_count;
        start_time = timer.start_time;
        elapsed_time = timer.elapsed_time;
    }

    void Start();

    void End();

    void EndAndMerge();

    void Merge(ClientTimer timer);

    void PrintStats();
};

#endif // end of #ifndef __CLIENT_TIMER_H__
