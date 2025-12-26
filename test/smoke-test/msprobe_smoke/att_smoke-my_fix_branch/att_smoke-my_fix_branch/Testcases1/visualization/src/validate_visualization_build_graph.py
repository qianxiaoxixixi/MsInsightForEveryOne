import sys
from validate_utils import validate_build_db

file_path = sys.argv[1]

validate_build_db(file_path, 2851)

print("Validate the .vis.db file successfully.")
