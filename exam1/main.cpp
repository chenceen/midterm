#include "mbed.h"
#include "uLCD_4DGL.h"
using namespace std::chrono;

uLCD_4DGL uLCD(D1, D0, D2);
InterruptIn confirm(D5);
AnalogIn Ain(A0);
DigitalIn sw(USER_BUTTON);
//DigitalOut led(LED2);
AnalogOut Aout(D7);
InterruptIn upbtn(D3);
InterruptIn downbtn(D4);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
Thread wave;
Thread sample;

int mode = 0;
int a = 1;
//float f=1;
float sr = 1;
float ADCdata[200];


void downdisplay()
{
    if(mode>0) mode--;
    else mode = 0;
    switch(mode)
    {
        case 0:
            uLCD.triangle(86, 35, 89, 32, 89, 38, WHITE);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;
        case 1:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, WHITE);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;       
        case 2:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, WHITE);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;      
        case 3:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, WHITE);
            break;    
        default:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK); 
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);       
            uLCD.printf("error\n");
    }

}
void updisplay()
{
    if(mode<3) mode++;
    else mode = 3;
    switch(mode)
    {
        case 0:
            uLCD.triangle(86, 35, 89, 32, 89, 38, WHITE);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;
        case 1:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, WHITE);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;       
        case 2:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, WHITE);
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);
            break;      
        case 3:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            uLCD.triangle(86, 86, 89, 83, 89, 89, WHITE);
            break;    
        default:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK); 
            uLCD.triangle(86, 86, 89, 83, 89, 89, BLACK);       
            uLCD.printf("error\n");
    }
}


void sampling()
{
    //float ADCdata[600];                                  
    while(1)
    {
        while(1)
        {
            if(sw==1) break;
            for(int i = 0; i < 200; i++)
            {
                ADCdata[i] = Ain;
                wait_us(5000);

            }
            //printf("%f\r\n", f);                         
            for(int i =0; i < 200; i++)
            {
                printf("%f\r\n", ADCdata[i]);
                wait_us(50000);
            }
        }
    }
}


void waveform()
{
    while(1)
    {
        //led = !led;
        for(float i = 0.0f; i < 1; i=i+0.001*a)
        {
            Aout = i;
            wait_us(100);
        }
        for(float i = 0.0f; i < 1-0.2*sr; i=i+0.01)
        {        
            Aout = 1;
            ThisThread::sleep_for(1ms);
        }  
       for(float i = 1; i > 0.0f; i=i-0.001*a)
        {
            Aout = i;
            wait_us(100);
        }    
    }
}

void output()
{
    if(mode==0)
    {
        uLCD.triangle(86, 35, 89, 32, 89, 38, GREEN);
        sr = 1/8;
        a = 8;
    }
    else if(mode==1)
    {
        uLCD.triangle(86, 52, 89, 49, 89, 55, GREEN);
        sr = 1/4;
        a = 4;
    }
    else if(mode==2)
    {
        uLCD.triangle(86, 69, 89, 66, 89, 72, GREEN);
        sr = 1/2;
        a = 2;
    }
    else if(mode==3)
    {
        uLCD.triangle(86, 86, 89, 83, 89, 89, GREEN);
        sr = 1;
        a = 1;
    }
    

}

int main()
{
    mode = 0;
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    upbtn.rise(queue.event(updisplay));
    downbtn.rise(queue.event(downdisplay));
    confirm.rise(queue.event(output));
    uLCD.printf("\nSelect a slew rate\n"); 
    uLCD.locate(5, 4);
    uLCD.printf("1/8\n");
    uLCD.locate(5, 6);
    uLCD.printf("1/4\n");
    uLCD.locate(5, 8);
    uLCD.printf("1/2\n");
    uLCD.locate(5, 10);
    uLCD.printf("  1\n");
    //led = 1;
    wave.start(waveform);
    //ThisThread::sleep_for(1020ms);
    sample.start(sampling);

}
