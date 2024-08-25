import requests
from bs4 import BeautifulSoup

session = requests.Session()

login_url = "https://sig.cefetmg.br/sigaa/logar.do?dispatch=logOn"
url = "https://sig.cefetmg.br/sigaa/portais/discente/discente.jsf"

payload = {
    'user.login': '',
    'user.senha': ''
}

try:
    # Perform the login
    login_response = session.post(login_url, data=payload)
    login_response.raise_for_status()
    
    # Check if login was successful by inspecting the response
    if "Invalid username or password" in login_response.text:
        print("Login failed. Check your credentials.")
    else:
        print("Login successful!")
        
        protected_response = session.get(url)
        protected_response.raise_for_status()
        
        soup = BeautifulSoup(protected_response.content,"html.parser")


        soup_rotater = soup.find("div", id="avaliacao-portal")

        rows = soup_rotater.select('tbody tr')

    all_tasks = ""

    for row in rows:
        text = row.prettify()
        if "dias" in text or "Hoje" in text:
            # Extract the raw text
            raw_text = row.get_text(separator="\n", strip=True)
            
            date_time_part = raw_text.split("\n", 2)[:2]
            task_part = raw_text.split("\n", 2)[2]

            subject_name = task_part.split("Tarefa:", 1)[0].strip()
            task_description = task_part.split("Tarefa:", 1)[1].strip()

            date_time_clean = ' '.join(part.strip() for part in date_time_part if part.strip())

            formatted_task = (
                f"{subject_name}\n"
                f"{task_description}\n"
                f"{date_time_clean}\n\n"
            )
            
            all_tasks += formatted_task

    all_tasks = all_tasks.strip()  + "\n\n"    

    f = open("tasks.txt", "w")
    f.write(all_tasks)
    f.close()





except requests.exceptions.RequestException as e:
    print(f"An error occurred: {e}")
