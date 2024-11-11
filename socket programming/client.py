# import socket and threading

# add server ip and port

# create a fucntion for receiving the message use while loop with true condition if message is there then simply print it else connection break

#define main function create socket for client and connect it with ip and port number create thread for receiving message use a while loop with true
# condition and take input for message now if message is quit break the loop else send the messages at the end close the server 


import socket
import threading

SERVER_IP  = '127.0.0.1'
PORT = 8080

def receive_message(client_socket):
    while True:
        try:
            message = client_socket.recv(1024)
            print(message.decode())
        except:
            print("Connection to the server is closed.")
            client_socket.close()
            break

def main():
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    client.connect((SERVER_IP,PORT))

    receive_thread = threading.Thread(target=receive_message,args=(client,))
    receive_thread.start()

    while True:
        message = input("Enter message to broadcast: ")
        if(message.lower()=='quit'):
            break
        client.send(message.encode())
    client.close()

if __name__ == '__main__':
    main()