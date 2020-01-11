//======================================================================
void ReadDataIdle(void) { }
//======================================================================

//======================================================================
void ReadStatus(void) {
  EnergyMeter_OnOff_Output(3,1);                        //  Led 1 (Red) shield Energy Meter ON
  Cmd_39F511.GenericFlags.ReadDataInProgress = 1;
  Cmd_39F511.MCP39F511_ReadStatusAndVersion();          //  Reads system status and MCP39F511 FW version
  ReadDataMCP39F511 = ReadEvent;                        //  Next Step
}
//======================================================================

//======================================================================
void ReadEvent(void) {
  Cmd_39F511.MCP39F511_ReadEventStatus();               //  Reads event status
  ReadDataMCP39F511 = ReadVoltage;                      //  Next Step
}
//======================================================================

//======================================================================
void ReadVoltage(void) {
  Cmd_39F511.MCP39F511_ReadRmsVoltageRaw();             //  Reads RMS Voltage
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateRmsVoltageRawArray();
#endif  
  ReadDataMCP39F511 = ReadCurrent;                      //  Next Step
}
//======================================================================

//======================================================================
void ReadCurrent(void) {
  Cmd_39F511.MCP39F511_ReadRmsCurrentRaw();             //  Reads RMS Current
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateRmsCurrentRawArray();
#endif  
  ReadDataMCP39F511 = ReadFrequency;                    //  Next Step
}
//======================================================================

//======================================================================
void ReadFrequency(void) {
  Cmd_39F511.MCP39F511_ReadLineFrequencyRaw();          //  Reads Line Frequency
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateLineFrequencyRawArray();
#endif  
  ReadDataMCP39F511 = ReadPowerFactor;                  //  Next Step
}
//======================================================================
    
//======================================================================
void ReadPowerFactor(void) {
  Cmd_39F511.MCP39F511_ReadPowerFactorRaw();            //  Reads Power Factor
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdatePowerFactorRawArray();
#endif  
  ReadDataMCP39F511 = ReadApparentPower;                //  Next Step
}
//======================================================================
    
//======================================================================
void ReadApparentPower(void) {
  Cmd_39F511.MCP39F511_ReadApparentPowerRaw();          //  Reads Apparent Power
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateApparentPowerRawArray();   //  Calc average Apparent Power
#endif  
  ReadDataMCP39F511 = ReadActivePower;                  //  Next Step
}
//======================================================================
    
//======================================================================
void ReadActivePower(void) {
  Cmd_39F511.MCP39F511_ReadActivePowerRaw();            //  Reads Reactive Power
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateActivePowerRawArray();     //  Calc average Active Power
#endif  
  ReadDataMCP39F511 = ReadReactivePower;                //  Next Step
}
//======================================================================
    
//======================================================================
void ReadReactivePower(void) {
  Cmd_39F511.MCP39F511_ReadReactivePowerRaw();          //  Reads Reactive Power
#ifdef ENABLE_AVERAGE_CALC   
  Cmd_39F511.MCP39F511_UpdateReactivePowerRawArray();   //  Calc average Reactive Power
#endif  
  ReadDataMCP39F511 = ReadDataIdle;                     //  Idle Step
  Cmd_39F511.GenericFlags.ReadDataInProgress = 0;
  EnergyMeter_OnOff_Output(3,0);                        //  Led 1 (Red) shield Energy Meter OFF
}
//======================================================================

//======================================================================
void PrintDataOnSerialMonitor(void) {
  //---------------------------------------------
  EnergyMeter_OnOff_Output(6,1);        //  Led 4 (Verde) shield Energy Meter ON
  if (Cmd_39F511.GenericFlags.ZCD_Error == 0) {
    LocalReadString_FLASH((uint8_t *)STR_ZCD, strlen(STR_ZCD), FALSE, FALSE);  
  } else {
    LocalReadString_FLASH((uint8_t *)STR_ZCD_ERROR, strlen(STR_ZCD_ERROR), FALSE, FALSE);  
  }
  //---------------------------------------------
  
  //---------------------------------------------
  //  MCP39F511 Revision
  LocalReadString_FLASH((uint8_t *)STR_REV, strlen(STR_REV), FALSE, FALSE);
  FormatDec(Cmd_39F511.SystemVersion.Day);
  Serial.print(":");
  FormatDec(Cmd_39F511.SystemVersion.Month);
  Serial.print(":20");
  FormatDec(Cmd_39F511.SystemVersion.Year);
  Serial.println();
  //---------------------------------------------
  
  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_VOLTAGE, strlen(STR_VOLTAGE), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.VoltageRmsRaw.uWord/(pow(10, VOLTAGE_DEC))));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC 
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_VOLTAGE, strlen(STR_AVERAGE_VOLTAGE), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.MCP39F511_RmsVoltageRawAverage())/(pow(10, VOLTAGE_DEC)));
#endif  
  //---------------------------------------------

  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_CURRENT, strlen(STR_CURRENT), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.CurrentRmsRaw.uDWord/(pow(10, CURRENT_DEC))));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC 
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_CURRENT, strlen(STR_AVERAGE_CURRENT), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.MCP39F511_RmsCurrentRawAverage())/(pow(10, CURRENT_DEC)));
#endif  
  //---------------------------------------------
  
  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_FREQUENCY, strlen(STR_FREQUENCY), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.LineFrequencyRaw.uWord/(pow(10, FREQUENCY_DEC))));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC   
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_FREQUENCY, strlen(STR_AVERAGE_FREQUENCY), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.MCP39F511_LineFrequencyRawAverage())/(pow(10, FREQUENCY_DEC)));
#endif  
  //---------------------------------------------
  
  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_POWER_FACTOR, strlen(STR_POWER_FACTOR), FALSE, FALSE);
  Cmd_39F511.CalcPowerFactor();
  if (Cmd_39F511.GenericFlags.PowerFactorSign == 1) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.CalcPowerFactor())/(pow(10, POWER_FACTOR_DEC)));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC   
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_POWER_FACTOR, strlen(STR_AVERAGE_POWER_FACTOR), FALSE, FALSE);
  Cmd_39F511.CalcAveragePowerFactor();
  if (Cmd_39F511.GenericFlags.PowerFactorAverageSign == 1) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.CalcAveragePowerFactor())/(pow(10, POWER_FACTOR_DEC)));
#endif  
  //---------------------------------------------
  
  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_APPARENT_POWER, strlen(STR_APPARENT_POWER), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.ApparentPowerRaw.uDWord)/(pow(10, APPARENT_POWER_DEC)));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC   
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_APPARENT_POWER, strlen(STR_AVERAGE_APPARENT_POWER), FALSE, FALSE);
  Serial.println((float)(Cmd_39F511.MCP39F511_ApparentPowerRawAverage())/(pow(10, APPARENT_POWER_DEC)));
#endif  
  //---------------------------------------------

  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_ACTIVE_POWER, strlen(STR_ACTIVE_POWER), FALSE, FALSE);
  if (Cmd_39F511.SystemStatus.Sign_PA == 0) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.ActivePowerRaw.uDWord)/(pow(10, ACTIVE_POWER_DEC)));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC   
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_ACTIVE_POWER, strlen(STR_AVERAGE_ACTIVE_POWER), FALSE, FALSE);
  if (Cmd_39F511.SystemStatus.Sign_PA == 0) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.MCP39F511_ActivePowerRawAverage())/(pow(10, ACTIVE_POWER_DEC)));
#endif  
  //---------------------------------------------
  
  //---------------------------------------------
  LocalReadString_FLASH((uint8_t *)STR_REACTIVE_POWER, strlen(STR_REACTIVE_POWER), FALSE, FALSE);
  if (Cmd_39F511.SystemStatus.Sign_PR == 0) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.ReactivePowerRaw.uDWord)/(pow(10, REACTIVE_POWER_DEC)));
  //---------------------------------------------

  //---------------------------------------------
#ifdef ENABLE_AVERAGE_CALC   
  LocalReadString_FLASH((uint8_t *)STR_AVERAGE_REACTIVE_POWER, strlen(STR_AVERAGE_REACTIVE_POWER), FALSE, FALSE);
  if (Cmd_39F511.SystemStatus.Sign_PR == 0) {
    Serial.print("-");
  }
  Serial.println((float)(Cmd_39F511.MCP39F511_ReactivePowerRawAverage())/(pow(10, REACTIVE_POWER_DEC)));
#endif  
  //---------------------------------------------

  EnergyMeter_OnOff_Output(6,0);        //  Led 4 (Verde) shield Energy Meter OFF
}
//======================================================================

//======================================================================
void FormatDec(uint8_t Data) {
    char Temp[4];
    
    sprintf(Temp, "%02u", Data);
    Serial.print(Temp);
}
//======================================================================

//======================================================================
String LocalReadString_FLASH(uint8_t *FlashPointer, uint8_t Lenght, boolean PrintCR, boolean NoPrint) {
  uint8_t k;
  char    myChar;
  String  TempString;
  
  for (k = 0; k < Lenght; k++) {
    myChar = pgm_read_byte_near(FlashPointer + k);
    if (NoPrint == FALSE) { 
      Serial.print(myChar);
    }
    TempString += myChar;
  }
  if (NoPrint == FALSE) { 
    if (PrintCR == TRUE) {
      Serial.print("\n");
    }
  }
  return(TempString); 
}
//======================================================================
