#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EVENTS 100
#define FILENAME "events.txt"

typedef struct {
    int year, month, day, hour, minute;
    char airline[10], details[100];
} Event;

int getChoice() {
    int choice;
    printf("\nMenu:\n1. Add Event\n2. Generate HTML\n3. Exit\nChoice: ");
    scanf("%d", &choice);
    return choice;
}

void addEvent(Event events[], int *numEvents) {
    if (*numEvents == MAX_EVENTS) {
        printf("Max events reached!\n");
        return;
    }

    Event newEvent;
    printf("Date (YYYY MM DD): ");
    scanf("%d %d %d", &newEvent.year, &newEvent.month, &newEvent.day);

    printf("Time (HH MM): ");
    scanf("%d %d", &newEvent.hour, &newEvent.minute);

    printf("Airline: ");
    scanf("%s", newEvent.airline);

    printf("Details: ");
    scanf(" %[^\n]", newEvent.details); 

    events[(*numEvents)++] = newEvent;
}

void generateHTML(Event events[], int numEvents) {
    FILE *fp = fopen("calendar.html", "w");
    if (!fp) {
        printf("Error creating HTML file!\n");
        return;
    }

    fprintf(fp, "<html><body><table>\n");
    for (int i = 0; i < numEvents; i++) {
        fprintf(fp, "<tr><td>%04d-%02d-%02d</td><td>%02d:%02d</td><td>%s</td><td>%s</td></tr>\n",
            events[i].year, events[i].month, events[i].day, events[i].hour, events[i].minute,
            events[i].airline, events[i].details);
    }
    fprintf(fp, "</table></body></html>\n");
    fclose(fp);
    printf("HTML calendar generated!\n");
}

int main() {
    Event events[MAX_EVENTS];
    int numEvents = 0; 
    FILE *fp = fopen(FILENAME, "r");
    if (fp) {
        while (fscanf(fp, "%d %d %d %d %d %s %[^\n]", 
                      &events[numEvents].year, &events[numEvents].month, &events[numEvents].day,
                      &events[numEvents].hour, &events[numEvents].minute, events[numEvents].airline,
                      events[numEvents].details) != EOF) {
            numEvents++;
        }
        fclose(fp);
    }

    while (1) {
        int choice = getChoice();
        switch (choice) {
            case 1: addEvent(events, &numEvents); break;
            case 2: generateHTML(events, numEvents); break;
            case 3: 
                fp = fopen(FILENAME, "w");
                if (fp) {
                    for (int i = 0; i < numEvents; i++) {
                        fprintf(fp, "%d %d %d %d %d %s %s\n",
                                events[i].year, events[i].month, events[i].day,
                                events[i].hour, events[i].minute, events[i].airline, events[i].details);
                    }
                    fclose(fp);
                }
                return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
