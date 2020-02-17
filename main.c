/* 
 * File:   main.c
 * Author: Mohd Azka
 *
 * Created on 20 May, 2017, 3:56 PM
 */


#include <18f4410.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "weight.h"

//unsigned long check_stability(unsigned long val, char direction)
//{
//    unsigned long data = 0;
//    unsigned int16 STABLITY_COUNTER= 0;
//    
//    for(unsigned int16 st = 0; st < STABILITY_FACTOR; st++)
//    {        
//        data = getRaw();
//        fprintf(comp,"current val and Original val is %ld & %ld \n\r",data, val);
//        if(direction == ADD)
//        {
//            if(data < val - WEIGHT_OF_OBJECT)
//                STABLITY_COUNTER++;
//        }
//        else if(direction == LIFT)
//        {
//            if(data > val + WEIGHT_OF_OBJECT)
//                STABLITY_COUNTER++;
//        }
//    }
//    
//    fprintf(comp,"stability counter = %ld\n\r", STABLITY_COUNTER);
//    
//    if(STABLITY_COUNTER >=  STABILITY_FACTOR - TOLERANT_VAL)
//        return data;
//    return 0;
//} 



BOOLEAN Check_Stability(unsigned long val)
{
    if(max_stab == 0)
    {
        arr_stab[max_stab++] = val;
        return;
    }
    for(char i = 0; i < max_stab; i++ )
    {
        if(val > arr_stab[i] + 2 || val < arr_stab[i] - 2)
        {            
            return 0;
        }
    }    
    arr_stab[max_stab++]  = val;
    return 1;
}

uint8_t Is_True_Weight()
{
    if( pre_val && (new_val > pre_val  || new_val < pre_val ) )
    {
        //fprintf(comp,"checking true new and pre val  are [%ld & %ld] \n\r",new_val, pre_val);
        uint32_t temp = 0; 
        for(uint8_t i = 0; i < 5; i++ )
        {
            temp = getRaw();
            if(temp > pre_val + 4  || temp < pre_val - 4)
                return 1;
        }
        return 0;
    }
    return 0;
}

void Is_Obj()
{
    max_stab = 0;
    new_val  = getRaw();
    fprintf(comp,"new and pre val  are [%ld & %ld] \n\r",new_val, pre_val);
    
    //if( pre_val && (new_val > pre_val  || new_val < pre_val) )
    if( Is_True_Weight() )
    {
        fprintf(comp,"TRUE WEIGHT FOUND\n\r");
        uint32_t temp              = 0;    //get current weight    
        uint32_t avg               = 0;
        uint32_t sum               = 0;
        uint32_t my_stable_val     = 0;
        
        while( max_stab < STABILITY_FACTOR )
        {
            temp = getRaw(); fputc('.',COMP);
            
            if(!Check_Stability(temp))
                max_stab = 0;
        }
        
        fprintf(comp,"OUR STABLE VALs\n\r");
        for(uint8_t i = 0; i < STABILITY_FACTOR; i++)
        {
            fprintf(comp,"[%ld] \n\r",arr_stab[i]);  delay_ms(100);    
            sum = sum + arr_stab[i];
            
        }  
        fprintf(comp,"end of vals\n\r");
        
        avg  = sum/STABILITY_FACTOR;
        my_stable_val = avg;
        
        
        if(my_stable_val > pre_val)
        {
            dif_val     = my_stable_val - pre_val;
            units       = dif_val/WEIGHT_OF_OBJECT;
            remainder   = dif_val%WEIGHT_OF_OBJECT;

            if(remainder >= WEIGHT_OF_OBJECT - 4)
                units  = units + 1;
            fprintf(comp,"              [UNITS]    = ");
            fputc('-',WIFI);
            fputc( units+48,WIFI);
            fputc('-',comp);
            fputc( units+48,comp);
            fprintf(comp,"\n\r");
        }
        else
        {
            dif_val     = pre_val - my_stable_val;
            units       = dif_val/WEIGHT_OF_OBJECT;
            remainder   = dif_val%WEIGHT_OF_OBJECT;
            
            if(remainder >= WEIGHT_OF_OBJECT - 4)
                units  = units + 1;
            fprintf(comp,"              [UNITS]    = ");
            fputc('+',WIFI);
            fputc( units+48,WIFI );
            fputc('+',comp);
            fputc( units+48,comp);
            fprintf(comp,"\n\r");
        }        
        pre_val = getRaw();    
        
        fprintf(comp,"             [DIFF]      = %ld\n\r",dif_val);
        fprintf(comp,"             [REMAINDER] = %f\n\r",remainder);
    }
    else
        pre_val = new_val;
}

void main() 
{
    setUp();    
    weightINIT();    
    
    while(TRUE)
    {          
        Is_Obj();
    }
}










