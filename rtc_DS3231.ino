/*
 * DS3231 real time clock
 *   internal alarm clock and temp sensor
 * 
 * 
 * use library:
 * https://github.com/adafruit/RTClib
 *
 * needs CR2032 battery
 * https://www.youtube.com/watch?v=9s61qyA29fg
 * 
 * module: DS3231
 * https://create.arduino.cc/projecthub/electropeak/interfacing-ds1307-rtc-module-with-arduino-make-a-reminder-08cb61
 * 32K - 32KHz oscillator 
 * SQW - internal oscillator (configurable)
 * SCL - i2c pins (on wemos: SDA => D2, SCL => D1)
 * SDA - ic2 pins
 * VCC - 3.3 or 5 VDC
 * GND - ground
 * 
 * other side is just a i2c passthru
 * a0, a1, a2 set s the i2c address
 * 
 * module: DS1307
 * SQ - Square wave output (1,4,8,32 KHz)
 * DS - Temperature sensor...1-wire protocol
 * SCL - i2c pins (on wemos: SDA => D2, SCL => D1)
 * SDA - i2c pins
 * VCC - 5VDC  (3.3?)
 * GND - ground
 * BAT - Battery voltage - ADC
 * 
 * other side is just a i2c passthru
 * 
 */

 
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

// pick the right module
//#define DS3231
#define DS1307


#ifdef DS3231
RTC_DS3231 rtc;
#endif
#ifdef DS1307
RTC_DS1307 rtc;
#endif


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  // start serial port
  Serial.begin(115200);
#ifdef DS3231
  Serial.print(F("\n\n DS3231 rtc test\n\n"));
#endif
#ifdef DS1307
  Serial.print(F("\n\n DS1307 rtc test\n\n"));
#endif
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

#ifdef DS3231
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
#endif

#ifdef DS1307
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // set initial time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
#endif

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days and 30 seconds into the future
//    DateTime future (now + TimeSpan(7,12,30,6));
//
//    Serial.print(" now + 7d + 30s: ");
//    Serial.print(future.year(), DEC);
//    Serial.print('/');
//    Serial.print(future.month(), DEC);
//    Serial.print('/');
//    Serial.print(future.day(), DEC);
//    Serial.print(' ');
//    Serial.print(future.hour(), DEC);
//    Serial.print(':');
//    Serial.print(future.minute(), DEC);
//    Serial.print(':');
//    Serial.print(future.second(), DEC);
//    Serial.println();

#ifdef DS3231
    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");
#endif

    Serial.println();
    delay(3000);
}
