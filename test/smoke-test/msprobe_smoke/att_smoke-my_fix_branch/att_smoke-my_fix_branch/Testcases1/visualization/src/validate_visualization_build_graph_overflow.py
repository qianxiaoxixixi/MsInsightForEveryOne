import sys
import json
from validate_utils import validate_build_db, get_query_result, CONTENT

file_path = sys.argv[1]

nodes = validate_build_db(file_path, 3326)

result = get_query_result(file_path, "SELECT overflow_level FROM tb_nodes WHERE step=0 AND rank=0")

m = h = c = 0
for item in result:
    ov_level = item[0]
    if ov_level == "medium":
        m += 1
    elif ov_level == "high":
        h += 1
    elif ov_level == "critical":
        c += 1

if m != 1 or h != 41 or c != 1:
    print(CONTENT, f"The number of overflow level: medium is {m}, high is {h}, critical is {c}, and the expected number is medium:1, high:41, critical:1.")
    exit()


print("Validate the .vis.db file successfully.")
