/*
 * getcolor.c
 *
 *  Created on: 14.01.2020.
 *      Author: Miodrag
 */
#include "getcolor.h"

uint32_t Change_me(float *temperature)
{
	uint8_t scaledHue = Scale_Hue(temperature); //returns Hue
	uint32_t rgb888 = HSV(scaledHue, 100, 100);	 //returns RGB_888
	uint32_t rgb565 = convertRGB(3, 2, rgb888);
	return rgb565;
}

uint8_t Scale_Hue(float *temperature)
{
	if(*temperature < minTempVal) *temperature = minTempVal;
	else if(*temperature > maxTempVal) *temperature = maxTempVal;

	float scaled_val = *temperature-minTempVal;
	const float scaledMax = 240.0f;
	scaled_val *= scaledMax;
	scaled_val /= (maxTempVal-minTempVal);
	//scaled_val =( (*temperature-minTempVal)  /(maxTempVal-minTempVal) ) ; //scaling 2 points method

	scaled_val -= scaledMax;
	scaled_val *= -1.0f;

	return (uint8_t)scaled_val;
}

uint32_t HSV(uint8_t h, uint8_t s, uint8_t v)
{
    uint8_t r,g,b;

    double hh=h, ss=s, vv=v;
    double rr, gg, bb;
    double  p,q,t;
    double fract;

    ss=ss/100;
    vv=vv/100;
    if(hh>240)hh=0;
    else hh=hh/60;
    fract = hh - floor(hh);


    p = vv*(1. - ss);
    q = vv*(1. - ss*fract);
    t = vv*(1. - ss*(1. - fract));

    if(hh<1)
    {
        rr = vv;
        gg = t;
        bb = p;
    }
    else if(hh<2)
    {
        rr = q;
        gg = vv;
        bb = p;
    }
    else if(hh<3)
    {
        rr = p;
        gg = vv;
        bb = t;
    }
    else if(hh<4)
    {
        rr = p;
        gg = q;
        bb = vv;
    }
    else if(hh<5)
    {
        rr = t;
        gg = p;
        bb = vv;
    }
    else if(hh<6)
    {
        rr = vv;
        gg = p;
        bb = q;
    }
    else
    {
        rr = 0;
        gg = 0;
        bb = 0;
    };

    r=(uint8_t)(rr*255);
    g=(uint8_t)(gg*255);
    b=(uint8_t)(bb*255);

    return ((r<<16)|(g<<8)|b);

}


uint32_t convertRGB(const uint8_t src, const uint8_t dst, uint32_t color)
{
    uint8_t r,g,b;

    if(src==0)//RGB111
    {
        r=(color>>2)&0x01;
        g=(color>>1)&0x01;
        b=(color&0x01);
        if(dst==1)      return ((r<<7)|(g<<4)|(b<<1));
        else if(dst==2) return ((r<<15)|(g<<10)|(b<<4));
        else if(dst==3) return ((r<<23)|(g<<15)|(b<<7));
        else            return color;
    }
    else if(src==1)//RGB332
    {
        r=(color>>5)&0x07;
        g=(color>>2)&0x07;
        b=(color&0x03);
        if(dst==0)      return (((r>>2)<<2)|((g>>2)<<1)|(b>>1));
        else if(dst==2) return ((r<<13)|(g<<8)|(b<<3));
        else if(dst==3) return ((r<<21)|(g<<13)|(b<<6));
        else            return color;

    }
    else if(src==2)//RGB565
    {
        r=(color>>11)&0x1F;
        g=(color>>5)&0x3F;
        b=(color&0x1F);
        if(dst==0)      return (((r>>4)<<2)|((g>>5)<<1)|(b>>4));
        else if(dst==1) return (((r>>2)<<5)|((g>>3)<<2)|(b>>3));
        else if(dst==3) return ((r<<19)|(g<<10)|(b<<3));
        else            return color;
    }
    else if(src==3)//RGB888
    {
        r=(color>>16)&0xFF;
        g=(color>>8)&0xFF;
        b=(color&0xFF);
        if(dst==0)      return (((r>>7)<<2)|((g>>7)<<1)|(b>>7));
        else if(dst==1) return (((r>>5)<<5)|((g>>5)<<2)|(b>>6));
        else if(dst==2) return (((r>>3)<<11)|((g>>2)<<5)|(b>>3));
        else            return color;
    }
    else return color;
}

