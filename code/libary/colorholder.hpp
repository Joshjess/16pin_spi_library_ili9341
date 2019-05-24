#ifndef colorholder_HPP
#define colorholder_HPP

class colorholder{
private:
int color1;
int color2;
public:
  colorholder( int color1 = 0, int color2 = 0 ):
     color1( color1 ), color2( color2 )
   {}
   int getcolor1(){
	   return color1;
   }
   int getcolor2(){
	   return color2;
   }
};

#endif