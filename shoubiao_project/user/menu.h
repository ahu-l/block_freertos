#ifndef _MENU_H
#define _MENU_H
#include  "stm32f10x.h"
#include "FreeRtos.h"
#include "task.h" 
typedef struct Function_Obj{  //小菜单结构体 
	struct Function_Obj *Next;  //指向上一个菜单 
	struct Function_Obj *Last;  //指向下一个菜单
	struct Function_Obj *Final; //指向最后一个菜单 
		struct Function_Obj *up;  //指向下一个菜单
	struct Function_Obj *down; //指向最后一个菜单 
	const char *Fname;  //菜单名 
	const char * img;
	TaskHandle_t  task_handler;
}Function;
void Funtction_up_list(Function *up_l,Function *low_l);
extern Function * menu_list[3];
Function* Function_init(const char *Fname,const char * img) ;
void Function_List(Function *Head,Function *FinalList) ;
void Menu_Init(void);
#endif
