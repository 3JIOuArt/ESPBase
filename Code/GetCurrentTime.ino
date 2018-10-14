void Time_init(){
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void GetTimeString(){
  time_t now = time(nullptr);
  ptm = gmtime ( &now );
  TimeString += (ptm->tm_hour)%24;
  TimeString += ":";
  if (char(ptm->tm_min) < 10){
  TimeString +="0";
  TimeString += ptm->tm_min;
  }
  else{
  TimeString += ptm->tm_min;
  }
  TimeString += ":";
  if (char(ptm->tm_sec) < 10){
  TimeString +="0";
  TimeString += ptm->tm_sec;
  }
  else{
  TimeString += ptm->tm_sec;
  }
  TimeString += ", ";
}

void GetDateChStr(void){
  time_t now = time(nullptr);
  ptm = gmtime ( &now );
  
  snprintf(Filename, sizeof(Filename), "/%02d-%02d-%04d.txt", ptm->tm_mday, ptm->tm_mon+ 1, ptm->tm_year+ 1900);
  
  DateString += ptm->tm_mday;
  DateString +=  "_";
  DateString += ptm->tm_mon+ 1;
  DateString +=  "_";
  DateString += ptm->tm_year+ 1900;
}
