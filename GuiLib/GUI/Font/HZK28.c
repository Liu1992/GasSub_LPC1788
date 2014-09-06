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
GUI_FLASH const GUI_CHARINFO GUI_FontHZ28_CharInfo[] = {
{ 10, 10, 1, (void GUI_FLASH *)0},  // �ַ���FLASH�е�ƫ����
{ 16, 16, 2, (void GUI_FLASH *)3840}, //������FLASH�е�ƫ����
};
*/

GUI_FLASH const GUI_CHARINFO GUI_FontHZ28_CharInfo[] = {
{ 28, 14, 2,  (void GUI_FLASH *)GUI_Font_SPI_ASC28_BaseAddr},  //FLASH��û�д��ַ�������Ϊ0
{ 28, 28, 4, (void GUI_FLASH *)GUI_Font_SPI_HZK28_BaseAddr}, //GUI_FontHZ28_Flash_BaseAddr��GUI.h�ж���
};

//���ֺ��ַ�������//////////////////////////////////

/*��--��*/

GUI_FLASH const GUI_FONT_PROP GUI_FontHZ28_Propf7a1= {
      0xf7a1,
      0xf7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)0 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf6a1= {
      0xf6a1,
      0xf6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf5a1= {
      0xf5a1,
      0xf5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf4a1= {
      0xf4a1,
      0xf4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf5a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf3a1= {
      0xf3a1,
      0xf3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf4a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf2a1= {
      0xf2a1,
      0xf2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf3a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf1a1= {
      0xf1a1,
      0xf1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf2a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propf0a1= {
      0xf0a1,
      0xf0fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf1a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propefa1= {
      0xefa1,
      0xeffe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propf0a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propeea1= {
      0xeea1,
      0xeefe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propefa1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propeda1= {
      0xeda1,
      0xedfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propeea1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propeca1= {
      0xeca1,
      0xecfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propeda1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propeba1= {
      0xeba1,
      0xebfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propeca1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propeaa1= {
      0xeaa1,
      0xeafe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propeba1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope9a1= {
      0xe9a1,
      0xe9fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propeaa1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope8a1= {
      0xe8a1,
      0xe8fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope9a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope7a1= {
      0xe7a1,
      0xe7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope8a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope6a1= {
      0xe6a1,
      0xe6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope7a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope5a1= {
      0xe5a1,
      0xe5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope6a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope4a1= {
      0xe4a1,
      0xe4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope5a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope3a1= {
      0xe3a1,
      0xe3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope4a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope2a1= {
      0xe2a1,
      0xe2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope3a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope1a1= {
      0xe1a1,
      0xe1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope2a1 
};
/*�--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prope0a1= {
      0xe0a1,
      0xe0fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope1a1 
};
/*ߡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdfa1= {
      0xdfa1,
      0xdffe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Prope0a1 
};
/*ޡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdea1= {
      0xdea1,
      0xdefe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdfa1 
};
/*ݡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdda1= {
      0xdda1,
      0xddfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdea1 
};
/*ܡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdca1= {
      0xdca1,
      0xdcfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdda1 
};
/*ۡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdba1= {
      0xdba1,
      0xdbfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdca1 
};
/*ڡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propdaa1= {
      0xdaa1,
      0xdafe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdba1 
};
/*١--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd9a1= {
      0xd9a1,
      0xd9fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propdaa1 
};
/*ء--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd8a1= {
      0xd8a1,
      0xd8fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd9a1 
};
/*ס--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd7a1= {
      0xd7a1,
      0xd7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd8a1 
};
/*֡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd6a1= {
      0xd6a1,
      0xd6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd7a1 
};
/*ա--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd5a1= {
      0xd5a1,
      0xd5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd6a1 
};
/*ԡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd4a1= {
      0xd4a1,
      0xd4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd5a1 
};
/*ӡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd3a1= {
      0xd3a1,
      0xd3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd4a1 
};
/*ҡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd2a1= {
      0xd2a1,
      0xd2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd3a1 
};
/*ѡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd1a1= {
      0xd1a1,
      0xd1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd2a1 
};
/*С--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propd0a1= {
      0xd0a1,
      0xd0fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd1a1 
};
/*ϡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcfa1= {
      0xcfa1,
      0xcffe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propd0a1 
};
/*Ρ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcea1= {
      0xcea1,
      0xcefe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcfa1 
};
/*͡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcda1= {
      0xcda1,
      0xcdfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcea1 
};
/*̡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcca1= {
      0xcca1,
      0xccfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcda1 
};
/*ˡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcba1= {
      0xcba1,
      0xcbfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcca1 
};
/*ʡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propcaa1= {
      0xcaa1,
      0xcafe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcba1 
};
/*ɡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc9a1= {
      0xc9a1,
      0xc9fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propcaa1 
};
/*ȡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc8a1= {
      0xc8a1,
      0xc8fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc9a1 
};
/*ǡ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc7a1= {
      0xc7a1,
      0xc7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc8a1 
};
/*ơ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc6a1= {
      0xc6a1,
      0xc6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc7a1 
};
/*š--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc5a1= {
      0xc5a1,
      0xc5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc6a1 
};
/*ġ--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc4a1= {
      0xc4a1,
      0xc4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc5a1 
};
/*á--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc3a1= {
      0xc3a1,
      0xc3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc4a1 
};
/*¡--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc2a1= {
      0xc2a1,
      0xc2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc1a1= {
      0xc1a1,
      0xc1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc2a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propc0a1= {
      0xc0a1,
      0xc0fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc1a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbfa1= {
      0xbfa1,
      0xbffe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propc0a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbea1= {
      0xbea1,
      0xbefe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbfa1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbda1= {
      0xbda1,
      0xbdfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbea1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbca1= {
      0xbca1,
      0xbcfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbda1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbba1= {
      0xbba1,
      0xbbfe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbca1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propbaa1= {
      0xbaa1,
      0xbafe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbba1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb9a1= {
      0xb9a1,
      0xb9fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propbaa1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb8a1= {
      0xb8a1,
      0xb8fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb9a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb7a1= {
      0xb7a1,
      0xb7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb8a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb6a1= {
      0xb6a1,
      0xb6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb5a1= {
      0xb5a1,
      0xb5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb4a1= {
      0xb4a1,
      0xb4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb5a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb3a1= {
      0xb3a1,
      0xb3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb4a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb2a1= {
      0xb2a1,
      0xb2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb1a1= {
      0xb1a1,
      0xb1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb2a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propb0a1= {
      0xb0a1,
      0xb0fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb1a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa9a1= {
      0xa9a1,
      0xa9fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propb0a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa8a1= {
      0xa8a1,
      0xa8fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa9a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa7a1= {
      0xa7a1,
      0xa7fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa8a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa6a1= {
      0xa6a1,
      0xa6fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa7a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa5a1= {
      0xa5a1,
      0xa5fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa6a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa4a1= {
      0xa4a1,
      0xa4fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa5a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa3a1= {
      0xa3a1,
      0xa3fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa4a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa2a1= {
      0xa2a1,
      0xa2fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa3a1 
};
/*��--��*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Propa1a1= {
      0xa1a1,
      0xa1fe,
      &GUI_FontHZ28_CharInfo[1],
      (void *)&GUI_FontHZ28_Propa2a1 
};
//ASC�ַ�
/* --*/ 
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ28_Prop0020= {
      0x0020,
      0x007f,
      &GUI_FontHZ28_CharInfo[0],
      (void *)&GUI_FontHZ28_Propa1a1 
};
GUI_FLASH const GUI_FONT GUI_FontHZ28 = {
      GUI_FONTTYPE_PROP_SJIS, 
      28, 
      28, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZ28_Prop0020
};


