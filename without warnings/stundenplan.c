#include "stundenplan.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myDateFunctions.h"
#include "prak5.h"

appointment *createAppointment(int *countAppointments, node *head, appointment *data) {
    appointment *newAppointment = malloc(sizeof(appointment));
    if(newAppointment == NULL) {
        puts("Speicher konnte nicht reserviert werden!");
        exit(1);
    }
    if(data == NULL) { // if data is NULL, the appointment is created by the user
        do {
            puts("Termin start eingeben:");
            newAppointment->startDate = getDateFromUser();
            newAppointment->startDate.tm_isdst = -1;
            if(mktime(&newAppointment->startDate) < 0) {
                puts("Ungueltiges Datum!");
            }
        } while (mktime(&newAppointment->startDate) < 0);
        newAppointment->duration = getDuration();
        newAppointment->title = getTitle();

        // add participants
        int userInput = 2;
        newAppointment->participants = malloc(sizeof(personList));
        if(newAppointment->participants == NULL) {
            puts("Speicher konnte nicht reserviert werden!");
            exit(1);
        }
        newAppointment->participants->head = NULL;
        newAppointment->participants->tail = NULL;
        do {
            userInput = 2;
            while (userInput != 1 && userInput != 0) {
                printf("Moechten Sie Teilnehmer hinzufuegen? (1 = Ja, 0 = Nein): ");
                userInput = getUserInput();
            }
            if (userInput == 1) {
                addPerson(newAppointment->participants, NULL, NULL);
            }
        } while (userInput == 1);
    }
    else { // if data is not NULL, the appointment is created by createAppointmentSeries
        newAppointment->startDate = data->startDate;
        newAppointment->duration = data->duration;
        newAppointment->title = data->title;
        mktime(&newAppointment->startDate); // mktime fills the missing fields of the struct

        newAppointment->participants = malloc(sizeof(personList));
        newAppointment->participants->head = NULL;
        newAppointment->participants->tail = NULL;

        if(data->participants->head != NULL) { // if the appointment has participants, copy them
            person *current = data->participants->head;
            while (current != NULL) {
                addPerson(newAppointment->participants, current->firstName, current->lastName);
                current = current->next;
            }
        }
    }
    (*countAppointments)++;
    addAppointment(head, newAppointment);

    printf("Termin %d angelegt!\n", *countAppointments);
    printAppointment(newAppointment);

    return newAppointment;
}

void createAppointmentSeries(int *countAppointments ,node *head, appointment *data) {
    int interval = 0;
    int amount = 0;

    while (amount < 1) {
        printf("Anzahl der Folgetermine: ");
        amount = getUserInput();
    }
    while (interval < 1) {
        printf("Intervall in Tagen: ");
        interval = getUserInput();
    }
    appointment *newDatePointer = malloc(sizeof(appointment));
    newDatePointer->duration = data->duration;
    newDatePointer->title = data->title;
    newDatePointer->startDate = data->startDate;
    newDatePointer->participants = data->participants;
    int prevDst = data->startDate.tm_isdst;
    for (int i = 0; i < amount; i++) { // create the amount of appointments the user entered with the interval he entered
        newDatePointer->startDate.tm_mday += interval;
        mktime(&newDatePointer->startDate); // mktime normalizes the date when out of range

        // Check if the date is in DST and the previous date was not
        if(prevDst == 0 && newDatePointer->startDate.tm_isdst == 1) { // Winter -> Summer
            newDatePointer->startDate.tm_hour -= 1;
            mktime(&data->startDate);
        }
        else if(prevDst == 1 && newDatePointer->startDate.tm_isdst == 0) { // Summer -> Winter
            newDatePointer->startDate.tm_hour += 1;
            mktime(&data->startDate);
        }
        prevDst = newDatePointer->startDate.tm_isdst; // Save the current DST state for the next iteration
        createAppointment(countAppointments, head, newDatePointer);
    }
    free(newDatePointer);
    printf("Terminserie angelegt!\n");
}

int getDuration() {
    int duration;
    printf("Dauer des Termins in Minuten: ");
    duration = getUserInput();
    return duration;
}

char *getTitle() {
    printf("Titel des Termins (max. 20 Zeichen): ");
    return getUserString(MAX_LENGTH);
}

char* getUserString(int max_length) {
    char* input = (char*)malloc((max_length + 1) * sizeof(char)); // +1 for null terminator
    if (input == NULL) {
        puts("Speicher konnte nicht reserviert werden!");
        exit(1);
    }

    int i = 0;
    char ch;

    do {
        i = 0; // Reset i for each new input
        while ((ch = getchar()) != '\n' && ch != EOF) { // Read chars until newline or EOF
            if (i < max_length) {
                input[i] = ch;
                i++;
            } else {
                break; // If the limit is reached, break the loop
            }
        }

        input[i] = '\0'; // Add null terminator to make it a valid string
        if (i > max_length) {
            printf("Limit von %d Charaktern. Bitte Nochmal.\n", max_length);
        }

        // Clear input buffer in case the input exceeded the limit
        while (ch != '\n' && ch != EOF) {
            ch = getchar();
        }
    } while (i > max_length); // Continue prompting until input is within limit

    return input;
}

node *createNode(appointment *data, node *next) {
    node *newNode = malloc(sizeof(node));
    if(newNode == NULL) {
        puts("Speicher konnte nicht reserviert werden!");
        exit(1);
    }
    newNode->data = data;
    newNode->next = next;
    return newNode;
}

node *addAppointment(node *head, appointment *data) {
    node *current = head;
    // Find the node before the one to insert. difftime returns the difference in seconds the first date minus the second
    while (current->next != NULL && difftime(mktime(&current->next->data->startDate), mktime(&data->startDate)) < 0) {
        current = current->next;
    }
    if (current->next == NULL) { // if the appointment is the last one in the list it has no next node
        current->next = createNode(data, NULL);
        return head;
    }
    current->next = createNode(data, current->next);
    return head;
}

void deleteAppointment(node *head, int *countAppointments){
    int userInput = 0;
    if(countAppointments == 0) {
        puts("Keine Termine vorhanden!");
        return;
    }
    node *current = head->next;
    printAllAppointments(head);
    while (userInput < 1 || userInput > *countAppointments){
        printf("Nummer des Termins zum loeschen eingeben: ");
        userInput = getUserInput();
    }
    // go to the node before the one to delete
    for (int i = 1; i < userInput - 1; i++) {
        current = current->next;
    }
    // Save the node to delete so it can be freed later
    node *temp = current->next;
    current->next = current->next->next;
    printf("Der Termin:\n");
    printAppointment(temp->data);
    printf("wurde geloescht!\n");
    free(temp);
    (*countAppointments)--;
}

void printAppointment(appointment *data) {
    printf("Titel: %s\n", data->title);
    char buf[80];
    printf("Start: ");
    strftime(buf, sizeof(buf), "%a %d.%m.%Y %H:%M %Z", &data->startDate);
    printf("%s\n", buf);
    printf("Dauer: %d Minuten\n", data->duration);
    printf("Teilnehmer: ");
    printPersonList(data->participants);
}

void printAllAppointments(node *head) {
    node *current = head->next;
    int i = 1;
    while (current != NULL) {
        printf("\tTermin %d\n", i++);
        printAppointment(current->data);
        printf("-----------------------------------\n");
        current = current->next;
    }
}

void printWeek(node *head, int countAppointments) {
    // printWeek turned out to be quite hard to do line by line with a linked list. So I had to create a dynamic 2d array

    if (countAppointments == 0) {
        puts("Keine Termine vorhanden!");
        return;
    }
    // array with the years and weeks that have appointments. [0] = year, [1] = week
    int apps[countAppointments][2];
    for (int i = 0; i < countAppointments; i++) { // fill the array with 0
        apps[i][0] = 0;
        apps[i][1] = 0;
    }
    node *current = head->next;
    int k = 0;
    int prevYear = 0;
    int prevWeek = 0;
    while (current != NULL) { // fill the array with the years and weeks of the appointments
        if (current->data->startDate.tm_year + 1900 != prevYear ||
            getCalendarWeek(current->data->startDate) != prevWeek) {
            apps[k][0] = current->data->startDate.tm_year + 1900;
            apps[k][1] = getCalendarWeek(current->data->startDate);
            prevWeek = apps[k][1];
            prevYear = apps[k][0];
            k++;
        }
        current = current->next;
    }

    // print the array to choose a week from
    printf("Es gibt Termine in den Kalenderwochen:\n");
    int j = 1;
    printf("%d:\n %d: KW%d\n", apps[0][0], j, apps[0][1]);
    while (apps[j][0] != 0) {
        if (apps[j][0] == apps[j - 1][0]) { // if the year is the same as the previous one, print the week
            printf("%d: KW%d\n", j + 1, apps[j][1]);
        } else { // if the year is different, print the year and the week
            printf("\n%d:\n%d: KW%d\n", apps[j][0], j + 1, apps[j][1]);
        }
        j++;
    }
    printf("\n");
    int userInput = 0;
    while (userInput < 1 || userInput > j) {
        printf("Nummer der Kalenderwoche zum anzeigen eingeben: ");
        userInput = getUserInput();
    }
    userInput--;
    int kw = apps[userInput][1]; // chosen week
    int year = apps[userInput][0]; // chosen year
    printf("\tTermine in KW%d\n", kw);

    // go to the first appointment of the chosen week in the linked list
    current = head->next;
    while (current->data->startDate.tm_year + 1900 != year || getCalendarWeek(current->data->startDate) != kw) {
        current = current->next;
    }

    // create a dynamic 2d array with the appointments of the chosen week to sort them by day and print them
    int appsPerDay[7] = {0}; // array with the amount of appointments per day
    appointment **kwApps = (appointment **) malloc(sizeof(appointment *) * 7);
    if (kwApps == NULL) {
        puts("Speicher konnte nicht reserviert werden!");
        exit(1);
    }
    for (int i = 0; i < 7; i++) {
        kwApps[i] = NULL;
    }

    // fill the array with the appointments of the chosen week and count the appointments per day
    while (current != NULL && current->data->startDate.tm_year + 1900 == year &&
            getCalendarWeek(current->data->startDate) == kw) { // while the current appointment is in the chosen week
        int day = current->data->startDate.tm_wday;
        kwApps[day] = (appointment *) realloc(kwApps[day], sizeof(appointment) * (appsPerDay[day] + 1));
        if (kwApps[day] == NULL) {
            puts("Speicher konnte nicht reserviert werden!");
            exit(1);
        }
        kwApps[day][appsPerDay[day]] = *current->data;
        appsPerDay[day]++;
        current = current->next;
    }

    printf(" Montag\t| Dienstag\t| Mittwoch\t| Donnerstag\t| Freitag\t| Samstag\t| Sonntag\n");
    for(int lines = 0; lines < 110; lines++){
        printf("-");
    }
    printf("\n");
    // print the appointments of the chosen week. Line by line, part by part (title, start, duration, line)
    int done = 0;
    int daysFinished[7] = {0};
    int appOfDay = 0; // Track the current appointment of the day to print
    while (!done){ // while not all days are finished
        for (int partToPrint = 0; partToPrint < 4; partToPrint++ ) { // print 0 = title, 1 = start, duration and line
            printf(" ");
            for (int i = 1; i < 8; i++) { // print the appointments of the day
                printLineOfDay(i, partToPrint, appOfDay, appsPerDay, kwApps, daysFinished);
            }
        }
        appOfDay++;
        if (daysFinished[0] && daysFinished[1] && daysFinished[2] && daysFinished[3] && daysFinished[4] && daysFinished[5] && daysFinished[6]){
            done = 1;
        }
    }
    printf("\n");

    // free the memory
    for (int i = 0; i < 7; i++) {
        free(kwApps[i]);
    }
    free(kwApps);
}

void printLineOfDay(int day, int partToPrint, int appOfDay, int appsPerDay[7], appointment **kwApps, int daysFinished[7]) {
    if (day == 7) { // print sunday at the end
        if (partToPrint == 0){ // print the title
            if (appOfDay < appsPerDay[0]) { // if there are appointments left to print
                printf("%s\n", kwApps[0][appOfDay].title);
            } else {
                daysFinished[0] = 1;
                printf("\n");
            }
        } else if (partToPrint == 1){
            if (appOfDay < appsPerDay[0]) {
                char buf[20];
                strftime(buf, sizeof(buf), "%H:%M", &kwApps[0][appOfDay].startDate);
                printf("%s\n", buf);
            } else {
                daysFinished[0] = 1;
                printf("\n");
            }
        } else if (partToPrint == 2){
            if (appOfDay < appsPerDay[0]) {
                printf("%d min\n", kwApps[0][appOfDay].duration);
            } else {
                daysFinished[0] = 1;
                printf("\n");
            }
        }
        else if (partToPrint == 3){
            if (appOfDay < appsPerDay[0]) {
                printf("-----\n");
            } else {
                daysFinished[0] = 1;
                printf("\n");
            }
        }
    } else { // print the other days
        if (partToPrint == 0){
            if (appOfDay < appsPerDay[day]) {
                printf("%s\t|\t", kwApps[day][appOfDay].title);
            } else {
                daysFinished[day] = 1;
                printf("\t|\t");
            }
        } else if (partToPrint == 1){
            if (appOfDay < appsPerDay[day]) {
                char buf[20];
                strftime(buf, sizeof(buf), "%H:%M", &kwApps[day][appOfDay].startDate);
                printf("%s\t|\t", buf);
            } else {
                daysFinished[day] = 1;
                printf("\t|\t");
            }
        } else if (partToPrint == 2){
            if (appOfDay < appsPerDay[day]) {
                printf("%d min\t|\t", kwApps[day][appOfDay].duration);
            } else {
                daysFinished[day] = 1;
                printf("\t|\t");
            }
        }
        else if (partToPrint == 3){
            if (appOfDay < appsPerDay[day]) {
                printf("-----\t|\t");
            } else {
                daysFinished[day] = 1;
                printf("\t|\t");
            }
        }
    }
}

int getCalendarWeek(struct tm date) {
    char buf[10];
    strftime(buf, sizeof(buf), "%W", &date);
    int week = strtol(buf, NULL, 10);
    return week;
}