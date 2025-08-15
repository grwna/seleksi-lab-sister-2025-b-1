import requests
import json

token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJncnduYSIsImVtYWlsIjoicmVzcGVjdHNlcmVuZGlwaXR5QGdtYWlsLmNvbSIsImlhdCI6MTc1NTI1NTI5NiwiZXhwIjoxNzU1MzQxNjk2fQ.vEZgT99STPSvaDMz1y5pchrSU7dDKayV9P6hOJXdxuw"

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
