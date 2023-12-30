#!/usr/bin/python3

from http.server import SimpleHTTPRequestHandler
from socketserver import TCPServer

host = "0.0.0.0"
port = 8000

class CustomHandler(SimpleHTTPRequestHandler):
	def __init__(self, *args, **kwargs):
		super().__init__(*args, directory="./html/", **kwargs)

handler = CustomHandler

with TCPServer((host, port), handler) as httpd:
	print(f"Serving on http://{host}:{port}") # f-string 3.6~
	httpd.serve_forever()

