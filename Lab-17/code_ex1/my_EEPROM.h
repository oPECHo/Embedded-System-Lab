//��͸Ժ�� �ѧ��ѹ����Ѻ��Ҿ��������� 2 ��� ��� ���˹�˹��¤����ӷ���ͧ�����¹��Ф�ҷ���ͧ�����¹
void EEPROM_Erase_and_Write1B(uint16_t addr, uint8_t data)
{
  while(EECR & (1<<EEPE)) ; //ǹ��Ө����ҺԵ EEPE ������¹�繵�áе��
  EECR = 0x00;      //mode erase and write
  EEAR = addr;      //�Ѵ�͡�����������Ѻ��� addr ����è������ EEAR
  EEDR = data;      //�Ѵ�͡�����������Ѻ��� data ����è������ EEDR
  char backupSREG;    //���������Ѻ�纤��ʶҹТͧ�è�������Ǻ觪��
  backupSREG = SREG;    //�Ѵ�͡�����è�������Ǻ觪�����㹵���÷�������
  cli();        //�Դ�ҧ��âѴ�ѧ�����ǹ��ҧ�ͧ��ǻ����ż�
  EECR |= (1<<EEMPE);   //������Ե EEMPE � EECR �繵�á��٧
  EECR |= (1<<EEPE);    //������Ե EEPE � EECR �繵�á��٧
  SREG = backupSREG;    //�׹����è�������Ǻ觪���Ѻ���ʶҹ����
}



void EEPROM_Erase_only(uint16_t addr)
{  
  while(EECR & (1<<EEPE)) ; //ǹ��Ө����ҺԵ EEPE ������¹�繵�áе��
  EECR = 0b01 << EEPM0;   //ź���ҧ���� 
  EEAR = addr;      //�Ѵ�͡�����������Ѻ��� addr ����è������ EEAR
  EEDR = 0xFF;      //�Ѵ�͡�����������Ѻ��� data ����è������ EEDR
  char backupSREG;    //���������Ѻ�纤��ʶҹТͧ�è�������Ǻ觪��
  backupSREG = SREG;    //�Ѵ�͡�����è�������Ǻ觪�����㹵���÷�������
  cli();        //�Դ�ҧ��âѴ�ѧ�����ǹ��ҧ�ͧ��ǻ����ż�
  EECR |= (1<<EEMPE);   //������Ե EEMPE � EECR �繵�á��٧
  EECR |= (1<<EEPE);    //������Ե EEPE � EECR �繵�á��٧
  SREG = backupSREG;    //�׹����è�������Ǻ觪���Ѻ���ʶҹ����
}


void EEPROM_Write_to_Empty_location(uint16_t addr, uint8_t data)
{
  while(EECR & (1<<EEPE)) ; //ǹ��Ө����ҺԵ EEPE ������¹�繵�áе��
  EECR = 0b10 << EEPM0;    //��¹�����˹觷��ź�������ǡ�͹˹�ҹ��
  EEAR = addr;      //�Ѵ�͡�����������Ѻ��� addr ����è������ EEAR
  EEDR = data;      //�Ѵ�͡�����������Ѻ��� data ����è������ EEDR
  char backupSREG;    //���������Ѻ�纤��ʶҹТͧ�è�������Ǻ觪��
  backupSREG = SREG;    //�Ѵ�͡�����è�������Ǻ觪�����㹵���÷�������
  cli();        //�Դ�ҧ��âѴ�ѧ�����ǹ��ҧ�ͧ��ǻ����ż�
  EECR |= (1<<EEMPE);   //������Ե EEMPE � EECR �繵�á��٧
  EECR |= (1<<EEPE);    //������Ե EEPE � EECR �繵�á��٧
  SREG = backupSREG;    //�׹����è�������Ǻ觪���Ѻ���ʶҹ����
}




unsigned char EEPROM_read1byte(uint16_t addr)  //���Ϳѧ��ѹ��о����������Ѻ���
{       
  while (EECR & (1<<EEPE)) ;    //ǹ��Ө����ҺԵ EEPE ������¹�繵�áе��
  EEAR = addr;        //�Ѵ�͡�����������Ѻ�������è������ EEAR
  EECR |= (1<<EERE);      //������Ե EERE �繵�á��٧��������������ҹ
  return EEDR;        //�Ӥ�ҷ���ҡ�� EEDR ���ҹ
}


void display_all_data_in_EEPROM()
{
  Serial.println("Data in the EEPROM");
  int i;
  uint8_t c;
  for (i=0;i<1024;i++)
  {
    c = EEPROM_read1byte(i);
    if( (i>0) && (i%16==0) )
        Serial.println("");
    Serial.print(c);
    Serial.print(" ");
  }
  Serial.println("\n\r--------------------------");  
}


void update_if_data_changed(uint16_t addr, uint8_t data)
{
  uint8_t x;
  x = EEPROM_read1byte(addr);
  if (x!=data)
    EEPROM_Erase_and_Write1B(addr, data);
}