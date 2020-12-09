import os
import requests
import urllib.parse

import cs50

from flask import redirect, render_template, request, session
from functools import wraps
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash


db = cs50.SQL ("sqlite:///finance.db")

def check_register (username, password, confirmation):
    if not username or (check_username (username) > 0):
        return ["username blank or already taken", 400]

    elif not password:
        return ["must provide password", 400]

    elif confirmation != password:
        return ["passwords mismatch", 400]

    register_new_user (username, generate_password_hash (password))
    return ["registered", 200]


def check_passwordchange (existing, password, confirmation):
    if not check_password_hash (db.execute ("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"], existing):
        return ["Existing password wrong", 400]

    if check_password_hash (db.execute ("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"], password):
        return ["Cannot reuse existing password", 400]

    print (generate_password_hash (existing))

    db.execute ("UPDATE users set hash = ? WHERE id = ?", generate_password_hash (password), session["user_id"])
    return ["Password changed successfully", 200]


def check_username (username):
    rows = db.execute ("SELECT COUNT(ROWID) FROM users where username = ?", username)
    return rows [0]["COUNT(ROWID)"]

def register_new_user (username, password):
    db.execute ("INSERT INTO users (username, hash) values (?, ?)", username, password)


def get_distinct_symbols ():
    return db.execute ("SELECT DISTINCT symbol FROM transactions where user_id = ?", session["user_id"])


def get_summary ():
    summary = [];

    for symbol in get_distinct_symbols ():
        transactions = db.execute ("SELECT symbol AS Symbol, SUM (quantity) AS Shares, SUM (quantity * price) AS TOTAL FROM transactions where user_id = ? AND symbol = ?", session["user_id"], symbol["symbol"])

        i = 0
        temp_row = {}
        for key in transactions[0]:
            if i == 1:
                temp_row.update ({"Name": lookup (transactions[0]["Symbol"])["name"]})
            if i == 2:
                temp_row.update ({"Price": lookup (transactions[0]["Symbol"])["price"]})

            temp_row.update ({key: transactions[0][key]})
            i += 1
        summary.append (temp_row)

    summary.append (get_balance () [0])

    return summary

def get_transactions ():
    rows = db.execute ("SELECT symbol as Symbol, quantity as Shares, price as Price, (quantity * price) AS TOTAL, transacted AS Transacted FROM transactions where user_id = ?", session["user_id"])

    temp_rows = []
    for row in rows:

        i = 0
        temp_row = {}
        for key in row:
            if i == 1:
                temp_row.update ({"Name": lookup (row["Symbol"])["name"]})
            temp_row.update ({key: row[key]})
            i += 1
        temp_rows.append (temp_row)

    return temp_rows


def get_balance ():
    rows = db.execute ("SELECT cash as CASH FROM users where id = ?", session["user_id"])
    return rows


def check_buy (symbol, quantity):
    existing_cash = get_balance ()[0]["CASH"]

    if existing_cash - (int (quantity) * lookup(symbol)["price"]) < 0:
        return False

    db.execute ("INSERT INTO transactions (user_id, symbol, quantity, price) values (?, ?, ?, ?)", session["user_id"], symbol, quantity, lookup(symbol)["price"])
    db.execute ("UPDATE users SET cash = ? WHERE id = ?", existing_cash - (int (quantity) * lookup(symbol)["price"]), session["user_id"])

    return True


def check_sell (symbol, quantity):
    if symbol == "":
        return get_distinct_symbols ()

    if int (quantity) > db.execute ("SELECT sum (quantity) AS quantity FROM transactions where user_id = ? and symbol = ?", session["user_id"], symbol) [0]["quantity"]:
        return [False, "Quantity greater than owned"]

    db.execute ("INSERT INTO transactions (user_id, symbol, quantity, price) values (?, ?, ?, ?)", session["user_id"], symbol, int (quantity) * -1, lookup(symbol)["price"])
    db.execute ("UPDATE users SET cash = ? WHERE id = ?", float (get_balance ()[0]["CASH"]) + (int (quantity) * float (lookup(symbol)["price"])), session["user_id"])
    return [True]


def add_cash (amount):
    db.execute ("UPDATE users SET cash = ? WHERE id = ?", float (get_balance ()[0]["CASH"]) + float (amount), session["user_id"])
    return "Added successfully. Your new balance is " + str (get_balance ()[0]["CASH"]) + "."


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        response = requests.get(f"https://cloud-sse.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}")
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


