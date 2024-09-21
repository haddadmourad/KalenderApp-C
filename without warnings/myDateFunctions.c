#include <stdio.h>
#include "myDateFunctions.h"
#include <time.h>
#include <stdlib.h>

#define ENGLISH 0
#define SPANISH 1
#define GERMAN 2
#define TAIWANESE 3
#define SERBIAN 4

#define ONEDAY 86400
#define ONEHOUR 3600
#define ONEMINUTE 60

#define MAXSERIES 100

void printCurrentTime() {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%a %d.%m.%Y %H:%M %Z", local);
    printf("%s\n", buf);
}

void compareDates(){
    printf("Erstes Datum:\n");
    struct tm date1 = getDateFromUser();
    printf("Zweites Datum:\n");
    struct tm date2 = getDateFromUser();

    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);

    double diff = difftime(time1, time2) / 60;
    diff = (diff < 0) ? diff * -1 : diff;
    //printf("Differenz: %f\n", diff);
    int intDiff = (int) diff;
    printf("Differenz in Minuten: %d\n", intDiff);
}

struct tm getDateFromUser(){
    struct tm date;
    date.tm_year = getYear() - 1900;
    date.tm_mon = getMonth() - 1;
    date.tm_mday = getDay(date.tm_mon +1, date.tm_year + 1900);
    date.tm_hour = getHour();
    date.tm_min = getMinute();
    date.tm_sec = getSecond();
    return date;
}

/**
 * Main function that offers a menu to the user to create and display dateseries
 */
void Terminserien(){
    long long termineArray[MAXSERIES][3]; // Termin, Intervall, Anzahl
    int terminSerien = 0;
    int input = 99;
    int anzeigeTermin = -1;
    while (input != 0) // 0 is the exit option. the menu will reopen after every option
    {
        printf("1: Neue Terminserie\n");
        printf("2: Terminserie anzeigen\n");
        printf("0: Beenden\n");
        input = getUserInput();

        switch (input) {
            case 0: printf("Programm wird beendet\n");
                break;
            case 1: terminErstellen(termineArray[terminSerien++]);
                printf("Terminserie %d erstellt\n", terminSerien - 1);
                break;
            case 2:
                if (terminSerien == 0){
                    printf("Keine Terminserien vorhanden\n");
                    break;
                }
                while (anzeigeTermin < 0 || anzeigeTermin > terminSerien - 1){
                    printf("Welche Terminserie soll angezeigt werden?\n");
                    printf("0 - %d\n", terminSerien - 1);
                    anzeigeTermin = getUserInput();
                }
                terminAnzeigen(termineArray[anzeigeTermin]);
                anzeigeTermin = -1;
                break;
            default: printf("Ungueltige Eingabe\n");
                break;
        }
    }
}
/**
 * Prints the given dateseries
 * @param termineArray the dateseries to print
 */
void terminAnzeigen(long long termineArray[]){
    printf("Termine:\n");
    for(int i = 0; i < termineArray[2]; i++){
        printf("%s\n", unixtimeToString(termineArray[0] + i * termineArray[1] * ONEDAY));
    }
}
/**
 * Gets a new dateseries from the user and saves it in the given array
 * @param termineArray the array to save the dateseries in
 */
void terminErstellen(long long termineArray[]){
    time_t now = time(0); // get current time as unixtime
    printf("Datum Heute: %s\n", unixtimeToString(now));
    long long unixTermin = 0;
    printf("Termin eingeben \n");
    while (unixTermin < now){
        int year = getYear();
        int month = getMonth();
        int day = getDay(month, year);
        int hour = getHour();
        int minute = getMinute();
        int second = getSecond();
        int termin[6] = {day, month, year, hour, minute, second};
        unixTermin = toUnixtime(termin);
        if(unixTermin < now){
            printf("Der Termin muss in der Zukunft liegen\n");
        }
    }
    termineArray[0] = unixTermin;

    int intervall = 0;
    printf("Intervall eingeben: \n");
    while (intervall < 1 || intervall > 3){
        printf("1: Taegliche Termine\n");
        printf("2: Woechentliche Termine\n");
        printf("3: 2-Woechentliche Termine\n");
        intervall = getUserInput();
    }
     switch (intervall) {
         case 1: termineArray[1] = 1;
         break;
         case 2: termineArray[1] = 7;
         break;
         case 3: termineArray[1] = 14;
         break;
     }


    int anzahl = 0;
    printf("Anzahl eingeben: \n");
    while (anzahl < 1 || anzahl > 100){
        anzahl = getUserInput();
        if(anzahl < 1 || anzahl > 100){
            printf("Die Anzahl muss zwischen 1 und 100 liegen.\n");
        }
    }
    termineArray[2] = anzahl;
}
/**
 * Gets User input and flushes the input buffer afterwards
 * @return User input
 */
int getUserInput() {
    char* end = NULL;
    char buf[255];
    long n = 0;
    while (fgets(buf, sizeof(buf), stdin)) {
        n = strtol(buf, &end, 10);
        if (end == buf || *end !='\n') {
            printf("Ungueltige Eingabe. Integer: ");
        } else break;
    }
    return (int) n;
}
/**
 * Main function that offers a menu to the user to test the other functions
 * @param argc number of arguments
 * @param argv arguments
 * @return 0 if successful
 */
void testZeitraum(){
    printf("Bitte geben Sie das Startdatum ein: \n");
    int startYear = getYear();
    int startMonth = getMonth();
    int startDate[6] = {getDay(startMonth, startYear), startMonth, startYear, getHour(), getMinute(), getSecond() - 1};

    printf("Bitte geben Sie das Enddatum ein: \n");
    int endYear = getYear();
    int endMonth = getMonth();
    int endDate[6] = {getDay(endMonth, endYear), endMonth, endYear, getHour(), getMinute(), getSecond() + 1};

    printf("Bitte geben Sie das Datum zum ueberpruefen ein: \n");
    int checkYear = getYear();
    int checkMonth = getMonth();
    int checkDate[6] = {getDay(checkMonth, checkYear), checkMonth, checkYear, getHour(), getMinute(), getSecond()};

    if(isInZeitraum(checkDate, startDate, endDate)){
        printf("Das Datum liegt im Zeitraum\n");
    } else{
        printf("Das Datum liegt nicht im Zeitraum\n");
    }
}
/**
 * Checks if the given date is in the given time period
 * @param date to check
 * @param start of the time period
 * @param end of the time period
 * @return 1 if in time period, 0 if not
 */
int isInZeitraum(int date[], int start[], int end[]){
    if(compare(start, date) == - 1 && compare(end, date) == 1){
        return 1;
    } else{
        return 0;
    }
}
/**
 * Compares two dates and returns 1 if the first date is later than the second date, -1 if the second date is
 * later than the first date and 0 if they are equal
 * @param first
 * @param second
 * @return
 */
int compare(int first[], int second[]){
    long long firstUnixtime = toUnixtime(first);
    long long secondUnixtime = toUnixtime(second);

    if(firstUnixtime > secondUnixtime){
        return 1;
    }
    else if(firstUnixtime < secondUnixtime){
        return -1;
    }
    else{
        return 0;
    }
}
/**
 * Returns the given timestamp as a string, formatted as german short date
 * @param timeStamp
 * @return
 */
const char *unixtimeToString(long long timeStamp) {
     struct tm *date;
     time_t time = timeStamp;
     date = localtime(&time);
     static char str[20];
     strftime(str, sizeof(str), "%d.%m.%Y %H:%M:%S", date);
     return str;
}

/**
 * Converts the given date to unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @return long long unixtime
 */
long long toUnixtime(int date[]){
    // Array: [day, month, year, hour, minute, second]
    long long unixtime = 0;

    // add seconds if is 1970 or later, else subtract seconds
    if(date[2] >= 1970){
        unixtime = addYears(date, unixtime);
        unixtime = addMonths(date, unixtime);
        unixtime = addDays(date, unixtime);
        unixtime = addHours(date, unixtime);
        unixtime = addMinutes(date, unixtime);
        unixtime = addSeconds(date, unixtime);
    }
    else if(date[2] < 1970){
        unixtime = subYears(date, unixtime);
        unixtime = subMonths(date, unixtime);
        unixtime = subDays(date, unixtime);
        unixtime = subHours(date, unixtime);
        unixtime = subMinutes(date, unixtime);
        unixtime = subSeconds(date, unixtime);
    }

    return unixtime;
}
/**
 * gets the year from the user
 * @return year
 */
int getYear(){
    int year = 0;
    while (year < 1 || year > 9999) // we dont want negative years or years with more than 4 digits for this program
    {
        printf("Jahr als Integer: ");
        year = getUserInput();
        printf("\n");
    }
    return year;
}
/**
 * gets the month from the user
 * @return month
 */
int getMonth() {
    int month = 0;
    while (month < 1 || month > 12) {
        printf("Monat als Integer: ");
        month = getUserInput();
        printf("\n");
    }
    return month;
}
int getDay(int month, int year){
    int day = 0;
    while (day < 1 || day > getDaysInMonth(month, year))
    {
        printf("Tag als Integer: ");
        day = getUserInput();
        printf("\n");
    }
    return day;
}
/**
 * gets the hour from the user
 * @return hour
 */
int getHour(){
    int hour = -1;
    while (hour < 0 || hour > 23)
    {
        printf("Stunde als Integer: ");
        hour = getUserInput();
        printf("\n");
    }
    return hour;
}
/**
 * gets the minute from the user
 * @return minute
 */
int getMinute() {
    int minute = -1;
    while (minute < 0 || minute > 59) {
        printf("Minute als Integer: ");
        minute = getUserInput();
        printf("\n");
    }
    return minute;
}
/**
 * gets the second from the user
 * @return second
 */
int getSecond() {
    int second = -1;
    while (second < 0 || second > 59) {
        printf("Sekunde als Integer: ");
        second = getUserInput();
        printf("\n");
    }
    return second;
}
/**
 * Adds the years of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added years
 */
long long addYears(int date[], long long unixtime){
    for(int i = date[2]; i > 1970; i--){
        if(isLeap(i)){
            unixtime += 366 * ONEDAY;
        }
        else{
            unixtime += 365 * ONEDAY;
        }
    }
    return unixtime;
}
/**
 * Subtracts the years of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted years
 */
long long subYears(int date[], long long unixtime){
    for(int i = date[2]; i < 1970; i++){
        if(isLeap(i)){
            unixtime -= 366 * ONEDAY;
        }
        else{
            unixtime -= 365 * ONEDAY;
        }
    }
    return unixtime;
}
/**
 * Adds the months of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added months
 */
long long addMonths(int date[], long long unixtime){
    for(int i = 1; i < date[1]; i++){
        unixtime += getDaysInMonth(i, date[3]) * ONEDAY;
    }
    return unixtime;
}
/**
 * Subtracts the months of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted months
 */
long long subMonths(int date[], long long unixtime){
    for(int i = date[1] + 1; i <= 12; i++){
        unixtime -= getDaysInMonth(i, date[3]) * ONEDAY;
    }
    return unixtime;
}
/**
 * Adds the days of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added days
 */
long long addDays(int date[], long long unixtime){
    unixtime += (date[0] - 1) * ONEDAY;
    return unixtime;
}
/**
 * Subtracts the days of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted days
 */
long long subDays(int date[], long long unixtime){
    unixtime += (getDaysInMonth(date[1], date[2]) - date[0]) * ONEDAY;
    return unixtime;
}
/**
 * Adds the hours of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added hours
 */
long long addHours(int date[], long long unixtime){
    unixtime += date[3] * ONEHOUR;
    return unixtime;
}
/**
 * Subtracts the hours of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted hours
 */
long long subHours(int date[], long long unixtime){
    unixtime -= (24 - date[3] - 1) * ONEHOUR;
    return unixtime;
}
/**
 * Adds the minutes of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added minutes
 */
long long addMinutes(int date[], long long unixtime){
    unixtime += date[4] * ONEMINUTE;
    return unixtime;
}
/**
 * Subtracts the minutes of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted minutes
 */
long long subMinutes(int date[], long long unixtime){
    unixtime -= (60 - date[4] - 1) * ONEMINUTE;
    return unixtime;
}
/**
 * Adds the seconds of the given date to the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to add to
 * @return unixtime with added seconds
 */
long long addSeconds(int date[], long long unixtime){
    unixtime += date[5];
    return unixtime;
}
/**
 * Subtracts the seconds of the given date from the given unixtime
 * @param date Array: [day, month, year, hour, minute, second]
 * @param unixtime to subtract from
 * @return unixtime with subtracted seconds
 */
long long subSeconds(int date[], long long unixtime){
    unixtime -= (60 - date[5]);
    return unixtime;
}


/**
 * Prints the given month of the given year in a calendar format
 * @param month to print
 * @param year to print
 */
void printMonth(int month, int year){
    int days = getDaysInMonth(month, year);
    int startWeekday = getWeekday(1, month, year);

    // Zellers congruence returns 0 for saturday, but we want 0 for monday
    if(startWeekday == 0){
        startWeekday = 5;
    } else if(startWeekday == 1) {
        startWeekday = 6;
    }else{
        startWeekday -= 2;
    }

    printf("Mo Di Mi Do Fr Sa So\n");
    for(int i = 0; i < startWeekday; i++){
        printf("   ");
    }

    for(int i = 1; i <= days; i++){
        printf("%2d ", i);
        if((i + startWeekday) % 7 == 0){
            printf("\n");
        }
    }
    printf("\n");
    printf("--------------------\n");
}

/**
 * Prints the calendar of the given year in nx7 format
 * @param year to print
 */
void printCalendar(int year){
    for(int month = 1; month <= 12; month++){
        switch (month) {
            case 1: printf("Januar %d\n", year);
                break;
            case 2: printf("Februar %d\n", year);
                break;
            case 3: printf("Maerz %d\n", year);
                break;
            case 4: printf("April %d\n", year);
                break;
            case 5: printf("Mai %d\n", year);
                break;
            case 6: printf("Juni %d\n", year);
                break;
            case 7: printf("Juli %d\n", year);
                break;
            case 8: printf("August %d\n", year);
                break;
            case 9: printf("September %d\n", year);
                break;
            case 10: printf("Oktober %d\n", year);
                break;
            case 11: printf("November %d\n", year);
                break;
            case 12: printf("Dezember %d\n", year);
                break;

        }
        printMonth(month, year);
    }
}

/**
 * Checks if the given year is a leap year
 * @param year to check
 * @return 1 if leap year, 0 if not
 */
int isLeap(int year){
    if(year % 4 == 0){
        if(year % 100 == 0){
            if(year % 400 == 0){
                return 1;
            }
            else return 0;
        }
        else return 1;
    }
    else return 0;
}

/**
 * Calculates the number of days in the given month of the given year
 * @param month to check
 * @param year to check
 * @return number of days in the given month
 */
int getDaysInMonth(int month, int year){
    switch(month){
        case 1: return 31;
        case 2:
            if(isLeap(year)){
                return 29;
            }
            else{
                return 28;
            }
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return 0;
}

/**
 * Prints the given date in the given language
 * @param day to print
 * @param month to print
 * @param year to print
 * @param language to print in
 */
void printDate_switch(int day, int month, int year, int language){
    switch(language){
        case ENGLISH: printf("Date: %02d/%02d/%04d\n", month, day, year);
            break;
        case SPANISH: printf("Fecha: %02d/%02d/%04d\n", day, month, year);
            break;
        case GERMAN: printf("Datum: %02d.%02d.%04d\n", day, month, year);
            break;
        case TAIWANESE: printf("日期 (Riqi): %04d/%02d/%02d\n", year, month, day);
            break;
        case SERBIAN: printf("датум (Datum): %d.%d.%04d\n", day, month, year);
            break;
        default: printf("Ungueltige Sprache. Deutsches Format\n");
            printf("Datum: %02d.%02d.%04d\n", day, month, year);
    }
    printf("--------------------\n");
}

/**
 * Prints the given date in the given language
 * @param day to print
 * @param month to print
 * @param year to print
 * @param language to print in
 */
void printDate_ifelse(int day, int month, int year, int language){
    if(language == ENGLISH){
        printf("Date: %02d/%02d/%04d\n", month, day, year);
    }
    else if(language == SPANISH){
        printf("Fecha: %02d/%02d/%04d\n", day, month, year);
    }
    else if(language == GERMAN){
        printf("Datum: %02d.%02d.%04d\n", day, month, year);
    }
    else if(language == TAIWANESE){
        printf("Riqi: %04d/%02d/%02d\n", year, month, day);
    }
    else if(language == SERBIAN){
        printf("Datum: %d.%d.%04d\n", day, month, year);
    }
    else{
        printf("Ungueltige Sprache. Deutsches Format\n");
        printf("Datum: %02d.%02d.%04d\n", day, month, year);
    }

    printf("--------------------\n");
}

/**
 * Creates a menu to print the given date in different languages
 * @param day to print
 * @param month to print
 * @param year to print
 */
void makros(int day, int month, int year){
    printf("English: %d \n", ENGLISH);
    printf("Spanish: %d \n", SPANISH);
    printf("German: %d \n", GERMAN);
    printf("Taiwanese: %d \n", TAIWANESE);
    printf("Serbian: %d \n", SERBIAN);
    printf("Code Eingegeben: ");

    int code = 0;
    scanf("%d", &code);

    printDate_switch(day, month, year, code);
    //printDate_ifelse(day, month, year, code);
}
/**
 * @brief Checks if the given date is a palindrome
 * @param day to check
 * @param month to check
 * @param year to check
 * @return 1 if palindrome, 0 if not
 */
int isPalindrom(int day, int month, int year){
    int day1 = day / 10; // first digit of day
    int day2 = day % 10; // second digit of day
    int month1 = month / 10; // first digit of month
    int month2 = month % 10;
    int year1 = year / 1000;
    int year2 = (year / 100) % 10;
    int year3 = (year / 10) % 10; // etc.
    int year4 = year % 10; // last digit of year

    // compare first to last, second to second last, etc.
    if(day1 == year4 && day2 == year3 && month1 == year2 && month2 == year1){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Tests the isPalindrom function
 */
void palindromTest(){
    int day = 1, month = 1, year = 2000;
    if(isPalindrom(day, month, year)){
        printf("Ist %d.%d.%d ein Palindrom?: Ja\n", day, month, year);
    } else{
        printf("Ist %d.%d.%d ein Palindrom?: Nein\n", day, month, year);
    }
    day = 22, month = 2, year = 2022;
    if(isPalindrom(day, month, year)){
        printf("Ist %d.%d.%d ein Palindrom?: Ja\n", day, month, year);
    } else{
        printf("Ist %d.%d.%d ein Palindrom?: Nein\n", day, month, year);
    }
    day = 3, month = 3, year = 3003;
    if(isPalindrom(day, month, year)){
        printf("Ist %d.%d.%d ein Palindrom?: Ja\n", day, month, year);
    } else{
        printf("Ist %d.%d.%d ein Palindrom?: Nein\n", day, month, year);
    }
    day = 30, month = 3, year = 3003;
    if(isPalindrom(day, month, year)){
        printf("Ist %d.%d.%d ein Palindrom?: Ja\n", day, month, year);
    } else{
        printf("Ist %d.%d.%d ein Palindrom?: Nein\n", day, month, year);
    }
    day = 30, month = 03, year = 3003;
    if(isPalindrom(day, month, year)){
        printf("Ist %d.%d.%d ein Palindrom?: Ja\n", day, month, year);
    } else{
        printf("Ist %d.%d.%d ein Palindrom?: Nein\n", day, month, year);
    }
}
/**
 * Returns the weekday of the given date using Zellers congruence
 * @param day to check
 * @param month to check
 * @param year to check
 * @return weekday of the given date, starting with 0 for saturday
 */
int getWeekday(int day, int month, int year){

    // the algorithm counted january and february as 13th and 14th month of the previous year
    if(month == 1 || month == 2){
        month += 12;
        year--;
    }

    int K = year % 100;
    int J = year / 100;

    // Zellers congruence used the mathematical modulo operator, which is not the same as the C modulo operator
    // i had to use this workaround to get the same result without negative numbers
    int weekday = ((day + ((13 * (month + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J)) % 7);

    return weekday;
}

/**
 * Prints the weekday of the given date
 * @param weekday to print
 */
void printWeekday(int weekday){
    printf("Der Wochentag ist: ");

    switch (weekday) {
        case 0: printf("Samstag\n");
            break;
        case 1: printf("Sonntag\n");
            break;
        case 2: printf("Montag\n");
            break;
        case 3: printf("Dienstag\n");
            break;
        case 4: printf("Mittwoch\n");
            break;
        case 5: printf("Donnerstag\n");
            break;
        case 6: printf("Freitag\n");
            break;
        default: printf("Error %d\n", weekday);
            break;
    }
    printf("--------------------\n");
}

/**
 * Tests the getWeekday function
 */
void weekdayTest(){
    printf("Test 1: 1.1.2023\n");
    printWeekday(getWeekday(1, 1, 2023));
    printf("Test 2: 1.2.1812\n");
    printWeekday(getWeekday(1, 2, 1812));
    printf("Test 3: 25.11.2459\n");
    printWeekday(getWeekday(25, 11, 2459));
}