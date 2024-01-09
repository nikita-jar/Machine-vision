import cv2
import mediapipe as mp
import pyautogui
import math
import requests
from requests.exceptions import HTTPError

cap = cv2.VideoCapture(0)

cap.set(3, 1200)
cap.set(4, 2000)

s, img = cap.read()
h, w, c =img.shape
print(h, w)

url = "http://172.16.3.17/move"

mpHands = mp.solutions.hands
hands = mpHands.Hands(False)
npDraw = mp.solutions.drawing_utils

def dist(x1, y1, x2, y2):
    c=math.sqrt(math.pow(x1-x2,2)+math.pow(y1-y2,2))
    return c

while True:
    s, img = cap.read()
    img = cv2.flip(img,1) 
    
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            npDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)
            for id, lm in enumerate(handLms.landmark):
                if  id == 8:
                    x5 , y5 = int(lm.x*w), int(lm.y*h)
                    if y5 < 50:
                        param = {'move': '4'}
                    elif y5 > 400:
                        param = {'move': '1'}
                    elif x5 < 100:
                        param={'move': '2'}
                    elif x5 > 800:
                        param={'move': '3'}
                    else:
                        param={'move': '0'}
                    response = requests.get(url,params=param)
 
    cv2.imshow('python', img)
    if cv2.waitKey(20) == 27: # exit on ESC
        break
cv2.destroyWindow("python")
cap.release()








                
