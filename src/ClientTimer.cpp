#include <iomanip> // For std::setprecision
#include <iostream> // For std::cout

#include "ClientTimer.h"

ClientTimer::ClientTimer() {
    // Initialize the sum of all recorded times to 0 microseconds
    sum = duration<double, std::micro>(0);
    // Initialize the maximum recorded time to 0, to be updated during operations
    max = duration<double, std::micro>(0);
    // Initialize the minimum recorded time to a large value, to be updated during operations
    min = duration<double, std::micro>(9999999999.9f);
    // Initialize the operation count to 0
    op_count = 0;
}

// Start timing an operation
void ClientTimer::Start() {
    // Record the current time as the start time
    start_time = high_resolution_clock::now();
}

// End timing an operation
void ClientTimer::End() {
    // Record the current time as the end time
    auto end_time = high_resolution_clock::now();
    // Calculate the elapsed time as the difference between end time and start time
    elapsed_time = (end_time - start_time);
}

// End the current timing session and merge its results into the aggregate stats
void ClientTimer::EndAndMerge() {
    End(); // First, stop the current timing session
    op_count++; // Increment the operation count
    sum += elapsed_time; // Add the elapsed time to the total sum
    // Update the minimum time if the current session's time is lower
    if (elapsed_time < min) {
        min = elapsed_time;
    }
    // Update the maximum time if the current session's time is higher
    if (elapsed_time > max) {
        max = elapsed_time;
    }
}

// Merge the stats from another timer into this one
void ClientTimer::Merge(ClientTimer timer) {
    sum += timer.sum; // Add the sum of times from the other timer
    op_count += timer.op_count; // Add the operation count from the other timer
    // Update the minimum time if the other timer's minimum is lower
    if (timer.min < min) {
        min = timer.min;
    }
    // Update the maximum time if the other timer's maximum is higher
    if (timer.max > max) {
        max = timer.max;
    }
}

// Print the statistical results of the timing sessions
void ClientTimer::PrintStats() {
    std::cout << std::fixed << std::setprecision(3); // Set the decimal precision for output
    // Print the average time per operation
    std::cout << sum.count() / op_count << "\t";
    // Print the minimum time recorded
    std::cout << min.count() << "\t";
    // Print the maximum time recorded
    std::cout << max.count() << "\t";
    // Print the operations per second (converted from microseconds)
    std::cout << op_count / elapsed_time.count() * 1000000.0f << std::endl;
}