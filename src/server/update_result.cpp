#include "server_header.h"

void Result::update_mean(int num, int answer)
{
    int new_mean;
    std::cout << "Updating mean\n";
    new_mean = ((mean_final * mean_elements) + (answer * num)) / (mean_elements + num);

    mean_elements += num;
    mean_final = new_mean;
}

void Result::update_range(int min, int max)
{
    std::cout << "Updating range\n";

    if (min < min_final)
       min_final = min;

    if (max > max_final)
       max_final = max;
}
