//          Copyright Joshua Offermans 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
// UTFT, TFT Touch and UTOUCH libary were used as example



#include "display.hpp"
#include "punt.hpp"

namespace hwlib{
class invert_port_out_from_pins : public port_out {
private:
   
   uint_fast8_t _number_of_pins;  

   // 8 must match the number of parameters of the constructor
   pin_out * pins[ 8 ];   
   
public:

   /// \brief
   /// construct a port_out from up to 16 pin_outs
   /// \details
   /// This constructor creates a port_out from up to 8 pin_out pins.
   /// The first pin is the lowest pin in the port, etc.
   invert_port_out_from_pins(
      pin_out & p0 = pin_out_dummy,
      pin_out & p1 = pin_out_dummy,
      pin_out & p2 = pin_out_dummy,
      pin_out & p3 = pin_out_dummy,
      pin_out & p4 = pin_out_dummy,
      pin_out & p5 = pin_out_dummy,
      pin_out & p6 = pin_out_dummy,
      pin_out & p7 = pin_out_dummy
   ):
      pins{ &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7}
   {
      for( _number_of_pins = 0; _number_of_pins < 8; ++_number_of_pins ){
         if( pins[ _number_of_pins ] == & pin_out_dummy ){
             break;
         }            
      }
   }            

   uint_fast8_t number_of_pins() override {
      return _number_of_pins;               
   }   
   
   void set( 
      uint_fast8_t x,
      buffering buf = buffering::unbuffered 
   ) override {
      for( uint_fast8_t i = 0; i < _number_of_pins; i++ ){
         pins[ i ]->set( ( x & 0x80 ) != 0 );
         x = x << 1;
      }         
   }

};

};






/// \brief
/// map is standard arduino function to arrang numbers
/// \details 
/// it re-maps a number from one range to another
int display::map(int x, int in_min, int in_max, int out_min, int out_max){
	  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/// \brief
/// cirle drawing algorithm
/// \details
/// An alogrithm from wikipedia same as hwlib thats draws circle based on x and y cordinate
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void display::drawcircle(int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int err = 0;
	cs.set(0);
    while (x >= y)
    {
        drawPixel(x0 + x, y0 + y);
        drawPixel(x0 + y, y0 + x);
        drawPixel(x0 - y, y0 + x);
        drawPixel(x0 - x, y0 + y);
        drawPixel(x0 - x, y0 - y);
        drawPixel(x0 - y, y0 - x);
        drawPixel(x0 + y, y0 - x);
        drawPixel(x0 + x, y0 - y); 

        y += 1;
        if (err <= 0)
        {
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
	}
	cs.set(1);
}


/// \brief
/// reads touch data
/// \details
/// reads touch data from the data line on basis of a clock line
uint16_t display::ReadData(void)
{
  uint8_t index;
  int Data;
  Data = 0;
  for (index = 12; index > 0; index--)
  {
    Data += T_do.get() << (index-1);
    T_clk.set(1); T_clk.set(0);
  }
  return Data;
}  

/// \brief
/// Chooses wich Axis to read
/// \details
/// Send command to the touch controller if it want to read X or Y Axis then it sets clock pulse and starts the read data function
int16_t display::ReadAxis(int Axis)
{
  int Data;
  
  T_cs.set(0);
  if (Axis)
  {
    OutputData(0x90);   
  }else
  {
    OutputData(0xD0);
  }

	T_clk.set(1);T_clk.set(0);
	Data = ReadData();
	T_clk.set(1);T_clk.set(0);
	T_clk.set(1);T_clk.set(0);
	T_clk.set(1);T_clk.set(0);
	T_cs.set(1);
	T_din.set(0);
	return Data;
}


/// \brief
/// sends a command to touchcontroller
/// \details
/// sends a 8 bit command to touch conrtroller uses a clockvoid display::OutputData(uint8_t Data)

void display::OutputData(uint8_t Data)
{
  uint8_t index;

  for (index = 8; index > 0; index--)
  {
    T_din.set((Data >> (index -1)) & 1);
    T_clk.set(1); T_clk.set(0);
  }
}





/// \brief
/// sends data to display controller
/// \details
/// sends data to display in 16 bit format in two 8 bit loops
/// also supports 16bit color format
/// \{
void display::LCD_write_Bus(uint8_t data1,uint8_t  data2){
	auto bus1 = hwlib::invert_port_out_from_pins(
		db15,
		db14,
		db13,
		db12,
		db11,
		db10,
		db9,
		db8
	);
	
	auto bus2 = hwlib::invert_port_out_from_pins(
		db7,
		db6,
		db5,
		db4,
		db3,
		db2,
		db1,
		db0
	);
	
	
	bus1.set(data1);
	bus2.set(data2);
	wr.set(0);
	wr.set(1);	
}
/// \}


/// \brief 
/// function sends command
/// \details
/// this function is used to send command to the display controller
void display::LCD_Write_COM(uint8_t com){
	rs.set(0);
	LCD_write_Bus(0x00,com);
}

/// \name
/// LCD_Write_DATA
/// \brief 
/// function sends pixel data
/// \details
/// this function is used to send 8 or 16 bit pixel data to the display controller
/// \{
void display::LCD_Write_DATA (uint8_t  data1, uint8_t  data2){
	rs.set(1);
	LCD_write_Bus(data1,data2);
	
}
void display::LCD_Write_DATA (uint8_t  data1){
	rs.set(1);
	LCD_Write_DATA(data1,data1);
}
/// \}

/// \brief
/// sends pixel location
/// \details
/// sends pixel data two places in the memory

void display::Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){

	LCD_Write_COM(0x2a);
	LCD_Write_DATA(y1>>8);
	LCD_Write_DATA(y1);
	LCD_Write_DATA(y2>>8);
	LCD_Write_DATA(y2); 
	LCD_Write_COM(0x2b);
	LCD_Write_DATA(x1>>8);
	LCD_Write_DATA(x1);
	LCD_Write_DATA(x2>>8);
	LCD_Write_DATA(x2);
	LCD_Write_COM(0x2c); 
	
}


/// \brief
/// needed for initialization
/// \details
/// sets the screensize
void display::clrXY(){
	Address_set(0,0,320,240);
}



/// \brief
/// initializes the display
/// \details
/// reset the display and initializes a the display
/// commands are copyed from utft libary

void display::init(){	
	
	
	rst.set(1);
	hwlib::wait_ms(5);
	rst.set(0);
	hwlib::wait_ms(15);
	rst.set(1);
	hwlib::wait_ms(15);
	
	cs.set(0);
	
	LCD_Write_COM(0x34); // tearing effect off
	
	// LCD_Write_COM(0x35);
	
    LCD_Write_COM(0xCB);  
    LCD_Write_DATA(0x39); 
    LCD_Write_DATA(0x2C); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x34); 
    LCD_Write_DATA(0x02); 

    LCD_Write_COM(0xCF);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0XC1); 
    LCD_Write_DATA(0X30); 

    LCD_Write_COM(0xE8);  
    LCD_Write_DATA(0x85); 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x78); 

    LCD_Write_COM(0xEA);  
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x00); 
 
    LCD_Write_COM(0xED);  
    LCD_Write_DATA(0x64); 
    LCD_Write_DATA(0x03); 
    LCD_Write_DATA(0X12); 
    LCD_Write_DATA(0X81); 

    LCD_Write_COM(0xF7);  
    LCD_Write_DATA(0x20); 
  
    LCD_Write_COM(0xC0);    //Power control 
    LCD_Write_DATA(0x23);   //VRH[5:0] 
 
    LCD_Write_COM(0xC1);    //Power control 
    LCD_Write_DATA(0x10);   //SAP[2:0];BT[3:0] 

    LCD_Write_COM(0xC5);    //VCM control 
    LCD_Write_DATA(0x3e);   //Contrast
    LCD_Write_DATA(0x28); 
 
    LCD_Write_COM(0xC7);    //VCM control2 
    LCD_Write_DATA(0x86);   //--
 
    LCD_Write_COM(0x36);    // Memory Access Control 
    LCD_Write_DATA(0x48);   

    LCD_Write_COM(0x3A);    
    LCD_Write_DATA(0x55); 

    LCD_Write_COM(0xB1);    
    LCD_Write_DATA(0x00);  
    LCD_Write_DATA(0x18); 
 
    LCD_Write_COM(0xB6);    // Display Function Control 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x82);
    LCD_Write_DATA(0x27);  

    LCD_Write_COM(0xF2);    // 3Gamma Function Disable 
    LCD_Write_DATA(0x00); 
 
    LCD_Write_COM(0x26);    //Gamma curve selected 
    LCD_Write_DATA(0x01); 

    LCD_Write_COM(0xE0);    //Set Gamma 
    LCD_Write_DATA(0x0F); 
    LCD_Write_DATA(0x31); 
    LCD_Write_DATA(0x2B); 
    LCD_Write_DATA(0x0C); 
    LCD_Write_DATA(0x0E); 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x4E); 
    LCD_Write_DATA(0xF1); 
    LCD_Write_DATA(0x37); 
    LCD_Write_DATA(0x07); 
    LCD_Write_DATA(0x10); 
    LCD_Write_DATA(0x03); 
    LCD_Write_DATA(0x0E); 
    LCD_Write_DATA(0x09); 
    LCD_Write_DATA(0x00); 

    LCD_Write_COM(0XE1);    //Set Gamma 
    LCD_Write_DATA(0x00); 
    LCD_Write_DATA(0x0E); 
    LCD_Write_DATA(0x14); 
    LCD_Write_DATA(0x03); 
    LCD_Write_DATA(0x11); 
    LCD_Write_DATA(0x07); 
    LCD_Write_DATA(0x31); 
    LCD_Write_DATA(0xC1);
    LCD_Write_DATA(0x48); 
    LCD_Write_DATA(0x08); 
    LCD_Write_DATA(0x0F); 
    LCD_Write_DATA(0x0C); 
    LCD_Write_DATA(0x31); 
    LCD_Write_DATA(0x36); 
    LCD_Write_DATA(0x0F); 

    LCD_Write_COM(0x11);    //Exit Sleep 
    hwlib::wait_ms(120);
				
    LCD_Write_COM(0x29);    //Display on 
    LCD_Write_COM(0x2c); 

	cs.set(1); 
	
}

/// \brief
/// drawspixels for functions.
/// \details
/// This drawpixel is for drawing multiple pixels after each other this one leaves the chipselect high to speed up the process
/// if you dont pass a color it prins black pixels you can pass 8bit or 16 bit color
/// 16 bit color can been sent in 8 bit format or 16 bit format
/// \{
void display::drawPixel(int x, int y){
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	LCD_Write_DATA(0x00,0x00);
	clrXY();
}

void display::drawPixel(int x, int y, uint8_t color1, uint8_t color2){
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	LCD_Write_DATA(color1,color2);
	clrXY();
}

void display::drawPixel(int x, int y, uint16_t color){
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	LCD_Write_DATA(color);
	clrXY();
}
/// \}


/// \brief
/// draws a single pixel
/// \details
/// draws a single pixel 
/// if you dont pass a color it prins black pixels you can pass 8bit or 16 bit color
/// 16 bit color can been sent in 8 bit format or 16 bit format
/// \{
void display::drawsingelPixel(int x, int y){
	cs.set(0);
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	LCD_Write_DATA(0x00,0x00); 
	cs.set(1);
	clrXY();
}
void display::drawsingelPixel(int x, int y, uint16_t color){
	cs.set(0);
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	if(!color && 0x100){
	LCD_Write_DATA(0x00,color);
	}
	else {
		LCD_Write_DATA(color>>8,color);
	}
	cs.set(1);
	clrXY();
}
void display::drawsingelPixel(int x, int y, uint8_t color1 , uint8_t color2){
	cs.set(0);
	x = (x * -1) + 320;
	Address_set(x,y,x,y);
	LCD_Write_DATA(color1,color2); //aanpasbaar maken
	cs.set(1);
	clrXY();
}
/// \}



/// \brief
/// make's the screen white
void display::clear(){
	
	fillrect(0,0,320,240,0xFF,0xFF);
}



/// \brief
/// draws a filled rectangle
/// \details
/// draws filled rectangle in the color of choice
/// \{
void display::fillrect(int x0, int y0, int x1, int y1,uint16_t color)
{
 
	cs.set(0);
    int h = y1 - y0 + 1;
    int w = x1 - x0 + 1;
    int pixel = h * w;
    Address_set(x0,y0,x1,y1);  // send pixel 
	if (color && 0x100) {
		for (int p=0; p<pixel; p++) {
			LCD_Write_DATA(0x00,color);
		}
    }
	else {
		for (int p=0; p<pixel; p++) {
			LCD_Write_DATA(color,color);
		}
	}
    cs.set(1);
    clrXY();
    return;
}



void display::fillrect(int x0, int y0, int x1, int y1,uint8_t color1,uint8_t color2)
{
 
	cs.set(0);
    int h = y1 - y0 + 1;
    int w = x1 - x0 + 1;
    int pixel = h * w;
    Address_set(x0,y0,x1,y1);
    for (int p=0; p<pixel; p++) {
        LCD_Write_DATA(color1,color2);
    }
    
    cs.set(1);
    clrXY();
}
/// \}

/// \brief
/// clears the screen with custom coördinates
void display::customclear(int x0, int y0, int x1, int y1){
	
	fillrect(x0,y0,x1,y1,0xFF,0xFF);
}


/// \brief
/// reads touch input
/// \details
/// reads touch input and preforms a few checks if its a valid coördinate
punt display::Touchread(){
	int x= 0 ;
	int y = 0; 
	int xtemp = 0;
	int ytemp = 0; 
	int axis = 0;
	int loop = 1;
	punt p(x,y);
	while (loop){
	if (!T_irq.get()){
	xtemp = ReadAxis(!axis);
	ytemp = ReadAxis(axis);
	hwlib::wait_ms(1);
	x = ReadAxis(!axis);
	y = ReadAxis(axis);
	if ((xtemp - x) > 5 && (xtemp - x) >5 ){
		if (x && y){
			x = ReadAxis(!axis);
			y = ReadAxis(axis);
			hwlib::wait_ms(1);  
			if ((xtemp - x) >5 && (xtemp - x) >5){
				x = map(x,350,3700,0,320);
				y = map(y,350,3700,0,240);
				x = x *-1 + 320;
				punt p(x,y);
				return p;
				loop = 0;
			}
				
		}
	}
	}
	}
	return p;
}