Python 3.5.1 (v3.5.1:37a07cee5969, Dec  6 2015, 01:38:48) [MSC v.1900 32 bit (Intel)] on win32
Type "copyright", "credits" or "license()" for more information.
>>> import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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
    """Show portfolio of stocks"""

    # get data from SQL transactions table for the user
    transactions = db.execute("SELECT symbol, shares, price, value FROM transactions WHERE id = :id", id=session["user_id"])

    # get cash from the SQL users table for user
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    user_funds = cash[0]["cash"]

    grand_total = 0

    # this would iterate over the transaction table in SQL we set the items to new variables so we can use in JINJA in index.html
    for stock in transactions:
        symbol = stock["symbol"]
        shares = stock["shares"]
        quote = lookup(symbol)
        price = stock["price"]
        value = stock["value"]

        # calculate the grand total by always adding any new value to it
        grand_total = grand_total + value

    # update the grand total by adding the users remaining cash to it
    grand_total += user_funds

    # return index.html, setting the appropriate variables in JINJA, html
    return render_template("index.html", stocks=transactions, user_funds=usd(user_funds), grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Symbol was not found")

        # Check if shares was a positive integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be a positive integer")

        # Check shares is less than 1
        if shares <= 0:
            return apology("Positive number required for shares")

        # selecting cash from database
        money = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # fetching user current cash from SQL table(database)
        user_funds = money[0]["cash"]
        stock_price = stock["price"]

        # Calculate the total price shares
        total_price = stock_price * shares

        if user_funds < total_price:
            return apology("You do not have enough funds")

        # implimenting datetime of when share was purchased into our SQL table
        now = datetime.now()
        formatted_date = now.strftime('%Y-%m-%d %H:%M:%S')

        # updating users cash
        db.execute("UPDATE users SET cash = cash - :price WHERE id = :id", price=total_price, id=session["user_id"])

        # if symbol isnt already in portfolio table, insert symbol
        s = db.execute("SELECT symbol, shares, value FROM transactions WHERE id= :id AND symbol= :symbol",
                       id=session["user_id"], symbol=stock["symbol"])
        if len(s) == 0:
            db.execute("INSERT INTO transactions (id, symbol, shares, price, value) VALUES(:id, :symbol, :shares, :price, :value)",
                       id=session["user_id"],
                       symbol=stock["symbol"],
                       shares=shares,
                       price=stock_price,
                       value=total_price)
            flash('Shares bought!')

        # else updating user portfolio in database by updating shares and price and value of that symbol
        else:
            new_shares = shares + s[0]["shares"]
            new_value = stock_price * new_shares
            db.execute("UPDATE transactions SET shares = :shares, price = :price, value = :value WHERE id=:id AND symbol=:symbol",
                       shares=new_shares,
                       price=stock_price,
                       value=new_value,
                       id=session["user_id"],
                       symbol=stock["symbol"])
            flash('Shares bought!')

        # update history table in SQL
        bought = "bought"
        db.execute("INSERT INTO history (id, action, symbol, shares, price, purchase_date) VALUES(:id, :action, :symbol, :shares, :price, :purchase_date)",
                   id=session["user_id"],
                   action=bought,
                   symbol=stock["symbol"],
                   shares=shares,
                   price=stock_price,
                   purchase_date=formatted_date)

        # return home page i.e portfolio table
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get data from SQL transactions table for the user
    history = db.execute("SELECT * FROM history WHERE id = :id", id=session["user_id"])

    # this would iterate over the transaction table in SQL we set the items to new variables so we can use in JINJA in index.html
    for stock in history:
        action = stock["action"]
        symbol = stock["symbol"]
        shares = stock["shares"]
        price = stock["price"]
        purchase_date = stock["purchase_date"]

    # return index.html, setting the appropriate variables in JINJA, html
    return render_template("history.html", stocks=history)


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
    """Get stock quote."""

    if request.method == "POST":

        # error checking
        if not request.form.get("symbol"):
            return apology("Missing Symbol")

        # check if symbol exists
        rows = lookup(request.form.get("symbol"))
        if not rows:
            return apology("Symbol was not found")

        rows['price'] = usd(rows['price'])

        return render_template("quoted.html", stock=rows)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # user reached route via "POST" method
    if request.method == "POST":

        # check for username and password
        if not request.form.get("username"):
            return apology("Missing Username")

        if not request.form.get("password"):
            return apology("Missing Password")

        if not request.form.get("confirmation"):
            return apology("Missing Confirmation Password")

        # confirm password
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password and Confirmation do not match")

        # hash password for security
        hash = generate_password_hash(request.form.get("password"))

        # update registeration info to database
        result = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",
                            username=request.form.get("username"), hash=hash)
        if not result:
            return apology("Username already exists")

        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        # Store id in session
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Symbol was not found")

        # Check if shares was a positive integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be a positive integer")

        # Check shares is less than 1
        if shares <= 0:
            return apology("Positive number required for shares")

        # check if user own the stock
        transactions = db.execute("SELECT shares FROM transactions WHERE id = :id AND symbol = :symbol",
                                  id=session["user_id"], symbol=stock["symbol"])
        if not transactions[0]["shares"]:
            return apology("you don't own this stock")

        # check if user own that many shares of the stock
        if shares > transactions[0]["shares"]:
            return apology("You do not own that many shares")

        total_price = (stock["price"] * shares)

        # loop for JINJA in sell.html
        symbol_2 = db.execute("SELECT symbol FROM transactions WHERE id = :id", id=session["user_id"])
        for stock in symbol_2:
            symbol = stock["symbol"]

        # query user cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        user_funds = cash[0]["cash"]

        # update cash
        db.execute("UPDATE users SET cash = cash + :price WHERE id = :id", price=total_price, id=session["user_id"])

        total_shares = transactions[0]["shares"] - shares

        # implimenting datetime of when share was purchased into our SQL table
        now = datetime.now()
        formatted_date = now.strftime('%Y-%m-%d %H:%M:%S')

        # if shares = max shares then delete the stock from the portfolio table
        if total_shares == 0:
            new_shares = total_shares - shares
            db.execute("DELETE FROM transactions WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=stock["symbol"])

            # update history table in SQL
            sold = "sold"
            db.execute("INSERT INTO history (id, action, symbol, shares, price, purchase_date) VALUES(:id, :action, :symbol, :shares, :price, :purchase_date)",
                       id=session["user_id"],
                       action=sold,
                       symbol=stock["symbol"],
                       shares=new_shares,
                       price=total_price,
                       purchase_date=formatted_date)

            flash('Shares sold', 'success')

        # else shares doesnt equal max shares, update portfolio by updating the shares
        else:
            db.execute("UPDATE transactions SET shares=:shares WHERE id=:id AND symbol=:symbol",
                       shares=total_shares,
                       id=session["user_id"],
                       symbol=stock["symbol"])

            # update history table in SQL
            sold = "sold"
            db.execute("INSERT INTO history (id, action, symbol, shares, price, purchase_date) VALUES(:id, :action, :symbol, :shares, :price, :purchase_date)",
                       id=session["user_id"],
                       action=sold,
                       symbol=stock["symbol"],
                       shares=total_shares,
                       price=total_price,
                       purchase_date=formatted_date)

            flash('Shares sold', 'success')

        return redirect("/")

    else:
        symbol_2 = db.execute("SELECT symbol FROM transactions WHERE id = :id", id=session["user_id"])
        return render_template("sell.html", stocks=symbol_2)


@app.route("/security", methods=["GET", "POST"])
def security():
    """Change user's password"""
    # user reached route via "POST" method
    if request.method == "POST":

        # check for username and password
        if not request.form.get("password"):
            return apology("Missing Password")

        elif not request.form.get("new_password"):
            return apology("Missing New Password")

        elif not request.form.get("confirmation"):
            return apology("Missing Confirmation Password")

        # confirm new password
        if request.form.get("new_password") != request.form.get("confirmation"):
            return apology("New Password and Confirmation do not match")

        # ensure new password is different from old
        if request.form.get("password") == request.form.get("new_password"):
            return apology("New Password must be different")

        # query database for users password
        rows = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])
        password_hash = rows[0]["hash"]

        # confirm password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("wrong password", 403)

        # hash password for security
        hash = generate_password_hash(request.form.get("new_password"))

        # update password info to user database
        result = db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash, id=session["user_id"])
        flash('Password has been updated', 'success')

        # return homepage
        return redirect('/')
    else:
        return render_template("security.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
