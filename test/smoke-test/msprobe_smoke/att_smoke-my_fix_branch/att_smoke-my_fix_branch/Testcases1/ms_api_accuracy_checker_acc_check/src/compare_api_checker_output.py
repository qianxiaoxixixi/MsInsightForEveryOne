import sys
import glob
import os
import pandas as pd

def get_args():
    if len(sys.argv) != 3:
        raise ValueError
    output_dir = sys.argv[1]
    target_dir = sys.argv[2]

    return output_dir, target_dir

def get_details_result_csv(directory):
    pattern_details_csv = os.path.join(directory, 'accuracy_checking_details_*.csv')
    details_csv_matching_files = glob.glob(pattern_details_csv)
    if len(details_csv_matching_files) != 1:
        print(f"number of details csv in {directory} is: {len(details_csv_matching_files)}, expected: 1")
        raise ValueError

    pattern_result_csv = os.path.join(directory, 'accuracy_checking_result_*.csv')
    result_csv_matching_files = glob.glob(pattern_result_csv)
    if len(result_csv_matching_files) != 1:
        print(f"number of result csv in {directory} is: {len(result_csv_matching_files)}, expected: 1")
        raise ValueError

    return details_csv_matching_files[0], result_csv_matching_files[0]


def check_csv(output_csv, target_csv, details_or_result):
    output_df = pd.read_csv(output_csv)
    target_df = pd.read_csv(target_csv)
    is_pass = True
    if details_or_result == "details":
        output_api_name = output_df['API Name']
        target_api_name = target_df['API Name']
        api_name_check = output_api_name.equals(target_api_name)
        if not api_name_check:
            print("output details csv is different from target in API Name")
            is_pass = False

        output_status = output_df['Status']
        target_status = target_df['Status']
        status_check = output_status.equals(target_status)
        if not status_check:
            print("output details csv is different from target in status")
            is_pass = False
    else:
        output_api_name = output_df['API Name']
        target_api_name = target_df['API Name']
        api_name_check = output_api_name.equals(target_api_name)
        if not api_name_check:
            print("output result csv is different from target in API Name")
            is_pass = False
        output_forward_test_success = output_df['Forward Test Success']
        target_forward_test_success = target_df['Forward Test Success']
        forward_test_success_check = output_forward_test_success.equals(target_forward_test_success)
        if not forward_test_success_check:
            print("output result csv is different from target in Forward Test Success")
            is_pass = False
        output_backward_test_success = output_df['Backward Test Success']
        target_backward_test_success = target_df['Backward Test Success']
        backward_test_success_check = output_backward_test_success.equals(target_backward_test_success)
        if not backward_test_success_check:
            print("output result csv is different from target in Backward Test Success")
            is_pass = False
    return is_pass


if __name__ == "__main__":
    output_dir, target_dir = get_args()
    output_details_csv, output_result_csv = get_details_result_csv(output_dir)
    target_details_csv, target_result_csv = get_details_result_csv(target_dir)
    is_pass_details_csv = check_csv(output_details_csv, target_details_csv, details_or_result="details")
    is_pass_result_csv = check_csv(output_result_csv, target_result_csv, details_or_result="result")
    is_pass = is_pass_details_csv and is_pass_result_csv
    if is_pass:
        sys.exit(0)
    else:
        sys.exit(1)
