import argparse
import csv
import os
import subprocess
import sys
import time

parser = argparse.ArgumentParser()
parser.add_argument("multi_client_bin")
parser.add_argument("target_hostname")
parser.add_argument("port")
parser.add_argument("num_clients", nargs="+")
parser.add_argument("messages_per_client")
args = parser.parse_args()


exec_name = args.multi_client_bin
host = args.target_hostname
port = args.port
num_clients = args.num_clients
num_messages = args.messages_per_client
repeat = 5

avg_elapsed_time = []
avg_conn_time = []

total_clients = len(num_clients)
for idx, i in enumerate(num_clients):
    print(f"[{idx + 1}/{total_clients}] Running analysis for {i} clients")
    temp = 0
    conn_time = 0
    for j in range(repeat):
        start_time = time.time()
        proc = subprocess.Popen(
            [os.path.join("./", exec_name), host, port, i, num_messages],
            stderr=subprocess.STDOUT,
            stdout=subprocess.PIPE,
        )
        proc.wait()
        temp += 1000.0 * (time.time() - start_time)
        out = str(proc.communicate()[0])
        print(out)

        g = out.split("average time:")
        conn_time += float(g[1].split()[0])
        time.sleep(1)
    avg_elapsed_time.append(temp / float(repeat))
    avg_conn_time.append(conn_time / float(repeat))

print("-------- results -------")
print("Average total time (in msec)")
for i in avg_elapsed_time:
    print(("%f" % i).replace(".", ","))

print("Average connection time (in msec)")
for i in avg_conn_time:
    print(("%f" % i).replace(".", ","))

f1 = open("output_avg_conn_time.csv", "w")
for i in avg_conn_time:
    f1.write("%f,\n" % i)

f2 = open("output_avg_total_time.csv", "w")
for i in avg_elapsed_time:
    f2.write("%f,\n" % i)
