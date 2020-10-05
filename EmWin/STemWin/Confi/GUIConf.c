#include "GUI.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Define the available number of bytes available for the GUI
#define USE_EXRAM				1//ʹ���ⲿRAM
//����EmWin�ڴ��С
#define GUI_NUMBYTES  (500*1024)
#define GUI_BLOCKSIZE		0X80	//���С
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
//GUI_X_Config
//��ʼ����ʱ����ã���������emwin��ʹ�õ��ڴ�
void GUI_X_Config(void) {
//ʹ���ⲿRAM
		static U32 *aMemory;
		aMemory = (U32 *)((U32)(0x68000000));
		GUI_ALLOC_AssignMemory(aMemory,GUI_NUMBYTES);//Ϊ�洢����
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);//���ô洢���ƽ�ӳߴ�
		GUI_SetDefaultFont(GUI_FONT_6X8);//����Ĭ������
}

/*************************** End of file ****************************/
