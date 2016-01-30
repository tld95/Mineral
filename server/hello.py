from flask import Flask
import flask
from flask import request
from flask.ext.cors import CORS

print "starting"

import search
import json

app = Flask(__name__)
CORS(app)

print "Ready!"
@app.route("/api", methods=['GET','POST'])
def hello():
    query = request.args.get('q')
    print "searching " + query
    answer = search.search(query)

    resp = flask.Response(json.dumps(answer))
    resp.headers['Access-Control-Allow-Origin'] = '*'
    resp.headers['Content-Type'] = 'application/json'
    return resp

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=True)
