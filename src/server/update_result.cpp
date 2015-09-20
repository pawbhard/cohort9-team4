#include "update_result.h"

void Result::update_mean(int num, double answer)
{
    double new_mean;

    new_mean = ((mean_final * mean_elements) + (answer * num)) / (mean_elements + num);

    mean_elements += num;
    mean_final = new_mean;
}
