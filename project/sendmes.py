import requests
import json
import time

# 設定閾值，當加速度大於這個值時，發送Line Notify訊息
THRESHOLD = 15

# 設定ThingSpeak和Line Notify的API Key
THINGSPEAK_API_KEY = "ZEQ6YIPZXVYPE264"
LINE_NOTIFY_TOKEN = "CidxRRiUl0YBVySIJxATudfu263dGCadc6M8LbCarsZ"

# 設定Line Notify發送的訊息格式
LINE_MESSAGE = "發生劇烈碰撞，使用者可能有危險"

def main():
    last_updated = 0
    while True:
        # 從ThingSpeak讀取最新的數據
        url = f"https://api.thingspeak.com/channels/2146591/fields/1.json?api_key={THINGSPEAK_API_KEY}"
        response = requests.get(url)
        data = json.loads(response.text)
        acceleration = float(data["feeds"][-1]["field1"])
        updated_at = data["feeds"][-1]["created_at"]
        updated_timestamp = time.mktime(time.strptime(updated_at, "%Y-%m-%dT%H:%M:%SZ"))

        # 如果數據大於閾值，並且更新時間大於上一次更新時間，則發送Line Notify訊息
        if acceleration > THRESHOLD and updated_timestamp > last_updated:
            message = LINE_MESSAGE.format(acceleration)
            payload = {"message": message}
            headers = {"Authorization": f"Bearer {LINE_NOTIFY_TOKEN}"}
            response = requests.post("https://notify-api.line.me/api/notify", data=payload, headers=headers)
            last_updated = updated_timestamp
            print("Line Notify訊息已發送")

        time.sleep(10)

if __name__ == "__main__":
    main()
