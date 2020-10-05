#include "GUI.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Define the available number of bytes available for the GUI
#define USE_EXRAM				1//使用外部RAM
//设置EmWin内存大小
#define GUI_NUMBYTES  (500*1024)
#define GUI_BLOCKSIZE		0X80	//块大小
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//GUI_X_Config
//初始化的时候调用，用来设置emwin所使用的内存
void GUI_X_Config(void) {
//使用外部RAM
		static U32 *aMemory;
		aMemory = (U32 *)((U32)(0x68000000));
		GUI_ALLOC_AssignMemory(aMemory,GUI_NUMBYTES);//为存储管理
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);//设置存储块的平居尺寸
		GUI_SetDefaultFont(GUI_FONT_6X8);//设置默认字体
}

/*************************** End of file ****************************/
