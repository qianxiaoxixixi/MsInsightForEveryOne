import sys
from validate_utils import validate_compare_db

file_path = sys.argv[1]

validate_compare_db(file_path, 98, 98, 'tensor')

print("Validate the .vis.db file successfully.")
