import os
import html

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import check_register, check_passwordchange, get_summary, get_transactions, check_buy, check_sell, add_cash, apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    if request.args.get ('source') == "history":
        title = "History"
        table_heading = '["Symbol","Name", "Shares", "Price", "TOTAL", "Transacted"]'
        table_data = get_transactions ()
    else:
        title = "Index"
        table_heading = '["Symbol","Name", "Shares", "Price", "TOTAL"]'
        table_data = get_summary ()

    return render_template ("index.html", title = title, table_heading = table_heading, table_data = table_data)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        if not request.form["symbol"] or (lookup (request.form["symbol"]) == None):
            return apology ("symbol blank or does not exist", 404)

        if (not request.form["shares"].isdigit ()) or (int (request.form["shares"]) <= 0):
            return apology ("shares must be positive",  400)

        if not check_buy (request.form["symbol"], request.form["shares"]):
            return apology ("not enough cash", 400)
        else:
            return render_template ("bought.html", symbol = request.form["symbol"], qty = request.form["shares"])

    else:
        return render_template ("buy.html", symbol = str (request.args.get ('symbol')))


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return redirect ("/?source=history")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form["username"])

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form["password"]):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        referrer = request.referrer.split ("/")
        json = lookup (request.form["symbol"])

        if json == None:
            return apology ("Invalid symbol", 400) if (referrer [len (referrer) - 1]) == "quote" else '{"name": 404}'

        return render_template ("quoted.html", json = json) if (referrer [len (referrer) - 1]) == "quote" else json

    else:
        return render_template ("quote.html", title = "Quote")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        resp = check_register (request.form["username"], request.form["password"], request.form["confirmation"])

        return apology (str (resp[0]), resp[1])

    else:
        return render_template ("register.html", title = "Register")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form["symbol"] or (str (lookup (request.form["symbol"])) == "None"):
            return apology ("No such stock", 400)

        resp = check_sell (request.form["symbol"], request.form["shares"])
        if resp [0] == False:
            return apology (str (resp[1]), 400)

        return render_template ("sold.html", symbol = request.form["symbol"], qty = request.form["shares"])
    else:
        return render_template ("sell.html", symbols = check_sell ("", 0), symbol = str (request.args.get ('symbol')))


@app.route("/passwordchange", methods=["GET", "POST"])
@login_required
def passwordchange ():
    if request.method == "POST":
        if not request.form["existing"]:
            return apology ("Existing password blank", 400)

        if not request.form["password"] or not request.form["confirmation"] or not (request.form["password"] == request.form["confirmation"]):
            return apology ("New passwords blank or mismatch", 400)

        resp = check_passwordchange (request.form["existing"], request.form["password"], request.form["confirmation"])
        return apology (str (resp[0]), resp[1])

    else:
        return render_template ("changepassword.html", title = "Change Password")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash ():
    if request.method == "POST":
        return add_cash (request.form["amount"])
    else:
        return render_template ("addcash.html", title = "Add Cash")






def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
