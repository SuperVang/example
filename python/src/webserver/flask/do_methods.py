from flask import Flask, request

app = Flask(__name__)


@app.route("/login", methods=["GET", "POST"])
def login():
    # request is a global object
    if request.method == 'POST':
        user = request.form['nm']
        return "post " + user
    else:
        user = request.args.get('nm')
        return "get " + user


if __name__ == '__main__':
    app.run(debug=True)
