from bs4 import BeautifulSoup
import requests
import sys
from requests.exceptions import ConnectionError
from urllib.error import HTTPError


# kurs dolara
url = "https://internetowykantor.pl/kurs-dolara/"

# pobieramy tresc html
try:
    html_content = requests.get(url)
except ConnectionError:
    print("no internet connection")
    exit(-1)
except HTTPError as err:
    if err.code == 404:
        print("The requested resource could not be found but may be available in the future.")
    elif err.code == 403:
        print("The request contained valid data and was understood by the server, but the server is refusing action.")
    elif err.code == 408:
        print("The server timed out waiting for the request.")
    elif err.code == 502:
        print("The server was acting as a gateway or proxy and received an invalid response from the upstream server.")
    else:
        print("error code: " + str(err.code))
    exit(-1)


# analizujemy zawartosc pobranej strony
if(html_content.status_code == 200):
    if "text/html" in html_content.headers["Content-type"]:

        soup = BeautifulSoup(html_content.text, "lxml")

        try:
            price = soup.find(
                "span", {"class": "bem-single-rate-box__item-rate"}).text
        except AttributeError:
            print("item not found")
            exit(-1)

        print("Aktualna cena za ktora mozna kupic dolara to")
        print(price + "pln")
        sys.exit(0)
    else: 
        sys.exit(1)
else:
    sys.exit(1)
