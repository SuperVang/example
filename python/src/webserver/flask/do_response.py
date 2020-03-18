from flask import Flask, make_response

app = Flask(__name__)


@app.route('/<type>')
def index(type):
    if type == 'str':
        return "str"
    elif type == 'dict':
        return dict(name='caibinbin', age=10)

if __name__ == '__main__':
    app.run(debug=True)
