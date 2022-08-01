import io
import os
import subprocess
import shutil
import unittest

from unittest import mock

def call_script(script: str, *script_args: str) -> subprocess.CompletedProcess:
    return subprocess.run(['bash', script, *script_args], stdout=subprocess.PIPE)

class NewProjectTest(unittest.TestCase):
    PROJECT_NAME = "TEST_PROJECT"
    PROJECT_LANGUAGE = "TEST_PROJECT"

    def tearDown(self):
        if os.path.isdir(self.PROJECT_NAME):
            shutil.rmtree(self.PROJECT_NAME)

    def call_new_project(self, *args):
        result = call_script('tooling/new_project.sh', *args)
        return result

    def create_new_project(self, *args):
        return self.call_new_project(
            '-l', self.PROJECT_LANGUAGE,
            '-n', self.PROJECT_NAME, *args
        )

    def assert_exit(self, result: subprocess.CompletedProcess, expected: int):
        self.assertEqual(result.returncode, expected)

    def assert_exit_success(self, result: subprocess.CompletedProcess):
        self.assert_exit(result, 0)

    def assert_stdout(self, result: subprocess.CompletedProcess, expected: str):
        self.assertEqual(result.stdout.decode('utf-8'), expected)
        
    def assert_file_contents_equal(self, file: io.FileIO, expected: str):
        raw_content = file.readlines()
        joined_content = ''.join(raw_content)
        self.assertEqual(joined_content, expected)

class NewProjectHelpTest(NewProjectTest):
    def test_exit_success(self):
        result = self.call_new_project('-h')
        self.assert_exit_success(result)

    def test_help_msg(self):
        result = self.call_new_project('-h')
        self.assert_stdout(
            result,
            "usage: bash new_project.sh [-t] <language> <project_name>\n"
        )

class NewProjectCTest(NewProjectTest):
    PROJECT_NAME = 'TEST_C_PROJECT'
    PROJECT_LANGUAGE = 'c'
    
    def test_no_test(self):
        result = self.create_new_project()
        self.assertTrue(os.path.isdir("TEST_C_PROJECT"))
        self.assertTrue(os.path.isdir("TEST_C_PROJECT/src"))
        self.assertTrue(os.path.isdir("TEST_C_PROJECT/include"))
        self.assertTrue(os.path.isfile("TEST_C_PROJECT/CMakeLists.txt"))
        expected = (
            'cmake_minimum_required(VERSION 3.6.0)\n\n'
            'project(TEST_C_PROJECT)\n\n'
            'set( CMAKE_CXX_STANDARD 17 )\n\n'
            'add_executable(\n'
            '    TEST_C_PROJECT\n'
            '    src/main.c\n)\n\n'
            'target_include_directories(TEST_C_PROJECT PUBLIC include/)\n'
        )
        with open(f'{self.PROJECT_NAME}/CMakeLists.txt') as cmakelist:
            self.assert_file_contents_equal(cmakelist, expected)
        self.assert_exit_success(result)
        self.assert_stdout(result, (
            'Creating new "c" project: "TEST_C_PROJECT"\n\n'
            'generating src directory: TEST_C_PROJECT/src\n'
            'generating include directory: TEST_C_PROJECT/include\n'
            'generating CMakeLists.txt: TEST_C_PROJECT/CMakeLists.txt\n'
            'skipping test generation\n'
        ))
        
    def test_with_test(self):
        result = self.create_new_project('-t')
        self.assertTrue(os.path.isdir("TEST_C_PROJECT"))
        self.assertTrue(os.path.isdir("TEST_C_PROJECT/src"))
        self.assertTrue(os.path.isdir("TEST_C_PROJECT/include"))
        self.assertTrue(os.path.isfile("TEST_C_PROJECT/CMakeLists.txt"))
        expected = (
            'cmake_minimum_required(VERSION 3.6.0)\n\n'
            'project(TEST_C_PROJECT)\n\n'
            'set( CMAKE_CXX_STANDARD 17 )\n\n'
            'add_executable(\n'
            '    TEST_C_PROJECT\n'
            '    src/main.c\n)\n\n'
            'target_include_directories(TEST_C_PROJECT PUBLIC include/)\n'
            '# gtest\n'
            'include(FetchContent)\n'
            'FetchContent_Declare(\n'
            '    googletest\n'
            '    URL https://github.com/google/googletest'
            '/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip\n'
            ')\n\n'
            'enable_testing()\n\n'
            'add_executable(\n'
            '    TEST_C_PROJECT_test\n'
            '    test/src/test_main.cc\n)\n\n'
            'target_link_libraries(\n'
            '    TEST_C_PROJECT_test\n'
            '    gtest\n'
            '    gtest_main\n'
            '    pthread\n)\n\n'
            'target_include_directories(\n'
            '    TEST_C_PROJECT_test PUBLIC\n'
            '    include/\n'
            '    test/include/\n)\n\n'
            'include(GoogleTest)\n'
            'gtest_discover_tests(TEST_C_PROJECT_test)\n'
        )
        with open(f'{self.PROJECT_NAME}/CMakeLists.txt') as cmakelist:
            self.assert_file_contents_equal(cmakelist, expected)
        self.assert_exit_success(result)
        self.assert_stdout(result, (
            'Creating new "c" project: "TEST_C_PROJECT"\n\n'
            'generating src directory: TEST_C_PROJECT/src\n'
            'generating include directory: TEST_C_PROJECT/include\n'
            'generating CMakeLists.txt: TEST_C_PROJECT/CMakeLists.txt\n'
            'generating test directory: TEST_C_PROJECT/test\n'
            'adding tests to CMakeLists.txt\n'
        ))

if __name__ == '__main__':
    unittest.main()
