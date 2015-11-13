/*
 * æ?? ä?? å??ï??RFID.pde
 * å?? å?º è??ï??Dr.Leong   ( WWW.B2CQSHOP.COM )
 * å??å?ºæ??æ??ï??2011.09.19
 * ä?? æ?? è??ï??
 * ä??æ??æ??æ??ï??
 * å??è??æ??è??ï??Mifare1 å??å??â??é??å??çª?â??é??å??â??è??å?? æ??å??
 */
 // the sensor communicates using SPI, so include the library:
#include <SPI.h>

#define  uchar unsigned char
#define uint  unsigned int

//æ??ç??æ??å??é??åº?
#define MAX_LEN 16

/////////////////////////////////////////////////////////////////////
//set the pin
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

//MF522å??ä??å??
#define PCD_IDLE              0x00               //NO action;å??æ??å??å??å??ä??
#define PCD_AUTHENT           0x0E               //éª?è??å??é??
#define PCD_RECEIVE           0x08               //æ??æ??æ??æ??
#define PCD_TRANSMIT          0x04               //å??é??æ??æ??
#define PCD_TRANSCEIVE        0x0C               //å??é??å??æ??æ??æ??æ??
#define PCD_RESETPHASE        0x0F               //å??ä??
#define PCD_CALCCRC           0x03               //CRCè??ç??

//Mifare_Oneå??ç??å??ä??å??
#define PICC_REQIDL           0x26               //å??å??çº?å?ºå??æ?ªè??å??ä??ç??ç??æ??
#define PICC_REQALL           0x52               //å??å??çº?å?ºå??å??é??å??
#define PICC_ANTICOLL         0x93               //é??å??æ??
#define PICC_SElECTTAG        0x93               //é??å??
#define PICC_AUTHENT1A        0x60               //éª?è??Aå??é??
#define PICC_AUTHENT1B        0x61               //éª?è??Bå??é??
#define PICC_READ             0x30               //è??å??
#define PICC_WRITE            0xA0               //å??å??
#define PICC_DECREMENT        0xC0               //æ??æ??
#define PICC_INCREMENT        0xC1               //å??å??
#define PICC_RESTORE          0xC2               //è??å??æ??æ??å??ç??å??å?º
#define PICC_TRANSFER         0xB0               //ä??å??ç??å??å?ºä??æ??æ??
#define PICC_HALT             0x50               //ä??ç??


//å??MF522é??è??æ??è??å??ç??é??è??ä??ç??
#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2


//------------------MFRC522å??å??å??---------------
//Page 0:Command and Status
#define     Reserved00            0x00    
#define     CommandReg            0x01    
#define     CommIEnReg            0x02    
#define     DivlEnReg             0x03    
#define     CommIrqReg            0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command     
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG    
#define     Reserved20            0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg            0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister     
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     Reserved31            0x3C   
#define     Reserved32            0x3D   
#define     Reserved33            0x3E   
#define     Reserved34        0x3F
//-----------------------------------------------

//4å­—èŠ‚å¡åºåˆ—å·ï¼Œç¬¬5å­—èŠ‚ä¸ºæ ¡éªŒå­—èŠ‚
uchar serNum[5];

uchar  writeData[16]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100};  //åˆå§‹åŒ– 100å…ƒé’±
uchar  moneyConsume = 18 ;  //æ¶ˆè´¹18å…ƒ
uchar  moneyAdd = 10 ;  //å……å€¼10å…ƒ
//æ‰‡åŒºAå¯†ç ï¼Œ16ä¸ªæ‰‡åŒºï¼Œæ¯ä¸ªæ‰‡åŒºå¯†ç 6Byte
 uchar sectorKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                             //{0x19, 0x84, 0x07, 0x15, 0x76, 0x14},
                             {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                            };
 uchar sectorNewKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                                {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0x19,0x84,0x07,0x15,0x76,0x14},
                                 //you can set another ket , such as  " 0x19, 0x84, 0x07, 0x15, 0x76, 0x14 "
                                 //{0x19, 0x84, 0x07, 0x15, 0x76, 0x14, 0xff,0x07,0x80,0x69, 0x19,0x84,0x07,0x15,0x76,0x14},
                                 // but when loop, please set the  sectorKeyA, the same key, so that RFID module can read the card
                                {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0x19,0x33,0x07,0x15,0x34,0x14},
                               };

void setup() {                
   Serial.begin(9600);                       // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
 // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);             // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);          // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);               // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);

  MFRC522_Init();  
}

void loop()
{
    uchar i,tmp;
  uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;  //é€‰æ‹©æ“ä½œçš„å—åœ°å€0ï½ž63
        String mynum = "";


    //å¯»å¡ï¼Œè¿”å›žå¡ç±»åž‹  
    status = MFRC522_Request(PICC_REQIDL, str); 
    if (status == MI_OK)
    {
                        Serial.println("Card detected");
      Serial.print(str[0],BIN);
                        Serial.print(" , ");
      Serial.print(str[1],BIN);
                        Serial.println(" ");
    }

    //é˜²å†²æ’žï¼Œè¿”å›žå¡çš„åºåˆ—å· 4å­—èŠ‚
    status = MFRC522_Anticoll(str);
    memcpy(serNum, str, 5);
    if (status == MI_OK)
    {

                        Serial.println("The card's number is  : ");
      Serial.print(serNum[0]);
                        Serial.print(" , ");
      Serial.print(serNum[1],BIN);
                        Serial.print(" , ");
      Serial.print(serNum[2],BIN);
                        Serial.print(" , ");
      Serial.print(serNum[3],BIN);
                        Serial.print(" , ");
      Serial.print(serNum[4],BIN);
                        Serial.println(" ");
                        
                        // Should really check all pairs, but for now we'll just use the first
                     
                        if(serNum[0] == 88) {
                          Serial.println("Hello Grant");
                        } else if(serNum[0] == 173) {
                          Serial.println("Hello David");
                        }
                        delay(1000);
    }
                //Serial.println(" ");
    MFRC522_Halt();     //å??ä??å??ç??è??å??ä??ç??ç??æ??              
          
}

/*
 * å?? æ?? å??ï??Write_MFRC5200
 * å??è??æ??è??ï??å??MFRC522ç??æ??ä??å??å??å??å??ä??ä?ªå??è??æ??æ??
 * è??å??å??æ??ï??addr--å??å??å??å??å??ï??val--è??å??å??ç??å??
 * è?? å?? å??ï??æ??
 */
void Write_MFRC522(uchar addr, uchar val)
{
  digitalWrite(chipSelectPin, LOW);

  //å??å??æ??å??ï??0XXXXXX0
  SPI.transfer((addr<<1)&0x7E); 
  SPI.transfer(val);
  
  digitalWrite(chipSelectPin, HIGH);
}


/*
 * å?? æ?? å??ï??Read_MFRC522
 * å??è??æ??è??ï??ä??MFRC522ç??æ??ä??å??å??å??è??ä??ä?ªå??è??æ??æ??
 * è??å??å??æ??ï??addr--å??å??å??å??å??
 * è?? å?? å??ï??è??å??è??å??å??ç??ä??ä?ªå??è??æ??æ??
 */
uchar Read_MFRC522(uchar addr)
{
  uchar val;

  digitalWrite(chipSelectPin, LOW);

  //å??å??æ??å??ï??1XXXXXX0
  SPI.transfer(((addr<<1)&0x7E) | 0x80);  
  val =SPI.transfer(0x00);
  
  digitalWrite(chipSelectPin, HIGH);
  
  return val; 
}

/*
 * å?? æ?? å??ï??SetBitMask
 * å??è??æ??è??ï??ç??RC522å??å??å??ä??
 * è??å??å??æ??ï??reg--å??å??å??å??å??;mask--ç??ä??å??
 * è?? å?? å??ï??æ??
 */
void SetBitMask(uchar reg, uchar mask)  
{
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);  // set bit mask
}


/*
 * å?? æ?? å??ï??ClearBitMask
 * å??è??æ??è??ï??æ??RC522å??å??å??ä??
 * è??å??å??æ??ï??reg--å??å??å??å??å??;mask--æ??ä??å??
 * è?? å?? å??ï??æ??
 */
void ClearBitMask(uchar reg, uchar mask)  
{
    uchar tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));  // clear bit mask
} 


/*
 * å?? æ?? å??ï??AntennaOn
 * å??è??æ??è??ï??å??å??å??çº?,æ??æ??å??å??æ??å??é??å??é??å??å??ä??é??åº?è??å??æ??1msç??é??é??
 * è??å??å??æ??ï??æ??
 * è?? å?? å??ï??æ??
 */
void AntennaOn(void)
{
  uchar temp;

  temp = Read_MFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    SetBitMask(TxControlReg, 0x03);
  }
}


/*
 * å?? æ?? å??ï??AntennaOff
 * å??è??æ??è??ï??å??é??å??çº?,æ??æ??å??å??æ??å??é??å??é??å??å??ä??é??åº?è??å??æ??1msç??é??é??
 * è??å??å??æ??ï??æ??
 * è?? å?? å??ï??æ??
 */
void AntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}


/*
 * å?? æ?? å??ï??ResetMFRC522
 * å??è??æ??è??ï??å??ä??RC522
 * è??å??å??æ??ï??æ??
 * è?? å?? å??ï??æ??
 */
void MFRC522_Reset(void)
{
    Write_MFRC522(CommandReg, PCD_RESETPHASE);
}


/*
 * å?? æ?? å??ï??InitMFRC522
 * å??è??æ??è??ï??å??å??å??RC522
 * è??å??å??æ??ï??æ??
 * è?? å?? å??ï??æ??
 */
void MFRC522_Init(void)
{
  digitalWrite(NRSTPD,HIGH);

  MFRC522_Reset();
    
  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_MFRC522(TModeReg, 0x8D);    //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(TPrescalerReg, 0x3E); //TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(TReloadRegL, 30);           
    Write_MFRC522(TReloadRegH, 0);
  
  Write_MFRC522(TxAutoReg, 0x40);   //100%ASK
  Write_MFRC522(ModeReg, 0x3D);   //CRCå??å??å??0x6363  ???

  //ClearBitMask(Status2Reg, 0x08);   //MFCrypto1On=0
  //Write_MFRC522(RxSelReg, 0x86);    //RxWait = RxSelReg[5..0]
  //Write_MFRC522(RFCfgReg, 0x7F);      //RxGain = 48dB

  AntennaOn();    //æ??å??å??çº?
}


/*
 * å?? æ?? å??ï??MFRC522_Request
 * å??è??æ??è??ï??å??å??ï??è??å??å??ç??å??å??
 * è??å??å??æ??ï??reqMode--å??å??æ??å??ï??
 *       TagType--è??å??å??ç??ç??å??
 *        0x4400 = Mifare_UltraLight
 *        0x0400 = Mifare_One(S50)
 *        0x0200 = Mifare_One(S70)
 *        0x0800 = Mifare_Pro(X)
 *        0x4403 = Mifare_DESFire
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_Request(uchar reqMode, uchar *TagType)
{
  uchar status;  
  uint backBits;      //æ??æ??å??ç??æ??æ??ä??æ??

  Write_MFRC522(BitFramingReg, 0x07);   //TxLastBists = BitFramingReg[2..0] ???
  
  TagType[0] = reqMode;
  status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

  if ((status != MI_OK) || (backBits != 0x10))
  {    
    status = MI_ERR;
  }
   
  return status;
}


/*
 * å?? æ?? å??ï??MFRC522_ToCard
 * å??è??æ??è??ï??RC522å??ISO14443å??é??è??
 * è??å??å??æ??ï??command--MF522å??ä??å??ï??
 *       sendData--é??è??RC522å??é??å??å??ç??ç??æ??æ??, 
 *       sendLen--å??é??ç??æ??æ??é??åº?    
 *       backData--æ??æ??å??ç??å??ç??è??å??æ??æ??ï??
 *       backLen--è??å??æ??æ??ç??ä??é??åº?
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
{
    uchar status = MI_ERR;
    uchar irqEn = 0x00;
    uchar waitIRq = 0x00;
    uchar lastBits;
    uchar n;
    uint i;

    switch (command)
    {
        case PCD_AUTHENT:   //è??è??å??å??
    {
      irqEn = 0x12;
      waitIRq = 0x10;
      break;
    }
    case PCD_TRANSCEIVE:  //å??é??FIFOä??æ??æ??
    {
      irqEn = 0x77;
      waitIRq = 0x30;
      break;
    }
    default:
      break;
    }
   
    Write_MFRC522(CommIEnReg, irqEn|0x80);  //å??è??ä??æ??è??æ??
    ClearBitMask(CommIrqReg, 0x80);     //æ??é??æ??æ??ä??æ??è??æ??ä??
    SetBitMask(FIFOLevelReg, 0x80);     //FlushBuffer=1, FIFOå??å??å??
    
  Write_MFRC522(CommandReg, PCD_IDLE);  //NO action;å??æ??å??å??å??ä??  ???

  //å??FIFOä??å??å??æ??æ??
    for (i=0; i<sendLen; i++)
    {   
    Write_MFRC522(FIFODataReg, sendData[i]);    
  }

  //æ??è??å??ä??
  Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {    
    SetBitMask(BitFramingReg, 0x80);    //StartSend=1,transmission of data starts  
  }   
    
  //ç??å??æ??æ??æ??æ??å??æ??
  i = 2000; //iæ??æ??æ??é??é??ç??è??æ??ï??æ??ä??M1å??æ??å??ç??å??æ??é??25ms ???
    do 
    {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);      //StartSend=0
  
    if (i != 0)
    {    
        if(!(Read_MFRC522(ErrorReg) & 0x1B))  //BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
        status = MI_NOTAGERR;     //??   
      }

            if (command == PCD_TRANSCEIVE)
            {
                n = Read_MFRC522(FIFOLevelReg);
                lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   
          *backLen = (n-1)*8 + lastBits;   
        }
                else
                {   
          *backLen = n*8;   
        }

                if (n == 0)
                {   
          n = 1;    
        }
                if (n > MAX_LEN)
                {   
          n = MAX_LEN;   
        }
        
        //è??å??FIFOä??æ??æ??å??ç??æ??æ??
                for (i=0; i<n; i++)
                {   
          backData[i] = Read_MFRC522(FIFODataReg);    
        }
            }
        }
        else
        {   
      status = MI_ERR;  
    }
        
    }
  
    //SetBitMask(ControlReg,0x80);           //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 

    return status;
}


/*
 * å?? æ?? å??ï??MFRC522_Anticoll
 * å??è??æ??è??ï??é??å??çª?æ??æµ?ï??è??å??é??ä??å??ç??ç??å??åº?å??å??
 * è??å??å??æ??ï??serNum--è??å??4å??è??å??åº?å??å??,ç??5å??è??ä?ºæ??éª?å??è??
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_Anticoll(uchar *serNum)
{
    uchar status;
    uchar i;
  uchar serNumCheck=0;
    uint unLen;
    

    //ClearBitMask(Status2Reg, 0x08);   //TempSensclear
    //ClearBitMask(CollReg,0x80);     //ValuesAfterColl
  Write_MFRC522(BitFramingReg, 0x00);   //TxLastBists = BitFramingReg[2..0]
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
  {
    //æ??éª?å??åº?å??å??
    for (i=0; i<4; i++)
    {   
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i])
    {   
      status = MI_ERR;    
    }
    }

    //SetBitMask(CollReg, 0x80);    //ValuesAfterColl=1

    return status;
} 


/*
 * å?? æ?? å??ï??CalulateCRC
 * å??è??æ??è??ï??ç??MF522è??ç??CRC
 * è??å??å??æ??ï??pIndata--è??è??æ??CRCç??æ??æ??ï??len--æ??æ??é??åº?ï??pOutData--è??ç??ç??CRCç??æ??
 * è?? å?? å??ï??æ??
 */
void CalulateCRC(uchar *pIndata, uchar len, uchar *pOutData)
{
    uchar i, n;

    ClearBitMask(DivIrqReg, 0x04);      //CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);     //æ??FIFOæ??é??
    //Write_MFRC522(CommandReg, PCD_IDLE);

  //å??FIFOä??å??å??æ??æ??  
    for (i=0; i<len; i++)
    {   
    Write_MFRC522(FIFODataReg, *(pIndata+i));   
  }
    Write_MFRC522(CommandReg, PCD_CALCCRC);

  //ç??å??CRCè??ç??å??æ??
    i = 0xFF;
    do 
    {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));      //CRCIrq = 1

  //è??å??CRCè??ç??ç??æ??
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}


/*
 * å?? æ?? å??ï??MFRC522_SelectTag
 * å??è??æ??è??ï??é??å??ï??è??å??å??å??å??å??å??é??
 * è??å??å??æ??ï??serNum--ä??å??å??åº?å??å??
 * è?? å?? å??ï??æ??å??è??å??å??å??é??
 */
uchar MFRC522_SelectTag(uchar *serNum)
{
    uchar i;
  uchar status;
  uchar size;
    uint recvBits;
    uchar buffer[9]; 

  //ClearBitMask(Status2Reg, 0x08);     //MFCrypto1On=0

    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++)
    {
      buffer[i+2] = *(serNum+i);
    }
  CalulateCRC(buffer, 7, &buffer[7]);   //??
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    
    if ((status == MI_OK) && (recvBits == 0x18))
    {   
    size = buffer[0]; 
  }
    else
    {   
    size = 0;    
  }

    return size;
}


/*
 * å?? æ?? å??ï??MFRC522_Auth
 * å??è??æ??è??ï??éª?è??å??ç??å??ç??
 * è??å??å??æ??ï??authMode--å??ç??éª?è??æ??å??
                 0x60 = éª?è??Aå??é??
                 0x61 = éª?è??Bå??é?? 
             BlockAddr--å??å??å??
             Sectorkey--æ??å?ºå??ç??
             serNum--å??ç??åº?å??å??ï??4å??è??
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_Auth(uchar authMode, uchar BlockAddr, uchar *Sectorkey, uchar *serNum)
{
    uchar status;
    uint recvBits;
    uchar i;
  uchar buff[12]; 

  //éª?è??æ??ä??+å??å??å??ï??æ??å?ºå??ç??ï??å??åº?å??å??
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++)
    {    
    buff[i+2] = *(Sectorkey+i);   
  }
    for (i=0; i<4; i++)
    {    
    buff[i+8] = *(serNum+i);   
  }
    status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {   
    status = MI_ERR;   
  }
    
    return status;
}


/*
 * å?? æ?? å??ï??MFRC522_Read
 * å??è??æ??è??ï??è??å??æ??æ??
 * è??å??å??æ??ï??blockAddr--å??å??å??;recvData--è??å?ºç??å??æ??æ??
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_Read(uchar blockAddr, uchar *recvData)
{
    uchar status;
    uint unLen;

    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    CalulateCRC(recvData,2, &recvData[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

    if ((status != MI_OK) || (unLen != 0x90))
    {
        status = MI_ERR;
    }
    
    return status;
}


/*
 * å?? æ?? å??ï??MFRC522_Write
 * å??è??æ??è??ï??å??å??æ??æ??
 * è??å??å??æ??ï??blockAddr--å??å??å??;writeData--å??å??å??16å??è??æ??æ??
 * è?? å?? å??ï??æ??å??è??å??MI_OK
 */
uchar MFRC522_Write(uchar blockAddr, uchar *writeData)
{
    uchar status;
    uint recvBits;
    uchar i;
  uchar buff[18]; 
    
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {   
    status = MI_ERR;   
  }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)    //å??FIFOå??16Byteæ??æ??
        {    
          buff[i] = *(writeData+i);   
        }
        CalulateCRC(buff, 16, &buff[16]);
        status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
        
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
        {   
      status = MI_ERR;   
    }
    }
    
    return status;
}


/*
 * å?? æ?? å??ï??MFRC522_Halt
 * å??è??æ??è??ï??å??ä??å??ç??è??å??ä??ç??ç??æ??
 * è??å??å??æ??ï??æ??
 * è?? å?? å??ï??æ??
 */
void MFRC522_Halt(void)
{
  uchar status;
    uint unLen;
    uchar buff[4]; 

    buff[0] = PICC_HALT;
    buff[1] = 0;
    CalulateCRC(buff, 2, &buff[2]);
 
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}

