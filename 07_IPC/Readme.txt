Refer: https://opensource.com/article/19/4/interprocess-communication-linux-storage
        https://condor.depaul.edu/mkalin/
        https://ttl.engineering/giao-tiep-lien-tien-trinh-inter-process-communication-ipc-trong-he-dieu-hanh-linux/
        https://en.wikipedia.org/wiki/Inter-process_communication

In C to clarify the following IPC mechanisms:
• Shared files
• Shared memory (with semaphores)
• Pipes (named and unnamed)
• Message queues
• Sockets
• Signals

Core concepts:
- Different processes, by default, do not share memory.

Some traits:

1. Shared storage:
    - Shared file(file-based): A record stored on disk, or a record synthesized on demand by a file server, which can be accessed by multiple processes.
        + (Create a NORMAL FILE -> open -> read/write as normal file -> need "flock" to synchronize)
        + need a "flock" to avoid race condition.
        + data shared through IPC is text.
        + file access is relatively slow.

    - Shared memory(memory-based): Multiple processes are given access to the same block of memory, which creates a shared buffer for the processes to communicate with each other.
        + (create a "BACKING FILE" -> allocate memory for it -> map a pointer to this memory and USE)
        + need semaphore,mutex,... to avoid race condition.

2. DATA Transfer:    In general, channel-based IPC is concurrent-safe (Race condition can't arise in multi-threads/processes).
Byte stream
    - pipe(channel-based):
        + named pipe (FIFO): Có thể  tính là "Shared file"
             https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/
             Appears as a file (FIFO FILE) on the local storage.
             DO NOT Race condition
        + unamed pipe:
             create 2 file descriptor (fd[2]) for pipechannel -> init channel by pipe(fd)
             -> fd[0] for reader, fd[1] for writer -> in reader, close fd of writer and nguoc lai.
             -> for dup2() to change type of pipe fd.
                    ex: dup2(fd[0],STDOUT_FILENO);

    - Sockets (channel-based): Data sent over a network interface; 4 types: stream, datagram, sequential packet, raw.
        + IPC sockets (aka Unix domain sockets) enable channel-based communication for processes on the same physical device (host) that is different from "Network socket".
        + IPC sockets rely upon the local system kernel to support communication (IPC sockets communicate using a local file as a socket address);
        + Sockets configured as streams are "bidirectional", and control follows a client/server pattern.
        + The sample server and client programs can run on the same machine because the server uses network address localhost (127.0.0.1), the address for the local machine on the local machine.

Message
    - SystemV message queue:
    - POSIX message queue:
        + A message queue is a sequence of messages.
        + Message is a struct includes: payload( is char[])  and type (is positive integer).
        + Message queues can behave in the same way but are flexible enough that byte chunks can be retrieved out of FIFO order.

            SENDER:   Generate key -> create queueID -> create msg's value -> send msg.
            RECEIVER: Generate key -> get queueID -> receive message -> remove queue (optional).

    - Signal: A system message sent from one process to another, not usually used to transfer data but instead used to remotely command the partnered process.
        + Set handler:
            * signal(sig, handler_func name)
            * sigaction(sig, sigaction struct, NULL)
        + Send signal:
            * kill(sig, pid) : system call sends a specified signal to a "specified" process.
            * raise(sig) : library function sends the specified signal to the "current" process. 
        + Signal handling is vulnerable to race conditions. (another signal (even of the same type) can be delivered to the process during execution of the signal handling routine).
            -> sigprocmask(..) call can be used to block and unblock delivery of signals. Blocked signals are not delivered to the process until unblocked ( except for SIGKILL and SIGSTOP).