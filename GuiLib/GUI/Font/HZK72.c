/*
*******************************************************
		 Structure : 
		 
		 History:

*******************************************************
*/

#include "GUI.H"
#ifndef GUI_FLASH
#define GUI_FLASH
#endif


/*
GUI_FLASH const GUI_CHARINFO GUI_FontHZ72_CharInfo[] = {
{ 10, 10, 1, (void GUI_FLASH *)0},  // �ַ���FLASH�е�ƫ����
{ 16, 16, 2, (void GUI_FLASH *)3840}, //������FLASH�е�ƫ����
};
*/

GUI_FLASH const GUI_CHARINFO GUI_FontHZ72_CharInfo[] = {
{ 72, 36, 5,  (void GUI_FLASH *)GUI_Font_SPI_ASC72_BaseAddr},  //FLASH��û�д��ַ�������Ϊ0
{ 72, 72, 9, (void GUI_FLASH *)GUI_Font_SPI_HZK72_BaseAddr}, //GUI_FontHZ72_Flash_BaseAddr��GUI.h�ж���
};

//���ֺ��ַ�������//////////////////////////////////

/*��--��*/

GUI_FLASH const GUI_FONT_PROP GUI_FontHZ72_Propf7a1= {
      0xf7a1,
      0xf7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)0 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf6a1= {
      0xf6a1,
      0xf6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf5a1= {
      0xf5a1,
      0xf5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf4a1= {
      0xf4a1,
      0xf4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf5a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf3a1= {
      0xf3a1,
      0xf3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf4a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf2a1= {
      0xf2a1,
      0xf2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf3a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf1a1= {
      0xf1a1,
      0xf1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf2a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propf0a1= {
      0xf0a1,
      0xf0fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf1a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propefa1= {
      0xefa1,
      0xeffe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propf0a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propeea1= {
      0xeea1,
      0xeefe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propefa1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propeda1= {
      0xeda1,
      0xedfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propeea1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propeca1= {
      0xeca1,
      0xecfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propeda1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propeba1= {
      0xeba1,
      0xebfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propeca1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propeaa1= {
      0xeaa1,
      0xeafe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propeba1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope9a1= {
      0xe9a1,
      0xe9fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propeaa1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope8a1= {
      0xe8a1,
      0xe8fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope9a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope7a1= {
      0xe7a1,
      0xe7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope8a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope6a1= {
      0xe6a1,
      0xe6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope7a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope5a1= {
      0xe5a1,
      0xe5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope6a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope4a1= {
      0xe4a1,
      0xe4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope5a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope3a1= {
      0xe3a1,
      0xe3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope4a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope2a1= {
      0xe2a1,
      0xe2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope3a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope1a1= {
      0xe1a1,
      0xe1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope2a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prope0a1= {
      0xe0a1,
      0xe0fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope1a1 
};
/*ߡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdfa1= {
      0xdfa1,
      0xdffe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Prope0a1 
};
/*ޡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdea1= {
      0xdea1,
      0xdefe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdfa1 
};
/*ݡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdda1= {
      0xdda1,
      0xddfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdea1 
};
/*ܡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdca1= {
      0xdca1,
      0xdcfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdda1 
};
/*ۡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdba1= {
      0xdba1,
      0xdbfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdca1 
};
/*ڡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propdaa1= {
      0xdaa1,
      0xdafe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdba1 
};
/*١--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd9a1= {
      0xd9a1,
      0xd9fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propdaa1 
};
/*ء--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd8a1= {
      0xd8a1,
      0xd8fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd9a1 
};
/*ס--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd7a1= {
      0xd7a1,
      0xd7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd8a1 
};
/*֡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd6a1= {
      0xd6a1,
      0xd6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd7a1 
};
/*ա--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd5a1= {
      0xd5a1,
      0xd5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd6a1 
};
/*ԡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd4a1= {
      0xd4a1,
      0xd4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd5a1 
};
/*ӡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd3a1= {
      0xd3a1,
      0xd3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd4a1 
};
/*ҡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd2a1= {
      0xd2a1,
      0xd2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd3a1 
};
/*ѡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd1a1= {
      0xd1a1,
      0xd1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd2a1 
};
/*С--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propd0a1= {
      0xd0a1,
      0xd0fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd1a1 
};
/*ϡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcfa1= {
      0xcfa1,
      0xcffe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propd0a1 
};
/*Ρ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcea1= {
      0xcea1,
      0xcefe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcfa1 
};
/*͡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcda1= {
      0xcda1,
      0xcdfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcea1 
};
/*̡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcca1= {
      0xcca1,
      0xccfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcda1 
};
/*ˡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcba1= {
      0xcba1,
      0xcbfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcca1 
};
/*ʡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propcaa1= {
      0xcaa1,
      0xcafe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcba1 
};
/*ɡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc9a1= {
      0xc9a1,
      0xc9fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propcaa1 
};
/*ȡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc8a1= {
      0xc8a1,
      0xc8fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc9a1 
};
/*ǡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc7a1= {
      0xc7a1,
      0xc7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc8a1 
};
/*ơ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc6a1= {
      0xc6a1,
      0xc6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc7a1 
};
/*š--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc5a1= {
      0xc5a1,
      0xc5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc6a1 
};
/*ġ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc4a1= {
      0xc4a1,
      0xc4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc5a1 
};
/*á--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc3a1= {
      0xc3a1,
      0xc3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc4a1 
};
/*¡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc2a1= {
      0xc2a1,
      0xc2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc1a1= {
      0xc1a1,
      0xc1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc2a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propc0a1= {
      0xc0a1,
      0xc0fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc1a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbfa1= {
      0xbfa1,
      0xbffe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propc0a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbea1= {
      0xbea1,
      0xbefe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbfa1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbda1= {
      0xbda1,
      0xbdfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbea1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbca1= {
      0xbca1,
      0xbcfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbda1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbba1= {
      0xbba1,
      0xbbfe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbca1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propbaa1= {
      0xbaa1,
      0xbafe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbba1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb9a1= {
      0xb9a1,
      0xb9fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propbaa1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb8a1= {
      0xb8a1,
      0xb8fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb9a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb7a1= {
      0xb7a1,
      0xb7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb8a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb6a1= {
      0xb6a1,
      0xb6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb5a1= {
      0xb5a1,
      0xb5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb4a1= {
      0xb4a1,
      0xb4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb5a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb3a1= {
      0xb3a1,
      0xb3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb4a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb2a1= {
      0xb2a1,
      0xb2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb1a1= {
      0xb1a1,
      0xb1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb2a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propb0a1= {
      0xb0a1,
      0xb0fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb1a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa9a1= {
      0xa9a1,
      0xa9fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propb0a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa8a1= {
      0xa8a1,
      0xa8fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa9a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa7a1= {
      0xa7a1,
      0xa7fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa8a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa6a1= {
      0xa6a1,
      0xa6fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa5a1= {
      0xa5a1,
      0xa5fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa4a1= {
      0xa4a1,
      0xa4fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa5a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa3a1= {
      0xa3a1,
      0xa3fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa4a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa2a1= {
      0xa2a1,
      0xa2fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Propa1a1= {
      0xa1a1,
      0xa1fe,
      &GUI_FontHZ72_CharInfo[1],
      (void *)&GUI_FontHZ72_Propa2a1 
};
//ASC�ַ�
/* --*/ 
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ72_Prop0020= {
      0x0020,
      0x007f,
      &GUI_FontHZ72_CharInfo[0],
      (void *)&GUI_FontHZ72_Propa1a1 
};
GUI_FLASH const GUI_FONT GUI_FontHZ72 = {
      GUI_FONTTYPE_PROP_SJIS, 
      72, 
      72, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZ72_Prop0020
};


