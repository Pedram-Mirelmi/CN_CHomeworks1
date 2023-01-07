import socket


PORT = 8000
HOST = "127.0.0.1"


if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    
    
    msgType = (1).to_bytes(1, "little")
    msgSize = (len("AliRanjbari")).to_bytes(4, "little")
    text = "AliRanjbari".encode("ascii")
    msg = msgType + msgSize + text
        
    s.send(msg)
    # a = s.recv(1024)
    # print(a)
    s.close()
