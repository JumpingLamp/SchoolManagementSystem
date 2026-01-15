-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 15, 2026 at 03:54 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `schoolmanagementdb`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `adminID` varchar(5) NOT NULL,
  `name` varchar(32) NOT NULL,
  `password` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`adminID`, `name`, `password`) VALUES
('A0000', 'admin', '040302'),
('A0001', 'test1', '123456'),
('A0002', 'test2', '123456'),
('A0004', 'TEST4', '123456'),
('A0005', 'TEST5', '123456'),
('A0006', 'TESTW6', '654321');

-- --------------------------------------------------------

--
-- Table structure for table `attendance`
--

CREATE TABLE `attendance` (
  `date` date NOT NULL,
  `studentID` varchar(5) NOT NULL,
  `status` enum('PRESENT','ABSENT') NOT NULL DEFAULT 'PRESENT',
  `remarks` varchar(16) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `attendance`
--

INSERT INTO `attendance` (`date`, `studentID`, `status`, `remarks`) VALUES
('2026-03-01', 'S0001', 'PRESENT', NULL),
('2026-03-01', 'S0002', 'ABSENT', 'Sick'),
('2026-03-01', 'S0003', 'PRESENT', NULL),
('2026-03-01', 'S0004', 'PRESENT', NULL),
('2026-03-01', 'S0005', 'ABSENT', 'Family'),
('2026-03-02', 'S0001', 'PRESENT', NULL),
('2026-03-02', 'S0002', 'PRESENT', NULL),
('2026-03-02', 'S0003', 'ABSENT', 'Sick'),
('2026-03-02', 'S0004', 'PRESENT', NULL),
('2026-03-02', 'S0005', 'PRESENT', NULL),
('2026-03-03', 'S0006', 'PRESENT', NULL),
('2026-03-03', 'S0007', 'ABSENT', 'Sick'),
('2026-03-03', 'S0008', 'PRESENT', NULL),
('2026-03-03', 'S0009', 'PRESENT', NULL),
('2026-03-03', 'S0010', 'ABSENT', 'Family');

-- --------------------------------------------------------

--
-- Table structure for table `child_list`
--

CREATE TABLE `child_list` (
  `studentID` varchar(5) NOT NULL,
  `parentID` varchar(5) NOT NULL,
  `relationship` enum('FATHER','MOTHER','GUARDIAN','') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `child_list`
--

INSERT INTO `child_list` (`studentID`, `parentID`, `relationship`) VALUES
('S0001', 'P0001', 'FATHER'),
('S0001', 'P0002', 'MOTHER'),
('S0002', 'P0001', 'FATHER'),
('S0002', 'P0002', 'MOTHER'),
('S0003', 'P0003', 'GUARDIAN'),
('S0004', 'P0004', 'FATHER');

-- --------------------------------------------------------

--
-- Table structure for table `class`
--

CREATE TABLE `class` (
  `classID` varchar(3) NOT NULL,
  `roomNum` varchar(6) NOT NULL,
  `teacherID` varchar(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `class`
--

INSERT INTO `class` (`classID`, `roomNum`, `teacherID`) VALUES
('C1A', '101', 'T0001'),
('C1B', '102', 'T0002'),
('C2A', '201', 'T0003'),
('C2B', '202', 'T0004'),
('C3A', '301', 'T0005');

-- --------------------------------------------------------

--
-- Table structure for table `class_schedule`
--

CREATE TABLE `class_schedule` (
  `period` int(11) NOT NULL,
  `day` enum('Monday','Tuesday','Wednesday','Thursday','Friday') NOT NULL,
  `classID` varchar(3) NOT NULL,
  `teacherID` varchar(5) NOT NULL,
  `subjectID` varchar(3) NOT NULL
) ;

--
-- Dumping data for table `class_schedule`
--

INSERT INTO `class_schedule` (`period`, `day`, `classID`, `teacherID`, `subjectID`) VALUES
(1, 'Monday', 'C1A', 'T0001', 'J01'),
(1, 'Monday', 'C2B', 'T0004', 'J01'),
(1, 'Tuesday', 'C1A', 'T0002', 'J02'),
(1, 'Tuesday', 'C2B', 'T0007', 'J02'),
(1, 'Wednesday', 'C1A', 'T0003', 'J03'),
(1, 'Wednesday', 'C2B', 'T0007', 'J03'),
(1, 'Thursday', 'C1A', 'T0004', 'J04'),
(1, 'Thursday', 'C2B', 'T0002', 'J04'),
(1, 'Friday', 'C1A', 'T0005', 'J05'),
(1, 'Friday', 'C2B', 'T0006', 'J05'),
(2, 'Monday', 'C1A', 'T0002', 'J02'),
(2, 'Monday', 'C2B', 'T0003', 'J03'),
(2, 'Tuesday', 'C1A', 'T0003', 'J03'),
(2, 'Tuesday', 'C2B', 'T0004', 'J04'),
(2, 'Wednesday', 'C1A', 'T0004', 'J04'),
(2, 'Wednesday', 'C2B', 'T0005', 'J05'),
(2, 'Thursday', 'C1A', 'T0005', 'J05'),
(2, 'Thursday', 'C2B', 'T0001', 'J01'),
(2, 'Friday', 'C1A', 'T0001', 'J01'),
(2, 'Friday', 'C2B', 'T0002', 'J02'),
(5, 'Monday', 'C1A', 'T0003', 'J03'),
(5, 'Monday', 'C2B', 'T0005', 'J05'),
(5, 'Tuesday', 'C1A', 'T0004', 'J04'),
(5, 'Tuesday', 'C2B', 'T0001', 'J01'),
(5, 'Wednesday', 'C1A', 'T0005', 'J05'),
(5, 'Wednesday', 'C2B', 'T0002', 'J02'),
(5, 'Thursday', 'C1A', 'T0001', 'J01'),
(5, 'Thursday', 'C2B', 'T0003', 'J03'),
(5, 'Friday', 'C1A', 'T0002', 'J02'),
(5, 'Friday', 'C2B', 'T0004', 'J04');

-- --------------------------------------------------------

--
-- Table structure for table `exam`
--

CREATE TABLE `exam` (
  `examID` varchar(5) NOT NULL,
  `description` varchar(32) NOT NULL,
  `startDate` date NOT NULL,
  `endDate` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `exam`
--

INSERT INTO `exam` (`examID`, `description`, `startDate`, `endDate`) VALUES
('E0001', 'Mid-Term Exam', '2026-03-01', '2026-03-05'),
('E0002', 'Final Exam', '2026-06-15', '2026-06-20');

-- --------------------------------------------------------

--
-- Table structure for table `exam_result`
--

CREATE TABLE `exam_result` (
  `studentID` varchar(5) NOT NULL,
  `examID` varchar(5) NOT NULL,
  `subjectID` varchar(3) NOT NULL,
  `result` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `exam_result`
--

INSERT INTO `exam_result` (`studentID`, `examID`, `subjectID`, `result`) VALUES
('S0001', 'E0001', 'J01', 85),
('S0001', 'E0001', 'J02', 78),
('S0002', 'E0001', 'J03', 92),
('S0002', 'E0001', 'J04', 67),
('S0003', 'E0001', 'J05', 74),
('S0003', 'E0002', 'J01', 88),
('S0004', 'E0002', 'J02', 81),
('S0004', 'E0002', 'J03', 90),
('S0005', 'E0002', 'J04', 73);

-- --------------------------------------------------------

--
-- Table structure for table `parent`
--

CREATE TABLE `parent` (
  `parentID` varchar(5) NOT NULL,
  `name` varchar(32) NOT NULL,
  `icNum` varchar(14) NOT NULL,
  `address` varchar(256) DEFAULT NULL,
  `phoneNum` varchar(13) DEFAULT NULL,
  `password` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `parent`
--

INSERT INTO `parent` (`parentID`, `name`, `icNum`, `address`, `phoneNum`, `password`) VALUES
('P0001', 'John Tan', '700101012345', '12 Jalan Bunga, KL', '0123456789', 'pass01'),
('P0002', 'Mary Tan', '710202022345', '12 Jalan Bunga, KL', '0123456790', 'pass02'),
('P0003', 'Lim Ah Kow', '680303032345', '34 Jalan Merdeka, KL', '0134567890', 'pass03'),
('P0004', 'Tan Mei Ling', '690404042345', '34 Jalan Merdeka, KL', '0134567891', 'pass04'),
('P0006', 'Chua Li', '730606062345', '78 Jalan Aman, KL', '0156789012', 'pass06'),
('P0007', 'Goh Kai Yang', '04030201-01-07', '32, Al-Jazari', '+60189012788', 'pass07'),
('P0008', 'GOH', '030303', '32 AJ', '0164059835', 'PASS08');

-- --------------------------------------------------------

--
-- Table structure for table `student`
--

CREATE TABLE `student` (
  `studentID` varchar(5) NOT NULL,
  `name` varchar(32) NOT NULL,
  `icNum` varchar(14) NOT NULL,
  `address` varchar(256) DEFAULT NULL,
  `phoneNum` varchar(13) DEFAULT NULL,
  `classID` varchar(3) NOT NULL,
  `password` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `student`
--

INSERT INTO `student` (`studentID`, `name`, `icNum`, `address`, `phoneNum`, `classID`, `password`) VALUES
('S0001', 'Alice Tan', '050101012345', '12 Jalan Bunga, KL', '0123456781', 'C1A', 'stud01'),
('S0002', 'Bob Tan', '050202022345', '12 Jalan Bunga, KL', '0123456782', 'C1B', 'stud02'),
('S0003', 'Catherine Lim', '050303032345', '34 Jalan Merdeka, KL', '0134567891', 'C2A', 'stud03'),
('S0004', 'David Lim', '050404042345', '34 Jalan Merdeka, KL', '0134567892', 'C2B', 'stud04'),
('S0005', 'Elaine Ng', '050505052345', '56 Jalan Setia, KL', '0145678901', 'C3A', 'stud05'),
('S0006', 'Frank Ng', '050606062345', '56 Jalan Setia, KL', '0145678902', 'C3A', 'stud06'),
('S0007', 'Grace Chua', '050707072345', '78 Jalan Aman, KL', '0156789011', 'C1A', 'stud07'),
('S0008', 'Henry Chua', '050808082345', '78 Jalan Aman, KL', '0156789013', 'C2A', 'stud08'),
('S0009', 'Irene Lim', '050909092345', '34 Jalan Merdeka, KL', '0134567893', 'C2B', 'stud09'),
('S0010', 'Jack Ng', '051010102345', '56 Jalan Setia, KL', '0145678903', 'C3A', 'stud10'),
('S0011', '12', '040302-01-0711', '32, Al-Jazari', '+60189012788', 'C3A', 'stud11'),
('S0012', 'Aiman', '040302-01-0711', '', '0123456789', 'C1A', 'stud12');

-- --------------------------------------------------------

--
-- Table structure for table `subject`
--

CREATE TABLE `subject` (
  `subjectID` varchar(3) NOT NULL,
  `subjectName` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `subject`
--

INSERT INTO `subject` (`subjectID`, `subjectName`) VALUES
('J01', 'Matematik'),
('J02', 'Sains'),
('J03', 'Bahasa English'),
('J04', 'Sejarah'),
('J05', 'Pendidikan Moral');

-- --------------------------------------------------------

--
-- Table structure for table `teacher`
--

CREATE TABLE `teacher` (
  `teacherID` varchar(5) NOT NULL,
  `name` varchar(32) NOT NULL,
  `icNum` varchar(14) NOT NULL,
  `address` varchar(256) DEFAULT NULL,
  `phoneNum` varchar(13) NOT NULL,
  `adminID` varchar(5) NOT NULL,
  `password` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `teacher`
--

INSERT INTO `teacher` (`teacherID`, `name`, `icNum`, `address`, `phoneNum`, `adminID`, `password`) VALUES
('T0001', 'KKK', '900101012345', '12 Jalan Bunga, KL', '0123456789', 'A0000', 'pass01'),
('T0002', 'Bob Lim', '880202022345', '34 Jalan Merdeka, KL', '0134567890', 'A0000', 'pass02'),
('T0003', 'Catherine Lee', '870303032345', '56 Jalan Setia, KL', '0145678901', 'A0000', 'pass03'),
('T0004', 'David Wong', '860404042345', '78 Jalan Aman, KL', '0156789012', 'A0000', 'pass04'),
('T0005', 'Elaine Chua', '850505052345', '90 Jalan Harmoni, KL', '0167890123', 'A0000', 'pass05'),
('T0006', 'Frank Ng', '840606062345', '22 Jalan Sutera, KL', '0178901234', 'A0000', 'pass06'),
('T0007', 'Grace Lim', '830707072345', '44 Jalan Damai, KL', '0189012345', 'A0000', 'pass07'),
('T0010', 'MR. GOH', '040302-01-0711', '', '+60189012788', 'A0000', 'pass10'),
('T0011', 'Encik Ahmad', '040302-01-0711', '', '0123456789', 'A0000', 'pass11');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`adminID`) USING BTREE;

--
-- Indexes for table `attendance`
--
ALTER TABLE `attendance`
  ADD PRIMARY KEY (`date`,`studentID`),
  ADD KEY `studentID` (`studentID`) USING BTREE;

--
-- Indexes for table `child_list`
--
ALTER TABLE `child_list`
  ADD PRIMARY KEY (`studentID`,`parentID`),
  ADD KEY `parentID` (`parentID`);

--
-- Indexes for table `class`
--
ALTER TABLE `class`
  ADD PRIMARY KEY (`classID`),
  ADD KEY `teacherID` (`teacherID`);

--
-- Indexes for table `class_schedule`
--
ALTER TABLE `class_schedule`
  ADD PRIMARY KEY (`period`,`day`,`classID`),
  ADD UNIQUE KEY `uniqueTeacher` (`teacherID`,`day`,`period`),
  ADD KEY `classID` (`classID`),
  ADD KEY `subjectID` (`subjectID`);

--
-- Indexes for table `exam`
--
ALTER TABLE `exam`
  ADD PRIMARY KEY (`examID`);

--
-- Indexes for table `exam_result`
--
ALTER TABLE `exam_result`
  ADD PRIMARY KEY (`studentID`,`examID`,`subjectID`),
  ADD KEY `examID` (`examID`),
  ADD KEY `subjectID` (`subjectID`);

--
-- Indexes for table `parent`
--
ALTER TABLE `parent`
  ADD PRIMARY KEY (`parentID`);

--
-- Indexes for table `student`
--
ALTER TABLE `student`
  ADD PRIMARY KEY (`studentID`),
  ADD KEY `classID` (`classID`);

--
-- Indexes for table `subject`
--
ALTER TABLE `subject`
  ADD PRIMARY KEY (`subjectID`);

--
-- Indexes for table `teacher`
--
ALTER TABLE `teacher`
  ADD PRIMARY KEY (`teacherID`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `attendance`
--
ALTER TABLE `attendance`
  ADD CONSTRAINT `attendance_ibfk_1` FOREIGN KEY (`studentID`) REFERENCES `student` (`studentID`);

--
-- Constraints for table `child_list`
--
ALTER TABLE `child_list`
  ADD CONSTRAINT `child_list_ibfk_1` FOREIGN KEY (`parentID`) REFERENCES `parent` (`parentID`),
  ADD CONSTRAINT `child_list_ibfk_2` FOREIGN KEY (`studentID`) REFERENCES `student` (`studentID`);

--
-- Constraints for table `class`
--
ALTER TABLE `class`
  ADD CONSTRAINT `class_ibfk_1` FOREIGN KEY (`teacherID`) REFERENCES `teacher` (`teacherID`);

--
-- Constraints for table `class_schedule`
--
ALTER TABLE `class_schedule`
  ADD CONSTRAINT `class_schedule_ibfk_1` FOREIGN KEY (`classID`) REFERENCES `class` (`classID`),
  ADD CONSTRAINT `class_schedule_ibfk_2` FOREIGN KEY (`teacherID`) REFERENCES `teacher` (`teacherID`),
  ADD CONSTRAINT `class_schedule_ibfk_3` FOREIGN KEY (`subjectID`) REFERENCES `subject` (`subjectID`);

--
-- Constraints for table `exam_result`
--
ALTER TABLE `exam_result`
  ADD CONSTRAINT `exam_result_ibfk_1` FOREIGN KEY (`studentID`) REFERENCES `student` (`studentID`),
  ADD CONSTRAINT `exam_result_ibfk_2` FOREIGN KEY (`examID`) REFERENCES `exam` (`examID`),
  ADD CONSTRAINT `exam_result_ibfk_3` FOREIGN KEY (`subjectID`) REFERENCES `subject` (`subjectID`);

--
-- Constraints for table `student`
--
ALTER TABLE `student`
  ADD CONSTRAINT `student_ibfk_1` FOREIGN KEY (`classID`) REFERENCES `class` (`classID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
