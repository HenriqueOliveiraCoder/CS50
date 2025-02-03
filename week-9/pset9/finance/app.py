try:
    import yfinance as yf
    import json
    import plotly.graph_objects as go
    import plotly
    import pandas as pd
except:
    error = "failed to load personal touch"

import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

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


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", )
@login_required
def index():
    """Show portfolio of stocks"""

    # Initialize table 2d list
    table = []

    # Get user's balance
    cash = float(db.execute("SELECT balance FROM users WHERE id = ?",
                 session["user_id"])[0]["balance"])

    # Populate table 2d list and total var

    # Creating total var
    total = cash

    # Loop based on
    for i in range(db.execute("SELECT COUNT(*) FROM portifolio WHERE id = ?", session["user_id"])[0]["COUNT(*)"]):
        stock_amount = db.execute("SELECT stock_amount FROM portifolio WHERE id = ?", session["user_id"])[
            i]["stock_amount"]
        stock_symbol = db.execute("SELECT stock_symbol FROM portifolio WHERE id = ?", session["user_id"])[
            i]["stock_symbol"].upper()
        stock_price = float("%.2f" % lookup(stock_symbol)["price"])
        total += stock_amount * stock_price
        table.append([stock_symbol, stock_amount, "$" + "%.2f" %
                     stock_price, "$" + "%.2f" % (stock_price * stock_amount)])

    return render_template("index.html", table=table, cash="$" + "%.2f" % cash, total="$" + "%.2f" % total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Check if symbol is not blank
        if request.form.get("symbol") == []:
            return apology("missing symbol")

        # Get user entered data
        symbol = request.form.get("symbol").upper()
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0:
                return apology("enter a valid number")
        except ValueError:
            return apology("enter a valid number")

        # Obtain stock price
        try:
            price = float("%.2f" % lookup(symbol)["price"])
        except:
            return apology("symbol not found")

        # Obtain user's balance
        cash_in_bank = db.execute("SELECT balance FROM users WHERE id = ?;",
                                  session["user_id"])[0]["balance"]

        # Obtain date
        date = datetime.datetime.now().strftime("%m-%d-%y %H:%M:%S")

        # Check if user affords the transaction
        if cash_in_bank < price * shares:
            return apology("can't afford")

        # Error handling
        try:
            # Update user's balance
            db.execute("UPDATE users SET balance=? WHERE id = ?",
                       cash_in_bank - price * shares, session["user_id"])

            # Register transaction
            db.execute("INSERT INTO history (id, stock_symbol, stock_amount, stock_price, transaction_date) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol, shares, price, date)
            if db.execute("SELECT * FROM portifolio WHERE id = ? AND stock_symbol = ?", session["user_id"], symbol):
                old_amount = db.execute(
                    "SELECT stock_amount FROM portifolio WHERE id = ? AND stock_symbol = ?", session["user_id"], symbol)[0]["stock_amount"]
                db.execute("UPDATE portifolio SET stock_amount=? WHERE id = ? AND stock_symbol = ?", int(
                    old_amount) + int(shares), session["user_id"], symbol)
            else:
                db.execute("INSERT INTO portifolio (id, stock_symbol, stock_amount) VALUES (?, ?, ?)",
                           session["user_id"], symbol, shares)

            # Redirect user to home page with a message confirming the transaction
            flash("Bought!")
            return redirect("/")
        except:
            return apology("something got wrong")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Initialize table 2d list
    table = []

    # Loop based on history to populate table
    for i in range(db.execute("SELECT COUNT(*) FROM history WHERE id = ?", session["user_id"])[0]["COUNT(*)"]):
        stock_symbol = db.execute("SELECT stock_symbol FROM history WHERE id = ?", session["user_id"])[
            i]["stock_symbol"].upper()
        stock_amount = db.execute("SELECT stock_amount FROM history WHERE id = ?", session["user_id"])[
            i]["stock_amount"]
        stock_price = float("%.2f" % lookup(stock_symbol)["price"])
        date = db.execute("SELECT transaction_date FROM history WHERE id = ?",
                          session["user_id"])[i]["transaction_date"]
        table.append([stock_symbol, stock_amount, "$" + "%.2f" % stock_price, date])

    return render_template("history.html", table=table)


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

        # Redirect user to home page with confirmation message
        flash("Registered!")
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
    # User reached via POST, from the quote.html
    if request.method == "POST":

        # Check for blank input
        if request.form.get("symbol") == []:
            return apology("missing symbol")

        # Use lookup to obtain financial data from the symbolt
        vlookup = lookup(request.form.get("symbol"))

        # Obtain name, price, and symbol, while checking for errors
        try:
            name = vlookup["name"]
            price = usd(vlookup["price"])
            symbol = vlookup["symbol"]
        except:
            return apology("symbol not found")

        # Errors handling for personal touch
        try:

            # Get the yfinance ticker and the pandas dataframe
            ticker = yf.Ticker(request.form.get("symbol").upper())
            df = ticker.history(period=request.form.get("dropdown"))

            # Creating the candlestick graph (personal touch)
            fig = go.Figure(data=[go.Candlestick(x=df.index,
                                                 open=df['Open'], high=df['High'],
                                                 low=df['Low'], close=df['Close'])
                                  ])

            # Adding a rangeslider and converting to a js object for the html
            fig.update_layout(xaxis_rangeslider_visible=True)
            graph = json.dumps(fig, cls=plotly.utils.PlotlyJSONEncoder)

            # obtaining more info for the page (those are also part of my personal touch)
            info = ticker.info["longBusinessSummary"]
            website = ticker.info["website"]
            currency = ticker.info["financialCurrency"]

            # Redirecting to the quote page with all info from the
            return render_template("quote_plot.html", name=name, price=price, symbol=symbol, graph=graph, info=info, website=website, currency=currency)

        except:
            # When the symbol doesn't exist
            return render_template("quote_simple.html", name=name, price=price, symbol=symbol)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Ensure the password was confirmed
        elif not request.form.get("confirmation"):
            return apology("must confirm password")

        # Ensure the confirming password was just as the original
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("must confirm password correctly")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't exists
        if len(rows) == 1:
            return apology("username already taken")

        # Insert username and password in the database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Remember which user has logged in
        session["user_id"] = db.execute(
            "SELECT id FROM users WHERE username = ?", request.form.get("username"))[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Get user entered data
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Obtain stock price
        price = float("%.2f" % lookup(symbol)["price"])

        # Obtain user's balance
        cash_in_bank = db.execute("SELECT balance FROM users WHERE id = ?;",
                                  session["user_id"])[0]["balance"]

        # Check if user have enought stocks to sell
        shares_in_bank = db.execute(
            "SELECT stock_amount FROM portifolio WHERE id = ? AND stock_symbol = ?", session["user_id"], symbol)[0]["stock_amount"]
        if shares > shares_in_bank:
            return apology("too many shares")

        # Obtain date
        date = datetime.datetime.now().strftime("%m-%d-%y %H:%M:%S")

        # Update user's balance
        db.execute("UPDATE users SET balance=? WHERE id = ?",
                   cash_in_bank + price * shares, session["user_id"])

        # Register transaction
        db.execute("INSERT INTO history (id, stock_symbol, stock_amount, stock_price, transaction_date) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], symbol, int(shares) * -1, price, date)

        if db.execute("SELECT * FROM portifolio WHERE id = ? AND stock_symbol = ?", session["user_id"], symbol):
            db.execute("UPDATE portifolio SET stock_amount=? WHERE id = ? AND stock_symbol = ?", int(
                shares_in_bank) - int(shares), session["user_id"], symbol)
        else:
            db.execute("INSERT INTO portifolio (id, stock_symbol, stock_amount) VALUES (?, ?, ?)",
                       session["user_id"], symbol, shares)

        # Check if user has no shares
        shares_in_bank = db.execute(
            "SELECT stock_amount FROM portifolio WHERE id = ? AND stock_symbol = ?", session["user_id"], symbol)[0]["stock_amount"]
        if shares_in_bank == 0:
            db.execute("DELETE FROM portifolio WHERE id = ? AND stock_symbol = ?",
                       session["user_id"], symbol)

        # Redirect user to home page with a message confirming the transaction
        flash("Sold!")
        return redirect("/")
    else:
        shares_list = db.execute(
            "SELECT stock_symbol FROM portifolio WHERE id = ?", session["user_id"])
        return render_template("sell.html", shares_list=shares_list)
