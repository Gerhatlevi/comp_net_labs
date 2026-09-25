# Running results
The following are the results from running the measurement script with the values 100, 300, 500.
The script was also run with a message size of 1000.
The script was run from a computer connecting to a raspi running the server code on the same
home network. The provided times are in milliseconds.

Note that both implementations ran into issues where too many file descriptors were held
open when number of connections was set to 1000. Both implementations also started running
into issues of dropped connections. Epoll had one instance of 8 dropped connections when
running for 500 connections. Select had a similar outcome with one instance of 7 dropped
connections.

It appears as though the new epoll server implementation does not provide any timing based
improvements based on these measurements.

## Epoll Server
### Average conn time
12,065975
12,753344
19,428442

### Average total time
4727,974367
7368,700218
12090,472841

## I/O multiplexing server

### Average conn time
7,712652
13,163079
18,996543

### Average total time
5068,591118
7553,783655
11940,226555
