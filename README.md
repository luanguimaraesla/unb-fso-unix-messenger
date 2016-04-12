# OSF Unix-Messenger
This repository was created for the operating systems lab of University of Brasília (BRA) and contains a simple messenger program to demonstrate how does message queues, shared memory and sockets works.

### Software requirements
This application is implemented with no graphic user interface, it's function is only demonstrate some behaviors of multiprocess/multithreading programming. But, to be friendlier to the user, it was decided that the software would run on tmux(https://tmux.github.io/), a powerful terminal multiplexer.

For Debian-based systems run:  
  ```shell
 $ sudo apt-get install tmux
  ```

### Shared memory/Socket implementations

Here are two implementations for this software. The first is based on UNIX shared memory and the second is based on UNIX domain socket. Signals, message queues and multiprocess programming is used in both.

* **_Shared Memory_**    
A shared memory is used to send and receive messages and is the main part of a full duplex communication between two modules. Each module is a block that contains three simple processes.  The first is an IO handler and communicates itself with the second process by a message queue. The second is a transmitter and receiver, it is like a bridge between the first process and another external module. Finally, the third is the module manager, as especific signal handler.  

To execute this version of the messenger  
1) _go to the shared_memory folder and compile_  
```shell
$ cd shared_memory
$ make
```  
2) _run_
```shell
$ make run
``` 
3) _or run with logs_  
```shell
$ make run_and_show_logs
```  
  
  
* **_Socket_**  
A socket is used to send and receive messages across a network connections and is the main part of a duplex communication. Each module preserves the same organization of the shared memory version, but the transmitter and receiver module has had to be improved with some parallelism (using POSIX threads) to support an event oriented software. Some signals were used to avoid the semaphore use, a requirement of the task. 

*To execute this version of the messenger*

1) _go to the socket folder and compile_  
```shell
$ cd socket  
$ make
```  
2) _run_  
```shell
$ make run
```  
3) _or run with logs_  
```shell
$ make run_and_show_logs
```  
