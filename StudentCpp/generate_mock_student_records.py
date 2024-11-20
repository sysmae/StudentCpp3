import random
import numpy as np
import csv
import os

# Function to read users (students and professors) from the CSV
def read_users_from_csv(filename):
    students = []
    professors = []
    with open(filename, mode='r', newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row["UserType"] == "Student":
                students.append(row["StudentID"])
            elif row["UserType"] == "Professor":
                professors.append(row["ID"])  # Collecting professor IDs
    return students, professors

def read_subjects_from_csv(filename):
    subjects = []
    with open(filename, mode='r', newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            subject = {
                "subjectID": int(row["ID"]),
                "subjectName": row["Name"],
                "credit": int(row["Credit"]),
                "type": row["Type"],
                "year": int(row["Year"]),
                "term": row["Term"],
                "professorID": row["ProfessorID"],  # Associate subject with professor ID
            }
            subjects.append(subject)
    return subjects

def get_student_year(student_id):
    return int(student_id[:4])

def generate_mock_data(num_rows, subjects, student_ids, professor_ids):
    grade_to_letter = {
        4.5: 'A+',
        4.0: 'A',
        3.5: 'B+',
        3.0: 'B',
        2.5: 'C+',
        2.0: 'C',
        1.5: 'D+',
        1.0: 'D',
        0.0: 'F'
    }
    mock_data = []
    student_subject_grades = {student_id: {} for student_id in student_ids}
    student_term_credits = {student_id: {} for student_id in student_ids}  # 학기별 학점 제한 추적

    # 최대 시도 횟수 설정을 더 높게
    max_attempts = num_rows * 10  # 시도 횟수 상한을 더 크게 설정
    attempts = 0

    while len(mock_data) < num_rows and attempts < max_attempts:
        attempts += 1

        # 학생 및 과목 선택
        student_id = random.choice(student_ids)
        student_admission_year = get_student_year(student_id)
        available_subjects = [
            subject for subject in subjects
            if (subject["year"] >= student_admission_year and 
                (subject["subjectName"] not in student_subject_grades[student_id] or 
                 student_subject_grades[student_id][subject["subjectName"]]["letterGrade"] == "F"))
        ]
        
        if not available_subjects:
            continue

        subject = random.choice(available_subjects)
        professor_id = subject["professorID"]

        # 해당 학생의 학기 추적
        term_key = f"{subject['year']}-{subject['term']}"  # 학기 키 (예: "2023-1")
        if term_key not in student_term_credits[student_id]:
            student_term_credits[student_id][term_key] = 0

        # 학점 제한 확인
        if student_term_credits[student_id][term_key] + subject["credit"] > 21:
            continue  # 이 과목을 추가하면 21학점을 초과하므로 건너뜀

        # 학점 부여 방식 선택
        case = random.choice([1, 2])
        if case == 1:
            # Case 1: A+, A (35%), B+, B (70%), C+, C, D+, D0, F 포함
            grades = [4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.0]
            weights = [0.175, 0.175, 0.245, 0.245, 0.1, 0.05, 0.03, 0.02, 0.01]
        else:
            # Case 2: A+, A (50%), B+, B (90%), C+, C, D+, D0, F 포함
            grades = [4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.0]
            weights = [0.25, 0.25, 0.2, 0.2, 0.1, 0.07, 0.05, 0.05, 0.03]

        grade = random.choices(grades, weights=weights, k=1)[0]
        letter_grade = grade_to_letter[grade]

        # 점수 생성
        score = int(np.random.normal(85 if grade >= 3.5 else 70 if grade >= 2.0 else 60, 5))
        score = max(0, min(100, score))

        # 학점 및 수강 정보 추가
        student_subject_grades[student_id][subject["subjectName"]] = {
            "grade": grade,
            "letterGrade": letter_grade
        }
        student_term_credits[student_id][term_key] += subject["credit"]  # 학점 누적

        # 데이터 추가
        mock_data.append([student_id, subject["subjectID"], score, grade, letter_grade, professor_id])

        # 시도 횟수 초기화
        attempts = 0

    if len(mock_data) < num_rows:
        print(f"Warning: Could only generate {len(mock_data)} rows out of {num_rows} requested.")

    return mock_data


def write_to_csv(filename, data):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["studentID", "subjectID", "score", "grade", "letterGrade", "ProfessorID"])
        writer.writerows(data)

if __name__ == "__main__":
    # Read the data
    student_ids, professor_ids = read_users_from_csv('users.csv')
    subjects = read_subjects_from_csv('subjects.csv')

    # Generate mock data
    mock_data = generate_mock_data(4500, subjects, student_ids, professor_ids)

    # Ensure the "mock" folder exists
    mock_folder = "mock"
    os.makedirs(mock_folder, exist_ok=True)

    # Write to CSV inside the "mock" folder
    csv_file = os.path.join(mock_folder, 'mock_student_records.csv')
    write_to_csv(csv_file, mock_data)

    print(f"Successfully generated {len(mock_data)} rows of mock data")
