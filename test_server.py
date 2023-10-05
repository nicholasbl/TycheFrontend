#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer

from jsonrpcserver import method, Result, Success, dispatch

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
            "id"          : "CE5A3EBA-3DBB-4A33-A20B-F88506D412FE",
            "starting_investment"  : 350526
        },{
            "name"        : "Inverter (R&D)",
            "description" : "This is an beta desc",
            "image"       : "https://picsum.photos/id/252/500/328",
            "id"          : "DA06153E-0207-47E6-BDB9-956DED2C7176",
            "starting_investment"  :  650101
        },{
            "name"        : "Module (R&D)",
            "description" : "This is an delta desc",
            "image"       : "https://picsum.photos/id/281/492/326",
            "id"          : "EE0C9AF7-A6B7-4282-89D0-661EEF4D58DC",
            "starting_investment"  :  593949
        },{
            "name"        : "Stand (R&D)",
            "description" : "This is an gamma desc",
            "image"       : "https://picsum.photos/id/297/500/322",
            "id"          : "2126E533-86E1-48A8-A511-A97403D103EF",
            "starting_investment"  :  348276
        }
    ],
    "metric_defs" : [
        {
            "name"        : "GHG",
            "description" : "This is an alpha desc",
            "id"          : "01814752-4410-463B-A5C0-79699415ED72",
            "image"       : "https://picsum.photos/id/99/491/326"
        },{
            "name"        : "LCOE",
            "description" : "This is an beta desc",
            "id"          : "DA1075AF-700D-4A8B-BE33-2EBCD9FE926E",
            "image"       : "https://picsum.photos/id/111/440/265"
        },{
            "name"        : "Labor",
            "description" : "This is an delta desc",
            "id"          : "62887BFC-17F7-4B44-A983-217DD83CF002",
            "image"       : "https://picsum.photos/id/175/289/194"
        },{
            "name"        : "WIA",
            "description" : "This is an gamma desc",
            "id"          : "8E1B9C3B-EB0E-4C65-9439-DB3003940915",
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
            "id"          : "32F307F1-3BAD-40B6-A54A-C0BEB1CF30A3",
            "starting_investment"  : 958978
        },{
            "name"        : "Inverter (R&D) 2",
            "description" : "This is an beta desc",
            "image"       : "https://placehold.co/100.png",
            "id"          : "4867A7DA-E777-41D2-A99D-B158A8DDE1A4E",
            "starting_investment"  :  345379
        },{
            "name"        : "Module (R&D) 2",
            "description" : "This is an delta desc",
            "image"       : "https://placehold.co/100.png",
            "id"          : "93504AEB-0EAB-4583-8401-446AF81799AD",
            "starting_investment"  :  920064
        },{
            "name"        : "Stand (R&D) 2",
            "description" : "This is an gamma desc",
            "image"       : "https://placehold.co/100.png",
            "id"          : "FF6D636D-04C7-4C81-A995-54274974F807",
            "starting_investment"  :  92774
        }
    ],
    "metric_defs" : [
        {
            "name"        : "GHG 2",
            "description" : "This is an alpha desc",
            "id"          : "0949D298-B8D2-417F-8776-E518861C2376",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "LCOE 2",
            "description" : "This is an beta desc",
            "id"          : "449685E7-C7A2-46B4-98D1-CF2C3D2B258B",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "Labor 2",
            "description" : "This is an delta desc",
            "id"          : "2A70019E-64AD-477C-98FC-FBDC8055BCA9",
            "image"       : "https://placehold.co/100.png"
        },{
            "name"        : "WIA 2",
            "description" : "This is an gamma desc",
            "id"          : "750FD99A-FF67-4C56-8B4D-122950738344",
            "image"       : "https://placehold.co/100.png"
        }
    ]
}
]
''')

def generate_random_normal(mean, stddev, count):
    return list(np.random.normal(mean, stddev, count))

@method
def get_scenarios():
    time.sleep(1)
    print("Sending scenario list")
    return Success(scenario_defs)

@method
def run_scenario(scenario_state):
    sds = to_object(scenario_defs)

    time.sleep(1) # pretend we have network latency
    print("Running scenario:", scenario_state)
    scenario_state = to_object(scenario_state)
    sid = scenario_state.scenario_id

    # find that scenario

    scenario = None

    for s in sds:
        if s.id != sid: continue
        scenario = s

    # generate some fun data

    per_cat_data = {}

    for c_i in scenario.category_defs:
        per_metric_data = {}
        count = 0
        for m_i in scenario.metric_defs:
            cell = generate_random_normal(count*10, 10, 25)
            count += 1
            per_metric_data[m_i.id] = cell
        per_cat_data[c_i.id] = per_metric_data

    ret = to_dict({
        "scenario_id" : sid,
        "category_state" : scenario_state.category_states,
        "cells" : per_cat_data
    })

    print(ret)
    return Success(ret)


@method
def optimize_scenario(scenario_opt):
    sds = to_object(scenario_defs)
    time.sleep(1)
    print("Running optim:", scenario_opt)
    scenario_state = to_object(scenario_opt)
    sid = scenario_state.scenario_id

    # find that scenario

    scenario = None

    for s in sds:
        if s.id != sid: continue
        scenario = s

    # generate some fun data

    per_cat_data = []

    per_cat_data = {}

    for c_i in scenario.category_defs:
        per_metric_data = {}
        count = 0
        for m_i in scenario.metric_defs:
            cell = generate_random_normal(count*10, 10, 25)
            count += 1
            per_metric_data[m_i.id] = cell
        per_cat_data[c_i.id] = per_metric_data

    ret = to_dict({
        "scenario_id" : sid,
        "category_state" : {i.id: np.random.random(4) * 10000 for i in scenario.category_defs},
        # dont need metric states
        "cells" : per_cat_data
    })

    print(ret)
    return Success(ret)

class JSONRPCHTTPServer(BaseHTTPRequestHandler):
    def do_POST(self):
        request = self.rfile.read(int(self.headers["Content-Length"])).decode()
        response = dispatch(request)

        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "*")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.send_header("Content-type", "application/json")
        self.end_headers()
        self.wfile.write(response.encode())

    def do_OPTIONS(self):
        print(self.headers)
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "*")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.send_header("Access-Control-Max-Age", "86400")
        self.end_headers()



if __name__ == "__main__":
    HTTPServer(('localhost', 8080), JSONRPCHTTPServer).serve_forever()
