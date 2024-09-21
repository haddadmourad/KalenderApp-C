#ifndef PRAKTIKUM_4_STUNDENPLAN_H
#define PRAKTIKUM_4_STUNDENPLAN_H

#include <time.h>
#include "prak5.h"
#define MAX_LENGTH 20
typedef struct appointment{
    struct tm startDate;
    int duration;
    char *title;
    struct personList *participants;
} appointment;

typedef struct node{
    appointment *data;
    struct node *next;
} node;


/**
 * Counts the length of a string and returns it
 * @param string The string to count the length of
 * @return int The length of the string
 */
int getDuration();
/**
 * Gets the title of an appointment from the user
 * @return char* The title of the appointment. Has to be freed after usage!
 */
char *getTitle();
/**
 * Gets a string from the user and returns it
 * @param maxLength The maximum length of the string
 * @return char* The string the user entered. Has to be freed after usage!
 */
char *getUserString(int maxLength);
/**
 * Creates a new node with the given data, next and prev pointers
 * @param data The data to store in the node
 * @param next The next node
 * @param prev The previous node
 * @return node* The created node. Has to be freed after usage!
 */
node *createNode(appointment *data, node *next);
/**
 * Adds an appointment to the list, sorted by date
 * @param head The head of the list
 * @param data The appointment to add
 * @return node* The new head of the list
 */
node *addAppointment(node *head, appointment *data);
/**
 * Deletes an appointment from the list
 * @param head The head of the list
 * @param countAppointments The amount of appointments in the list
 */
void deleteAppointment(node *head, int *countAppointments);
/**
 * Creates an appointment and adds it to the list, sorted by date. If data is not NULL, the appointment will be created
 * with the data from data so it can be used to create a series of appointments
 * @param countAppointments The amount of appointments in the list
 * @param head The head of the list
 * @param data The appointment to add
 * @return appointment* The created appointment
 */
appointment *createAppointment(int *countAppointments, node *head, appointment *data);
/**
 * prints the given appointment
 * @param data the appointment to print
 */
void printAppointment(appointment *data);
/**
 * Creates a series of appointments
 * @param countAppointments The amount of appointments in the list
 * @param head The head of the list
 * @param data the data of the first appointment
 */
void createAppointmentSeries(int *countAppointments , node *head, appointment *data);
/**
 * Frees the memory of the given appointment
 * @param data The appointment to free
 */
void printAllAppointments(node *head);
/**
 * Uses strftime to return the week of the year of the given date
 * @param date The date to get the week of the year from
 * @return The week of the year as an int
 */
int getCalendarWeek(struct tm date);
/**
 * Prints the week view of the given list
 * @param head The head of the list
 * @param countAppointments The amount of appointments in the list
 */
void printWeek(node *head, int countAppointments);
/**
 * Prints the given line of the week view
 * @param day The day of the week to print
 * @param partToPrint The part of the day to print
 * @param appOfDay The amount of appointments on the given day
 * @param appsPerDay The amount of appointments on each day of the week
 * @param kwApps The appointments of the week
 * @param daysFinished The days that have already been printed
 */
void printLineOfDay(int day, int partToPrint, int appOfDay, int appsPerDay[7], appointment **kwApps, int daysFinished[7]);
#endif //PRAKTIKUM_4_STUNDENPLAN_H
