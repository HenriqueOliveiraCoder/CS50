import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        id = db.execute("SELECT id FROM birthdays ORDER BY id DESC LIMIT 1")[0]["id"] + 1
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        db.execute("INSERT INTO birthdays VALUES (?, ?, ?, ?)", id, name, month, day)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        dates = []
        names = []
        for i in range(0, db.execute("SELECT COUNT(*) FROM birthdays")[0]["COUNT(*)"], 1):
            dates.append(str(db.execute("SELECT month FROM birthdays WHERE id = ?", i + 1)[0]["month"]) + "/" + str(db.execute("SELECT day FROM birthdays WHERE id = ?", i + 1)[0]["day"]))
            names.append(db.execute("SELECT name FROM birthdays WHERE id = ?", i + 1)[0]["name"])

        return render_template("index.html", names=names, dates=dates, length=len(names))


