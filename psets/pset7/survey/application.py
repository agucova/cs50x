import cs50
import csv
import re
import json

from flask import Flask, jsonify, redirect, render_template, request

# Check database
try:
    fr = open('survey.csv', 'r')
except FileNotFoundError:
    with open('survey.csv', 'w', newline='') as survey:
        writer = csv.DictWriter(survey, fieldnames=["Name", "House", "Position"])
        writer.writeheader()

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    try:
        form = {
            "Name": request.form['Name'],
            "House": request.form['House'],
            "Position": request.form['Position']
        }

        if valid_form(form['Name'], form['House'], form['Position']):
            write_row(form)
            return redirect("/sheet")
        else:
            return render_template("error.html", message="You didn't specifiy some value.")
    except KeyError:
        return render_template("error.html", message="Your form is missing a complete header.")


@app.route("/sheet", methods=["GET"])
def sheet():
    return render_template("sheet.html", message="TODO")


def valid_form(name, house, position):
    houses = ["Gryffindor", "Ravenclaw", "Slytherin", "Hufflepuff"]
    positions = ["Keeper", "Beater", "Chaser", "Seeker"]
    if house not in houses:
        return False
    elif name == "":
        return False
    elif position not in positions:
        return False
    else:
        return True


def write_row(form):
    with open('survey.csv', 'a', newline='') as survey:
        writer = csv.DictWriter(survey, fieldnames=["Name", "House", "Position"])
        writer.writerow(form)
    update_sheet()


def update_sheet():
    with open('survey.csv', 'r', newline='') as survey:
        reader = csv.DictReader(survey, fieldnames=["Name", "House", "Position"])
        next(reader, None)
        sheet = json.dumps([row for row in reader])
    with open('static/sheet.json', 'w', newline='') as sheetj:
        sheetj.write(sheet)