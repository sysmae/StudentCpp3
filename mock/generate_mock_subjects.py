import csv
import os
import random


# 기본 과목 리스트 (course 이름, 학점, 과목 유형, 학기)
courses = [
    ("General Physics (1)", 3, "Basic", "FIRST_TERM"),
    ("General Physics Lab (1)", 1, "Basic", "FIRST_TERM"),
    ("Calculus (1)", 3, "Basic", "FIRST_TERM"),
    ("General Physics (2)", 3, "Basic", "SECOND_TERM"),
    ("General Physics Lab (2)", 1, "Basic", "SECOND_TERM"),
    ("Calculus (2)", 3, "Basic", "SECOND_TERM"),
    ("Linear Algebra", 2, "Basic", "SECOND_TERM"),
    ("Mathematical Physics (1)", 3, "Required", "FIRST_TERM"),
    ("General Dynamics (2)", 3, "Required", "SECOND_TERM"),
    ("Electromagnetism (2)", 3, "Required", "SECOND_TERM"),
    ("Thermodynamics", 3, "Required", "FIRST_TERM"),
    ("Physical Research", 2, "Required", "SECOND_TERM"),
    ("Quantum Mechanics (2)", 3, "Required", "SECOND_TERM"),
    ("Physics Lab (1)", 2, "Elective", "FIRST_TERM"),
    ("History and Philosophy of Physics", 3, "Elective", "FIRST_TERM"),
    ("General Dynamics (1)", 3, "Elective", "FIRST_TERM"),
    ("Electromagnetism (1)", 3, "Elective", "FIRST_TERM"),
    ("Physics Lab (2)", 2, "Elective", "SECOND_TERM"),
    ("Mathematical Physics (2)", 3, "Elective", "SECOND_TERM"),
    ("Modern Physics", 3, "Elective", "SECOND_TERM"),
    ("Optics", 3, "Elective", "FIRST_TERM"),
    ("Physics Lab (3)", 2, "Elective", "FIRST_TERM"),
    ("Quantum Mechanics (1)", 3, "Elective", "FIRST_TERM"),
    ("Computational Physics", 3, "Elective", "FIRST_TERM"),
    ("Semiconductor Physics", 3, "Elective", "SECOND_TERM"),
    ("Biophysics", 3, "Elective", "SECOND_TERM"),
    ("Statistical Mechanics", 3, "Elective", "SECOND_TERM"),
    ("Solid State Physics", 3, "Elective", "FIRST_TERM"),
    ("Relativity and Cosmology", 3, "Elective", "FIRST_TERM"),
    ("Nuclear Physics", 3, "Elective", "FIRST_TERM"),
    ("Advanced Solid State Physics", 3, "Elective", "SECOND_TERM"),
    ("Particle Physics", 3, "Elective", "SECOND_TERM"),
    ("Plasma Physics", 3, "Elective", "SECOND_TERM")
]

# 교수 데이터 읽기
professors = []

try:
    with open('users.csv', mode='r', newline='', encoding='utf-8') as file:
        reader = csv.reader(file)
        header = next(reader)  # 헤더 행 읽기
        
        # 교수만 필터링: UserType이 'Professor'인 행만
        professors = [
            row[0]  # 교수 ID만 저장
            for row in reader
            if row[5] == 'Professor'
        ]

except FileNotFoundError:
    print("Error: The file 'users.csv' was not found.")
except Exception as e:
    print(f"An unexpected error occurred while reading 'users.csv': {e}")

# 교수 목록이 비어있는지 확인
if not professors:
    print("Error: No professors found in users.csv.")
else:
    # 각 과목에 대한 교수 배정을 정의
    course_professor_mapping = {course[0]: [] for course in courses}

    # 각 과목에 대해 교수 배정
    professors_count = len(professors)
    for i, course in enumerate(course_professor_mapping.keys()):
        course_professor_mapping[course].append(professors[i % professors_count])

    # 강의 데이터를 각 학기 및 교수 정보와 결합하여 새로운 리스트 생성
    years = range(2017, 2025)
    new_courses = []

    course_id_counter = 1001  # 코스 아이디 시작 값

    # 각 과목에 대해 교수와 학기 정보를 결합하여 ID를 자동으로 생성
    for course_name, credit, course_type, semester in courses:
        # 연도만 반복하여 과목에 교수와 학기 추가 (고정된 학기 사용)
        for year in years:
            assigned_professors = course_professor_mapping[course_name]
            for professor_id in assigned_professors:
                new_courses.append((
                    course_id_counter, 
                    course_name, 
                    credit, 
                    course_type, 
                    year, 
                    semester, 
                    professor_id
                ))
                course_id_counter += 1  # 코스 아이디 증가

    # 'mock' 폴더가 없으면 생성
    os.makedirs('mock', exist_ok=True)

    # 새로운 데이터는 mock/mock_subjects.csv 파일에 작성
    header = ["ID", "Name", "Credit", "Type", "Year", "Term", "ProfessorID"]

    try:
        with open('mock/mock_subjects.csv', mode='w', newline='', encoding='utf-8') as file:
            writer = csv.writer(file)
            writer.writerow(header)
            writer.writerows(new_courses)
        print("CSV file 'mock/mock_subjects.csv' has been created.")
    except Exception as e:
        print(f"An error occurred while writing to 'mock/mock_subjects.csv': {e}")