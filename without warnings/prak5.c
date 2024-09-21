#include "prak5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stundenplan.h"
#include "myDateFunctions.h"

void exportAppointments(node *head) {
    FILE *file;
    file = fopen("termine.txt", "w");
    if (file == NULL) {
        printf("Fehler beim Oeffnen der Datei!\n");
        return;
    }
    node *current = head->next;
    while (current != NULL) {
        printAppToFile(current->data, file);
        current = current->next;
    }
    fclose(file);
    printf("Termine exportiert!\n");

}

void printAppToFile(appointment *data, FILE *file) {
    // file needs to be opened before calling this function and closed after calling it
    fseek(file, 0, SEEK_END);
    fprintf(file,"Titel: %s\n", data->title);
    char buf[80];                                                                         
    fprintf(file,"Start: ");
    strftime(buf, sizeof(buf), "%a %d.%m.%Y %H:%M %Z", &data->startDate);
    fprintf(file,"%s\n", buf);
    fprintf(file,"Dauer: %d Minuten\n", data->duration);
    fprintf(file,"Teilnehmer: ");
    if (data->participants->head == NULL) {                                               
        fprintf(file,"Keine Teilnehmer\n");
    } else {
        person *currentPerson = data->participants->head;
        while (currentPerson != NULL) {
            fprintf(file,"%s %s", currentPerson->firstName, currentPerson->lastName);
            if (currentPerson->next != NULL) {
                fprintf(file,", ");
            }
            currentPerson = currentPerson->next;
        }
        fprintf(file,"\n");
    }
    fprintf(file,"\n");
}

void editAppointment(node *head, int countAppointments) {
    int userInput2 = 0;
    while (userInput2 != 3) {
        printf("Teilnehmer hinzufuegen oder entfernen?\n");
        puts("1. Teilnehmer hinzufuegen");
        puts("2. Teilnehmer entfernen");
        puts("3. Zurueck");
        userInput2 = getUserInput();
        switch (userInput2) {
            case 1:
                addPersonToAppointment(head, countAppointments);
            break;
            case 2:
                userRemovePerson(head, countAppointments);
            break;
            case 3:
                return;
            default:
                puts("Ungueltige Eingabe!");
        }
    }
}

void userRemovePerson(node *head, int countAppointments){
    node *current = head; // current appointment
    int userInput = 0;
    printf("Aus welchem Termin moechten Sie eine Person entfernen?\n");
    printAllAppointments(head); // Print all appointments
    while (userInput < 1 || userInput > countAppointments) {
        printf("Bitte geben Sie eine Zahl zwischen 1 und %d ein: ", countAppointments);
        userInput = getUserInput();
    }
    for (int i = 0; i < userInput; ++i) { // Go to selected appointment
        current = current->next;
    }
    if (current->data->participants->head == NULL) {
        printf("Keine Teilnehmer!\n");
        return;
    }
    person *currentPerson = current->data->participants->head; // first person in list

    printf("Welche Person moechten Sie entfernen?\n");
    printPersonList(current->data->participants);
    // Get name of person to remove
    person *personToRemove = createPerson(); // create person to remove

    removePerson(currentPerson, personToRemove, current);
}

void removePerson(person *currentPerson, person *personToRemove, node *current) { //current=current appointment
    // Check if person is in list and remove it if it is 
    while (currentPerson != NULL) {
        if (strcmp(currentPerson->firstName, personToRemove->firstName) == 0 && strcmp(currentPerson->lastName, personToRemove->lastName) == 0) {
            if (currentPerson->prev == NULL) { // remove head
                if (currentPerson->next != NULL) {
                    currentPerson->next->prev = NULL;
                    current->data->participants->head = currentPerson->next;
                } else { // remove only person
                    current->data->participants->tail = NULL;
                    current->data->participants->head = NULL;
                }
            } else if (currentPerson->next == NULL) { // remove tail
                current->data->participants->tail = currentPerson->prev;
                currentPerson->prev->next = NULL;
            } else { // remove in between
                currentPerson->prev->next = currentPerson->next;
                currentPerson->next->prev = currentPerson->prev;
            }
            free(currentPerson);
            printf("Person entfernt!\n");
            return;
        }
        currentPerson = currentPerson->next;
    }
    printf("Person nicht gefunden!\n");
}

void printPersonsApps(node *head, person *printPerson) {
    node *current = head->next; // current appointment
    int printCount = 0;
    while (current != NULL) {
        if (current->data->participants->head == NULL) {
            current = current->next;
            continue;
        }
        person * currentPerson = current->data->participants->head;
        while (currentPerson != NULL) {
            if (strcmp(currentPerson->firstName, printPerson->firstName) == 0 && strcmp(currentPerson->lastName, printPerson->lastName) == 0) {
                printAppointment(current->data);
                printCount++;
            }
            currentPerson = currentPerson->next;
        }
        current = current->next;
    }
    if (printCount == 0) {
        printf("Keine Termine gefunden!\n");
    }
}

void addPersonToAppointment(node* head, int countAppointments) {
    int userInput = 0;
    printf("Welchem Termin moechten Sie eine Person hinzufuegen?\n");
    printAllAppointments(head);
    while (userInput < 1 || userInput > countAppointments) {
        printf("Bitte geben Sie eine Zahl zwischen 1 und %d ein: ", countAppointments);
        userInput = getUserInput();
    }
    node *current = head;
    for (int i = 0; i < userInput; ++i) {
        current = current->next;
    }
    addPerson(current->data->participants, NULL, NULL);
}

void printPersonList(personList *list) {
    if (list->head == NULL) {
        printf("Keine Teilnehmer!\n");
        return;
    }
    person *current = list->head;
    while (current != NULL) {
        printf("%s %s", current->firstName, current->lastName);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("\n");
}

void addPerson(personList *list, char *firstName, char *lastName) {
    person *newPerson;
    if(firstName == NULL || lastName == NULL) {
        newPerson = createPerson();
    } else{
        newPerson = malloc(sizeof(person));
        newPerson->firstName = firstName;
        newPerson->lastName = lastName;
        newPerson->next = NULL;
        newPerson->prev = NULL;
    }

    if (list->head == NULL) {
        list->head = newPerson;
        list->tail = newPerson;
    } else { // insert sorted by last name
        person *current = list->head;
        while (current != NULL) {
            if (strcmp(current->lastName, newPerson->lastName) == 0){
                printf("Diese Person ist bereits in der Liste!\n");
                free(newPerson);
                return;
            }
            if (strcmp(current->lastName, newPerson->lastName) > 0) {
                if (current->prev == NULL) { // insert at head
                    newPerson->next = current;
                    current->prev = newPerson;
                    list->head = newPerson;
                } else { // insert in between
                    newPerson->next = current;
                    newPerson->prev = current->prev;
                    current->prev->next = newPerson;
                    current->prev = newPerson;
                }
                break;
            } else if (current->next == NULL) { // insert at tail
                current->next = newPerson;
                newPerson->prev = current;
                list->tail = newPerson;
                break;
            }
            current = current->next;
        }
    }
}

person *createPerson() {
    int maxNameLength = 50;
    person *newPerson = malloc(sizeof(person));
    char *firstNameBuf = malloc(maxNameLength);
    char *lastNameBuf = malloc(maxNameLength);
    printf("Vorname: ");
    firstNameBuf = getUserString(maxNameLength);
    printf("Nachname: ");
    lastNameBuf = getUserString(maxNameLength);
    char *firstName = malloc(strlen(firstNameBuf) + 1); // +1 for the null terminator
    char *lastName = malloc(strlen(lastNameBuf) + 1);
    strcpy(firstName, firstNameBuf);
    strcpy(lastName, lastNameBuf);
    free(firstNameBuf);
    free(lastNameBuf);
    newPerson->firstName = firstName;
    newPerson->lastName = lastName;
    newPerson->next = NULL;
    newPerson->prev = NULL;
    return newPerson;
}

void unsafeFunction() {
    char *overflow = malloc(8);
    overflow = "overflow";
    char *dest;
    strcpy(dest, overflow);
    printf("%s", dest);
    free(dest);
}