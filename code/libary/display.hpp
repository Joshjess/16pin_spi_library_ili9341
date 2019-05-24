//          Copyright Joshua Offermans 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#include "hwlib.hpp"
#include "punt.hpp"
// UTFT, TFT Touch and UTOUCH libary were used as example
#ifndef display_HPP
#define display_HPP


class display {
private:
	hwlib::target::pin_out & db0;
	hwlib::target::pin_out & db1;
	hwlib::target::pin_out & db2;
	hwlib::target::pin_out & db3;
	hwlib::target::pin_out & db4;
	hwlib::target::pin_out & db5;
	hwlib::target::pin_out & db6;
	hwlib::target::pin_out & db7;
	hwlib::target::pin_out & db8;
	hwlib::target::pin_out & db9;
	hwlib::target::pin_out & db10;
	hwlib::target::pin_out & db11;
	hwlib::target::pin_out & db12;
	hwlib::target::pin_out & db13;
	hwlib::target::pin_out & db14;
	hwlib::target::pin_out & db15;
	hwlib::target::pin_out & rst;
	hwlib::target::pin_out & cs;
	hwlib::target::pin_out & rs;
	hwlib::target::pin_out & wr;
	hwlib::target::pin_in & T_do;
	hwlib::target::pin_out & T_clk;
	hwlib::target::pin_out & T_cs;
	hwlib::target::pin_out & T_din;
	hwlib::target::pin_in & T_irq;
public:
	display(
		hwlib::target::pin_out & db0,
		hwlib::target::pin_out & db1,
		hwlib::target::pin_out & db2,
		hwlib::target::pin_out & db3,
		hwlib::target::pin_out & db4,
		hwlib::target::pin_out & db5,
		hwlib::target::pin_out & db6,
		hwlib::target::pin_out & db7,
		hwlib::target::pin_out & db8,
		hwlib::target::pin_out & db9,
		hwlib::target::pin_out & db10,
		hwlib::target::pin_out & db11,
		hwlib::target::pin_out & db12,
		hwlib::target::pin_out & db13,
		hwlib::target::pin_out & db14,
		hwlib::target::pin_out & db15,
		hwlib::target::pin_out & rst,
		hwlib::target::pin_out & cs,
		hwlib::target::pin_out & rs,
		hwlib::target::pin_out & wr,
		hwlib::target::pin_in & T_do,
		hwlib::target::pin_out & T_clk,
		hwlib::target::pin_out & T_cs,
		hwlib::target::pin_out & T_din,
		hwlib::target::pin_in & T_irq
	):
	db0(db0),
	db1(db1),
	db2(db2),
	db3(db3),
	db4(db4),
	db5(db5),
	db6(db6),
	db7(db7),
	db8(db8),
	db9(db9),
	db10(db10),
	db11(db11),
	db12(db12),
	db13(db13),
	db14(db14),
	db15(db15),
	rst(rst),
	cs(cs),
	rs(rs),
	wr(wr),
	T_do(T_do),
	T_clk(T_clk),
	T_cs(T_cs),
	T_din(T_din),
	T_irq(T_irq)
	{}
	void LCD_write_Bus(uint8_t data1, uint8_t  data2);
	void LCD_write_Bus(uint16_t data1);
	//void Pant(unsigned int  vh,unsigned int  vl);
	void drawPixel(int x, int y, uint8_t color1, uint8_t color2);
	void drawPixel(int x, int y, uint16_t color);
	void drawPixel(int x, int y);	
	void drawsingelPixel(int x, int y, uint8_t color1, uint8_t color2);
	void drawsingelPixel(int x, int y, uint16_t color);
	void drawsingelPixel(int x, int y);
	void clear();
	int drawcolor1 = 0x00;
	int drawcolor2 = 0x00;
	void customclear(int x0,int y0, int x1, int y1);
	//void _fast_fill_16(int ch, int cl, long pix);
	void LCD_Write_COM (uint8_t  com);
	void fillrect(int x0, int y0, int x1, int y1,uint16_t color);
	void fillrect(int x0, int y0, int x1, int y1,uint8_t color1, uint8_t color2);
	void LCD_Write_DATA (uint8_t  data1, uint8_t  data2);
	void LCD_Write_DATA (uint8_t  data1);
	void drawline(uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1);
	void clrXY();
	void clrScr();
	void init();
	void drawcircle(int x0, int y0, int radius);
	int map(int x, int in_min, int in_max, int out_min, int out_max);
	void pincontrol(auto pin, unsigned short status);
	punt Touchread();
	int16_t ReadAxis(int Axis);
	uint16_t ReadData();
	void OutputData(uint8_t Data);
	void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
};

#endif

