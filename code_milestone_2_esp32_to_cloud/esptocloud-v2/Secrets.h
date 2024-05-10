
/*********
Header file

Instructions for AWS - https://how2electronics.com/connecting-esp32-to-amazon-aws-iot-core-using-mqtt/

*********/

#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "Kittylog_ESP32"                      //TO DO: type your thing name between the quotation marks
 
const char WIFI_SSID[] = "UW MPSK";              //TO DO: type the WiFi network name (eg. "UW MPSK")
const char WIFI_PASSWORD[] = ";7Wc=9fGi#";          //TO DO: type the WiFi password in the quotes
const char AWS_IOT_ENDPOINT[] = "a1ow9068fxlaoe-ats.iot.us-east-2.amazonaws.com";       //TO DO: type your AWS IoT endpoint, located on the Settings page of the AWS IoT Console
 
// Amazon Root CA 1                           //TO DO: copy the Amazon Root CA 1 Certificate Key below
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                         //TO DO: copy and paste the Device Certificate Key below
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUIOXX8KefrUFr2E3KPOEjkFU/0EQwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDQyNjIxNDc1
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOQ/2NNieMqX/WhUAq84
O99rqNSzkU7TP2xUAjuL/caQ+lipNgTBDJe//Dp/weMJvsc8eDeIdPR42+CCF/kp
Hd7Pcti5cFfsNwv68KX5IKjllfsC9a2ch3qGIXdw4wU/bJXA48Vo+rwQZOb+RIbo
i33rzg9yz6uToRMviUFgPWppem2ntJMOmWHsfnzdGumvxNGb4Xo6aptX+uj9CLSu
/g5X6ym63XUauP1edY5PxQGczP7IUX5qMeV/Ad5LnXEDyfMNVILRAJjC28qWzh10
6b64usy6LM6ODK/XhE2Gz+iYlkCuxl2KNu8svgnMwUoAvR/pC+zBR+Ozb97q9K3I
9NUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUbXhN7lVMRrD928drWQ63OZ7Yv38wHQYD
VR0OBBYEFHxwtpgYAW59c6nV2pBjKldmehUcMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQC8tGaMcrKsgxM//vguHm0xxcxs
oBViSBbDiw5+z8wyLQAYxDGUJZ2LMQiEQuipQdXBzwGEtCRLS5/qybyVdu+8SCqU
xLwQ274EKU48u45DqYGwzcw+kmeHi9qdC/5ZvNCs1iSZZLxCKIUvxFr/PniHzKx4
AhV4y8LZl7N0TAx8aRReXy/fD9pufpekVEQ76XexGTGRAK3eTpApeAnLSjn6GW17
Jf4AYTilr40Wfv28NZk/g1Ig5pKc/i1zYDGFBUPJ0T/Ma/rMIAXwVo6q9EoseIW4
dS8jMvm5t8q/95CFZc85ZLBHkRLm/mwsd6RGj8f0CNjrwAJxpuHX+gtzZpQo
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                         //TO DO: copy and paste the Device Private Key below
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA5D/Y02J4ypf9aFQCrzg732uo1LORTtM/bFQCO4v9xpD6WKk2
BMEMl7/8On/B4wm+xzx4N4h09Hjb4IIX+Skd3s9y2LlwV+w3C/rwpfkgqOWV+wL1
rZyHeoYhd3DjBT9slcDjxWj6vBBk5v5EhuiLfevOD3LPq5OhEy+JQWA9aml6bae0
kw6ZYex+fN0a6a/E0Zvhejpqm1f66P0ItK7+DlfrKbrddRq4/V51jk/FAZzM/shR
fmox5X8B3kudcQPJ8w1UgtEAmMLbypbOHXTpvri6zLoszo4Mr9eETYbP6JiWQK7G
XYo27yy+CczBSgC9H+kL7MFH47Nv3ur0rcj01QIDAQABAoIBAQDTUr2oMKKPSBtJ
bYayvcjJOT8t9zcQmQs+e6U5hoomPvJaBghKbIYBNbIMY16amto0UqsnF0LZWdCR
U0Fo4YWTVdQKDd6SuNPaVrJZqIdxeGbc6MeXvrW8XdU8ZZr6r9cT62u2yVRjrHYP
W9kMYt9LbXfx26WkQGHAhqF4kucpzdDE5RrZ1Bnhf0p6mb0ufeRiuCqUBgjDraHE
YrenRJEMx11OIvbol9atHwphog+FeYZ6cja0ct1H94hImx+QcODv6YjiJEcgKd47
8GOtpzKSpzi2KYe+k+5U2QtzKOHw0NCVag/XfALR97yErKFVPP1enwHdb54+XnYw
aMckuykBAoGBAPlrqMyFqBY/KzS1YiKwhSKFFbsoxtWmVXodUBaVG6GnTQw1EBA0
sdOrxL8QK+c5RKKlVWUVL0tV1j8g/Sso90cbmLYBi8AiBz5RGFwcSiAE/pAzHUO7
7f5ivzl+fGGXh5bK9v5cVFNNCX9dhDlzd0G1/gW7Db9X8pXovD8gzwxBAoGBAOpF
N/QV3B/A1HOG/KKM0Uu+BvCC/TeMWrJNUAiQiS3TqzyyaxkUFMVRPSHCcMzeznWt
OVHrO90cdnbWhaTH/oghBXXdNi0MzyOneqQvlworBtm8Y78sPH2YwglWJGLb5Ulm
8s4LS3BaD6QY9RCctHruIROmrU2dNoovF/DX3hOVAoGAWO0fg7vrj/L5rpM05/VT
olkcxFvneGkTdLRax5JmBZ6mg8GvdnezJyrIvO5BGPF8HRkhdya0dAGon11dpjO2
5i1apxmp/9iiCH/VVZHog74EHNl8h5fLZLVERHVqV0k1WHgI0+uLTR/cUQLAj9Mx
I8aFSPnGDwCGRxhxdXh3GYECgYArafJpkFd/put4EU7mDLSIlcmRbqTgU2lHW552
R9q0HGLCV8oTFq6WtmxewjrHLI8ULarOrIomeqzVbwHLPKjFr2Dkq77rcFE+qT3I
CW2zG7AkT44oklkPaWyj8EvD6j4+cQ6T8nHjez0kyt83WeL0WQbWLnA1TuoZh4qn
PknQVQKBgDj/6BlwbY6bI3v+1lwYwmaHS10jRfMWbWGHX7nZ/tqOUpUv/qCK+IFL
BTZjP0RUQPHTbYYhGm2BDiTiBxKUawWzvseC9xPtV+ellnGH3Qijkhawo3BuZU+1
Hylc3eIogu1NL1xgzqZ6FpIo8/IcfPY5OTYs8rCvtq+MfaahCHTA
-----END RSA PRIVATE KEY-----
)KEY";
