/* 
 * File:   weigh.h
 * Author: Asus
 *
 * Created on 1 June, 2017, 2:02 PM
 */

#ifndef WEIGHT_H
#define	WEIGHT_H

#define ADSK_high       output_high(PIN_C4)
#define ADSK_low        output_low(PIN_C4)
#define EQUAL_TO_5KG    3//3  //into change 110=5kg or 1kg=22 point//
#define EQUAL_TO_3KG    66
#define TOTAL_READING   100//100

//weight variables
uint32_t g_rawData        = 0;
uint32_t calibratedWeight = 0;
uint32_t arr_stab[71]     = {0};
uint32_t new_val          = 0;
uint32_t pre_val          = 0;
uint16_t dif_val          = 0;
uint8_t  units            = 0;
uint8_t  max_stab         = 0;
uint8_t  onlyOnce         = 0;
float    remainder        = 0;
float    sum              = 0;
float    avg              = 0;


#define WEIGHT_OF_OBJECT              14//20//17
#define STABILITY_FACTOR              70
#define TOLERANT_VAL                  10//10
#define ADD                           1
#define LIFT                          0

void weightINIT()
{
    ADSK_low;                 //compulsory low pin
}

uint32_t readStrainGuage()
{
    uint32_t pulse = 0;
    while(input(PIN_C3));    
    for (uint8_t i=0; i<24; i++)
    {
        ADSK_high;                   
        pulse=pulse<<1;
        ADSK_low;                  
        if( input(PIN_C3) ) pulse++;
    }
    
    ADSK_high;                 
    pulse=pulse^0x800000;
    ADSK_low;      
    return pulse;
}

uint32_t calibrateWeight()
{
    uint32_t avg = 0;    
    
    for(uint32_t calIndex = 0; calIndex < TOTAL_READING; calIndex++)
    {
        g_rawData = readStrainGuage();
        g_rawData = g_rawData/1000;
        avg += g_rawData;
        putc('.');
    }    
    avg = (avg/TOTAL_READING);
    //fprintf(comp,"\rCALIBRATED AT: = %ld\r",avg);
    return avg;
}

void mode()
{
    uint16_t mode[100],maxValue;
    uint8_t maxCount = 0;    
    for(uint8_t modeIndex = 0; modeIndex < sizeof(mode)/sizeof(mode[0]); modeIndex++)
    {
        mode[modeIndex] = readStrainGuage()/1000;
        if(onlyOnce)
        {
            if(mode[modeIndex] < calibratedWeight - EQUAL_TO_5KG) return;
        }
        delay_ms(50);
        fputc('.',comp);
    }    
    for(uint8_t i = 0; i < sizeof(mode)/sizeof(mode[0]); ++i)
    {
        int8 count = 0;        
        for (uint8_t j = 0; j < sizeof(mode)/sizeof(mode[0]); ++j)
        {
            if (mode[j] == mode[i])
            ++count;
        }
        if (count > maxCount)
        {
           maxCount = count;
           maxValue = mode[i];
        }
    }    
    calibratedWeight =  maxValue;
    fprintf(comp,"Calibrated at : = %ld\r",calibratedWeight);
}

uint32_t getRaw()
{
    return readStrainGuage()/1000;
}



//void isWeight()
//{ 
//    char height[10];
//    char inWhile = 0; 
//    float temp = 0;
//    long value2=0;      
//    if(getRaw() < calibratedWeight - EQUAL_TO_5KG)
//    {   
//       
//        while(getRaw() < calibratedWeight - EQUAL_TO_5KG)
//        {            
//            if(oprMode == RFID_MODE) return;
//            disable_interrupts(INT_RDA);
//            if(fgetc(HITE) == '#')
//            {
//                char data  = 0;              
//                char index = 0;
//                while(1)
//                {
//                    data = fgetc(HITE);
//                    if(data == '^')break;
//                    else
//                    {
//                        height[index++] = data;
//                    }
//                }
//            }
//            enable_interrupts(INT_RDA);
////             mode();
//            //fprintf(comp,"#cal weight:%lf--^\n\r",(double)(calibratedWeight - getRaw())/22.7);            
//            temp =(calibratedWeight - getRaw())/22.7; 
//            
////          fprintf(comp," temp value:#----%lf--^",temp);    
//            
//            if(!compareVal(temp))
//            {
//                max_val = 0;
//                arr[max_val++] = temp;
//            }
//            if(max_val == 5)
//            {
//                max_val = 0;
//                sum = 0;
//                for(char i=0; i < 5; i++)
//                {
//                    sum = arr[i] + sum;
////                    fprintf(comp,"arr value:#----%lf--^\n\r",(double)arr[i]);            
//                }
//                avg     = sum/5;
//                long value = avg*100;                 
//                if(avg > old_val + 0.2 || avg < old_val - 0.2)
//                {
//                    
//                }
//                for(int j=0;j<18;j++)
//                {
//                    fprintf(comp,"\n_");
//                }                
//                fprintf(comp,"\r| YOUR WEIGHT | %lf  |\n\r",(double)avg);
//                fprintf(comp,"| YOUR WEIGHT OLD | %lf  |\n\r",(double)old_val);
//                                    }
//                for(int k=0;k<18;k++)
//                {
//                    fprintf(comp,"_");
//                }
//                old_val = avg;                                          
//                }                  
//                else
//                {
////                    value2 = value;
//                    value2 = old_val*100;                    
//                    fprintf(comp,"\n\r| YOUR WEIGHT OLD | %lf  |\n\r",(double)old_val);
//                    fprintf(comp,"| YOUR WEIGHT | %lf  |\n\r",(double)avg);
//                }                
//                while(getRaw() < calibratedWeight - EQUAL_TO_5KG);               
//            }            
//       
////           if(strlen(height) > 4)
////                fprintf(comp,"#--%s--%lf--^",height,(double)(calibratedWeight - getRaw())/22.7);            
////            memset(height,'\0',sizeof(height));
//        }
//                          
//        delay_ms(500);              
//        mode();               
//    }   
//}

#endif	/* WEIGH_H */