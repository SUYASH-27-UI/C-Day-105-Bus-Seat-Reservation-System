
#include <stdio.h>
#include <string.h>

#define FILE_NAME "reservations.txt"
#define TOTAL_SEATS 20

struct Reservation {
    int seat;
    char name[50];
};

int findSeat(int seat) {
    struct Reservation r;
    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        return 0;
    }

    while (fscanf(fp, "%d|%49[^\n]\n",
                  &r.seat, r.name) == 2) {
        if (r.seat == seat) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void displaySeats() {
    printf("\n--- BUS SEAT STATUS ---\n");

    for (int i = 1; i <= TOTAL_SEATS; i++) {
        if (findSeat(i)) {
            printf("Seat %02d: Booked\n", i);
        } else {
            printf("Seat %02d: Available\n", i);
        }
    }
}

void reserveSeat() {
    struct Reservation r;

    printf("\nEnter seat number (1-20): ");

    if (scanf("%d", &r.seat) != 1) {
        printf("Invalid seat number.\n");
        return;
    }

    if (r.seat < 1 || r.seat > TOTAL_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (findSeat(r.seat)) {
        printf("This seat is already booked.\n");
        return;
    }

    printf("Enter passenger name: ");
    scanf(" %49[^\n]", r.name);

    if (strchr(r.name, '|') != NULL) {
        printf("Invalid passenger name.\n");
        return;
    }

    FILE *fp = fopen(FILE_NAME, "a");

    if (fp == NULL) {
        printf("Unable to open reservation file.\n");
        return;
    }

    fprintf(fp, "%d|%s\n", r.seat, r.name);
    fclose(fp);

    printf("Seat %d reserved successfully for %s.\n",
           r.seat, r.name);
}

void searchReservation() {
    struct Reservation r;
    int seat;
    int found = 0;

    printf("\nEnter seat number to search: ");

    if (scanf("%d", &seat) != 1) {
        printf("Invalid seat number.\n");
        return;
    }

    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("No reservations found.\n");
        return;
    }

    while (fscanf(fp, "%d|%49[^\n]\n",
                  &r.seat, r.name) == 2) {

        if (r.seat == seat) {
            printf("\nReservation Found!\n");
            printf("Seat Number: %d\n", r.seat);
            printf("Passenger Name: %s\n", r.name);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No reservation found for this seat.\n");
    }
}

void cancelReservation() {
    struct Reservation r;
    int seat;
    int found = 0;

    printf("\nEnter seat number to cancel: ");

    if (scanf("%d", &seat) != 1) {
        printf("Invalid seat number.\n");
        return;
    }

    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("No reservations found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");

    if (temp == NULL) {
        printf("Unable to create temporary file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d|%49[^\n]\n",
                  &r.seat, r.name) == 2) {

        if (r.seat == seat) {
            found = 1;
        } else {
            fprintf(temp, "%d|%s\n", r.seat, r.name);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        if (remove(FILE_NAME) != 0 ||
            rename("temp.txt", FILE_NAME) != 0) {
            printf("Error updating reservation file.\n");
            return;
        }

        printf("Reservation cancelled successfully.\n");
    } else {
        remove("temp.txt");
        printf("Reservation not found.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n===== BUS RESERVATION SYSTEM =====\n");
        printf("1. Display Seats\n");
        printf("2. Reserve Seat\n");
        printf("3. Search Reservation\n");
        printf("4. Cancel Reservation\n");
        printf("5. Exit\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            break;
        }

        switch (choice) {
            case 1:
                displaySeats();
                break;

            case 2:
                reserveSeat();
                break;

            case 3:
                searchReservation();
                break;

            case 4:
                cancelReservation();
                break;

            case 5:
                printf("Bus reservation system closed.\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
