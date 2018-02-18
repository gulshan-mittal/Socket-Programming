#!/usr/bin/python2

import sys
import socket
import time

def sendData (sock, data):
    """
    Send data to the client using socket 'sock'
    """
    try :
        sock.send(data)
    except socket.error, e :
        print("Error sending data: %s" %e)
        sys.exit(1)

def recvData (sock, size):
    """
    Receive data of size 'size' from the client using socket 'sock'
    """
    try :
        data = sock.recv(size)
    except socket.error, e :
        print("Error receiving data: %s" %e)
        sys.exit(1)

    return data

def fileRecv (sock, filename):
    """
    Function to receive the file 'filename' from the server 
    using the socket 'sock' and write in on the client side
    """
    confirm = recvData(sock, size=2048)
    
    if "---File Present---" in confirm:
        print("---File Present---")
        with open('received_' + filename, 'wb') as f:
            print('file opened')

            sendData(sock, data='OK')
            print('Receiving Data...')
            while True:
                #print('receiving data...')
                data = recvData(sock, size=4096)
                #print("data = %s", (data))

                if not data or data == 'EOF':
                    break
                    
                # write data to a file
                f.write(data)
                sendData(sock, data='OK')

        f.close()
        print('Successfully got the file:', filename)
    else :
        print("---File Not Present---")

def nonPersistentConnection (host, port, filenames):
    """
    Function to exchange data with the server through Non Persistent Connection
    """
    for filename in filenames:

        try :
            client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error, e :
            print("Error creating server socket: %s" %e)
            sys.exit(1)
        
        try:
            client_sock.connect((host, port))
        except socket.gaierror, e:
            print("Address-related error connecting to server: %s" % e)
            sys.exit(1)
        except socket.error, e:
            print("Connection Error: %s" %e)
            sys.exit(1)

        print("Sending filename:", filename)
        sendData(client_sock, filename)

        fileRecv(client_sock, filename)

        client_sock.close()
        print('connection closed')

def persistentConnection (host, port, filenames):
    """
    Function to exchange data with the server through Persistent Connection
    """
    try :
        client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error, e :
        print("Error creating server socket: %s" %e)
        sys.exit(1)

    try:
        client_sock.connect((host, port))
    except socket.gaierror, e:
        print("Address-related error connecting to server: %s" % e)
    except socket.error, e:
        print("Connection Error: %s" %e)
    
    for filename in filenames:
        print("Sending filename:", filename)
        sendData(client_sock, filename)

        fileRecv(client_sock, filename)

    client_sock.close()
    print('connection closed')

if __name__ == '__main__':
    host = ""
    port = 60007                

    filenames = raw_input("Enter files to get:").strip().split()
    
    # Initial time in milliseconds before accepting a connection
    initial_time = int(round(time.time() * 1000))

    if sys.argv[1] == '1':
        nonPersistentConnection(host, port, filenames)
    elif sys.argv[1] == '2':
        persistentConnection(host, port, filenames)

    # Final time in milliseconds after closing of the connection
    final_time = int(round(time.time() * 1000))

    print(final_time - initial_time)