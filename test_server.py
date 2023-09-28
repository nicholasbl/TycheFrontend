from jsonrpclib.SimpleJSONRPCServer import SimpleJSONRPCServer
from types import SimpleNamespace
import json
import time
import numpy as np

def to_object(d):
    if isinstance(d, dict):
        ns = SimpleNamespace()
        for k,v in d.items():
            setattr(ns, k, to_object(v))
        return ns
    elif isinstance(d, list):
        return [to_object(v) for v in d]
    else:
        return d

def to_dict(obj):
    if isinstance(obj, (int, float, str, bool, type(None))):
        return obj
    elif isinstance(obj, dict):
        return {key: to_dict(value) for key, value in obj.items()}
    elif isinstance(obj, (list, tuple)):
        return [to_dict(item) for item in obj]
    elif hasattr(obj, '__dict__'):
        return {key: to_dict(value) for key, value in vars(obj).items()}
    else:
        raise ValueError(f"Unsupported type: {type(obj)}")

scenario_defs = json.loads('''
[
{
    "name" : "Scenario 1",
    "description" : "This is a testing scenario",
    "image" : "https://picsum.photos/id/77/1631/1102",
    "id" : "376A56C2-337D-4467-95D0-2D4CB45777D9",
    "category_defs" : [
        {
            "name"        : "BoS (R&D)",
            "description" : "This is an alpha desc",
            "image"       : "https://picsum.photos/id/220/387/241",
            "starting_investment"  : 350526
        },{
            "name"        : "Inverter (R&D)",
            "description" : "This is an beta desc",
            "image"       : "https://picsum.photos/id/252/500/328",
            "starting_investment"  :  650101
        },{
            "name"        : "Module (R&D)",
            "description" : "This is an delta desc",
            "image"       : "https://picsum.photos/id/281/492/326",
            "starting_investment"  :  593949
        },{
            "name"        : "Stand (R&D)",
            "description" : "This is an gamma desc",
            "image"       : "https://picsum.photos/id/297/500/322",
            "starting_investment"  :  348276
        }
    ],
    "metric_defs" : [
        {
            "name"        : "GHG",
            "description" : "This is an alpha desc",
            "image"       : "https://picsum.photos/id/99/491/326"
        },{
            "name"        : "LCOE",
            "description" : "This is an beta desc",
            "image"       : "https://picsum.photos/id/111/440/265"
        },{
            "name"        : "Labor",
            "description" : "This is an delta desc",
            "image"       : "https://picsum.photos/id/175/289/194"
        },{
            "name"        : "WIA",
            "description" : "This is an gamma desc",
            "image"       : "https://picsum.photos/id/180/240/160"
        }
    ]
},{
    "name" : "Scenario 2",
    "description" : "This is a testing scenario 2",
    "image" : "https://picsum.photos/id/78/1584/2376",
    "id" : "3FAB6015-ACB1-4733-B104-E4AA8732813D",
    "category_defs" : [
        {
            "name"        : "BoS (R&D) 2",
            "description" : "This is an alpha desc",
            "image"       : "https://placehold.co/100.png",
            "starting_investment"  : 958978
        },{
            "name"        : "Inverter (R&D) 2",
            "description" : "This is an beta desc",
            "image"       : "https://placehold.co/100.png",
            "starting_investment"  :  345379
        },{
            "name"        : "Module (R&D) 2",
            "description" : "This is an delta desc",
            "image"       : "https://placehold.co/100.png",
            "starting_investment"  :  920064
        },{
            "name"        : "Stand (R&D) 2",
            "description" : "This is an gamma desc",
            "image"       : "https://placehold.co/100.png",
            "starting_investment"  :  92774
        }
    ],
    "metric_defs" : [
        {
            "name"        : "GHG 2",
            "description" : "This is an alpha desc",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "LCOE 2",
            "description" : "This is an beta desc",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "Labor 2",
            "description" : "This is an delta desc",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "WIA 2",
            "description" : "This is an gamma desc",
            "image"       : "https://placehold.co/100.png"
        }
    ]
}
]
''')

def generate_random_normal(mean, stddev, count):
    return list(np.random.normal(mean, stddev, count))

def get_scenarios():
    time.sleep(1)
    print("Sending scenario list")
    return scenario_defs

def run_scenario(scenario_state):
    time.sleep(1)
    print("Running scenario:", scenario_state)
    scenario_state = to_object(scenario_state)
    sid = scenario_state.scenario_id

    # generate some fun data

    per_cat_data = []

    for c_i in range(4):
        per_metric_data = []
        for m_i in range(4):
            cell = generate_random_normal(m_i*10, 10, 25)
            cell = { "x" : cell }
            per_metric_data.append(cell)
        per_cat_data.append(per_metric_data)

    ret = to_dict({
        "scenario_id" : sid,
        "category_state" : [i.investment for i in scenario_state.category_states],
        # dont need metric states
        "cells" : per_cat_data
    })

    print(ret)
    return ret



server = SimpleJSONRPCServer(('localhost', 8080))
server.register_function(get_scenarios)
server.register_function(run_scenario)
server.serve_forever()

