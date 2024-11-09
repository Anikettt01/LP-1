# import socket and threading1

# add host and port number

# create a array clients which will store all the client

# define broadcast function which will be simply decide to whome the message should display we will simply use one loop on array and if client now 
# equal to sender_socket then send message else dont send as we dont sent the message to the client which is sening message

# create one handler for client which will simple use loop which is always true and will take 1024 buffer size for the message if message is there
# then print it and send to all the client by simply calling broadcast function

# define main create a server socket  we are keeping limit of 5 for the number of clients start a true condition loop create a client thread and start it

# finally define main function

import socket
import threading

HOST = '127.0.0.1'
PORT = 8080

clients = []

def broadcast(message,sender_socket):
    for client in clients:
        if client != sender_socket:
            try:
                client.send(message)
            except:
                clients.remove(client)

def client_handler(client_socket):
    while True:
        try:
            message = client_socket.recv(1024)
            if not message:
                break
            print(f"Received: {message.decode()}")
            broadcast(message,client_socket)
        except:
            break
    client_socket.close()

def main():
    server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server.bind((HOST,PORT))

    server.listen(5)
    print(f'server listening on {HOST}:{PORT}')

    while True:
        client_socket,addr = server.accept()
        clients.append(client_socket)
        client_thread = threading.Thread(target=client_handler,args=(client_socket,))
        client_thread.start()

if __name__ == '__main__':
    main()