var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
	if (xhr.readyState==4 && xhr.status==200) {
 	  callback(this.responseText);
	} else {
	  console.log(xhr.readyState + " " + xhr.status);	
	}
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
    var query = e.payload["1"];
    var q2 = encodeURI(query);
    //var url = 'http://192.168.43.160:5000/api?q=' + q2;

	var url = 'http://192.168.43.160:5000/api?q=' + q2
	console.log(url);
    xhrRequest(url, 'GET',
      function(responseText) {
        var json = JSON.parse(responseText);
	var ans = json.answer;
	var arr = json.searchResults;
	var titles = "";
	var urls = "";
	for(var i = 0; i < arr.length; i++) {
	  titles = titles + arr[i].title + "\t";
	  urls = urls + arr[i].url + "\t";
	}
	console.log(ans);
	console.log(titles);
	console.log(urls);
	var dict = {2: ans, 3:titles, 4:urls};
	  Pebble.sendAppMessage(dict,
          function(e) {
            console.log('Sent back successfully');
          },
          function(e) {
            console.log('Failed');
          }
        )
	});
  
  }                     
);



