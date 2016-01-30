from flask import Flask
from flask import request
import search
import json

app = Flask(__name__)
print "Ready!"
@app.route("/api", methods=['GET','POST'])
def hello():
    query = request.args.get('q')
    print "searching " + query
    answer = search.search(query)
    response = {};
    response['answer'] = answer
    return json.dumps(response);

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=True)
