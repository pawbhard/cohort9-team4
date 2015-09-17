
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
}
