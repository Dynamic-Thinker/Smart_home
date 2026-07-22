import requests

ESP = "http://192.168.1.50"

# Toggle tube
requests.post(
    ESP + "/toggle",
    data={"d": "tube"}
)

# Set fan speed
requests.post(
    ESP + "/speed",
    data={"v": 80}
)

# Get state
state = requests.get(
    ESP + "/status"
).json()

print(state)