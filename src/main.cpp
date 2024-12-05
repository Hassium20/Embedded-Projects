#include "mbed.h"
#include "C12832.h"

C12832 lcd(D11, D13, D12, D7, D10);
//-----------------------------------------------------------------------
void drawProgressBar(int elapsedSeconds, int totalSeconds){

    // Bar dimensions 
    const int barX = 5;
    const int barY = 35;
    const int barWidth = 118; 
    const int barHeight = 5;

    int filledWidth = (elapsedSeconds * barWidth) / totalSeconds;

    // Draw the progress bar outline 
    lcd.locate(5,35);
    lcd.rect(barX, barY, (barX + barWidth), (barY + barHeight), 0); 

    // Fill the progress bar
    lcd.fillrect(barX, barY, (barX + filledWidth), (barY + barHeight), 1);
}

//-----------------------------------------------------------------------
void drawHourglass() {
    int centerX = 64;   // X-center of the screen
    int centerY = 16;   // Y-center of the screen
    int halfWidth = 20; // Half-width of the hourglass (pixels)
    int halfHeight = 10; // Half-height of the hourglass (pixels)

    // Rectangle dimensions
    int rectWidth = 60;  // Width of the rectangles (longer than hourglass base)
    int rectHeight = 6;  // Height of the rectangles

    // Coordinates for rectangles
    int topRectX1 = centerX - rectWidth / 2;
    int topRectY1 = centerY - halfHeight - rectHeight;
    int topRectX2 = centerX + rectWidth / 2;
    int topRectY2 = centerY - halfHeight;

    int bottomRectX1 = centerX - rectWidth / 2;
    int bottomRectY1 = centerY + halfHeight;
    int bottomRectX2 = centerX + rectWidth / 2;
    int bottomRectY2 = centerY + halfHeight + rectHeight;

    // Draw top rectangle
    lcd.fillrect(topRectX1, topRectY1, topRectX2, topRectY2, 1);

    // Draw bottom rectangle
    lcd.fillrect(bottomRectX1, bottomRectY1, bottomRectX2, bottomRectY2, 1);

    // Draw hourglass
    // Coordinates for top triangle
    int topLeftX = centerX - halfWidth;
    int topLeftY = centerY - halfHeight;
    int topRightX = centerX + halfWidth;
    int topRightY = centerY - halfHeight;

    // Coordinates for bottom triangle
    int bottomLeftX = centerX - halfWidth;
    int bottomLeftY = centerY + halfHeight;
    int bottomRightX = centerX + halfWidth;
    int bottomRightY = centerY + halfHeight;

    // Draw top triangle
    lcd.line(topLeftX, topLeftY, topRightX, topRightY, 1); // Top horizontal
    lcd.line(topLeftX, topLeftY, centerX, centerY, 1);     // Left diagonal
    lcd.line(topRightX, topRightY, centerX, centerY, 1);   // Right diagonal

    // Draw bottom triangle
    lcd.line(bottomLeftX, bottomLeftY, bottomRightX, bottomRightY, 1); // Bottom horizontal
    lcd.line(bottomLeftX, bottomLeftY, centerX, centerY, 1);           // Left diagonal
    lcd.line(bottomRightX, bottomRightY, centerX, centerY, 1);         // Right diagonal

}
//-----------------------------------------------------------------------
void drawHBars(int minutes, int seconds, int prevMinutes, int prevSeconds){
  
    lcd.locate(30,0);
    lcd.printf("Set time duration");

    //bar dimensions
    const int maxBarWidth = 50;
    const int barHeight = 6;  
    const int minutesBarX = 5;
    const int secondsBarX = 70;
    const int barsY = 12;

     // Update Minutes Bar 

        // Clear the previous bar
        lcd.fillrect(minutesBarX, barsY, minutesBarX + maxBarWidth, barsY + barHeight, 0);
        // Draw the updated bar
        int minutesWidth = (minutes * maxBarWidth) / 9;
        lcd.fillrect(minutesBarX, barsY, minutesBarX + minutesWidth, barsY + barHeight, 1);
        // Redraw the outline
        lcd.rect(minutesBarX, barsY, minutesBarX + maxBarWidth, barsY + barHeight, 0);

        // Update the label
        lcd.locate(minutesBarX + 10, barsY + 10);
        lcd.printf("Mins: %d  ", minutes); // Extra spaces to clear old text

         lcd.rect(5, 12, 55, 18, 1); //mintues bar outine (constant)
    
   // Update Seconds Bar 
    
        // Clear the previous bar
        lcd.fillrect(secondsBarX, barsY, secondsBarX + maxBarWidth, barsY + barHeight, 0);
        // Draw the updated bar
        int secondsWidth = (seconds * maxBarWidth) / 59;
        lcd.fillrect(secondsBarX, barsY, secondsBarX + secondsWidth, barsY + barHeight, 1);
        // Redraw the outline
        lcd.rect(secondsBarX, barsY, secondsBarX + maxBarWidth, barsY + barHeight, 0);

        // Update the label
        lcd.locate(secondsBarX + 10, barsY + 10);
        lcd.printf("Sec: %d  ", seconds); // Extra spaces to clear old text
        lcd.rect(70, 12, 120, 18, 1); //seconds bar outline (constant)


 

    
}


float flashTime = 250; //ms
//------------------------------
class Buzzer{////<-------------------------------------------------------------------------------------
//<---------------------------------------------------------------------------------------------------------------
    private:
    PwmOut dutyCycle;
   
    float buzzerFreq; 
    bool soundStatus;

    public:
    Buzzer(PinName buzzerPin, float freq)
    :dutyCycle(buzzerPin), buzzerFreq(freq){
        soundOff();

        
  }
    
    void soundOff(){
        dutyCycle = 0.0; //duty cycle set to 0
        soundStatus = false;
    }

    void soundOn(){
        dutyCycle.period(1/buzzerFreq);
        dutyCycle.write(0.5) ; //duty cycle 0.5 
        soundStatus = true;
    }

    

};

//-------------------------------------------------------------
class LED{
    private:
    DigitalOut outputSignal; 
    bool LEDstatus;
    bool isToggling;
    
    protected:
    Ticker flash;

    public:
    LED(PinName ledPin)
    :outputSignal(ledPin){
        off();
    }

    void on(){
        outputSignal = 0;
        LEDstatus = true;
        isToggling = false;
    }
    void off(){
        outputSignal = 1;
        LEDstatus = false;
          isToggling = false;

    }

    bool isFlashing() const{
        return isToggling;
    }
    
    bool getStatus(){
        return LEDstatus;
    }

    void startToggle(float toggleFreq){
        if(!isToggling){ 
        flash.attach(callback(this, &LED::toggle), 1.0/toggleFreq);
        isToggling = true;
        }
    }

    void toggle(){
     outputSignal = !outputSignal;
     
    }

    void stopToggle(){
        flash.detach();
        off();
    }
    
};

//------------------------------------------------------------------
class Potentiometer{
    private:

    
    protected:
    float current_adcVal, current_voltage;
    AnalogIn inputSignal;

    public:
        Potentiometer(PinName potPin, float volt)
        : inputSignal(potPin), current_voltage(volt){

        }

        virtual float adcValue(){
            return inputSignal.read();
        }
       
        float adcVoltage(void){
            return inputSignal.read()*3300; // Vref = 3300 mV
        }

        float store_adcVal(){
            return current_adcVal;
        }
        float store_voltage(){
            return current_voltage;
        }

};
//----------------------------------------------------------------------
class SamplingPotentiometer
:public Potentiometer {

private:
    float samplingFrequency, samplingPeriod; 
    Ticker sampler; // how often the potentiometer is read

public:
     
    virtual float adcValue(){
        return inputSignal.read();
    }

    SamplingPotentiometer(PinName potPin, float volt, float fsample)
    : Potentiometer(potPin, volt), samplingFrequency(fsample){

        period_calc();
        //samplingFreq will be defined when instantiating the object in main()
        sampler.attach(callback(this, &SamplingPotentiometer::sample), samplingPeriod);
    }

    void period_calc(){
        samplingPeriod = 1.0f/samplingFrequency;
    }
    float periodVal(){
        return samplingPeriod;
    }

    void sample(){
        current_adcVal = adcValue();
    }

    virtual float getSample(){
        return current_adcVal;
    }
};
//---------------------------------------------------
typedef enum{
        Initialisation,
        countdownState, //new countdown state
        timer_running,
        timer_paused,
        timer_reset, 
        timer_elapsed
}ProgramState;

volatile ProgramState state;
//-----------------------------------------------------------------------
class CountdownTimer
{
    private: 
    AnalogIn minutesPot;
    AnalogIn secondsPot;
    Ticker ticker;
    Buzzer buzzerInstance;
    Timeout hourglassCount; 
    
    int totalTimeSec;
    int minutes;  //minute part of timer
    int seconds;  //seconds part of timer
    bool timerPausedFlag;
    int resumeCurrentTime;
    int initialTimeSec;
    bool firstCall;
    
    int countdownValue; //countdown from 3
    
    float countdown_time_min;
    float countdown_time_sec;

    public:
    CountdownTimer(PinName minutesPin, PinName secondsPin, PinName buzzerPin, float freq )
    :minutesPot(minutesPin), secondsPot(secondsPin), totalTimeSec(0), timerPausedFlag(false), buzzerInstance(buzzerPin, freq), firstCall(true){}

     int getInitialTimeSec() const {
        return initialTimeSec;
    }
    void setTimeFromPots(int mins, int secs){ // gives you total time <------------
        totalTimeSec = (mins*60) + secs;
        initialTimeSec = totalTimeSec; //stores value of countdown time for reset
        countdownValue = 3; 
    }
//--------------------------------------- hourglass timing funcitonality
      void countdownHandler() {
        countdownValue = 3; // Reset countdown to 3
        decrementHourglass();
    }

    void decrementHourglass() {
        if (countdownValue > 0) {
            // Display the countdown number
            lcd.rect(5, 5, 123, 27, 1);
            lcd.locate(15, 15);
            lcd.printf("Timer starting in : %d", countdownValue);
            countdownValue--;
            // Call timeout again after 1 second 
            hourglassCount.attach(callback(this, &CountdownTimer::decrementHourglass), 1.0f);

        } else {
             lcd.cls();
            // Countdown complete, start the timer
            countdownValue = 3;
             startTimer(); 
            state = timer_running;
        }
    }
        //------------------------------------------------------
    void startTimer(){    
        timerPausedFlag = false;
        updateTime();
        ticker.detach();
        ticker.attach(callback(this, &CountdownTimer::updateTime), 1.0f);
    }
    void pauseTimer(){
        if (!timerPausedFlag){
            timerPausedFlag = true;
             ticker.detach();
        }
    }
    void resumeTimer(){
            lcd.cls();
            timerPausedFlag = false;
            ticker.attach(callback(this, &CountdownTimer::updateTime), 1.0f);  
    }
    void updateTime(){
        if(!timerPausedFlag && totalTimeSec > 0){
            totalTimeSec--;
        
         if (totalTimeSec == 0){
               
                buzzerInstance.soundOn(); //activate buzzer when timeout reaches 0
                Timeout buzzerOffTimeout;  
                buzzerOffTimeout.attach(callback(&buzzerInstance, &Buzzer::soundOff), 2);//<<----BUZZER TIMEOUT
                ticker.detach();
                state = timer_elapsed;
            }
        }
     }

    void resetAndStart(){
        totalTimeSec = initialTimeSec;
       timerPausedFlag = false;

       ticker.attach(callback(this, &CountdownTimer::updateTime), 1.0f);
      
    }

    void timeout_done(){
        state = timer_elapsed;
    }

    int getMinutes() const {      
        return totalTimeSec / 60;
    }
    int getSeconds() const{
        return totalTimeSec % 60;
    }
    
};
//------------------------------

bool timerRunningFlag = false;
bool timerPausedFlag = false;

//initialize / construct
CountdownTimer timer(A0, A1, D6, 1000); 
SamplingPotentiometer leftPot(A0, 3300, 10);  
SamplingPotentiometer rightPot(A1, 3300, 10);

void fireISR(){
    
    static int minutes = 0;
    static int seconds = 0;


    switch (state){
        case Initialisation:
        if (minutes>9){minutes = 9;}
        if (seconds>59){seconds = 59;}
            
            timer.setTimeFromPots(minutes, seconds); 

            state = countdownState;
            timer.countdownHandler();
            break;

        case timer_running:
          
            state = timer_paused;
            timer.pauseTimer();
            timerPausedFlag = true;
            timerRunningFlag = false;
            break;

        case timer_paused:
         
            state = timer_running;
            timer.resumeTimer();

            break;
        
        case timer_elapsed:

            timerPausedFlag = false;
            timerRunningFlag = false;
            state = Initialisation;
            break;

        default:
            break;
    }
}
void upISR(){
  
    switch (state){
    case timer_paused:
        state = timer_reset;
            timer.resetAndStart();
    break;
        default:
            break;

    }
}

void downISR(){

    switch (state){

        case timer_paused:
            state = Initialisation;
            

            break;

            default:
                break;
    }
}
//----------------------------------------------------------------------
int main() {
  InterruptIn fireJoystick(D4);
  InterruptIn upJoystick(A2);
  InterruptIn downJoystick(A3);

  LED redLED(D5);
  LED greenLED(D9);
  LED blueLED(D8);

  Buzzer buzzer(D6, 10);

  redLED.off();
  greenLED.off();
  blueLED.off();

  state = Initialisation;

  fireJoystick.rise(&fireISR);
  upJoystick.rise(&upISR);
  downJoystick.rise(&downISR);

bool timerRunningFlag;
bool timerElapsedPrinted;
bool timerPausedFlag;
bool countdownFlag;

int totalSeconds;
int elapsedSeconds;
int prevMinutes = -1;
int prevSeconds = -1;
ProgramState prevState = ProgramState(-1);


  while(true){

    
      int minutes = 0;
      int seconds = 0;
    if (state == Initialisation){

        minutes = floor(leftPot.getSample() * 10);
        seconds = floor(rightPot.getSample() * 60);

        if(minutes >9){
            minutes = 9;
        }
        if(seconds > 59){
            seconds = 59;
        }

    }
//------------------------------
      switch (state){
        case Initialisation:
            countdownFlag = false;
            if (prevState != state){
                lcd.cls();
                prevState = state;

            }
             
drawHBars(minutes, seconds, prevMinutes, prevSeconds);
                prevMinutes = minutes;
                prevSeconds = seconds;

        timerPausedFlag = false;
        timerElapsedPrinted = false;
        timerRunningFlag = false;
        countdownFlag = false;

         timer.setTimeFromPots(minutes, seconds);
         

               redLED.on();
               greenLED.off();
               blueLED.off();

               buzzer.soundOff();
           
           
           
           
            break;


//------------------------------

        case countdownState: 
      
         redLED.off();

         if(!countdownState){
             countdownFlag = true;
             lcd.locate(50, 25); // Position cursor near the hourglass
            lcd.printf("Countdown");
         }

         if (prevState != state) {
             lcd.cls();
        prevState = state; // Update previous state to avoid redundant clearing
        countdownFlag = false;
    }
       
        break;
        

    //-------------------------------------------------------
        case timer_running:

        timerPausedFlag = false;
        countdownFlag = false;
        timerElapsedPrinted = false;

           if(timerRunningFlag == false){
        lcd.cls();
        timerRunningFlag = true;
        drawProgressBar(elapsedSeconds, totalSeconds);
    }
        
         if (prevState != state) {
                 //lcd.cls();
                 prevState = state; // Update previous state to avoid redundant clearing
                }

    redLED.off();
    blueLED.off();

    // Start green LED flashing if not already toggling
    if (!greenLED.isFlashing()) {
        greenLED.startToggle(1); // Start toggling at 1 Hz
    }

    // Update minutes and seconds from the timer
    minutes = timer.getMinutes();
    seconds = timer.getSeconds();

    // Display the timer running countdown
    
     elapsedSeconds = timer.getInitialTimeSec() - (timer.getMinutes() * 60 - timer.getSeconds());
     totalSeconds = timer.getInitialTimeSec();


    drawProgressBar(totalSeconds - elapsedSeconds, totalSeconds);


    lcd.locate(5, 0);
    lcd.printf("Timer running\n%02d : %02d  ", minutes, seconds); // Clear remnants with padding

    break;
//------------------------------
        case timer_paused: 
                
                if (prevState != state) {
               
                 prevState = state; // Update previous state to avoid redundant clearing
                }

                greenLED.stopToggle();
                redLED.on();
                greenLED.on();

                blueLED.off();

                if(timerPausedFlag == false){
                    lcd.cls();
                    timerPausedFlag = true;
                     lcd.rect(0,9, 127, 25, 1);
                lcd.locate(1,0);
                lcd.printf("Paused at: %02d : %02d", timer.getMinutes(), timer.getSeconds());
                lcd.locate(1,15);
                lcd.printf("U(rst) | D(quit) | F(resume)");
                }
                break;
//------------------------------
        case timer_reset:

        if (prevState != state) {
        lcd.cls();
        prevState = state; // Update previous state to avoid redundant clearing
        }

            greenLED.stopToggle();
            redLED.off();
            greenLED.off();
            blueLED.off();

            timer.resetAndStart();
            state = timer_running;
            break;

//------------------------------

        case timer_elapsed:

        if (prevState != state) {
            
            prevState = state; // Update previous state to avoid redundant clearing
        }     
                greenLED.stopToggle();
                redLED.off();
                greenLED.off();
                blueLED.on();

                if(!timerElapsedPrinted){
                lcd.cls();
                lcd.rect(5,5,123,27,1);
                lcd.locate(15,15);
                lcd.printf("Timer Elapsed");
                 //wait_ms(10);
                timerElapsedPrinted = true;
               
                }
                
            break;
//------------------------------
        default:
            state = Initialisation;
            break;
      }
      
  }
        
}
