#!/usr/bin/python2

import socket
import sys
import time

def sendData (sock, data):
    """
    Send data to the server using socket 'sock'
    """
    try :
        sock.send(data)
    except socket.error, e :
        print("Error sending data: %s" %e)
        sys.exit(1)

def recvData (sock, size):
    """
    Receive data of size 'size' from the server using socket 'sock'
    """
    try :
        data = sock.recv(size)
    except socket.error, e :
        print("Error receiving data: %s" %e)
        sys.exit(1)
    
    return data


def fileSend (sock, filename):
    """
    Function to read and send the file 'filename' 
    to the client using the socket 'sock'
    """
    try :
        fd = open('./Data/' + filename, 'rb')
        sock.send("---File Present---")
    except IOError :
        sock.send("---File Not Present---")
        return
    ack = recvData(sock, size=64)

    print('Sending Data...')
    read_data = fd.read(4096)
    while (read_data):
       #print('sending data...')
       sendData(sock, read_data)
       #print('Sent ',repr(read_data))
       ack = recvData(sock, size=64)
       read_data = fd.read(4096)
    fd.close()
    
    sendData(sock, data='EOF')



def nonPersistentConnection():
    """
    Function to exchange data with the client through Non Persistent Connection
    """
    conn, addr = s.accept()
    s.settimeout(None)
    print('Got connection from', addr)
    filename = recvData(conn, size=4096)
    print('Server received', repr(filename))

    fileSend(conn, filename)

    print('Done sending')
    #conn.send('Thank you for connecting')
    conn.close()
    print('connection closed')

def persistentConnection():
    """
    Function to exchange data with the client through Persistent Connection
    """
    conn, addr = s.accept()
    conn.settimeout(10)
    print('Got connection from', addr)
    
    filename = recvData(conn, size=2048)
    
    while filename:
        print('Server received', repr(filename))

        fileSend(conn, filename)

        filename = recvData(conn, size=2048)

    print('Done sending')
    #conn.send('Thank you for connecting')
    conn.close()
    print('connection closed')


if __name__ == '__main__':
    port = 60007
    host = ""

    try :
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error, e :
        print("Error creating server socket: %s" %e)
        sys.exit(1)

    try :
        s.bind((host, port))
    except socket.error, e :
        print("Error in binding host and port: %s" %e)

    s.listen(5)


    print('Server listening....')
    while True:
        # Initial time in milliseconds before accepting a connection
        initial_time = int(round(time.time() * 1000))

        if sys.argv[1] == '1':
            nonPersistentConnection()
        elif sys.argv[1] == '2':
            persistentConnection()

        # Final time in milliseconds after closing of the connection
        final_time = int(round(time.time() * 1000))

        print(final_time - initial_time)