#include <sqlite3.h>
#include <iostream>
#include <cstdlib>
#include <cstring>


// Structure to represent a student
struct student {
  int student_no;
  char name[50];
  int p_marks, d_marks, cm_marks, f_marks, cl_marks;
  float per;
  char grade,gender[10];
  
};

// Global variables
struct student st;
sqlite3 *db;        // Database connection
char *err_msg = 0;  // Error message

// Function to calculate grade based on percentage
void calculate_grade() {
  // Calculate percentage
  st.per =
      (st.p_marks + st.d_marks + st.cm_marks + st.f_marks + st.cl_marks) / 5.0;
  // Assign grade based on percentage
  
//  if (st.per >= 90)
//    st.grade = 'A';
  if (st.per >= 80 && st.per <= 100)
    st.grade = 'A';
else if (st.per >= 70 && st.per < 80)
    st.grade = 'B';
else if (st.per >= 60 && st.per < 70)
    st.grade = 'C';
else if (st.per >= 50 && st.per < 60)
    st.grade = 'D';
else
    st.grade = 'F';
}

// Function to write student data to database
void write_student() {
  // Prompt user to enter student details

  std::cout << "\nPlease Enter The New Details of student \n";
  std::string student_no_str;
  std::cout << "\nEnter The Student Number of student: ";
  std::cin >> student_no_str;
  long long student_no = std::stoll(student_no_str);
  st.student_no = student_no;
  std::cin.ignore();
  std::cout << "\n\nEnter The Name of student: ";
  std::cin.getline(st.name, 50);
  std::cout << "\nEnter Gender of student (Male/Female): ";
  std::cin.getline(st.gender, 10);
  std::cout << "\nEnter The marks in Probability Theory  out of 100: ";
  std::cin >> st.p_marks;
  std::cout << "\nEnter The marks in Descriptive Statistics  out of 100: ";
  std::cin >> st.d_marks;
  std::cout << "\nEnter The marks in Calculus out of 100: ";
  std::cin >> st.cm_marks;
  std::cout << "\nEnter The marks in French 1 out of 100: ";
  std::cin >> st.f_marks;
  std::cout << "\nEnter The marks in computer literacy  out of 100: ";
  std::cin >> st.cl_marks;

  // Calculate grade
  calculate_grade();

  // Insert data into database
  char sql[256];
  sprintf(
      sql,
      "INSERT INTO students (student_no, name, gender, p_marks, d_marks, cm_marks, f_marks, cl_marks, per, grade) "
           "VALUES (%d, '%s', '%s', %d, %d, %d, %d, %d, %f, '%c');",
           st.student_no, st.name, st.gender, st.p_marks, st.d_marks, st.cm_marks, st.f_marks, st.cl_marks, st.per, st.grade);
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    std::cout << "SQL error: " << err_msg << std::endl;
    sqlite3_free(err_msg);
  } else {
    std::cout << "\n\nStudent Record Has Been Created " << std::endl;
  }
}

// Function to display all student records
void display_all() {
  std::cout << "\n\n\n\t\tDISPLAY ALL RECORDS\n\n";

  char *sql = "SELECT * FROM students;";
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    return;
  }

  while (sqlite3_step(res) == SQLITE_ROW) {
    std::cout << "\nStudent Number of Student: " << sqlite3_column_int(res, 0);
    std::cout << "\nName of student: " << sqlite3_column_text(res, 1);
    std::cout << "\nGender: " << sqlite3_column_text(res, 2);
    std::cout << "\nMarks in Probability Theory : " << sqlite3_column_int(res, 3);
    std::cout << "\nMarks in Descriptive Statistics : " << sqlite3_column_int(res, 4);
    std::cout << "\nMarks in Calculus: " << sqlite3_column_int(res, 5);
    std::cout << "\nMarks in French 1: " << sqlite3_column_int(res, 6);
    std::cout << "\nMarks in Computer literacy : " << sqlite3_column_int(res, 7);
    std::cout << "\nPercentage of student is: " << sqlite3_column_double(res, 8);
    std::cout << "\nGrade of student is: " << sqlite3_column_text(res, 9)[0];
    std::cout << "\n\n====================================\n";
  }

  sqlite3_finalize(res);
}

void display_sp(int n) {
  char sql[128];
  sprintf(sql, "SELECT * FROM students WHERE student_no=%d;", n);
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    return;
  }

  int step = sqlite3_step(res);
  if (step == SQLITE_ROW) {
    std::cout << "\nReg Number of student: " << sqlite3_column_int(res, 0);
    std::cout << "\nName of student: " << sqlite3_column_text(res, 1);
    std::cout << "\nGender: " << sqlite3_column_text(res, 2);
    std::cout << "\nMarks in Probability Theory : " << sqlite3_column_int(res, 3);
    std::cout << "\nMarks in Descriptive Statistics : " << sqlite3_column_int(res, 4);
    std::cout << "\nMarks in Calculus: " << sqlite3_column_int(res, 5);
    std::cout << "\nMarks in French 1: " << sqlite3_column_int(res, 6);
    std::cout << "\nMarks in Computer literacy : " << sqlite3_column_int(res, 7);
    std::cout << "\nPercentage of student is: " << sqlite3_column_double(res, 8);
    std::cout << "\nGrade of student is: " << sqlite3_column_text(res, 9)[0];
  } else {
    std::cout << "\n\nRecord not exist" << std::endl;
  }

  sqlite3_finalize(res);
}

void modify_student() {
  int no, found = 0;
  std::cout << "\nEnter The Student Number of student to modify: ";
  std::cin >> no;

  char sql[128];
  sprintf(sql, "SELECT * FROM students WHERE student_no=%d;", no);
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    return;
  }

  if (sqlite3_step(res) == SQLITE_ROW) {
    found = 1;
    std::cout << "\nStudent Number of student: " << sqlite3_column_int(res, 0);
    std::cout << "\nName of student: " << sqlite3_column_text(res, 1);
    std::cout << "\nEnter new marks in Probability Theory : ";
    std::cin >> st.p_marks;
    std::cout << "\nEnter new marks in Descriptive Statistics : ";
    std::cin >> st.d_marks;
    std::cout << "\nEnter new marks in Calculus: ";
    std::cin >> st.cm_marks;
    std::cout << "\nEnter new marks in French 1: ";
    std::cin >> st.f_marks;
    std::cout << "\nEnter new marks in Computer literacy : ";
    std::cin >> st.cl_marks;
    calculate_grade();

    sprintf(
        sql,
        "UPDATE students SET p_marks=%d, d_marks=%d, cm_marks=%d, f_marks=%d, "
        "cl_marks=%d, per=%.2f, grade='%c' WHERE student_no=%d;",
        st.p_marks, st.d_marks, st.cm_marks, st.f_marks, st.cl_marks, st.per,
        st.grade, no);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
      std::cout << "SQL error: " << err_msg << std::endl;
      sqlite3_free(err_msg);
    } else {
      std::cout << "\n\nRecord Updated" << std::endl;
    }
  }

  if (!found) {
    std::cout << "\n\nRecord not found" << std::endl;
  }

  sqlite3_finalize(res);
}

void alert_below_average() {
  std::cout << "\n\n\n\t\tALERT FOR BELOW AVERAGE STUDENTS\n\n";

    char *sql = "SELECT * FROM students WHERE per < 50;";
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
    return;
  }

  while (sqlite3_step(res) == SQLITE_ROW) {
    std::cout << "\nStudent Number of Student: " << sqlite3_column_int(res, 0);
    std::cout << "\nName of student: " << sqlite3_column_text(res, 1);
    std::cout << "\nGender: " << sqlite3_column_text(res, 2);
    std::cout << "\nPercentage of student is: " << sqlite3_column_double(res, 8);
    std::cout << "\nGrade of student is: " << sqlite3_column_text(res, 9)[0];
    std::cout << "\n\n====================================\n";
  }

  sqlite3_finalize(res);
}


void track_tutoring_sessions() {
  std::cout<<"\n\n\n\t\tTRACK TUTORING SESSIONS\n\n";

  char *sql = "SELECT * FROM tutoring_sessions;";
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout<<"Failed to fetch data: %s\n", sqlite3_errmsg(db);
    return;
  }

  while (sqlite3_step(res) == SQLITE_ROW) {
    std::cout<<"\nSession ID: "<< sqlite3_column_int(res, 0);
    std::cout<<"\nStudent Number: "<< sqlite3_column_int(res, 1);
    std::cout<<"\nSubject: "<<sqlite3_column_text(res, 2);
    std::cout<<"\nTutor: "<<sqlite3_column_text(res, 3);
    std::cout<<"\nDate: "<<sqlite3_column_text(res, 4);
    std::cout<<"\n\n====================================\n";
  }

  sqlite3_finalize(res);
}

void add_tutoring_session() {
    
    std::string subject, tutor, date;

//    std::cout << "\nEnter The Student Number of student: ";
//    std::cin >> student_no;
	std::string student_no_str;	
	std::cout << "\nEnter The Student Number of student: ";
  	std::cin >> student_no_str;
  	long long student_no = std::stoll(student_no_str);
  	st.student_no = student_no;
  	std::cin.ignore();

    std::cout << "\nEnter The Subject: ";
    std::getline(std::cin, subject);

    std::cout << "\nEnter The Tutor Name: ";
    std::getline(std::cin, tutor);

    std::cout << "\nEnter The Date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    char sql[256];
    sprintf(sql, "INSERT INTO tutoring_sessions (student_no, subject, tutor, date) VALUES(%d, '%s', '%s', '%s');",
            student_no, subject.c_str(), tutor.c_str(), date.c_str());

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg != NULL) {
            std::cout << "SQL error: " << err_msg << "\n";
            sqlite3_free(err_msg);
        } else {
            std::cout << "SQL error: unknown error\n";
        }
    } else {
        std::cout << "\n\nTutoring Session Added";
    }
}






void generate_progress_report() {
  std::cout<<"\n\n\n\t\tPROGRESS REPORT\n\n";

  char *sql = "SELECT * FROM students;";
  sqlite3_stmt *res;

  int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
  if (rc != SQLITE_OK) {
    std::cout<<"Failed to fetch data: %s\n", sqlite3_errmsg(db);
    return;
  }

  while (sqlite3_step(res) == SQLITE_ROW) {
    std::cout<<"\nStudent Number of Student: %d", sqlite3_column_int(res, 0);
    std::cout<<"\nName of student: %s", sqlite3_column_text(res, 1);
    std::cout<<"\nMarks in Probability Theory : %d", sqlite3_column_int(res, 2);
    std::cout<<"\nMarks in Descriptive Statistics : %d",
           sqlite3_column_int(res, 3);
    std::cout<<"\nMarks in Calculus: %d", sqlite3_column_int(res, 4);
    std::cout<<"\nMarks in French 1: %d", sqlite3_column_int(res, 5);
    std::cout<<"\nMarks in Computer literacy : %d", sqlite3_column_int(res, 6);
    std::cout<<"\nPercentage of student is: %.2f", sqlite3_column_double(res, 7);
    std::cout<<"\nGrade of student is: %c", sqlite3_column_text(res, 8)[0];
    std::cout<<"\n\n====================================\n";
  }

  sqlite3_finalize(res);
}









int main() {
  

    int rc = sqlite3_open("school.db", &db);
  if (rc) {
    std::cout<<"Can't open database: %s\n", sqlite3_errmsg(db);
    return (0);
  } else {
    std::cout<<"Opened database successfully\n";
  }





  char *sql =
      "CREATE TABLE IF NOT EXISTS students(reg_no INT PRIMARY KEY, name TEXT,gender TEXT NOT NULL, "
      "p_marks INT, d_marks INT, cm_marks INT, f_marks INT, cl_marks INT, per "
      "REAL, grade CHAR);";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    std::cout<<"SQL error: %s\n", err_msg;
    sqlite3_free(err_msg);
  }

  sql =
      "CREATE TABLE IF NOT EXISTS tutoring_sessions(id INTEGER PRIMARY KEY "
      "AUTOINCREMENT, reg_no INT, subject TEXT, tutor TEXT, date TEXT);";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    std::cout<<"SQL error: %s\n", err_msg;
    sqlite3_free(err_msg);
  }
	int choice;
  do {
  	std::cout<<"\n\n\n\tRECESS GROUP 20\n";
  	std::cout<<"\n\n\n\tMAIN MENU";
    std::cout <<"\n\n\t1. CREATE STUDENT RECORD";
    std::cout << "\n\n\t2. DISPLAY ALL STUDENTS RECORDS";
    std::cout << "\n\n\t3. SEARCH STUDENT RECORD ";
    std::cout << "\n\n\t4. MODIFY STUDENT RECORD";
    std::cout << "\n\n\t5. ALERT BELOW AVERAGE STUDENTS";
    std::cout << "\n\n\t6. TRACK TUTORING SESSIONS";
    std::cout << "\n\n\t7. ADD TUTORING SESSION";
    std::cout<<"\n\n\t8. EXIT";
    std::cout<<"\n\n\tPlease Select Your Option (1-8) :";
    std::cin >> choice;
    switch (choice) {
      case 1:
        write_student();
        break;
      case 2:
        display_all();
        break;
      case 3:
        int num;
        std::cout<<"\n\n\tPlease Enter The Student Number: ";
        std::cin >> num;
        display_sp(num);
        break;
      case 4:
        modify_student();
        break;
      case 5:
        alert_below_average();
        break;
      case 6:
        track_tutoring_sessions();
        break;
      case 7:
        add_tutoring_session();
        break;
      case 8:
        sqlite3_close(db);
        exit(0);
      default:
        std::cout<<"\a";
    }
  } while (choice != 8);

  sqlite3_close(db);
  return 0;
}
