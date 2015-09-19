
class Result
{
    private : 
             Result()
             {
                DATABASE_DEBUG("Created object of database");
             }
    public : 
             static Result get_result_instance(void)
             {
               static Result result;
               return result;
             }
             float update_avg (int total_num, float updated_answer, int num, float ans);
//             int update_deviation (int total_num, int updated_answer, int num, int ans);
}
