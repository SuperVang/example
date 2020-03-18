import sys, os
from flask import Flask, request

app = Flask(__name__)


@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    data_dir = os.path.join(os.path.abspath(__file__), "../../../data")
    if request.method == "POST":
        f = request.files['the_file']
        f.save(os.path.join(data_dir, f.filename))


if __name__ == '__main__':
    app.run(debug=True)
