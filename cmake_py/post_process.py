# will execute function "run()" after "cmake.py" build "CMakeLists.txt"


'''
A python script that executes the run() function after building cmake.
Typically used to run "make" to compile, run testcases, etc.

"project_defs" is a dict that was parsed from "project_defs.mk".

'''

import os
import subprocess
import sys
import shutil


def run_cmake(project_defs):
    print("\n***----- Start CMake -----***")

    subprocess.run("cmake .", shell=True, check=True)
    if os.path.exists("output/include"):
        shutil.rmtree("output/include")
    if project_defs["MODULE_TYPE"] in ["slib", "dlib"]:
        shutil.copytree("../include/", "output/include")

def run_make():
    subprocess.run("make -j4", shell=True, check=True)

def run_bin(project_defs):
    print("\n***----- Start Running Bin -----***")
    subprocess.run("output/bin/{}".format(project_defs["MODULE_NAME"]), shell=True, check=True)

def auto_test_report(test_results_list):
    pass_list = []
    fail_list = []
    for prog_test in test_results_list:
        lines = prog_test[1].split("------------------------------\n")
        for line in lines:
            if " -> pass" in line:
                pass_list.append(line)
            elif " -> fail" in line :
                fail_list.append("Fail Testcase Is From : " + prog_test[0])
                fail_list.append(line)
    print("\n***----- Auto Test Report -----***")
    print("PASSED: ", len(pass_list), " test are passed")
    print("FAILED: ", int(len(fail_list)/2), " test are failed")
    for fail_test in fail_list:
        print("--------------------")
        print(fail_test)

def run_test():
    print("\n***----- Start Running Test -----***")
    directory_path = "output/test/"
    test_names = os.listdir(directory_path)  

    test_results = []

    for index, test_name in enumerate(test_names):
        file_path = os.path.join(directory_path, test_name)
        if os.path.isfile(file_path):
            try:
                print("----- test_{} : {} -----".format(index, test_name))
                result = subprocess.run(file_path, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                output_str = result.stdout.decode('utf-8')
                print(output_str)
                test_results.append((test_name, output_str))
            except subprocess.CalledProcessError as err:
                print(f"Error running {test_name}: {err}")

    auto_test_report(test_results)


def run(project_defs):
    print("----- Start Cmake Postprocess -----")
    run_cmake(project_defs)
    run_make()
    if project_defs["MODULE_TYPE"] == "bin":
        run_bin(project_defs)
    elif project_defs["MODULE_TYPE"] in ["slib", "dlib"]:
        run_test()
