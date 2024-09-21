#ifndef PRAKTIKUM_4_PRAK5_H
#define PRAKTIKUM_4_PRAK5_H

#include "stundenplan.h"
#include <stdio.h>

typedef struct person {
    char *firstName;
    char *lastName;
    struct person *next;
    struct person *prev;
} person;

typedef struct personList {
    person *head;
    person *tail; 
} personList;
struct appointment;
struct node;

/**
 * Prints an appointment to a file
 * The file needs to be opened before calling this function and closed after calling it
 * @param data The appointment to print
 * @param file The file to print the appointment to
 */
void printAppToFile(struct appointment *data, FILE *file);

/**
 * Prints all appointments into a file
 * @param head Head of the linked list of appointments
 */
void exportAppointments(struct node *head);

/**
 * Opens the a menu to let the user Change the Title, Duration or Participants of an appointment
 * @param head Head of the linked list of appointments
 * @param countAppointments Amount of appointments in the list
 */
void editAppointment(struct node *head, int countAppointments);

/**
 * Gets User input for which person to remove from which appointment
 * @param head
 * @param countAppointments
 */
void userRemovePerson(struct node*head, int countAppointments);
/**
 * Removes a person from the list
 * @param head Head of the list of appointments
 * @param countAppointments Amount of appointments in the list
 */
void removePerson(person *currentPerson, person *personToRemove, struct node *current);

/**
 * Prints all appointments of a person
 * @param head Head of the list of appointments
 * @param printPerson the person to print the appointments of
 */
void printPersonsApps(struct node *head, person *printPerson);

/**
 * Adds a person to an appointment that the user selects
 * @param head Head of the list of appointments
 * @param countAppointments Amount of appointments in the list
 */
void addPersonToAppointment(struct node *head, int countAppointments);

/**
 * unsafeFunction() copies a string into a buffer that is too small and uses strcpy() without null terminator
 */
void unsafeFunction();

/**
 * Gets a full name from the user and returns it as a struct person
 * @return *person The created person the user entered
 */
person *createPerson();

/**
 * Prints a list of persons
 * @param list List of persons to print
 */
void printPersonList(personList *list);

/**
 * Calls createPerson and adds the new person to the list
 * @param list List to add the person to
 */
void addPerson(personList *list, char *firstName, char *lastName);
#endif //PRAKTIKUM_4_PRAK5_H
