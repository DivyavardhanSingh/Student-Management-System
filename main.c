#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define FILENAME "students.dat"

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int age;
    float grade;
} Student;

void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();
void saveStudent(Student s);
void loadStudents(Student *students, int *count);

int main() {
    int choice;

    do {
        printf("Student Management System\n");
        printf("1. Add a new student\n");
        printf("2. View all students\n");
        printf("3. Update student information\n");
        printf("4. Delete a student record\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character left by scanf

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void addStudent() {
    Student s;
    FILE *file = fopen(FILENAME, "ab");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    printf("Enter student ID: ");
    scanf("%d", &s.id);
    getchar(); // To consume the newline character left by scanf
    printf("Enter student name: ");
    fgets(s.name, MAX_NAME_LEN, stdin);
    s.name[strcspn(s.name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter student age: ");
    scanf("%d", &s.age);
    printf("Enter student grade: ");
    scanf("%f", &s.grade);

    fwrite(&s, sizeof(Student), 1, file);
    fclose(file);
    printf("Student added successfully!\n");
}

void viewStudents() {
    Student s;
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    printf("Student Records:\n");
    while (fread(&s, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s, Age: %d, Grade: %.2f\n", s.id, s.name, s.age, s.grade);
    }

    fclose(file);
}

void updateStudent() {
    int id, found = 0;
    Student s;
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    printf("Enter student ID to update: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id == id) {
            found = 1;
            printf("Enter new student name: ");
            getchar(); // To consume the newline character left by scanf
            fgets(s.name, MAX_NAME_LEN, stdin);
            s.name[strcspn(s.name, "\n")] = '\0'; // Remove trailing newline
            printf("Enter new student age: ");
            scanf("%d", &s.age);
            printf("Enter new student grade: ");
            scanf("%f", &s.grade);

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, file);
            printf("Student record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student ID not found.\n");
    }

    fclose(file);
}

void deleteStudent() {
    int id, found = 0;
    Student s;
    FILE *file = fopen(FILENAME, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");

    if (!file || !tempFile) {
        perror("Failed to open file");
        return;
    }

    printf("Enter student ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(Student), 1, file)) {
        if (s.id != id) {
            fwrite(&s, sizeof(Student), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(FILENAME);
        rename("temp.dat", FILENAME);
        printf("Student record deleted successfully!\n");
    } else {
        printf("Student ID not found.\n");
        remove("temp.dat");
    }
}
