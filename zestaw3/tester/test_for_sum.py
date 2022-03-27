import unittest, os
import subprocess as sp
import re

def get_number_of_file(file_name):
    return re.findall(r'\d+', file_name)[0]

list_of_files = os.listdir("./test")
path_to_current_dir = os.path.abspath(os.getcwd())  #path to localizaction
                                                    # of current directory

test_folder = os.path.join(path_to_current_dir, "test") #path to localizaction
                                                        # of test files

list_of_test_data = [x for x in list_of_files if "test-dane" in x ] 
list_of_test_result = [x for x in list_of_files if "test-wynik" in x ] 

passed_tests = 0 # counter of passed tests

for index, obj in enumerate(list_of_test_result):
    data_file_name = f"test-dane{get_number_of_file(obj)}.txt"
    path_test_data = os.path.join(test_folder, data_file_name)
    path_test_result = os.path.join(test_folder, obj)
    
    

    with open(path_test_data,"r") as data_file, open(path_test_result, "rb") as result_file:
    
        data_lines = data_file.read()
        result = sp.check_output(f'printf "{data_lines}" | socat -t 1.0 stdio udp4:127.0.0.1:2020', shell=True)
        expected_result = result_file.read()
        if result == expected_result:
            print(f"{data_file_name:<20}|: {result} ==  {expected_result}: True")
            passed_tests += 1
        else:
            print(f"{data_file_name:<20}|: {result} ==  {expected_result}: False")

print(f'Passed tests: {passed_tests}/{len(list_of_test_data)}')

    
    
# class TestSum(unittest.TestCase):
#     def setUp(self):

        
#     def test_sum(self):
#         pass
    
# if __name__ == "__main__":
#     unittest.main()