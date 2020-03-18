from flask import Flask, abort

app = Flask(__name__)


@app.route("/error/<int:num>")
def index(num):
    if num >= 400 and num < 500:
        abort(num)
    else:
        return "Invalid Error Num"


if __name__ == '__main__':
    app.run(debug=True)
