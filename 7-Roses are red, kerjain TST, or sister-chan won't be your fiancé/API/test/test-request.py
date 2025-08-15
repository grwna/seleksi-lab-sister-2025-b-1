import requests
import json

token = "{JWT_TOKEN_HERE}"

text_to_summarize = "This is a long text that I want to summarize using the API without a browser."

headers = {
    "Authorization": f"Bearer {token}",
    "Content-Type": "application/json"
}

data = {
    "text": text_to_summarize
}

response = requests.post("http://localhost:3000/api/summarize", headers=headers, data=json.dumps(data))

if response.status_code == 200:
    print("Success!")
    print(response.json())
else:
    print("Error:")
    print(response.status_code, response.text)

response = requests.get("http://localhost:3000/api/history", headers=headers, data=json.dumps(data))

if response.status_code == 200:
    print("Success!")
    history = response.json()
    print(json.dumps(history, indent=2))
else:
    print("Error:")
    print(response.status_code, response.text)
