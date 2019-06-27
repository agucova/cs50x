import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from json import loads, dumps
from helpers import apology, login_required, lookup, usd

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


@app.route("/")
@login_required
def index():
    userid = session["user_id"]
    user = db.execute(
        "SELECT cash, stocks FROM users WHERE id = :userid",
        userid=userid)[0]
    stocks = loads(user["stocks"])
    balance = float(user["cash"])
    index = {}
    totalworth = balance
    for symbol, shares in stocks.items():
        price = lookup(symbol)["price"]
        worth = price * shares
        totalworth += worth
        index[symbol] = {
            "price": usd(price),
            "shares": shares,
            "worth": usd(worth)}
    return render_template(
        "index.html",
        index=index,
        balance=usd(balance),
        totalworth=usd(totalworth))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")
        elif not request.form.get("shares"):
            return apology("must provide number of shares")

        # Record the userid
        userid = session["user_id"]
        # Map the stock and shares requested
        symbol = request.form.get("symbol").upper()
        # Convert the number of shares to an int
        try:
            shares = int(request.form.get("shares"))
        except BaseException:
            return apology("not a number")
        if shares < 1:
            return apology("must be bigger than one")
        # Get total price of the buy
        look = lookup(symbol)

        if look is None:
            return apology("invalid stock symbol")

        cost = look["price"] * shares
        # Get the user name and balance.
        user = db.execute(
            "SELECT username, cash FROM users WHERE id = :userid",
            userid=userid)
        balance, username = user[0]["cash"], user[0]["username"]

        if balance < cost:
            return apology("not enough money", 400)
        else:
            # Get JSON stock list from the user and convert to dictionary
            ustocks = loads(
                db.execute(
                    "SELECT stocks FROM users WHERE id = :userid",
                    userid=userid)[0]["stocks"])
            # Push new stocks to the dict of user stocks, add if it already
            # exists.
            if symbol in ustocks:
                ustocks[symbol] = ustocks[symbol] + shares
            else:
                ustocks[symbol] = shares
            # Deduct the cost from the balance
            newbalance = balance - cost
            db.execute(
                "UPDATE users SET cash = :newbalance WHERE id = :userid",
                userid=userid,
                newbalance=newbalance)  # Deduct the cost
            # Save new stocks dict
            db.execute(
                "UPDATE users SET stocks = :stocks WHERE id = :userid",
                userid=userid,
                stocks=dumps(ustocks))
            # Add transaction to history
            db.execute(
                "INSERT INTO history (username, type, symbol, shares, money) VALUES (:username, :ttype, :symbol, :shares, :money)",
                username=username,
                ttype="BUY",
                symbol=symbol,
                shares=shares,
                money=cost)
            return render_template(
                "bought.html",
                cost=usd(cost),
                balance=usd(newbalance),
                name=look["name"],
                shares=shares)
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    username = request.args.get('username')
    if username is None:
        return "false"
    elif len(username) > 1 and db.execute("SELECT * FROM users WHERE username = :username;", username=username):
        return "false"
    else:
        return "true"


@app.route("/history")
@login_required
def history():
    userid = session["user_id"]
    username = db.execute(
        "SELECT username FROM users WHERE id = :userid",
        userid=userid)[0]["username"]
    history = db.execute(
        "SELECT type,symbol,shares,money FROM history WHERE username = :username",
        username=username)
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
                rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("you didn't input the stock symbol", 400)
        else:
            symbol = request.form.get("symbol").upper()
            look = lookup(symbol)
            if look is None:
                return apology("the stock price couldn't be found.", 400)
            else:
                return render_template(
                    "quoted.html",
                    name=look["name"],
                    symbol=look["symbol"],
                    price=usd(
                        look["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)
        # Check if both passwords match
        elif not request.form.get("confirmation") == request.form.get("password"):
            return apology("the passwords do not match", 400)
        # Assign fields into variables
        username, password = request.form.get(
            "username"), generate_password_hash(request.form.get("password"))
        # Check if the user exists
        if db.execute(
            "SELECT * FROM users WHERE username = :username;",
                username=username):
            return apology("username already exists", 400)
        else:
            # Insert user into database
            db.execute(
                "INSERT INTO users (username, hash) VALUES (:username, :password)",
                username=username,
                password=password)
            # Get the id
            exid = db.execute(
                "SELECT id FROM users WHERE username = :username;",
                username=username)
            # Set the user_id in session.
            session["user_id"] = exid[0]["id"]
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # Record the userid
    userid = session["user_id"]
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")
        elif not request.form.get("shares"):
            return apology("must provide number of shares")

        # Map the stock and shares requested
        symbol = request.form.get("symbol").upper()
        # Convert the number of shares to an int
        try:
            shares = int(request.form.get("shares"))
        except BaseException:
            return apology("not a number")

        if shares < 1:
            return apology("must be bigger than one")
        # Get worth
        look = lookup(symbol)

        if look is None:
            return apology("invalid stock symbol")

        worth = look["price"] * shares
        # Get the user name and balance.
        user = db.execute(
            "SELECT username, cash FROM users WHERE id = :userid",
            userid=userid)
        balance, username = user[0]["cash"], user[0]["username"]

        # Get JSON stock list from the user and convert to dictionary
        ustocks = loads(
            db.execute(
                "SELECT stocks FROM users WHERE id = :userid",
                userid=userid)[0]["stocks"])
        # Push new stocks to the dict of user stocks, add if it already exists.
        if symbol in ustocks:
            if ustocks[symbol] < shares:
                return apology("you do not have enough shares")
            ustocks[symbol] = ustocks[symbol] - shares
        else:
            return apology("you do not have that stock", 400)
        # Add the worth to the balance
        newbalance = balance + worth
        db.execute(
            "UPDATE users SET cash = :newbalance WHERE id = :userid",
            userid=userid,
            newbalance=newbalance)  # Deduct the cost
        # Save new stocks dict
        db.execute(
            "UPDATE users SET stocks = :stocks WHERE id = :userid",
            userid=userid,
            stocks=dumps(ustocks))
        # Add transaction to history
        db.execute(
            "INSERT INTO history (username, type, symbol, shares, money) VALUES (:username, :ttype, :symbol, :shares, :money)",
            username=username,
            ttype="SELL",
            symbol=symbol,
            shares=shares,
            money=worth)
        return render_template(
            "sold.html",
            worth=usd(worth),
            balance=usd(newbalance),
            name=look["name"],
            shares=shares)
    else:
        ustocks = loads(
            db.execute(
                "SELECT stocks FROM users WHERE id = :userid",
                userid=userid)[0]["stocks"])
        return render_template("sell.html", stocks=ustocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
