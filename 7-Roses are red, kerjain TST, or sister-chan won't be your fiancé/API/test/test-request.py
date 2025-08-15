import requests
import json

token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJncnduYSIsImVtYWlsIjoicmVzcGVjdHNlcmVuZGlwaXR5QGdtYWlsLmNvbSIsImlhdCI6MTc1NTI2NDIxNywiZXhwIjoxNzU1MzUwNjE3fQ.wnOcCTvZdPuZMcVe6D0IcFBkAimAJO3Dgji_ACU76iY"

text_to_summarize = ""#"This is a long text that I want to summarize using the API without a browser."

headers = {
    "Authorization": f"Bearer {token}",
    "Content-Type": "application/json"
}

data = {
    "text": text_to_summarize
}
response = requests.get("http://localhost:3000/auth/logout", headers=headers, data=json.dumps(data))

if response.status_code == 200:
    print("Success!")
    print(response)
else:
    print("Error:")
    print(response.status_code, response.text)

# if response.status_code == 200:
#     print("Success!")
#     print(response.json())
# else:
#     print("Error:")
#     print(response.status_code, response.text)

# response = requests.get("http://localhost:3000/api/history/1", headers=headers, data=json.dumps(data))

# if response.status_code == 200:
#     print("Success!")
#     history = response.json()
#     print(json.dumps(history, indent=2))
# else:
#     print("Error:")
#     print(response.status_code, response.text)
