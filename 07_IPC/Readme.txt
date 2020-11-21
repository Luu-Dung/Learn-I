Refer: https://opensource.com/article/19/4/interprocess-communication-linux-storage
        https://condor.depaul.edu/mkalin/
        https://ttl.engineering/giao-tiep-lien-tien-trinh-inter-process-communication-ipc-trong-he-dieu-hanh-linux/

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
    - Shared file(file-based): (Create a NORMAL FILE -> open -> read/write as normal file -> need "flock" to synchronize)
        + need a "flock" to avoid race condition.
        + data shared through IPC is text.
        + file access is relatively slow.

    - Shared memory(memory-based): (create a "BACKING FILE" -> allocate memory for it -> map a pointer to this memory and USE)
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

    - Sockets:

Message
    - SystemV message queue:
    - POSIX message queue:
        + A message queue is a sequence of messages.
        + Message is a struct includes: payload( is char[])  and type (is positive integer).
        + Message queues can behave in the same way but are flexible enough that byte chunks can be retrieved out of FIFO order.

            SENDER:   Generate key -> create queueID -> create msg's value -> send msg.
            RECEIVER: Generate key -> get queueID -> receive message -> remove queue (optional).