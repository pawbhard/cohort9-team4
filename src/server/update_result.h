#define MEAN 0

class Result
{
   double mean_final;
   int mean_elements;

   Result() { mean_final = 0; mean_elements = 0;}
   static Result *result;
   Result ( Result const& ) {};
   Result& operator = ( Result const& ) {};

   public :
   static Result* get_instance ()
   { 
       if (!result) result = new Result;
       return result;
   }

   void update_result (int task, int num, double answer)
   {
       switch (task)
       {
          case MEAN :
             update_mean (num, answer);
             break;

          default : ; // some error handling
       }
   }

   void update_mean (int num, double answer);
   double get_mean () {return mean_final;}
   int get_mean_elements () {return mean_elements;}
};
