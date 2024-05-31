import io
import logging
import socketserver
from http import server
from threading import Condition

# Import necessary modules from picamera2 library
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput

# Define the HTML page content for the video stream
PAGE = """\
<html>
<head>
</head>
<body style="margin: 0;">
<img src="stream.mjpg" style="width: 100%; height: 100%; object-fit: cover;">
</body>
</html>
"""

# Class to handle the streaming output
class StreamingOutput(io.BufferedIOBase):
    def __init__(self):
        self.frame = None
        self.condition = Condition()

    # Write method to update the frame and notify all waiting threads
    def write(self, buf):
        with self.condition:
            self.frame = buf
            self.condition.notify_all()

# Class to handle HTTP requests
class StreamingHandler(server.BaseHTTPRequestHandler):
    # Method to handle GET requests
    def do_GET(self):
        if self.path == '/':
            # Redirect to /index.html
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            # Serve the HTML page
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            # Stream the MJPEG video
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            # Send a 404 error for unknown paths
            self.send_error(404)
            self.end_headers()

# Class to handle the streaming server
class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

# Initialize and configure the Picamera2 object
picam2 = Picamera2()
picam2.configure(picam2.create_video_configuration(main={"size": (640, 480)}))
output = StreamingOutput()

# Start recording with the JpegEncoder and FileOutput
picam2.start_recording(JpegEncoder(), FileOutput(output))

try:
    # Set the server address and start the HTTP server
    address = ('', 7123)
    server = StreamingServer(address, StreamingHandler)
    server.serve_forever()
finally:
    # Stop the recording when the server is stopped
    picam2.stop_recording()
