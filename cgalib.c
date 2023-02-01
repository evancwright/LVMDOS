#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <i86.h>

#ifndef FONT_DATA_H
#define FONT_DATA_H
#include "fontData.h"
#endif

#define BLOCK_SIZE 8000
#define LINE_SIZE 80

#include "cgalib.h"

int row=0;
int col=0;
FILE *fp=0;
char buffer[BLOCK_SIZE];

//CGA info
//320x200 resolution (4 color)
//80 bytes per line
//B800:0000 
//B800:2000
 
void wait_key();
 
void wait_key()
{
	_asm
	{
		xor ah,ah
		int 16h
	};
}


void start_cga()
{
	_asm
	{
		;set mode 5
		mov ax,4
		int 10h
		
		;set background color
		mov ah,0bh ; operation code
		mov bx,0  ; 0 = black
		int 10h
		
		;set palete
		mov ah,0Bh
		mov bx,0000h ; palette zero
		int 10h
	
	};
}

void stop_cga()
{
	_asm
	{
		;set mode 3
		mov ax,3
		int 10h
	};
}
/*
void print_str(char *str)
{
	int i=0;
	int len = strlen(str);
	for (;i < len; i++)
	{
		char_out(str[i]);
	}
}

void char_out(char ch)
{
	draw_char(col, row, ch);
	col++;
	if (col == 40)
	{
		col =0;
		row++;
		if (row == 25)
		{
			Scroll();
			row = 24;
		}
	}
}*/
/*
//draws a double wide character at x,y
void draw_char(int x, int y, char ch)
{
    int index = ch  << 4; //times 16 bytes per char
    unsigned int offset = x*2 + y*320; //640 = 8 lines of 40 bytes
    char far *evenLinePtr = MK_FP( 0xB800, offset);
	char far *oddLinePtr = MK_FP( 0xB800, 0x2000 + offset);
    int i=0;
    for (; i < 4; i++) //each horizontal line
    {
        //copy two bytes of even lines and odd lines		
        *evenLinePtr = fontData[index];
		evenLinePtr++; index++;
		*evenLinePtr = fontData[index];
		evenLinePtr++; index++;
		*oddLinePtr = fontData[index];
		oddLinePtr++; index++;
		*oddLinePtr = fontData[index];
		oddLinePtr++; index++;
		evenLinePtr += 78; //next line is 80 bytes ahead
		oddLinePtr += 78; //next line is 80 bytes ahead
    }
}

//scrolls the screen up 8 lines
void Scroll()
{
	//move even lines back 320 byte (80*4)
	//move the odd lines back 160 bytes
	//three lines of data 80x3 = 240 bytes
	int i=0;
	unsigned char far *evenPtr = MK_FP(0xB800,0);
	unsigned char far *oddPtr = MK_FP(0xB800,0x2000);
	unsigned char far *evenSrcPtr = MK_FP(0xB800,320);
	unsigned char far *oddSrcPtr = MK_FP(0xB800,0x2000 + 320);
	 
	for (i=0;i < 7680; i++)
	{
		*evenPtr = *evenSrcPtr;
		*oddPtr = *oddSrcPtr;
		oddPtr++;
		evenPtr++;
		oddSrcPtr++;
		evenSrcPtr++;
	}
	//zero out the bottom four top lines  
	//oddScrPtr and destPtrs should now point to end of data
	
	for (i=0; i < 320; i++)
	{
		*oddPtr=0;
		*evenPtr=0;
		oddPtr++;
		evenPtr++;
	}
	
}

void ScrollExceptTop()
{

}
void NewLine()
{
	col = 0;
	row++;
	if (row == 25)
	{
		Scroll();
		row = 24;
	} 
}
*/

void show_image(unsigned char id)
{
	char fileName[20];
	sprintf(fileName,"%d.img",id);
	show_image_path(fileName);
}

void show_image_path(char *fileName)
{
	FILE *fp=0;
	char buf[LINE_SIZE] ; //size of one line
	int i=0;
	int j=0;
	unsigned char height=0;
	char far *evenPtr = MK_FP(0xB800,0x0000);
	char far *oddPtr = MK_FP(0xB800,0x2000);
	
	fp = fopen(fileName, "rb");
	if (fp)
	{
		cga_cls();
		//read the height
		fread(&height, 1, 1, fp);
		//printf("height=%d",height);
		for (; i < height/2 ;i++)
		{
			fread(buf,1,LINE_SIZE,fp);
			for (j=0; j < LINE_SIZE; j++)
			{
				*evenPtr = buf[j];
				evenPtr++;
			} 
		}
		i=0;
		for (; i < height/2 ;i++)
		{
			fread(buf,1,LINE_SIZE,fp);
			for (j=0; j < LINE_SIZE; j++)
			{
				*oddPtr = buf[j];
				oddPtr++;
			}
		}

		fclose(fp);
		printf("\x1b[%d;0H", (height/8)+1); //position cursor
	}
	else
	{
		printf("Unable to open file:%s",fileName);
	}
}

void cga_cls()
{
	int i=0,j=0;
	char far *evenPtr = MK_FP(0xB800,0x0000);
	char far *oddPtr = MK_FP(0xB800,0x2000);
 
	//read the height
 	for (; i < 100 ;i++)
	{
		for (j=0; j < LINE_SIZE; j++)
		{
			*evenPtr = 0;
			evenPtr++;
			*oddPtr = 0;
			oddPtr++;
		} 
	}
}

/*
//data is a 16K block
//assumes data is in even line, odd line format
void draw_background(char *data)
{ 
		_asm
		{
			push ax
			push bx
			push cx
			push dx
			push es
			mov ax,0B800h ; set segment address
			mov es,ax
			mov cx,8000 ; repeat count
			 
			mov si,0h
			mov di,0h  ; 0 bytes past start of CGA seg
lp:			
			mov al,data[si] ;
			stosb
			inc si
			loop lp
			 
			mov ax,0B800h ; set segment address
			mov es,ax
			;odd lines
			mov cx,8000 ; repeat count
			mov di,2000h  ; second CGA area
;			mov si,0
lp2:			
			mov al,data[si] ;
			stosb
			inc si
			loop lp2		
			pop es
			pop dx
			pop cx
			pop bx
			pop ax
		}; 
}
*/
