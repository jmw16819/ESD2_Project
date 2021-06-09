'''
https://github.com/jmw16819/ESD2_Project
WeFlow - a water leak detection system
by Jordyn Washington 2021 - MIT License
'''

'''
Python file used to make webserver for water leak detection
system. Consists of a main web handler to render the webpage,
as well as 7 WebSocket handlers for the webpage client and 
the 6 ESP32 clients. on_message callbacks for each client, as
well as the functions after the websocket classes used to send
messeages are defined as asynchronous so that messages can be
sent between all the clients at any time
'''

import tornado.web
import tornado.websocket
import tornado.ioloop
import tornado.httpserver
import asyncio
import os.path

socket_clients = [] # Empty list that stores all clients connected to the server
MCU_socket_clients = [] # Empty list that stores all ESP32 clients connected to the server
page_socket_clients = [] # Empty list to hold the webpage client when connected
# Data dictionary that will be sent to the webpage client
page_data_dict = {
    "sensors":{
        "s1":{
            "pressure": 0,
            "LED":{
                "green": "",
                "yellow": "",
                "red": ""
            }
        },
        "s2":{
            "pressure": 0,
            "LED":{
                "green": "",
                "yellow": "",
                "red": ""
            }
        },
        "s3":{
            "pressure": 0,
            "LED":{
                "green": "",
                "yellow": "",
                "red": "",
            } 
        }
    },
    "valves":{
        "v1": "",
        "v2": "",
        "v3": ""
    }
}

# web.RequestHandler to render the monitor webpage
class mainHandler(tornado.web.RequestHandler):
    def get(self):
        self.render("index.html")
        
# WebSocketHandler for the webpage client
class pageSocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Monitor websocket opened")
        socket_clients.append(self)
        page_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        await MCU_send_data(message)

    def on_close(self):
        print("Monitor websocket closed")
        socket_clients.remove(self)
        page_socket_clients.remove(self)

# WebSocketHandler for the Sensor 1 client
class sensor1SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Sensor 1 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find("pressure")
        j = message.find(";")
        page_data_dict["sensors"]["s1"]["pressure"] = message[i+10:j]
        i = message.find("red")
        page_data_dict["sensors"]["s1"]["LED"]["red"] = message[i+4:i+5]
        i = message.find("yellow")
        page_data_dict["sensors"]["s1"]["LED"]["yellow"] = message[i+7:i+8]
        i = message.find("green")
        page_data_dict["sensors"]["s1"]["LED"]["green"] = message[i+6:i+7]
        await MCU_send_data("Sensor 1: Data received")
        await asyncio.sleep(0.5)
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Sensor 1 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

# WebSocketHandler for the Sensor 2 client
class sensor2SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Sensor 2 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find("pressure")
        j = message.find(";")
        page_data_dict["sensors"]["s2"]["pressure"] = message[i+10:j]
        i = message.find("red")
        page_data_dict["sensors"]["s2"]["LED"]["red"] = message[i+4:i+5]
        i = message.find("yellow")
        page_data_dict["sensors"]["s2"]["LED"]["yellow"] = message[i+7:i+8]
        i = message.find("green")
        page_data_dict["sensors"]["s2"]["LED"]["green"] = message[i+6:i+7]
        await MCU_send_data("Sensor 2: Data received")
        await asyncio.sleep(0.5)
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Sensor 2 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

# WebSocketHandler for the Sensor 3 client
class sensor3SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Sensor 3 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find("pressure")
        j = message.find(";")
        page_data_dict["sensors"]["s3"]["pressure"] = message[i+10:j]
        i = message.find("red")
        page_data_dict["sensors"]["s3"]["LED"]["red"] = message[i+4:i+5]
        i = message.find("yellow")
        page_data_dict["sensors"]["s3"]["LED"]["yellow"] = message[i+7:i+8]
        i = message.find("green")
        page_data_dict["sensors"]["s3"]["LED"]["green"] = message[i+6:i+7]
        await MCU_send_data("Sensor 3: Data received")
        await asyncio.sleep(0.5)
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Sensor 3 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

# WebSocketHandler for the Valve 1 client
class valve1SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Valve 1 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find('1')
        page_data_dict['valves']['v1'] = message[i+2:]
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Valve 1 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

# WebSocketHandler for the Valve 2 client
class valve2SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Valve 2 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find('2')
        page_data_dict['valves']['v2'] = message[i+2:]
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Valve 2 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

# WebSocketHandler for the Valve 3 client
class valve3SocketHandler(tornado.websocket.WebSocketHandler):
    def check_origin(self, origin):
        return True
    
    def open(self):
        print("Valve 3 websocket opened")
        socket_clients.append(self)
        MCU_socket_clients.append(self)
        update_clients()

    async def on_message(self, message):
        # Break down the message and put the proper values in the dictionary
        i = message.find('3')
        page_data_dict['valves']['v3'] = message[i+2:]
        await page_send_data(page_data_dict)
        
    def on_close(self):
        print("Valve 3 websocket closed")
        socket_clients.remove(self)
        MCU_socket_clients.remove(self)

''' Function to send messages to all clients connected to the server '''
def update_clients():
    for client in socket_clients:
        client.write_message("Ready to receive data.")

''' Function to send data to the webpage client. d represents a data dictionary '''
async def page_send_data(d):
    for client in page_socket_clients:
        await client.write_message(d)

''' Function to send data to the ESP32 clients. msg is the data that needs to be sent
    At the moment all messages will be sent to all clients, and it will be left to each
    client to find any messages that may pertain to it '''
async def MCU_send_data(msg):
    for client in MCU_socket_clients:
        await client.write_message(msg)

if __name__ == "__main__":
    app = tornado.web.Application(
        [
            (r"/", mainHandler),
            (r"/pagesocket", pageSocketHandler),
            (r"/s1socket", sensor1SocketHandler),
            (r"/s2socket", sensor2SocketHandler),
            (r"/s3socket", sensor3SocketHandler),
            (r"/v1socket", valve1SocketHandler),
            (r"/v2socket", valve2SocketHandler),
            (r"/v3socket", valve3SocketHandler),
        ],
        template_path = os.path.join(os.path.dirname(__file__), "templates"),
        static_path = os.path.join(os.path.dirname(__file__), "templates/static"),
    )
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()