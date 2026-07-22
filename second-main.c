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

// ==================== فراموشی رمز ====================

void forgot_password() {
    char username[20];
    char answer1[MAX_STRING], answer2[MAX_STRING], answer3[MAX_STRING];
    int student_index;
    int retry_main = 1;

    while(retry_main) {
        clear_screen();
        printf("\n========== FORGOT PASSWORD ==========\n\n");
        printf("Enter your username: ");
        scanf("%s", username);

        // جستجو در دانشجویان
        student_index = find_student_by_username(username);

        if(student_index == -1) {
            printf("\nUsername not found.\n");
            printf("\n1. Retry\n");
            printf("2. Go to login menu\n");
            printf("Enter an option: ");
            int option;
            scanf("%d", &option);
            if(option == 2) {
                return;  // بازگشت به منوی اصلی
            }
            continue;  // Retry
        }

        // ========== پرسیدن سوالات امنیتی ==========
        int retry_questions = 1;

        while(retry_questions) {
            clear_screen();
            printf("\n========== FORGOT PASSWORD ==========\n\n");
            printf("Enter your username: %s\n", username);

            // سوال اول
            printf("%s ", students[student_index].security_q1);
            scanf(" %[^\n]", answer1); // یعنی هر انچه جز \n بود بخوان

            if(strcmp(students[student_index].security_a1, answer1) != 0) {
                printf("\nIncorrect answer.\n");
                printf("\n1. Retry\n");
                printf("2. Go to login menu\n");
                printf("Enter an option: ");
                int option;
                scanf("%d", &option);
                if(option == 2) {
                    return;  // بازگشت به منوی اصلی
                }
                continue;  // دوباره سوالات
            }

            // سوال دوم
            printf("%s ", students[student_index].security_q2);
            scanf(" %[^\n]", answer2);

            if(strcmp(students[student_index].security_a2, answer2) != 0) {
                printf("\nIncorrect answer.\n");
                printf("\n1. Retry\n");
                printf("2. Go to login menu\n");
                printf("Enter an option: ");
                int option;
                scanf("%d", &option);
                if(option == 2) {
                    return;
                }
                continue;
            }

            // سوال سوم
            printf("%s ", students[student_index].security_q3);
            scanf(" %[^\n]", answer3);

            if(strcmp(students[student_index].security_a3, answer3) != 0) {
                printf("\nIncorrect answer.\n");
                printf("\n1. Retry\n");
                printf("2. Go to login menu\n");
                printf("Enter an option: ");
                int option;
                scanf("%d", &option);
                if(option == 2) {
                    return;
                }
                continue;
            }

            // ========== اگر همه پاسخ‌ها درست بود ==========
            printf("\nAuthentication successful.\n");

            // گرفتن رمز جدید
            char new_password[MAX_STRING];
            char confirm_password[MAX_STRING];
            int retry_password = 1;

            while(retry_password) {
                printf("Enter your new password: ");
                scanf("%s", new_password);
                printf("Confirm your password: ");
                scanf("%s", confirm_password);

                if(strcmp(new_password, confirm_password) == 0) {
                    strcpy(students[student_index].password, new_password);
                    printf("\nPassword changed successfully.\n");
                    save_data();  // ذخیره در فایل
                    printf("Press any key to go to login menu...");
                    getchar();
                    getchar();
                    return;  // بازگشت به منوی اصلی
                } else {
                    printf("\nPasswords aren't matching.\n");
                    printf("\n1. Retry.\n");
                    printf("2. Cancel (go to login menu).\n");
                    printf("Enter an option: ");
                    int option;
                    scanf("%d", &option);
                    if(option == 2) {
                        return;  // بازگشت به منوی اصلی
                    }
                    // اگر option == 1، حلقه ادامه پیدا می‌کند
                }
            }

            retry_questions = 0;  // از حلقه سوالات خارج شو
        }

        retry_main = 0;  // از حلقه اصلی خارج شو
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

// ==================== توابع بارگذاری و ذخیره‌سازی ====================

// ---------- بارگذاری دانشجویان از فایل ----------
void load_students() {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No students file found. Starting with empty student list.\n");
        student_count = 0;
        return;
    }

    student_count = 0;
    char line[1000];  // برای خواندن هر خط

    while (fgets(line, sizeof(line), file) != NULL && student_count < MAX_STUDENTS) {
        // حذف newline از انتهای خط
        line[strcspn(line, "\n")] = '\0';

        // پارس کردن خط با جداکننده |
        char* token = strtok(line, "|"); // تکه کردن رشته ی هر دانشجو بر اساس | ، * برای رشته
        if (token == NULL) continue;

        Student* s = &students[student_count]; // پوینتر برای پیدا کردن دانشجو با پوینتر و روش ->

        // خواندن فیلدها به ترتیب
        strcpy(s->student_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->first_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->last_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->national_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->major, token);

        token = strtok(NULL, "|");
        if (token != NULL) s->entry_year = atoi(token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->degree, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->supervisor, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->department, token);

        // سوالات امنیتی
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_q1, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_a1, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_q2, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_a2, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_q3, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->security_a3, token);

        // رمز عبور
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(s->password, token);

        // تعداد دروس گرفته شده
        token = strtok(NULL, "|");
        if (token != NULL) {
            s->enrollment_count = atoi(token); // تبدیل رشته به int
        } else {
            s->enrollment_count = 0;
        }

        // خواندن دروس گرفته شده (offering_id و نمره)
        for (int i = 0; i < s->enrollment_count && i < MAX_ENROLLMENTS; i++) {
            token = strtok(NULL, "|");
            if (token != NULL) {
                s->enrollments[i].offering_id = atoi(token); // تبدیل رشته به int
            }

            token = strtok(NULL, "|");
            if (token != NULL) {
                s->enrollments[i].grade = atof(token); // تبدیل رشته به float
            }
        }

        student_count++;
    }

    fclose(file);
    printf("Loaded %d students.\n", student_count);
}

// ---------- ذخیره‌سازی دانشجویان در فایل ----------
void save_students() {
    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Error: Cannot open students.txt for writing.\n");
        return;
    }

    for (int i = 0; i < student_count; i++) {
        Student* s = &students[i];

        // نوشتن اطلاعات پایه
        fprintf(file, "%s|%s|%s|%s|%s|%d|%s|%s|%s|",
                s->student_id,
                s->first_name,
                s->last_name,
                s->national_id,
                s->major,
                s->entry_year,
                s->degree,
                s->supervisor,
                s->department);

        // سوالات امنیتی
        fprintf(file, "%s|%s|%s|%s|%s|%s|",
                s->security_q1,
                s->security_a1,
                s->security_q2,
                s->security_a2,
                s->security_q3,
                s->security_a3);

        // رمز عبور
        fprintf(file, "%s|", s->password);

        // تعداد دروس گرفته شده
        fprintf(file, "%d|", s->enrollment_count);

        // دروس گرفته شده
        for (int j = 0; j < s->enrollment_count && j < MAX_ENROLLMENTS; j++) {
            fprintf(file, "%d|%.2f|",
                    s->enrollments[j].offering_id,
                    s->enrollments[j].grade);
        }

        fprintf(file, "\n");
    }

    fclose(file);
    printf("Saved %d students.\n", student_count);
}

// ---------- بارگذاری اساتید از فایل ----------
void load_faculty() {
    FILE* file = fopen("faculty.txt", "r");
    if (file == NULL) {
        printf("No faculty file found. Starting with empty faculty list.\n");
        faculty_count = 0;
        return;
    }

    faculty_count = 0;
    char line[500];

    while (fgets(line, sizeof(line), file) != NULL && faculty_count < MAX_FACULTY) {
        line[strcspn(line, "\n")] = '\0';

        char* token = strtok(line, "|");
        if (token == NULL) continue;

        Faculty* f = &faculties[faculty_count];

        // کد استادی
        strcpy(f->faculty_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->first_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->last_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->national_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->department, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->major, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(f->password, token);

        faculty_count++;
    }

    fclose(file);
    printf("Loaded %d faculty members.\n", faculty_count);
}

// ---------- ذخیره‌سازی اساتید در فایل ----------
void save_faculty() {
    FILE* file = fopen("faculty.txt", "w");
    if (file == NULL) {
        printf("Error: Cannot open faculty.txt for writing.\n");
        return;
    }

    for (int i = 0; i < faculty_count; i++) {
        Faculty* f = &faculties[i];
        fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n",
                f->faculty_id,
                f->first_name,
                f->last_name,
                f->national_id,
                f->department,
                f->major,
                f->password);
    }

    fclose(file);
    printf("Saved %d faculty members.\n", faculty_count);
}

// ---------- تابع اصلی بارگذاری دانشجو/استاد ---------- باید ببینم باشه یا نه
void load_data() {
    printf("\n=== Loading Data ===\n");
    load_students();
    load_faculty();
    printf("=== Data Load Complete ===\n\n");
}

// ---------- تابع اصلی ذخیره‌سازی دانشجو/استاد ----------
void save_data() {
    printf("\n=== Saving Data ===\n");
    save_students();
    save_faculty();
    save_settings();
    printf("=== Data Save Complete ===\n\n");
}

// $$$$$$$$$$$ توابع داشبورد $$$$$$$$$$$$$
// ADMIN DASHBOARD
// نوشتن آپشن اول از پنل ادمین &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// ==================== مدیریت تقویم توسط ادمین ====================

typedef struct {
    int offering_enabled;        // 1 = فعال, 0 = غیرفعال
    int unit_selection_enabled;
    int class_exams_enabled;
    int grade_recording_enabled;
} SemesterSettings;

SemesterSettings settings = {0, 0, 0, 0};  // همه غیرفعالن در اجرای اول

// ==================== توابع بارگذاری برای فایل setting و ذخیره‌سازی تنظیمات ====================
// فایل setting محل نگه داری تایم های فعال شده/نشده است.
void load_settings() {
    FILE* file = fopen("settings.txt", "r");
    if (file == NULL) {
        // اگر فایل وجود نداشت، همه را غیرفعال کن
        settings.offering_enabled = 0;
        settings.unit_selection_enabled = 0;
        settings.class_exams_enabled = 0;
        settings.grade_recording_enabled = 0;
        printf("No settings file found. Starting with all features disabled.\n");
        return;
    }

    fscanf(file, "%d|%d|%d|%d",
           &settings.offering_enabled,
           &settings.unit_selection_enabled,
           &settings.class_exams_enabled,
           &settings.grade_recording_enabled);

    fclose(file);
    printf("Settings loaded successfully.\n");
}

void save_settings() {
    FILE* file = fopen("settings.txt", "w");
    if (file == NULL) {
        printf("Error: Cannot open settings.txt for writing.\n");
        return;
    }

    fprintf(file, "%d|%d|%d|%d",
            settings.offering_enabled,
            settings.unit_selection_enabled,
            settings.class_exams_enabled,
            settings.grade_recording_enabled);

    fclose(file);
    printf("Settings saved successfully.\n");
}
// ==================== مدیریت تقویم توسط ادمین ====================
void admin_calendar() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========== CALENDAR MANAGEMENT ==========\n\n");
        printf("1. offering: %s\n", settings.offering_enabled ? "enabled" : "disabled");
        printf("2. unit selection: %s\n", settings.unit_selection_enabled ? "enabled" : "disabled");
        printf("3. class & exams: %s\n", settings.class_exams_enabled ? "enabled" : "disabled");
        printf("4. grade recording: %s\n", settings.grade_recording_enabled ? "enabled" : "disabled");
        printf("5. go to main menu\n");
        printf("\nEnter a time to trigger: ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                settings.offering_enabled = !settings.offering_enabled;
                save_settings();
                break;

            case 2:
                if (settings.unit_selection_enabled == 0) {
                    // الان میخوایم فعال کنیم
                    if (settings.offering_enabled == 1) {
                        settings.unit_selection_enabled = 1;
                        save_settings();
                    } else {
                        printf("\nError: Cannot enable unit selection. Offering is disabled.\n");
                        press_any_key();
                    }
                } else {
                    // الان میخوایم غیرفعال کنیم
                    if (settings.class_exams_enabled == 0) {
                        settings.unit_selection_enabled = 0;
                        save_settings();
                    } else {
                        printf("\nError: Cannot disable unit selection. Class & exams is enabled.\n");
                        press_any_key();
                    }
                }
                break;

            case 3:  // Toggle class & exams
                if (settings.class_exams_enabled == 0) {
                    // می‌خواهیم فعال کنیم
                    if (settings.unit_selection_enabled == 1) {
                        settings.class_exams_enabled = 1;
                        save_settings();
                    } else {
                        printf("\nError: Cannot enable class & exams. Unit selection is disabled.\n");
                        press_any_key();
                    }
                } else {
                    // می‌خواهیم غیرفعال کنیم
                    if (settings.grade_recording_enabled == 0) {
                        settings.class_exams_enabled = 0;
                        save_settings();
                    } else {
                        printf("\nError: Cannot disable class & exams. Grade recording is enabled.\n");
                        press_any_key();
                    }
                }
                break;

            case 4:  // Toggle grade recording
                if (settings.grade_recording_enabled == 0) {
                    // می‌خواهیم فعال کنیم
                    if (settings.class_exams_enabled == 1) {
                        settings.grade_recording_enabled = 1;
                        save_settings();
                    } else {
                        printf("\nError: Cannot enable grade recording. Class & exams is disabled.\n");
                        press_any_key();
                    }
                } else {
                    // می‌خواهیم غیرفعال کنیم
                    settings.grade_recording_enabled = 0;
                    save_settings();
                }
                break;

            case 5:
                printf("\nReturning to main menu...\n");
                press_any_key();
                return;

            default:
                printf("\nInvalid option!\n");
                press_any_key();
        }
    }
}
// نوشتن آپشن دوم از پنل ادمین &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

// ==================== تابع جستجو در دانشجویان ====================

void search_students() {
    char keyword[MAX_STRING];
    int found = 0;

    clear_screen();
    printf("\n========== SEARCH STUDENTS ==========\n\n");
    printf("Enter keyword to search: ");
    scanf(" %[^\n]", keyword);

    printf("\nSearch results for: \"%s\"\n\n", keyword);

    for (int i = 0; i < student_count; i++) {
        // جستجو در فیلدهای مختلف
        if (strstr(students[i].student_id, keyword) != NULL ||
            strstr(students[i].first_name, keyword) != NULL ||
            strstr(students[i].last_name, keyword) != NULL ||
            strstr(students[i].major, keyword) != NULL ||
            strstr(students[i].department, keyword) != NULL) {

            printf("|%s |%s |%s |%s |%s |%d |%s |%s |%s |\n",
                   students[i].first_name,
                   students[i].last_name,
                   students[i].student_id,
                   students[i].national_id,
                   students[i].major,
                   students[i].entry_year,
                   students[i].degree,
                   students[i].supervisor,
                   students[i].department);
            found = 1;
            }
    }

    if (!found) {
        printf("No students found matching \"%s\"\n", keyword);
    }

    printf("\nPress any key to continue...");
    getchar();
    getchar();
}

// ==================== نمایش لیست دانشجویان ====================

void show_students_list() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========== STUDENTS LIST ==========\n\n");

        // نمایش همه دانشجویان
        if (student_count == 0) {
            printf("No students found.\n");
        } else {
            printf("Total students: %d\n\n", student_count);

            // نمایش هر دانشجو در یک خط با فرمت ساده (مثل خروجی)
            for (int i = 0; i < student_count; i++) {
                printf("|%s |%s |%s |%s |%s |%d |%s |%s |%s |\n",
                       students[i].first_name,
                       students[i].last_name,
                       students[i].student_id,
                       students[i].national_id,
                       students[i].major,
                       students[i].entry_year,
                       students[i].degree,
                       students[i].supervisor,
                       students[i].department);
            }
        }

        printf("\n1. Search\n");
        printf("2. Go back\n");
        printf("\nEnter an option: ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                search_students();
                break;
            case 2:
                return;
            default:
                printf("\nInvalid option!\n");
                press_any_key();
        }
    }
}

// ==================== ثبت نام دانشجو ====================
// ==================== ثبت یک دانشجو ====================

void register_one_student() {
    Student new_student;
    char confirm;

    clear_screen();
    printf("\n========== REGISTER ONE STUDENT ==========\n\n");

    // بررسی اینکه جا برای دانشجو وجود دارد
    if (student_count >= MAX_STUDENTS) {
        printf("Error: Maximum number of students reached (%d).\n", MAX_STUDENTS);
        press_any_key();
        return;
    }

    printf("Enter student ID: ");
    scanf("%s", new_student.student_id);

    // بررسی اینکه دانشجو تکراری نباشد
    if (find_student_by_username(new_student.student_id) != -1) {
        printf("Error: Student with ID %s already exists!\n", new_student.student_id);
        press_any_key();
        return;
    }

    printf("Enter first name: ");
    scanf("%s", new_student.first_name);

    printf("Enter last name: ");
    scanf("%s", new_student.last_name);

    printf("Enter national ID: ");
    scanf("%s", new_student.national_id);

    printf("Enter major: ");
    scanf(" %[^\n]", new_student.major);

    printf("Enter entry year: ");
    scanf("%d", &new_student.entry_year);

    printf("Enter degree (BSc/MSc/PhD): ");
    scanf("%s", new_student.degree);

    printf("Enter supervisor: ");
    scanf(" %[^\n]", new_student.supervisor);

    printf("Enter department: ");
    scanf(" %[^\n]", new_student.department);

    // سوالات امنیتی
    printf("\n--- Security Questions ---\n");
    printf("Enter security question 1: ");
    scanf(" %[^\n]", new_student.security_q1);
    printf("Enter answer 1: ");
    scanf(" %[^\n]", new_student.security_a1);

    printf("Enter security question 2: ");
    scanf(" %[^\n]", new_student.security_q2);
    printf("Enter answer 2: ");
    scanf(" %[^\n]", new_student.security_a2);

    printf("Enter security question 3: ");
    scanf(" %[^\n]", new_student.security_q3);
    printf("Enter answer 3: ");
    scanf(" %[^\n]", new_student.security_a3);

    printf("\nEnter password: ");
    scanf("%s", new_student.password);

    new_student.enrollment_count = 0;  // تازه ثبت‌نام شده، هیچ درسی ندارد

    // نمایش خلاصه و تایید
    printf("\n--- Student Information Summary ---\n");
    printf("ID: %s\n", new_student.student_id);
    printf("Name: %s %s\n", new_student.first_name, new_student.last_name);
    printf("Major: %s\n", new_student.major);
    printf("Degree: %s\n", new_student.degree);

    printf("\nRegister this student? [y/n]: ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        students[student_count] = new_student;
        student_count++;
        save_data();
        printf("\nStudent registered successfully!\n");
    } else {
        printf("\nRegistration cancelled.\n");
    }

    press_any_key();
}

// ==================== ثبت گروهی دانشجویان (از فایل) ====================

void register_group_students() {
    char filename[MAX_STRING];
    FILE* file;
    char line[1000];
    int added_count = 0;

    clear_screen();
    printf("\n========== REGISTER GROUP STUDENTS ==========\n\n");
    printf("Enter the filename to import (for example : students_import.txt): ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        press_any_key();
        return;
    }

    printf("\nImporting students from %s...\n", filename); //  که اگه طول کشید اینو نشون بده

    while (fgets(line, sizeof(line), file) != NULL && student_count < MAX_STUDENTS) {
        line[strcspn(line, "\n")] = '\0';

        Student new_student;
        char* token = strtok(line, "|");
        if (token == NULL) continue;

        // خواندن فیلدها به همان ترتیب فایل
        strcpy(new_student.student_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.first_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.last_name, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.national_id, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.major, token);

        token = strtok(NULL, "|");
        if (token != NULL) new_student.entry_year = atoi(token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.degree, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.supervisor, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.department, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_q1, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_a1, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_q2, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_a2, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_q3, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.security_a3, token);

        token = strtok(NULL, "|");
        if (token != NULL) strcpy(new_student.password, token);

        new_student.enrollment_count = 0;

        // بررسی تکراری نبودن
        if (find_student_by_username(new_student.student_id) == -1) {
            students[student_count] = new_student;
            student_count++;
            added_count++;
        } else {
            printf("Warning: Student %s already exists. Skipping...\n", new_student.student_id);
        }
    }

    fclose(file);

    if (added_count > 0) {
        save_data();
        printf("\nSuccessfully imported %d students.\n", added_count);
    } else {
        printf("\nNo new students were imported.\n");
    }

    press_any_key();
}



void register_students() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========== REGISTER STUDENTS ==========\n\n");
        printf("1. Register one student\n");
        printf("2. Register a group of students (import a file)\n");
        printf("3. Go back\n");
        printf("\nEnter an option: ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                register_one_student();
                break;
            case 2:
                register_group_students();
                break;
            case 3:
                return;
            default:
                printf("\nInvalid option!\n");
                press_any_key();
        }
    }
}


// ==================== منوی دانشجویان پنل ادمین ====================
void admin_students() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========== STUDENTS MANAGEMENT ==========\n\n");
        printf("1. Students list\n");
        printf("2. Register student(s)\n");
        printf("3. Remove student(s)\n");
        printf("4. Go back\n");
        printf("\nEnter an option: ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                show_students_list();
                break;
            case 2:
                register_students();
                break;
            case 3:
                remove_student(); // مانده
                break;
            case 4:
                printf("\nReturning to main menu...\n");
                press_any_key();
                return;
            default:
                printf("\nInvalid option!\n");
                press_any_key();
        }
    }
}


//++++++++++++++++++++++++=
void admin_dashboard() {
    int option;

    while(1) {
        clear_screen();
        printf("\n========== ADMIN DASHBOARD ==========\n");
        printf("Welcome, %s!\n\n", current_username);
        printf("1. Calendar\n");
        printf("2. Students\n");
        printf("3. Faculty members\n");
        printf("4. Requests\n");
        printf("5. Offerings\n");
        printf("6. Courses\n");
        printf("7. Log out\n");
        printf("\nEnter an option: ");

        scanf("%d", &option);

        switch(option) {
            case 1:
                admin_calendar();
                break;
            case 2:
                admin_students();
                break;
            case 3:
                press_any_key();
                break;
            case 4:
                press_any_key();
                break;
            case 5:
                press_any_key();
                break;
            case 6:
                press_any_key();
                break;
            case 7:
                press_any_key();
                return;
            default:
                printf("\nInvalid option!\n");
                press_any_key();
        }
    }
}




int main() {
    // بارگذاری داده‌ها از فایل
    load_data();
    load_settings();

    // نمایش منوی اصلی
    show_main_menu();

    return 0;
}