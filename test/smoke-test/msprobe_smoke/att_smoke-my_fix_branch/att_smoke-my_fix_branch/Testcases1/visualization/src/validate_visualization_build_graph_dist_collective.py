import sys
from validate_utils import validate_build_db, get_query_result, CONTENT, validate_one_node_matched_distributed, check_dist_matched_num
file_path = sys.argv[1]

broadcast0_template = {
    "communications_type": "send",
    "nodes_info": {
        "1": ["Nan", "Distributed.broadcast.0.forward"],
        "2": ["Nan", "Distributed.broadcast.0.forward"],
        "3": ["Nan", "Distributed.broadcast.0.forward"],
        "4": ["Nan", "Distributed.broadcast.0.forward"],
        "5": ["Nan", "Distributed.broadcast.0.forward"],
        "6": ["Nan", "Distributed.broadcast.0.forward"],
        "7": ["Nan", "Distributed.broadcast.0.forward"]
    }
}
_all_gather_base55_template = {
    "communications_type": "send_receive",
    "nodes_info": {
        "1": ["Nan", "Distributed._all_gather_base.55.forward"],
        "2": ["Nan", "Distributed._all_gather_base.55.forward"],
        "3": ["Nan", "Distributed._all_gather_base.55.forward"],
        "4": ["Nan", "Distributed._all_gather_base.55.forward"],
        "5": ["Nan", "Distributed._all_gather_base.55.forward"],
        "6": ["Nan", "Distributed._all_gather_base.55.forward"],
        "7": ["Nan", "Distributed._all_gather_base.55.forward"]
    }
}

nodes = validate_build_db(file_path, 55968)

validate_one_node_matched_distributed(file_path, "Distributed.broadcast.0.forward", broadcast0_template)
validate_one_node_matched_distributed(file_path, "Distributed._all_gather_base.55.forward", _all_gather_base55_template)

check_dist_matched_num(file_path, 689)

print("Validate the .vis.db file successfully.")
