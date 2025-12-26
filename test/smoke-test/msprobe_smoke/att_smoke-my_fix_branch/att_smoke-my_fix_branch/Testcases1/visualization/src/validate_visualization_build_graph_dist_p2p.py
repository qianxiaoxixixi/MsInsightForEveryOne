import sys
from validate_utils import validate_build_db, get_query_result, CONTENT, validate_one_node_matched_distributed, check_dist_matched_num

file_path = sys.argv[1]

nodes = validate_build_db(file_path, 3838)

validate_one_node_matched_distributed(file_path, "Distributed.isend.0.forward", {"communications_type": "send", "nodes_info": {"1": ["Nan", "Distributed.irecv.0.forward"]}})
validate_one_node_matched_distributed(file_path, "Distributed.irecv.0.forward", {"communications_type": "receive", "nodes_info": {"1": ["Nan", "Distributed.isend.4.forward"]}})

check_dist_matched_num(file_path, 8)

print("Validate the .vis.db file successfully.")
