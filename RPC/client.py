import xmlrpc.client

# Create an XML-RPC client
server_url = "http://localhost:8000/"
client = xmlrpc.client.ServerProxy(server_url)


# Call remote procedures
x = int(input("Enter the value of x : "))
y = int(input("Enter the value of y : "))
result_add = client.add(x, y)
result_subtract = client.subtract(x, y)
result_multiply = client.multiply(x, y)

# Print Result of remote fucntions
print(f"{x} + {y} = {result_add}")
print(f"{x} - {y} = {result_subtract}")
print(f"{x} * {y} = {result_multiply}")