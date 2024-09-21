#ifndef PRAKTIKUM_3_MYDATEFUNCTIONS_H
#define PRAKTIKUM_3_MYDATEFUNCTIONS_H

void printCalendar(int year);
void printMonth(int month, int year);
int isLeap(int year);
int getDaysInMonth(int month, int year);
void printDate_switch(int day, int month, int year, int language);
void printDate_ifelse(int day, int month, int year, int language);
void makros(int day, int month, int year);
int isPalindrom(int day, int month, int year);
void palindromTest();
int getWeekday(int day, int month, int year);
void printWeekday(int weekday);
void weekdayTest();

void Terminserien();
void terminErstellen(long long termineArray[]);
void terminAnzeigen(long long termineArray[]);
int getUserInput();
/**
 * Prints the current time in the format: "Mon 06.12.2023 12:00 CET"
 */
void printCurrentTime();
/**
 * Gets a date from the user and returns it as a struct tm
 * @return struct tm The date the user entered
 */
struct tm getDateFromUser();
/**
 * Compares two dates and prints the difference in seconds
 */
void compareDates();
long long toUnixtime(int date[]);
void testZeitraum();
int isInZeitraum(int date[], int start[], int end[]);
int compare(int firstDate[], int secondDate[]);
int getYear();
int getMonth();
int getDay(int month, int year);
int getHour();
int getMinute();
int getSecond();
const char *unixtimeToString(long long timeStamp);
long long addYears(int date[], long long unixtime);
long long addMonths(int date[], long long unixtime);
long long addDays(int date[], long long unixtime);
long long addHours(int date[], long long unixtime);
long long addMinutes(int date[], long long unixtime);
long long addSeconds(int date[], long long unixtime);
long long subYears(int date[], long long unixtime);
long long subMonths(int date[], long long unixtime);
long long subDays(int date[], long long unixtime);
long long subHours(int date[], long long unixtime);
long long subMinutes(int date[], long long unixtime);
long long subSeconds(int date[], long long unixtime);



#endif //PRAKTIKUM_3_MYDATEFUNCTIONS_H
