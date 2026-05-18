#include <Arduino.h>

int motor1Pin1 = 25; 
int motor1Pin2 = 26; 
int motor2Pin1 = 27;
int motor2Pin2 = 14;
int en1 = 16;
int en2 = 17;
int ir1 = 35;
int ir2 = 33;
int ir3 = 32;
int ir4 = 34;
int s1, s2 ,s3, s4;
int limiar = 90,PWM_MAX = 240;
float kp = 0.17,ki =0,kd = 0,error;

float ComparaErro();//comparar se o erro está próximo de 0
//se sim, retorna o valor do incremento,caso contrario 0

float CorrigePWM(float pwmsomado,float erro){
    float sinal = (pwmsomado >= 0) ? 1.0 : -1.0;
    if(fabs(pwmsomado) >= limiar){
        return (int) sinal * constrain(fabs(pwmsomado),limiar,250);
    }
    if (fabs(erro) >= 700){
        return -1*constrain(fabs(pwmsomado) ,250,limiar);
    }
    return 0;
}

float PID(){
    float proporcional,derivada,pid;
    static float lasterror;
    
    error = (s1*-2 + s2*-1 + s3*1 + s4*2)/6;

    proporcional = error * kp;

    derivada = error - lasterror;

    lasterror = error;

    pid = proporcional + (kd*derivada);
    
    return pid;
}

void set_motor_1(float pwm){
    int magnitude = (int)fabs(pwm);
    if (pwm >= 0){
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH); 
    }else{
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW); 
    }
    analogWrite(en1,magnitude);
}
void set_motor_2(float pwm){
    int magnitude = (int)fabs(pwm);
    if (pwm >= 0){
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
    }else{
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW); 
    }
    analogWrite(en2,magnitude);
}


void controlarMotores(int v){
    float velcorrigida, velesq, veldir;
    float pid;
    pid = PID();
    velesq = v - pid;
    veldir = v + pid;
    
    set_motor_1(CorrigePWM(velesq,error));
    set_motor_2(CorrigePWM(veldir,error));
    
}

void setup(){
    Serial.begin(115200);
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(en1, OUTPUT);
    pinMode(en2, OUTPUT);
}

void loop(){
    s1 = analogRead(ir1);
    s2 = analogRead(ir2);
    s3 = analogRead(ir3);
    s4 = analogRead(ir4);
    Serial.printf("s1:%d s2:%d s3:%d s4:%d\nerro:%d\npid:*%d*",s1,s2,s3,s4,(s1*-2 + s2*-1 + s3*1 + s4*2)/6),PID();
    controlarMotores(130);

}


