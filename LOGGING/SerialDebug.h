#ifndef _SERIAL_DEBUG_H_
#define _SERIAL_DEBUG_H_

#ifndef SERIAL_DEBUG

#define log_print(msg)
#define log_println(msg)

#else

#pragma message "SERIAL_DEBUG is active"

#define log_print(msg) Serial.print(msg)
#define log_println(msg) Serial.println(msg)

#endif

#endif
