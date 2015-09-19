#include "server_collect.h"

using namespace std;

float Result::update_avg(int total_num, float updated_answer, int num, float answer)
{
    float average;

    average = ((total_num * updated_answer) + (num * answer)) / (total_num + num);
    return average;
}


