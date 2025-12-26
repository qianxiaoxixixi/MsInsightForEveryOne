import sys
import json
from validate_utils import validate_build_db, get_query_result, CONTENT, validate_one_node_matched_distributed, check_dist_matched_num

file_path = sys.argv[1]

nodes = validate_build_db(file_path, 111936)

broadcast0_template = {
    "communications_type": "receive",
    "nodes_info": {
        "0": ["Nan", "Distributed.broadcast.0.forward"]
    }
}
_all_gather_base55_template = {
    "communications_type": "send_receive",
    "nodes_info": {
        "0": ["Nan", "Distributed._all_gather_base.55.forward"],
        "2": ["Nan", "Distributed._all_gather_base.55.forward"],
        "3": ["Nan", "Distributed._all_gather_base.55.forward"],
        "4": ["Nan", "Distributed._all_gather_base.55.forward"],
        "5": ["Nan", "Distributed._all_gather_base.55.forward"],
        "6": ["Nan", "Distributed._all_gather_base.55.forward"],
        "7": ["Nan", "Distributed._all_gather_base.55.forward"]
    }
}



validate_one_node_matched_distributed(file_path, 'Distributed.broadcast.0.forward', broadcast0_template, 1, 1)
validate_one_node_matched_distributed(file_path, 'Distributed._all_gather_base.55.forward', _all_gather_base55_template, 1, 1)

check_dist_matched_num(file_path, 689, 1, 1)

print("Validate the .vis.db file successfully.")
