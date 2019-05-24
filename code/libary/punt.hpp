#ifndef punt_HPP
#define punt_HPP

class punt{
private:
int x;
int y;
public:
  punt( int x = 0, int y = 0 ):
     x( x ), y( y )
   {}
   int getx(){
	   return x;
   }
   int gety(){
	   return y;
   }
};

#endif