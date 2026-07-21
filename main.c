#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// macros to avoid repeating constants
#define MAX_STRING 200
#define MAX_STUDENTS 1000
#define MAX_FACULTY 200
//#define MAX_ADMINS 10
#define MAX_ENROLLMENTS 50

// ==================== Structs ====================

// student struct
typedef struct {
    char first_name[MAX_STRING];
    char last_name[MAX_STRING];
    char student_id[20];        // = username
    char national_id[20];
    char major[MAX_STRING];
    int entry_year;
    char degree[20];            // BSc, MSc, PhD
    char supervisor[MAX_STRING];
    char department[MAX_STRING];

    // security questions
    char security_q1[MAX_STRING];
    char security_a1[MAX_STRING];
    char security_q2[MAX_STRING];
    char security_a2[MAX_STRING];
    char security_q3[MAX_STRING];
    char security_a3[MAX_STRING];

    // enrolled courses دروس گرفته شده
    struct {
        int offering_id;
        float grade;
        int is_taken;  // if enrolled = 1 , else = 0
    } enrollments[MAX_ENROLLMENTS];
    int enrollment_count;

    char password[MAX_STRING];  // student password
} Student;

// faculty structure
typedef struct {
    char first_name[MAX_STRING];
    char last_name[MAX_STRING];
    char faculty_id[20];        //username
    char national_id[20];
    char department[MAX_STRING];
    char major[MAX_STRING];
    char password[MAX_STRING];  // faculty password
} Faculty;

// ساختار ادمین (Hard-coded)
typedef struct {
    char username[20];
    char password[20];
} Admin;

// ==================== متغیرهای سراسری ====================
Student students[MAX_STUDENTS]; // لیستی از دانشجویان
int student_count = 0; // برای اینکه هنگام سرچ و یا حذف و اضافه تا شماره خاصی جلو بری

Faculty faculties[MAX_FACULTY];
int faculty_count = 0;

// ادمین همیشه یک نفر است.
Admin admin = {"admin", "admin123"};

int admin_count = 1;

// دانشجو یا استاد لاگین کرده (برای استفاده در منوها)
char current_username[20];
int current_user_type;  // 1=دانشجو, 2=استاد, 3=ادمین

// ==================== functions ====================
void show_main_menu();
void student_login();
void faculty_login();
void admin_login();
void forgot_password();
int find_student_by_username(char* username);
int find_faculty_by_username(char* username);
void student_dashboard();
void faculty_dashboard();
void admin_dashboard();
void load_data();
void save_data();

// ==================== توابع کمکی ====================

// پاک کردن صفحه (برای زیبایی)
void clear_screen() {
    system("cls");  // برای ویندوز
    // system("clear");  // برای لینوکس
}

// توقف تا فشردن کلید
void press_any_key() {
    printf("\nPress any key to continue...");
    getchar();
    getchar();
}


//======== توابع اصلی =============
void show_main_menu() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========================================\n");
        printf("      WELCOME TO EDU SYSTEM\n");
        printf("========================================\n\n");
        printf("1. Login as student\n");
        printf("2. Login as faculty\n");
        printf("3. Login as admin\n");
        printf("4. Forgot password\n");
        printf("5. Exit\n");
        printf("\nEnter an option : ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                student_login();
                break;
            case 2:
                faculty_login();
                break;
            case 3:
                admin_login();
                break;
            case 4:
                forgot_password();
                break;
            case 5:
                printf("\nGoodbye!\n");
                save_data();  // ذخیره داده‌ها قبل از خروج
                exit(0);
            default:
                printf("\nInvalid option! Please try again.\n");
                press_any_key();
        }
    }
}
//==============================

void student_login() {
    char username[20];
    char password[20];
    int index;
    int retry = 1;

    while(retry) {
        clear_screen();
        printf("\n========== STUDENT LOGIN ==========\n\n");
        printf("Enter your username: ");
        scanf("%s", username);

        // پیدا کردن دانشجو
        index = find_student_by_username(username);

        if(index == -1) {
            printf("\nUsername not found.\n");
            printf("\n1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int option;
            scanf("%d", &option);
            if(option == 2) {
                return;  // بازگشت به منوی اصلی
            }
            // اگر option == 1، حلقه ادامه پیدا می‌کند (Retry)
            continue;
        }

        // اگر یوزرنیم پیدا شد، پسورد را بگیر
        printf("Enter password: ");
        scanf("%s", password);

        if(strcmp(students[index].password, password) == 0) {
            printf("\nLogin successful!\n");
            strcpy(current_username, username); // برای نشان دادن در تیتر صفحه ورودی دانشجو
            current_user_type = 1;
            press_any_key();
            student_dashboard();  // رفتن به داشبورد دانشجو
            return;  // بعد از خروج از داشبورد، به منوی اصلی برمی‌گردد
        } else {
            printf("\nIncorrect password.\n");
            printf("\n1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int option;
            scanf("%d", &option);
            if(option == 2) {
                return;  // بازگشت به منوی اصلی
            }
            //  چون retry همواره یک است اگر option == 1، حلقه ادامه پیدا می‌کند (Retry)
        }
    }
}


void faculty_login() {
    char username[20];
    char password[20];
    int index;
    int retry = 1;

    while(retry) {
        clear_screen();
        printf("\n========== FACULTY LOGIN ==========\n\n");
        printf("Enter your username: ");
        scanf("%s", username);

        // پیدا کردن استاد
        index = find_faculty_by_username(username);

        if(index == -1) {
            printf("\nUsername not found.\n");
            printf("\n1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int option;
            scanf("%d", &option);
            if(option == 2) {
                return;  // بازگشت به منوی اصلی
            }
            // اگر option == 1، حلقه ادامه پیدا می‌کند (Retry)
            continue;
        }

        // اگر یوزرنیم پیدا شد، پسورد را بگیر
        printf("Enter password: ");
        scanf("%s", password);

        if(strcmp(faculties[index].password, password) == 0) {
            printf("\nLogin successful!\n");
            strcpy(current_username, username);
            current_user_type = 2;
            press_any_key();
            faculty_dashboard();  // رفتن به داشبورد استاد
            return;  // بعد از خروج از داشبورد، به منوی اصلی برمی‌گردد
        } else {
            printf("\nIncorrect password.\n");
            printf("\n1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int option;
            scanf("%d", &option);
            if(option == 2) {
                return;  // بازگشت به منوی اصلی
            }
            // اگر option == 1، حلقه ادامه پیدا می‌کند (Retry)
        }
    }
}


//==================
void admin_login() {
    char username[20];
    char password[20];
    int retry = 1;

    while(retry) {
        clear_screen();
        printf("\n========== ADMIN LOGIN ==========\n\n");
        printf("Enter your username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        // بررسی ادمین (Hard-coded)
        if(strcmp(admin.username, username) == 0 &&
           strcmp(admin.password, password) == 0) {
            printf("\nLogin successful!\n");
            strcpy(current_username, username);
            current_user_type = 3;
            press_any_key();
            admin_dashboard();  // رفتن به داشبورد ادمین
            return;  // بعد از خروج از داشبورد، به منوی اصلی برمی‌گردد
           } else {
               printf("\nIncorrect username or password.\n");
               printf("\n1. Retry\n");
               printf("2. Go to login menu\n");
               printf("Enter an option: ");
               int option;
               scanf("%d", &option);
               if(option == 2) {
                   return;  // بازگشت به منوی اصلی
               }
               // اگر option == 1، حلقه ادامه پیدا می‌کند (Retry)
           }
    }
}