from flask import Flask, request, make_response

app = Flask(__name__)


@app.route("/init_cookies/<name>")
def init_cookies(name):
    resp = make_response()
    resp.set_cookie("username", name)
    return resp


@app.route("/test_cookies")
def test_cookies():
    username = request.cookies.get('username')
    if username:
        return "%s is valid cookie" % username
    else:
        return "cookie is invalid"


if __name__ == '__main__':
    app.run(debug=True)
