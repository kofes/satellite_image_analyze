#include "../inc/Passport.hpp"

satellite::passport::Default::Default ( const std::string& fileName ) {
  std::ifstream file(fileName);
  if (!file.is_open()) return;

  file.read(reinterpret_cast<char *>(&FFh1_FORMAT), 1);
  file.read(SAT2_NAME, 13); SAT2_NAME[13] = '\0';
  file.read(reinterpret_cast<char *>(&SAT_ID), sizeof(SAT_ID));
  file.read(reinterpret_cast<char *>(&NODE_NUM), sizeof(NODE_NUM));
  file.read(reinterpret_cast<char *>(&DATE_YEAR), sizeof(DATE_YEAR));
  file.read(reinterpret_cast<char *>(&DATE_DAY), sizeof(DATE_DAY));
  file.read(reinterpret_cast<char *>(&DATE_TIME), sizeof(DATE_TIME));
  file.read(reinterpret_cast<char *>(RESERVE_1), 8);
  file.read(reinterpret_cast<char *>(SERVICE_1), 22);
  file.read(reinterpret_cast<char *>(RESERVE_2), 2);
  file.read(reinterpret_cast<char *>(DATA_TYPE), 2);

  file.close();
}

satellite::passport::Default::Default ( const satellite::passport::Default& src ) {
  FFh1_FORMAT = src.FFh1_FORMAT;
  for (int i = 0; i < 14; ++i)
    SAT2_NAME[i] = src.SAT2_NAME[i];
  SAT_ID = src.SAT_ID;
  NODE_NUM = src.NODE_NUM;
  DATE_YEAR = src.DATE_YEAR;
  DATE_DAY = src.DATE_DAY;
  DATE_TIME = src.DATE_TIME;
  for (int i = 0; i < 8; ++i)
    RESERVE_1[i] = src.RESERVE_1[i];
  for (int i = 0; i < 22; ++i)
    SERVICE_1[i] = src.SERVICE_1[i];
  for (int i = 0; i < 2; ++i)
    RESERVE_2[i] = src.RESERVE_2[i];
  for (int i = 0; i < 2; ++i)
    DATA_TYPE[i] = src.DATA_TYPE[i];
}

const Date satellite::passport::Default::getDate () const {
  Date result;
  unsigned short MON, DAY, addDay = ((DATE_YEAR % 4 == 0) ? 1 : 0);
  //Jan
  if      (DATE_DAY <=  31)          { MON =  1; DAY = DATE_DAY; }
  //Feb
  else if (DATE_DAY <=  59 + addDay) { MON =  2; DAY = DATE_DAY -  31 - addDay; }
  //Mar
  else if (DATE_DAY <=  90 + addDay) { MON =  3; DAY = DATE_DAY -  59 - addDay; }
  //Apr
  else if (DATE_DAY <= 120 + addDay) { MON =  4; DAY = DATE_DAY -  90 - addDay; }
  //May
  else if (DATE_DAY <= 151 + addDay) { MON =  5; DAY = DATE_DAY - 120 - addDay; }
  //Jun
  else if (DATE_DAY <= 181 + addDay) { MON =  6; DAY = DATE_DAY - 151 - addDay; }
  //Jul
  else if (DATE_DAY <= 212 + addDay) { MON =  7; DAY = DATE_DAY - 181 - addDay; }
  //Aug
  else if (DATE_DAY <= 243 + addDay) { MON =  8; DAY = DATE_DAY - 212 - addDay; }
  //Sep
  else if (DATE_DAY <= 273 + addDay) { MON =  9; DAY = DATE_DAY - 243 - addDay; }
  //Oct
  else if (DATE_DAY <= 304 + addDay) { MON = 10; DAY = DATE_DAY - 273 - addDay; }
  //Nov
  else if (DATE_DAY <= 334 + addDay) { MON = 11; DAY = DATE_DAY - 304 - addDay; }
  //Dec
  else { MON = 12; DAY = DATE_DAY - 334 - addDay; }

  return Date(DATE_YEAR, MON, DAY);
}

const Time satellite::passport::Default::getTime () const {

  short hours, minutes, seconds;
  unsigned int milliseconds;

  milliseconds = DATE_TIME;
  hours = milliseconds / (60 * 60 * 1000);
  milliseconds %= (60 * 60 * 1000);
  minutes = milliseconds / (60 * 1000);
  milliseconds %= (60 * 1000);
  seconds = milliseconds / (1000);
  milliseconds %= (1000);

  return Time(hours, minutes, seconds, (short)milliseconds);
}

const std::string satellite::passport::Default::getDataType () const {
  switch (DATA_TYPE[0]) {
    case 0:
      if (DATA_TYPE[1] == 1)
        return "Исходные данные: HRPT NOAA";
      return "Исходные данные: GMS S-VISSR";
    case 2:
      if (DATA_TYPE[1] == 1)
        return "Одноканальные данные: NOAA AVHRR";
      return "Одноканальные данные: GMS S-VISSR";
    case 3:
      if (DATA_TYPE[1] == 1)
        return "Проекция: NOAA AVHRR";
      return "Проекция: GMS S-VISSR";
    case 4:
      if (DATA_TYPE[1] == 1)
        return "Данные телеметрии: NOAA AVHRR";
      return "Данные телеметрии: GMS S-VISSR (данные из DOC-сектора)";
  }
  return "Ошибка";
}

satellite::passport::Proection::Proection ( const std::string& fileName ) {
  std::ifstream file(fileName);
  if (!file.is_open()) return;

  file.read(reinterpret_cast<char *>(&FFh1_FORMAT), 1);
  file.read(SAT2_NAME, 13); SAT2_NAME[13] = '\0';
  file.read(reinterpret_cast<char *>(&SAT_ID), sizeof(SAT_ID));
  file.read(reinterpret_cast<char *>(&NODE_NUM), sizeof(NODE_NUM));
  file.read(reinterpret_cast<char *>(&DATE_YEAR), sizeof(DATE_YEAR));
  file.read(reinterpret_cast<char *>(&DATE_DAY), sizeof(DATE_DAY));
  file.read(reinterpret_cast<char *>(&DATE_TIME), sizeof(DATE_TIME));
  file.read(reinterpret_cast<char *>(RESERVE_1), 8);
  file.read(reinterpret_cast<char *>(SERVICE_1), 22);
  file.read(reinterpret_cast<char *>(RESERVE_2), 2);
  file.read(reinterpret_cast<char *>(DATA_TYPE), 2);

  file.read(reinterpret_cast<char *>(&STAGE_DATA), sizeof(STAGE_DATA));
  file.read(reinterpret_cast<char *>(&CHANNEL_NUM), sizeof(CHANNEL_NUM));
  file.read(reinterpret_cast<char *>(&SIGNIFICANT_PIXELS_MAX_VALUE), sizeof(SIGNIFICANT_PIXELS_MAX_VALUE));
  file.read(reinterpret_cast<char *>(&PROECTION_TYPE), sizeof(PROECTION_TYPE));
  file.read(reinterpret_cast<char *>(&STRINGS_COUNT), sizeof(STRINGS_COUNT));
  file.read(reinterpret_cast<char *>(&COUNT_PIXELS), sizeof(COUNT_PIXELS));
  file.read(reinterpret_cast<char *>(&LATITUDE), sizeof(LATITUDE));
  file.read(reinterpret_cast<char *>(&LONGITUDE), sizeof(LONGITUDE));
  file.read(reinterpret_cast<char *>(&SIZE_LATITUDE), sizeof(SIZE_LATITUDE));
  file.read(reinterpret_cast<char *>(&SIZE_LONGITUDE), sizeof(SIZE_LONGITUDE));
  file.read(reinterpret_cast<char *>(&STEP_LATITUDE), sizeof(STEP_LATITUDE));
  file.read(reinterpret_cast<char *>(&STEP_LONGITUDE), sizeof(STEP_LONGITUDE));
  file.read(reinterpret_cast<char *>(&COEFFICIENT_A), sizeof(COEFFICIENT_A));
  file.read(reinterpret_cast<char *>(&COEFFICIENT_B), sizeof(COEFFICIENT_B));
  file.read(reinterpret_cast<char *>(RESERVE_3), 10);
  file.read(reinterpret_cast<char *>(&NODE_NUM), sizeof(NODE_NUM));
  file.read(reinterpret_cast<char *>(&ELEMENTS_NUM), sizeof(ELEMENTS_NUM));
  file.read(reinterpret_cast<char *>(&EFERMIDS_TYPE), sizeof(EFERMIDS_TYPE));
  file.read(reinterpret_cast<char *>(&NODE_YEAR), sizeof(NODE_YEAR));
  file.read(reinterpret_cast<char *>(&DAY_NUM), sizeof(DAY_NUM));
  file.read(reinterpret_cast<char *>(&AVERAGE_MOV), sizeof(AVERAGE_MOV));
  file.read(reinterpret_cast<char *>(&AIR_RESISTANCE), sizeof(AIR_RESISTANCE));
  file.read(reinterpret_cast<char *>(&OBLIQUITY), sizeof(OBLIQUITY));
  file.read(reinterpret_cast<char *>(&RIGHT_ASCENSION_ASCENDING_NODE), sizeof(RIGHT_ASCENSION_ASCENDING_NODE));
  file.read(reinterpret_cast<char *>(&ECCENTRICITY), sizeof(ECCENTRICITY));
  file.read(reinterpret_cast<char *>(&PERIGEE_ARGUMENT), sizeof(PERIGEE_ARGUMENT));
  file.read(reinterpret_cast<char *>(&MEAN_ANOMALY), sizeof(MEAN_ANOMALY));
  file.read(reinterpret_cast<char *>(RESERVE_4), 54);
  file.read(reinterpret_cast<char *>(&CORRECTION_VERSION_NUM), sizeof(CORRECTION_TIME));
  file.read(reinterpret_cast<char *>(&MANDAREL_BOARD_CLOCK), sizeof(MANDAREL_BOARD_CLOCK));
  file.read(reinterpret_cast<char *>(&CORRECTION_TIME), sizeof(CORRECTION_TIME));
  file.read(reinterpret_cast<char *>(&ROLL), sizeof(ROLL));
  file.read(reinterpret_cast<char *>(&PITCH), sizeof(PITCH));
  file.read(reinterpret_cast<char *>(&YAW), sizeof(YAW));
  file.read(reinterpret_cast<char *>(RESERVE_5), 226);

  file.close();
}

satellite::passport::Proection::Proection ( const satellite::passport::Proection& src ) {
  STAGE_DATA = src.STAGE_DATA;
  CHANNEL_NUM = src.CHANNEL_NUM;
  SIGNIFICANT_PIXELS_MAX_VALUE = src.SIGNIFICANT_PIXELS_MAX_VALUE;
  PROECTION_TYPE = src.PROECTION_TYPE;
  STRINGS_COUNT = src.STRINGS_COUNT;
  COUNT_PIXELS = src.COUNT_PIXELS;
  LATITUDE = src.LATITUDE;
  LONGITUDE = src.LONGITUDE;
  SIZE_LATITUDE = src.SIZE_LATITUDE;
  SIZE_LONGITUDE = src.SIZE_LONGITUDE;
  STEP_LATITUDE = src.STEP_LATITUDE;
  STEP_LONGITUDE = src.STEP_LONGITUDE;
  COEFFICIENT_A = src.COEFFICIENT_A;
  COEFFICIENT_B = src.COEFFICIENT_B;
  for (int i = 0; i < 10; ++i)
    RESERVE_3[i] = src.RESERVE_3[i];
  NODE_NUM = src.NODE_NUM;
  ELEMENTS_NUM = src.ELEMENTS_NUM;
  EFERMIDS_TYPE = src.EFERMIDS_TYPE;
  NODE_YEAR = src.NODE_YEAR;
  DAY_NUM = src.DAY_NUM;
  AVERAGE_MOV = src.AVERAGE_MOV;
  AIR_RESISTANCE = src.AIR_RESISTANCE;
  OBLIQUITY = src.OBLIQUITY;
  RIGHT_ASCENSION_ASCENDING_NODE = src.RIGHT_ASCENSION_ASCENDING_NODE;
  ECCENTRICITY = src.ECCENTRICITY;
  PERIGEE_ARGUMENT = src.PERIGEE_ARGUMENT;
  MEAN_ANOMALY = src.MEAN_ANOMALY;
  for (int i = 0; i < 54; ++i)
    RESERVE_4[i] = src.RESERVE_4[i];
  CORRECTION_VERSION_NUM = src.CORRECTION_VERSION_NUM;
  MANDAREL_BOARD_CLOCK = src.MANDAREL_BOARD_CLOCK;
  CORRECTION_TIME = src.CORRECTION_TIME;
  ROLL = src.ROLL;
  PITCH = src.PITCH;
  YAW = src.YAW;
  for (int i = 0; i < 226; ++i)
    RESERVE_5[i] = src.RESERVE_5[i];
}

const std::string satellite::passport::Proection::getStageData () const {
  std::string result = "колибровка: ";

  if (STAGE_DATA & 0x01)
    result += "проведена\n";
  else
    result += "не проведена\n";

  result += "атмоферная коррекция: ";

  if (STAGE_DATA & 0x02)
    result += "проведена\n";
  else
    result += "не проведена\n";

  result += "отсчение суши: ";

  if (STAGE_DATA & 0x10)
    result += "проведено\n";
  else
    result += "не проведено\n";

  result += "отсчение моря: ";

  if (STAGE_DATA & 0x11)
    result += "проведено\n";
  else
    result += "не проведено\n";

  return result;
}
