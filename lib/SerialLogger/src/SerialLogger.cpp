#include <SerialLogger.hpp>


void SerialLogger::serialHandler(int argCnt, char **args){
  if(p_serial_logger != nullptr){
    if(strcmp(args[1], "g") == 0){
        Serial.print(p_serial_logger->get_debug_level());
    }else if (strcmp(args[1], "d") == 0){
        p_serial_logger->set_debug_level(Debugger::DEBUG);
    }else if (strcmp(args[1], "n") == 0){
        p_serial_logger->set_debug_level(Debugger::NONE);
    }else if (strcmp(args[1], "e") == 0){
        p_serial_logger->set_debug_level(Debugger::ERROR);
    }else if (strcmp(args[1], "w") == 0){
        p_serial_logger->set_debug_level(Debugger::WARNING);
    }else if (strcmp(args[1], "i") == 0){
        p_serial_logger->set_debug_level(Debugger::INFO);
    }
  }
}

void SerialLogger::log(String message, Debugger::DebugLevels level){
    if(debug_level >= Debugger::NONE){
        switch (level)
        {
        case Debugger::DEBUG:
            Serial.print(F("[DEBUG] "));
            Serial.println(message);
            break;
        case Debugger::ERROR:
            Serial.print(F("[Error] "));
            Serial.println(message);
            break;
        case Debugger::WARNING:
            Serial.print(F("[Warning] "));
            Serial.println(message);
            break;
        case Debugger::INFO:
            Serial.print(F("[INFO] "));
            Serial.println(message);
            break;
        
        default:
            break;
        }
    }
}

String SerialLogger::get_debug_level(){
    return Debugger::state_to_string(debug_level);
}

namespace Debugger {
    String state_to_string(DebugLevels state){
        switch (state) {
        case NONE:
            return F("NONE");
            break;
        case ERROR:
            return F("ERROR");
            break;
        case WARNING:
            return F("WARNING");
            break;
        case INFO:
            return F("INFO");
            break;
        case DEBUG:
            return F("DEBUG");
            break;
        }
        return F("UNKNOWN");
    }
}