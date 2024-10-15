from xmlrpc.server import SimpleXMLRPCServer

# Define remote functions
def add(x, y):
    return x + y

def subtract(x, y):
    return x - y

def multiply(x, y):
    return x*y

# Create an XML-RPC server
server = SimpleXMLRPCServer(("localhost", 8000))
print("Listening on port 8000...")

# Register functions
server.register_function(add, "add")
server.register_function(subtract, "subtract")
server.register_function(multiply, "multiply")

# Run the server
server.serve_forever()
