// PROG 102 Project - Smart University Student Management and Result System

#include <stdio.h>
#include <string.h>

#define MAX 100
#define SUB 5
#define SEM 8

int id[MAX];
char name[MAX][50];
char dept[MAX][50];

int marks[MAX][SEM][SUB];
int hasResult[MAX][SEM];

int count = 0;

char subjects[SUB][15] = {
    "DS 107",
    "IRE 103",
    "IRE 101",
    "PROG 101",
    "PHY 105"
};

int credits[SUB] = {3, 3, 3, 3, 3};


void clearInput()
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);
}


void getInput(char message[], char text[], int size)
{
    do
    {
        printf("%s", message);
        fgets(text, size, stdin);

        text[strcspn(text, "\n")] = '\0';

    } while (strlen(text) == 0);
}


// takes only number input, if user type letter it will ask again
int readInt()
{
    int n, r;

    while ((r = scanf("%d", &n)) != 1)
    {
        if (r == EOF)
            return 10;

        clearInput();
        printf("Please enter a valid number: ");
    }

    clearInput();
    return n;
}


// search student by id
int searchID(int num)
{
    int i;

    for (i = 0; i < count; i++)
    {
        int *p = &id[i];

        if (*p == num)
            return i;
    }

    return -1;
}


char *getGrade(int mark)
{
    if (mark >= 80)
        return "A+";
    else if (mark >= 75)
        return "A";
    else if (mark >= 70)
        return "A-";
    else if (mark >= 65)
        return "B+";
    else if (mark >= 60)
        return "B";
    else if (mark >= 55)
        return "B-";
    else if (mark >= 50)
        return "C+";
    else if (mark >= 45)
        return "C";
    else if (mark >= 40)
        return "D";
    else
        return "F";
}


float getPoint(int mark)
{
    if (mark >= 80)
        return 4.00;
    else if (mark >= 75)
        return 3.75;
    else if (mark >= 70)
        return 3.50;
    else if (mark >= 65)
        return 3.25;
    else if (mark >= 60)
        return 3.00;
    else if (mark >= 55)
        return 2.75;
    else if (mark >= 50)
        return 2.50;
    else if (mark >= 45)
        return 2.25;
    else if (mark >= 40)
        return 2.00;
    else
        return 0.00;
}


int checkResult(int index)
{
    int i;

    for (i = 0; i < SEM; i++)
    {
        if (hasResult[index][i] == 1)
            return 1;
    }

    return 0;
}


// sgpa of one semester
float getSGPA(int index, int sem)
{
    float totalPoints = 0;
    int totalCredits = 0;
    int j;

    for (j = 0; j < SUB; j++)
    {
        totalPoints += getPoint(marks[index][sem][j])
                       * credits[j];

        totalCredits += credits[j];
    }

    return totalPoints / totalCredits;
}


// cgpa of all semesters which have result
float getCGPA(int index)
{
    float totalPoints = 0;
    int totalCredits = 0;
    int i, j;

    for (i = 0; i < SEM; i++)
    {
        if (hasResult[index][i] == 1)
        {
            for (j = 0; j < SUB; j++)
            {
                totalPoints += getPoint(marks[index][i][j])
                               * credits[j];

                totalCredits += credits[j];
            }
        }
    }

    if (totalCredits == 0)
        return 0;

    return totalPoints / totalCredits;
}


void addStudent()
{
    int num;

    if (count >= MAX)
    {
        printf("\nStudent limit is full!\n");
        return;
    }

    printf("\nEnter Student ID: ");
    num = readInt();

    if (searchID(num) != -1)
    {
        printf("\nThis ID already exists!\n");
        return;
    }

    id[count] = num;

    getInput("Enter Name: ", name[count], 50);
    getInput("Enter Department: ", dept[count], 50);

    int i, j;

    for (i = 0; i < SEM; i++)
    {
        hasResult[count][i] = 0;

        for (j = 0; j < SUB; j++)
        {
            marks[count][i][j] = -1;
        }
    }

    count++;

    printf("\nStudent added successfully!\n");
}


void viewStudents()
{
    int i;

    if (count == 0)
    {
        printf("\nNo student found!\n");
        return;
    }

    printf("\nAll Student Information\n");

    for (i = 0; i < count; i++)
    {
        printf("\nID: %d\n", id[i]);
        printf("Name: %s\n", name[i]);
        printf("Department: %s\n", dept[i]);

        if (checkResult(i))
            printf("CGPA: %.2f\n", getCGPA(i));
        else
            printf("CGPA: Not Available\n");
    }
}


void enterMarks()
{
    int num, index, sem;
    int i, mark;

    printf("\nEnter Student ID: ");
    num = readInt();

    index = searchID(num);

    if (index == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    printf("Enter Semester (1-8): ");
    sem = readInt();

    if (sem < 1 || sem > SEM)
    {
        printf("\nInvalid Semester!\n");
        return;
    }

    sem--;

    printf("\nEnter Subject Marks\n");

    for (i = 0; i < SUB; i++)
    {
        while (1)
        {
            printf("%s: ", subjects[i]);
            mark = readInt();

            if (mark < 0 || mark > 100)
            {
                printf("Enter marks between 0 and 100.\n");
                continue;
            }

            marks[index][sem][i] = mark;
            break;
        }
    }

    hasResult[index][sem] = 1;

    printf("\nMarks entered successfully!\n");
    printf("SGPA: %.2f\n", getSGPA(index, sem));
    printf("CGPA: %.2f\n", getCGPA(index));
}


void showResult(int index, int sem)
{
    int i;
    int total = 0;
    int fail = 0;

    printf("\nStudent Name: %s\n", name[index]);
    printf("Student ID: %d\n", id[index]);
    printf("Semester: %d\n", sem + 1);

    printf("\nSubject       Marks   Grade   Point\n");

    for (i = 0; i < SUB; i++)
    {
        int m = marks[index][sem][i];

        printf("%-13s %-7d %-7s %.2f\n",
               subjects[i],
               m,
               getGrade(m),
               getPoint(m));

        total += m;

        if (m < 40)
            fail++;
    }

    printf("\nTotal Marks: %d / %d\n", total, SUB * 100);
    printf("Average: %.2f\n", (float) total / SUB);

    printf("SGPA: %.2f\n", getSGPA(index, sem));

    if (fail == 0)
        printf("Result: PASS\n");
    else
        printf("Result: FAIL\n");
}


void viewResult()
{
    int num, index, sem;
    int i, found = 0;

    printf("\nEnter Student ID: ");
    num = readInt();

    index = searchID(num);

    if (index == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    printf("Enter Semester (1-8, 0 for all): ");
    sem = readInt();

    if (sem == 0)
    {
        for (i = 0; i < SEM; i++)
        {
            if (hasResult[index][i] == 1)
            {
                showResult(index, i);
                found = 1;
            }
        }

        if (found == 0)
            printf("\nNo result available!\n");
        else
            printf("\nOverall CGPA: %.2f\n", getCGPA(index));
    }
    else if (sem >= 1 && sem <= SEM)
    {
        sem--;

        if (hasResult[index][sem] == 0)
        {
            printf("\nResult not entered!\n");
            return;
        }

        showResult(index, sem);

        printf("Overall CGPA: %.2f\n", getCGPA(index));
    }
    else
    {
        printf("\nInvalid Semester!\n");
    }
}


void searchStudent()
{
    int num, index;

    printf("\nEnter Student ID: ");
    num = readInt();

    index = searchID(num);

    if (index == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    printf("\nStudent Information\n");
    printf("ID: %d\n", id[index]);
    printf("Name: %s\n", name[index]);
    printf("Department: %s\n", dept[index]);

    if (checkResult(index))
        printf("CGPA: %.2f\n", getCGPA(index));
    else
        printf("CGPA: Not Available\n");
}


void updateStudent()
{
    int num, index;

    printf("\nEnter Student ID: ");
    num = readInt();

    index = searchID(num);

    if (index == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    getInput("Enter New Name: ", name[index], 50);
    getInput("Enter New Department: ", dept[index], 50);

    printf("\nInformation updated successfully!\n");
}


void deleteStudent()
{
    int num, index;
    int i, j, k;

    printf("\nEnter Student ID: ");
    num = readInt();

    index = searchID(num);

    if (index == -1)
    {
        printf("\nStudent not found!\n");
        return;
    }

    // shifting all next students one step left
    for (i = index; i < count - 1; i++)
    {
        id[i] = id[i + 1];

        strcpy(name[i], name[i + 1]);
        strcpy(dept[i], dept[i + 1]);

        for (j = 0; j < SEM; j++)
        {
            hasResult[i][j] = hasResult[i + 1][j];

            for (k = 0; k < SUB; k++)
            {
                marks[i][j][k] = marks[i + 1][j][k];
            }
        }
    }

    count--;

    printf("\nStudent deleted successfully!\n");
}


void studentStatistics()
{
    int i, j;
    int resultCount = 0;
    int totalResults = 0;
    float totalCGPA = 0;

    if (count == 0)
    {
        printf("\nNo student available!\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (checkResult(i))
        {
            totalCGPA += getCGPA(i);
            resultCount++;
        }

        for (j = 0; j < SEM; j++)
        {
            if (hasResult[i][j] == 1)
                totalResults++;
        }
    }

    printf("\nStudent Statistics\n");
    printf("Total Students: %d\n", count);
    printf("Total Semester Results: %d\n", totalResults);

    if (resultCount > 0)
        printf("Average CGPA: %.2f\n",
               totalCGPA / resultCount);
    else
        printf("Average CGPA: Not Available\n");
}


void topStudent()
{
    int i, topIndex = -1;
    float maxCGPA = -1;

    for (i = 0; i < count; i++)
    {
        if (checkResult(i))
        {
            float cgpa = getCGPA(i);

            if (cgpa > maxCGPA)
            {
                maxCGPA = cgpa;
                topIndex = i;
            }
        }
    }

    if (topIndex == -1)
    {
        printf("\nNo result available!\n");
        return;
    }

    printf("\nTop Student\n");
    printf("ID: %d\n", id[topIndex]);
    printf("Name: %s\n", name[topIndex]);
    printf("Department: %s\n", dept[topIndex]);
    printf("CGPA: %.2f\n", maxCGPA);
}


int main()
{
    int choice;

    do
    {
        printf("\nUniversity Management System\n\n");

        printf("1. Add New Student\n");
        printf("2. View All Students\n");
        printf("3. Enter Student Marks\n");
        printf("4. View Student Result\n");
        printf("5. Search Student\n");
        printf("6. Update Student Information\n");
        printf("7. Delete Student\n");
        printf("8. Student Statistics\n");
        printf("9. Top Student\n");
        printf("10. Exit\n");

        printf("\nEnter Your Choice: ");
        choice = readInt();

        switch (choice)
        {
            case 1:
                addStudent();
                break;

            case 2:
                viewStudents();
                break;

            case 3:
                enterMarks();
                break;

            case 4:
                viewResult();
                break;

            case 5:
                searchStudent();
                break;

            case 6:
                updateStudent();
                break;

            case 7:
                deleteStudent();
                break;

            case 8:
                studentStatistics();
                break;

            case 9:
                topStudent();
                break;

            case 10:
                printf("\nThank you!\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 10);

    return 0;
}