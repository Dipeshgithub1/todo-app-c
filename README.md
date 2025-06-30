# ✅ TODO App in C

This repository contains a **simple HTTP server written in C** that responds to client requests with a basic "Hello, World!" HTML page. The server listens on **port 8080** and handles **HTTP GET** requests.

This project is designed for **educational purposes** to demonstrate how to build an HTTP server from scratch using **C and Winsock API (Windows)**.

---

## 🚀 Features

- 📥 Handles incoming **HTTP GET** requests
- 🌐 Sends back a basic **HTML response**
- 🧠 Demonstrates **socket programming** using the Winsock API
- 🪟 Built for **Windows** environments

---

## 🛠️ Setup Instructions

### 1. Clone the Repository
```bash
git clone https://github.com/your-username/todo-app-c.git
cd todo-app-c

2. Compile the Server (on Windows)
Ensure you have GCC installed (e.g., via MinGW), then run:

bash
Copy
Edit
gcc todo.c -o todo -lws2_32
3. Run the Server
bash
Copy
Edit
.\todo
4. View in Browser
Open your browser and visit:

arduino
Copy
Edit
http://localhost:8080
You should see a Hello, World! HTML response.

📋 Prerequisites
🪟 Windows operating system

🧰 GCC compiler (MinGW or similar)

👨‍💻 Basic knowledge of C and socket programming

📄 License
This project is licensed under the MIT License.


---

### ✅ Next Steps:
- Replace `your-username` with your actual GitHub username.
- Add a `LICENSE` file if you haven't already (MIT is a good default).
- Optionally, include a screenshot or terminal preview under a `## 📸 Preview` section.

Let me know if you'd like a Linux-compatible version or a version with advanced features like handling more HTTP methods.
