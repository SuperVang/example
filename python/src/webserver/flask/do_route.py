from flask import Flask, redirect, url_for, request

app = Flask(__name__)


@app.route('/')
def hello_world():
    return "Hello World"


def hello_flask():
    return "Hello Flask"


app.add_url_rule("/flask", "flask", hello_flask)


@app.route("/hello/<name>")
def hello_name(name):
    return "Hello %s!" % name


def bye_name(name):
    return "Bye %s!" % name


app.add_url_rule("/bye/<name>", "bye", bye_name)


@app.route("/page/<int:page_num>")
def get_page(page_num):
    return "books page %d is found!" % page_num


@app.route("/success/<name>")
def success(name):
    return "welcome %s" % name




if __name__ == "__main__":
    app.run(debug=True)
