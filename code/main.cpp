#include "display.hpp"
#include "hwlib.hpp"
#include "punt.hpp"
#include "colorholder.hpp"




colorholder paint(punt p, display &d , int drawcolor1, int drawcolor2){
	int x = p.getx();
	int y = p.gety();
	if (x >= 0 && x <= 40){
		if (y >= 200 && y <= 240 ){
			d.customclear(40,0,320,240);
		}
		else if(y >= 40  && y <= 80 ){
			drawcolor1 = 0x3;
			drawcolor2 = 0xC0;
		}	
		else if(y >= 80  && y <= 120 ){
			drawcolor1 = 0xE0;
			drawcolor2 = 0x00;
		}	
		else if(y >= 120  && y <= 160 ){
			drawcolor1 = 0xFF;
			drawcolor2 = 0xc0;
		}		
		else if(y >= 160  && y <= 200 ){
			drawcolor1 = 0x01;
			drawcolor2 = 0xFF;
		}		
		else if(y >= 0 && y <= 40 ){
			drawcolor1 = 0x00;
			drawcolor2 = 0x00;
		}
	}
	else if(x > 40) {
		d.fillrect(x,y,x+5,y+5,drawcolor1,drawcolor2);
	} 
	colorholder c(drawcolor1,drawcolor2);
	return c;
}

int main(void){
	WDT->WDT_MR = WDT_MR_WDDIS;
	auto db0 = hwlib::target::pin_out(2,5);
	auto db1 = hwlib::target::pin_out(2,4);
	auto  db2 = hwlib::target::pin_out(2,3);
	auto  db3 = hwlib::target::pin_out(2,2);
	
	auto  db4 = hwlib::target::pin_out(2,1);
	auto  db5 = hwlib::target::pin_out(3,10);
	auto  db6 = hwlib::target::pin_out(0,7);
	auto  db7 = hwlib::target::pin_out(3,9);
	
	auto  db8 = hwlib::target::pin_out(3,3);
	auto  db9 = hwlib::target::pin_out(3,1);
	auto  db10 = hwlib::target::pin_out(0,15);
	auto  db11 = hwlib::target::pin_out(1,26);
	
	auto  db12 = hwlib::target::pin_out(1,25);
	auto  db13 = hwlib::target::pin_out(2,28);
	auto  db14 = hwlib::target::pin_out(2,26);
	auto  db15 = hwlib::target::pin_out(2,25);
	auto  rst = hwlib::target::pin_out(2,24);
	auto  cs = hwlib::target::pin_out(2,23);
	auto  rs = hwlib::target::pin_out(2,22);
	auto  wr = hwlib::target::pin_out(2,21);
	auto  T_do = hwlib::target::pin_in(2,6);
	auto  T_clk = hwlib::target::pin_out(2,29);
	auto  T_cs = hwlib::target::pin_out(3,7);
	auto  T_din = hwlib::target::pin_out(3,8);
	auto  T_irq = hwlib::target::pin_in(1,27);
	display d(db0,db1,db2,db3,db4,db5,db6,db7,db8,db9,db10,db11,db12,db13,db14,db15,rst,cs,rs,wr,T_do,T_clk,T_cs,T_din,T_irq);
	d.init();
	d.clrXY();
	d.clear();
	d.fillrect(0,0,40,40,0x00);
	d.fillrect(0,40,40,80,0x3,0xC0);
	d.fillrect(0,80,40,120,0xE0,0x00);
	d.fillrect(0,120,40,160,0xFF,0xc0);
	d.fillrect(0,160,40,200,0x01,0xFF);
	d.fillrect(5,222,35,227,0x00,0x00);
	d.fillrect(5,213,35,218,0x00,0x00);
	int drawcolor1;
	int  drawcolor2; 
	colorholder color(0,0);
	while(1){
	punt p = d.Touchread();
	drawcolor1 = color.getcolor1();
	drawcolor2 = color.getcolor2();
	color = paint(p,d,drawcolor1,drawcolor2);
	} 
}