import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # For each symbol, sum the shares_number of each symbol
    rows_symbol_shares = db.execute("SELECT symbol, overall_number FROM shares_has WHERE user_id = ?", session["user_id"])

    for row in rows_symbol_shares:

        # Find current price of the stock and insert it into the list of dicts
        row["current_price"] = lookup(row["symbol"])["price"]

        # Count value of each holding (shares times price)
        row["value_of_holding"] = row["overall_number"] * row["current_price"]

    # Current cash balance
    cash_row = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    current_balance = cash_row[0]["cash"]

    # Grand total (stock's total value plus cash)
    grand_total = 0
    for row in rows_symbol_shares:
        grand_total += row["value_of_holding"]
    grand_total += current_balance
    return render_template("index.html", rows_symbol_shares=rows_symbol_shares, current_balance=current_balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    symbol = request.form.get("symbol")
    if lookup(symbol) == None:
        return apology("Are you sure you've provided a valid symbol?")
    price = lookup(symbol)["price"]
    if not price:
        return apology("Are you sure you've provided a valid symbol?")

    shares_number = request.form.get("shares")
    try:
        shares_number = int(shares_number)
    except ValueError:
        return apology("Shares number should be an integer")
    if shares_number <= 0:
        return apology("Is the number of shares a positive integer?")
    row3 = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    cash_user_has = row3[0]["cash"]
    transaction_sum = price * shares_number
    if cash_user_has < transaction_sum:
        return apology("Sorry, not enough cash on your account")
    time = datetime.now()
    transaction_type = "bought"
    db.execute("INSERT INTO transactions (user_id, symbol, shares_number, price, transaction_sum, time, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?)",
               session["user_id"], symbol, shares_number, price, transaction_sum, time, transaction_type)
    db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash_user_has - transaction_sum), session["user_id"])

    # Add shares to the table shares_has, where all shares are summed up

    # Is the user present in shares_has table?
    users_list_dict = db.execute("SELECT user_id FROM shares_has WHERE user_id = ?", session["user_id"])
    # if it's an empty list (=if user is not in the table)
    if not users_list_dict:
        db.execute("INSERT INTO shares_has (user_id, symbol, overall_number) VALUES (?, ?, ?)",
                   session["user_id"], symbol, shares_number)
        return redirect("/")

    # If the user is in shares_has, does he already have shares of this symbol in shares_has?
    shares_list_dict = db.execute("SELECT * FROM shares_has WHERE symbol = ? AND user_id = ?", symbol, session["user_id"])
    if not shares_list_dict:
        db.execute("INSERT INTO shares_has (user_id, symbol, overall_number) VALUES (?, ?, ?)", session["user_id"],
                   symbol, shares_number)
        return redirect("/")

    # If the user is in shares_has and already has share of this symbol in shares_has, we sum them up w/new and insert
    shares_had_before_dict = db.execute("SELECT overall_number FROM shares_has WHERE user_id = ? AND symbol = ?",
                                        session["user_id"], symbol)
    shares_had_before = shares_had_before_dict[0]["overall_number"]
    shares_has_now = shares_had_before + shares_number
    db.execute("UPDATE shares_has SET overall_number = ? WHERE user_id = ?", shares_has_now, session["user_id"])
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    row_transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", row_transactions=row_transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if not request.form.get("symbol"):
        return apology("this isn't a valid symbol")

    symbol = request.form.get("symbol").upper()

    resp = lookup(symbol)
    if resp == None:
        return apology("this isn't a valid symbol")

    name = resp["name"]
    price = resp["price"]
    symboll = resp["symbol"]
    return render_template("quoted.html", name=name, symbol=symboll, price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("type in a username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation or password != confirmation:
            return apology("Have you filled in the password and confirmation fields properly?")
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        except ValueError:
            return apology("this username already exists")
        row_id = db.execute("SELECT id FROM users WHERE username = ?", username)
        session["user_id"] = row_id[0]["id"]
        return redirect("/")

    if request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbol_list = []
    symbol_list_dict = db.execute("SELECT symbol FROM shares_has WHERE user_id = ?", session["user_id"])
    for symbol_dict in symbol_list_dict:
        symbol_list.append(symbol_dict["symbol"])

    if request.method == "POST":
        symbol_for_sale = request.form.get("symbol")
        if not symbol_for_sale:
            return apology("Please select shares for sale")
        if symbol_for_sale not in symbol_list:
            return apology("These stocks aren't in your portfolio")

        shares_number = int(request.form.get("shares"))
        shares_available_dict = db.execute(
            "SELECT overall_number FROM shares_has WHERE user_id = ? AND symbol = ?", session["user_id"], symbol_for_sale)
        shares_available = shares_available_dict[0]["overall_number"]
        if shares_number > shares_available:
            return apology("You can't sell more chares than you have")
        price = lookup(symbol_for_sale)["price"]
        transaction_sum = price * shares_number
        time = datetime.now()
        cash_before_transaction = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash_after_transaction = cash_before_transaction[0]["cash"] + transaction_sum
        if shares_number == shares_available:
            # Remove shares from shares_has, add to transactions, update cash
            db.execute("DELETE FROM shares_has WHERE user_id = ? AND symbol = ?", session["user_id"], symbol_for_sale)
            db.execute("INSERT INTO transactions (user_id, symbol, shares_number, price, transaction_sum, time, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                       session["user_id"], symbol_for_sale, shares_number, price, transaction_sum, time, "sold")
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after_transaction, session["user_id"])
            return redirect("/")
        db.execute("UPDATE shares_has SET overall_number = ? WHERE user_id = ? AND symbol = ?",
                   (shares_available - shares_number), session["user_id"], symbol_for_sale)
        db.execute("INSERT INTO transactions (user_id, symbol, shares_number, price, transaction_sum, time, transaction_type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol_for_sale, shares_number, price, transaction_sum, time, "sold")
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_after_transaction, session["user_id"])
        return redirect("/")

    return render_template("sell.html", symbol_list=symbol_list)
