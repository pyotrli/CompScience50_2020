import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    rows0 = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
    cash = rows0[0]["cash"]
    rows1 = db.execute("SELECT symbol, sum(quantity) as quantity FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING sum(quantity) > 0", user_id=user_id)
    grand_total = cash
    for row in rows1:
        response = lookup(row["symbol"])
        name = response["name"]
        price = response["price"]
        grand_total += price * row["quantity"]
        row.update([("name", name), ("price", usd(price)), ("TOTAL", usd((row["quantity"] * price)))])
    return render_template("index.html", cash=usd(cash), grand_total=usd(grand_total), rows1=rows1)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    # if method POST - validate symbol and number of shares
    else:
        response = lookup(request.form.get("symbol"))
        if not response:
            return apology("invalid symbol", 400)
        else:
            shares = int(request.form.get("shares"))
            price = response["price"]
            name = response["name"]
            symbol = response["symbol"]
            user_id = session["user_id"]
            rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
            cash = rows[0]["cash"]
            # check if user can afford to buy
            if ((shares * price) > cash):
                return apology("you don't have enough cash for this purchase", 400)
            # Insert transaction into table
            db.execute("INSERT INTO transactions (user_id, symbol, quantity, datetime, price) VALUES (:user_id, :symbol, :quantity, :datetime, :price)",
                            user_id=user_id, symbol=symbol, quantity=shares, datetime=datetime.datetime.now(), price=price)
            # Update cash balance
            db.execute("UPDATE users SET cash = :new_balance WHERE id = :user_id", new_balance = cash-(shares*price), user_id=user_id)
            return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    rows = db.execute("SELECT symbol, quantity as shares, price, datetime as transacted FROM transactions WHERE user_id = :user_id ORDER BY transacted DESC", user_id=user_id)
    return render_template("transactions.html", rows=rows)

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

        # Query database for username. db.execute return a list of rows.
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    if request.method == "GET":
        return render_template("quote.html")
    # If POST request check for stock price
    else:
        symbol = request.form.get("symbol")
        response = lookup(symbol)
        if not response:
            return apology("invalid symbol", 400)
        else:
            price = response["price"]
            name = response["name"]
            symbol = response["symbol"]
        return render_template("quoted.html", symbol=symbol, price=usd(price), name=name)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    # If user tries to register via POST
    else:
        if not request.form.get("username"):
            return apology("must provide username", 403)
        if not request.form.get("password"):
            return apology("must provide password", 403)
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("password and confirmation must match", 403)
        # Query database to ensure unique username. db.execute return a list of rows.
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) > 0:
            return apology("username already exists", 403)
        # Register user
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :passwordhash)",
                         username=request.form.get("username"), passwordhash=generate_password_hash(request.form.get("password")) )
        # Redirect user to home page
        return redirect("/")

@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    if request.method == "GET":
        return render_template("changepassword.html")
    # If user tries to change pwd via POST
    else:
        if not request.form.get("password"):
            return apology("must provide password", 403)
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("password and confirmation must match", 403)
        user_id=session["user_id"]
        oldpasswordhash = db.execute("SELECT hash FROM users WHERE id = :user_id", user_id=user_id)[0]["hash"]
        newpassword = request.form.get("password")
        # check if new password is different to old
        if check_password_hash(oldpasswordhash, newpassword):
            return apology("please select new password", 403)
        # Change pwd
        newpasswordhash = generate_password_hash(newpassword)
        db.execute("UPDATE users SET hash = :passwordhash WHERE id = :user_id",
                         user_id=user_id, passwordhash=newpasswordhash)
        # Redirect user to home page
        return redirect("/")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    rows = db.execute("SELECT symbol, sum(quantity) as quantity FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING sum(quantity) > 0", user_id=user_id)
    if request.method == "GET":
        return render_template("sell.html", rows=rows)
    else:
        found = False
        for row in rows:
            if row["symbol"] == request.form.get("symbol"):
                found = True
                symbol = row["symbol"]
                shares_owned = row["quantity"]
                shares_to_sell = int(request.form.get("shares"))
                price = lookup(symbol)["price"]
        if not found:
            return apology("select correct stock symbol", 400)
        elif shares_owned < shares_to_sell:
            return apology("select correct number of shares", 400)
        # if above checks pass, update transactions
        db.execute("INSERT INTO transactions (user_id, symbol, quantity, datetime, price) VALUES (:user_id, :symbol, :quantity, :datetime, :price)",
                user_id=user_id, symbol=symbol, quantity=shares_to_sell * -1, datetime=datetime.datetime.now(), price=price)
        # Update cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)[0]["cash"]
        db.execute("UPDATE users SET cash = :new_balance WHERE id = :user_id",
                new_balance = cash+(shares_to_sell*price), user_id=user_id)
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
