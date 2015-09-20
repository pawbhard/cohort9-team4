#define MEAN 0

class Result
{
   int mean_final;
   int mean_elements;
   int min_final;
   int max_final; 

   Result() { mean_final = 0; mean_elements = 0; min_final = 0; max_final = 0;}
   static Result *result;
   Result ( Result const& ) {};
   Result& operator = ( Result const& ) {};

   public :
   static Result* get_instance ()
   { 
       if (!result) result = new Result;
       return result;
   }

   void update_result (int task_id, int num1, int num2)
   {
       task = db->get_opcode(task_id);
       switch (task)
       {
          case MEAN :
             update_mean (num1, num2);
             break;

          case RANGE :
             update_range (num1, num2);
             break;

          default : ; // some error handling
       }
   }

   void update_mean (int num1, int num2);
   void update_range (int num1, int num2);
   int get_mean () {return mean_final;}
   int get_mean_elements () {return mean_elements;}
};
