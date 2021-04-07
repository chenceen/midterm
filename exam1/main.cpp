#include "mbed.h"
#include "uLCD_4DGL.h"
using namespace std::chrono;

uLCD_4DGL uLCD(D1, D0, D2);
InterruptIn confirm(D5);
//DigitalIn confirm(D5);
AnalogIn Ain(A0);
AnalogOut Aout(D7);
InterruptIn upbtn(D3);
InterruptIn downbtn(D4);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t(osPriorityHigh);
Thread wave(osPriorityNormal);
Thread sample(osPriorityNormal);

int mode = 0;
float f=1;
float ADCdata[500];


void downdisplay()
{
    if(mode>0) mode--;
    else mode = 0;
    //while(!confirm){
    switch(mode)
    {
        case 0:
            //f = 1;
            uLCD.triangle(86, 35, 89, 32, 89, 38, WHITE);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            break;
        case 1:
            //f = 185;
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, WHITE);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            break;       
        case 2:
            //f = 400;
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, WHITE);
            break;      
        default:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);        
            uLCD.printf("error\n");
    }
    //}
    //if(mode==0) uLCD.triangle(86, 35, 89, 32, 89, 38, GREEN);
    //else if(mode==1) uLCD.triangle(86, 52, 89, 49, 89, 55, GREEN);
    //else if(mode==2) uLCD.triangle(86, 69, 89, 66, 89, 72, GREEN);
}
void updisplay()
{
    if(mode<2) mode++;
    else mode = 2;
    //while(!confirm){
    switch(mode)
    {
        case 0:
            //f = 1;
            uLCD.triangle(86, 35, 89, 32, 89, 38, WHITE);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            break;
        case 1:
            //f = 185;
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, WHITE);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);
            break;       
        case 2:
            //f = 400;
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, WHITE);
            break;      
        default:
            uLCD.triangle(86, 35, 89, 32, 89, 38, BLACK);
            uLCD.triangle(86, 52, 89, 49, 89, 55, BLACK);
            uLCD.triangle(86, 69, 89, 66, 89, 72, BLACK);        
            uLCD.printf("error\n");
    }
    //}
    //if(mode==0) uLCD.triangle(86, 35, 89, 32, 89, 38, GREEN);
    //else if(mode==1) uLCD.triangle(86, 52, 89, 49, 89, 55, GREEN);
    //else if(mode==2) uLCD.triangle(86, 69, 89, 66, 89, 72, GREEN);
}
void sampling()
{
    while(1)
    {
        for(int i = 0; i < 500; i++)
        {
            ADCdata[i] = Ain;
            wait_us(2000);
        }
        for(int i =0; i < 500; i++)
        {
            printf("%f\r\n", ADCdata[i]);
            ThisThread::sleep_for(100ms);
        }
    }
}

void waveform()
{
    while(1){
    for(float i = 0.0f; i < 1; i=i+f/4000)
    {
        Aout = i;
        wait_us(37);
    }
    for(float i = 0.0f; i < 4; i=i+f/4000)
    {        
        Aout = (4-i)/4.0f;
        wait_us(37);
    }      
    }
}

void output()
{
    if(mode==0)
    {
        uLCD.triangle(86, 35, 89, 32, 89, 38, GREEN);
        f = 1;
    }
    else if(mode==1)
    {
        uLCD.triangle(86, 52, 89, 49, 89, 55, GREEN);
        f = 185;
    }
    else if(mode==2)
    {
        uLCD.triangle(86, 69, 89, 66, 89, 72, GREEN);
        f = 800;
    }
    //for(int i =0; i < 250; i++)
    //    ADCdata[i] = 0.0f;

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
    wave.start(waveform);
    ThisThread::sleep_for(1020ms);
    sample.start(sampling);

}
