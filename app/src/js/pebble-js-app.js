var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

Pebble.addEventListener('ready', function(e) {
  console.log('JavaScript app ready and running!');
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    var query = e.payload.query;
    var q2 = encodeURI(query);
    var url = 'http://192.241.169.235:5000/api?q=' + q2;
    console.log(url);
    xhrRequest(url, 'GET',
      function(responseText) {
        var json = JSON.parse(responseText);
	var ans = json.answer;
	console.log(ans);
	var dict = {'ANSWER': ans};
	Pebble.sendAppMessage(dict,
          function(e) {
	    console.log('Weather info sent to Pebble successfully!');
	  },
	  function(e) {
	    console.log('Error sending weather info to Pebble!');
	  }
      }  
  }                     
);



