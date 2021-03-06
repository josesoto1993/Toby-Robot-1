/*
    LibToby.h
    Libreria para controlar al robot Toby
    Creada por Jose Soto, Fecha 12/10/2014
    Ultima modificacion 08/01/2015
*/
#include <Arduino.h>
#include <LibToby.h>
float pi = 3.141593;

Toby::Toby(int pinA[3],int pinB[3])
{
    for (int k=0;k<3;k++)
    {
        pinMode(pinA[k], OUTPUT);
        pinMode(pinB[k], OUTPUT);
        digitalWrite(pinB[k], LOW);
        digitalWrite(pinB[k], LOW);
        _pinA[k]=pinA[k];
        _pinB[k]=pinB[k];
    }
    float _defase[3];
    _defase[0] = 39.2255*(pi/180);//el motor 1 no esta alineado con el frente
    _defase[1] = 39.2255*(pi/180)+4*pi/3;    //defase entre ruedas 1 y 2, se espera 2pi/3
    _defase[2] = 39.2255*(pi/180)+2*pi/3;
    float _wm[3]={0,0,0};
    float _dir=0;
}

float fwmax(float w1,float w2,float w3)
{
    if(w1<0){
        w1=-w1;
    }
    if(w2<0){
        w2=-w2;
    }
    if(w3<0){
        w3=-w3;
    }
    if (w1<w2){
        w1=w2;
    }
    if (w1<w3){
        w1=w3;
    }
    return w1;
}

float voltaje(int motor,float w)
{
    /*
    solo para w positivo en pu y motor = 1 2 o 3
    estos ensayos deben repetirse para mejorar la precicion ya que se hicieron a poca bateria en el motor 3 y a eso se debe el 0.7
    */
    w=w*14.2419;      //wmaxima de los motores =14.2419
    switch (motor)
    {
        case 1:
            if(w>3.97){
                return 0.0789*w-0.125;
            }else{
                return 0.012565*w+0.15;
            }
        break;

        case 2:
            if(w>3.24){
                return 0.0741*w-0.0553;
            }else{
                return 0.0154*w+0.15;
            }
        break;

        case 3:
            if(w>3.97){
                return (0.0741*w-0.0553)*0.68;
            }else{
                return 0.0154*w+0.15;
            }
          break;
    }
}

void Toby::freno(int ftime)
{
    for (int k=0;k<3;k++)
    {
        digitalWrite(_pinA[k], LOW);
        digitalWrite(_pinB[k], LOW);
    }
    delay(ftime);
}

void Toby::setVMotor()
{
    int vaux=0;    //variable auxiliar
    for (int k=0;k<3;k++)
    {
        if(_wm[k]>0){
            vaux=255*voltaje(k+1,_wm[k]);
            digitalWrite(_pinB[k],LOW);
            analogWrite(_pinA[k],vaux);
        }else{
            vaux=255*voltaje(k+1,-1*_wm[k]);
            digitalWrite(_pinA[k],LOW);
            analogWrite(_pinB[k],vaux);
        }
    }
}

void Toby::setWMotor()
{
    switch ( int(_mod) )
    {
        case 0:
        {
            float wmax=0;  //variable auxiliar
            float wm[3];
            wm[0]=sin(_dir-_defase[0]);
            wm[1]=sin(_dir-_defase[1]);
            wm[2]=sin(_dir-_defase[2]);
            wmax=fwmax(wm[0],wm[1],wm[2]);
            wm[0]=_spd*wm[0]/wmax;
            wm[1]=_spd*wm[1]/wmax;
            wm[2]=_spd*wm[2]/wmax;
            break;
        }
        case 1:
        {
            float wmax=0;  //variable auxiliar
            float wm[3];
            wm[0]=sin(_dir-_defase[0])*cos(_rot)+sin(_rot);
            wm[1]=sin(_dir-_defase[1])*cos(_rot)+sin(_rot);
            wm[2]=sin(_dir-_defase[2])*cos(_rot)+sin(_rot);
            wmax=fwmax(wm[0],wm[1],wm[2]);
            wm[0]=_spd*wm[0]/wmax;
            wm[1]=_spd*wm[1]/wmax;
            wm[2]=_spd*wm[2]/wmax;
            break;
        }
        default:
        {
            break;
        }
    }
}

void Toby::setSpd(float spd)
{
    _spd=spd;
}

void Toby::setDir(float dir)
{
    _dir=dir;
}

void Toby::setMod(int mod)
{
    _mod=mod;
}

void Toby::setRot(int rot)
{
    _rot=rot;
}
