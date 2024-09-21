#include <stdio.h>
#include <stdlib.h>
#include "myDateFunctions.h"
#include "stundenplan.h"
#include "prak5.h"

int main(void)
{
    node *head = malloc(sizeof(node));
    setbuf(stdout, 0); // My IDE doesnt print in debug mode without this line
    int countAppointments = 0, userInputMainMenu;
    person *searchPerson;
    while (1) {
        puts("1. Termin anlegen");
        puts("2. Terminserie anlegen");
        puts("3. Termin in Datei exportieren");
        puts("4. Alle Termine anzeigen");
        puts("5. Kalenderausgabe");
        puts("6. Termin bearbeiten");
        puts("7. Alle Termine einer Person anzeigen");
        puts("8. Strcpy Sicherheitstest");
        puts("9. Programm beenden");

        userInputMainMenu = getUserInput();
        switch (userInputMainMenu) {
            case 1: {
                createAppointment(&countAppointments, head, NULL);
            }break;
            case 2: {
                createAppointmentSeries(&countAppointments, head, createAppointment(&countAppointments, head, NULL));
            }break;
            case 3: {
                exportAppointments(head);
            } break;
            case 4: {
                printAllAppointments(head);
            }break;
            case 5: printWeek(head, countAppointments); break;
            case 6: editAppointment(head, countAppointments); break;
            case 7:
                printf("Welche Person moechten Sie anzeigen?\n");
                searchPerson = createPerson();
                printPersonsApps(head, searchPerson);
                free(searchPerson);
                break;
            case 8: unsafeFunction(); break; // Strcpy Sicherheitstest
            case 9:
                return 0; // Programm beenden
            default:
                puts("Ungueltige Eingabe!");
        }
    }
    return 0;
}