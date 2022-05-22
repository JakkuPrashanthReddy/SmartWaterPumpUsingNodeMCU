import json
import configuration as conf
import requests
import time

flag = 0

while True:
    url = conf.Thingspeak_url
    msg = requests.request('GET', url)
    response = json.loads(msg.text)
    #print(response)
    data = response['feeds']
    print("this is the depth : ")
    data1 = data[0]
    data2 = data1['field1']
    print(data2)
    data3 = float(data2)
    if data3 >=140:
        url1 = conf.Telegram_url
        data = {
            "chat_id": conf.CHAT_ID,
            "text": "Water Pump Turned on"
        }
        try:
            if flag == 0:
                response = requests.request("POST", url1, params=data)
                # print("This is the Telegram URL")
                # print(url)
                print("This is the Telegram response")
                print(response.text)
                flag = 1
        except Exception as e:
            print("An error occurred in sending the alert message via Telegram")
            print(e)
    elif data3<=10 :
        url1 = conf.Telegram_url
        data = {
            "chat_id": conf.CHAT_ID,
            "text": "Water Pump Turned OFF"
        }
        try:
            if flag == 1:
                response = requests.request("POST", url1, params=data)
                # print("This is the Telegram URL")
                # print(url)
                print("This is the Telegram response")
                print(response.text)
                flag = 0
        except Exception as e:
            print("An error occurred in sending the alert message via Telegram")
            print(e)
    time.sleep(11)
