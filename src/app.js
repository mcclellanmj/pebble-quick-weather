var Constants = {
  "OpenWeatherMap" : {
    "appId" : "%OPEN_WEATHER_MAP_KEY%",
    "baseUrl" : "http://api.openweathermap.org/data/2.5/forecast/daily",
    "format" : "json",
    "maxResults" : 10,
    "units" : "imperial"
  },
  "MessageTypes" : {
    "PHONE_READY" : 0,
    "WEATHER_REPORT" : 1,
    "FETCH_WEATHER" : 2,
    "WEATHER_FAILED" : 3
  },
  "LocationOptions" : {
    "enableHighAccuracy" : false, 
    "maximumAge" : 240000,
    "timeout" : 15000
  },
  "HTTP" : {
    "requestTimeout" : 30000
  },
};

var KeyValueFunctions = (function() {
  var self = {};
  
  self.join = function(pairs, keyValueSeperator, pairSeperator) {
    return Object.keys(pairs).map(function(key) {
      return encodeURIComponent(key) + keyValueSeperator + encodeURIComponent(pairs[key]);
    }).join(pairSeperator);
  };
  
  return {
    'join' : self.join
  };
})();

var Urls = (function() {
  var self = {};
  
  self.generateUrl = function(path, queryParams) {
    return path + "?" + KeyValueFunctions.join(queryParams, "=", "&");
  };
  
  return {
    'generateUrl' : self.generateUrl
  };
})();

var HTTPServices = (function() {
  var self = {};
  
  self.makeRequest = function(method, url, callback, errCallback) {
  	var req = new XMLHttpRequest();
    
    req.onerror = errCallback;
    req.onreadystatechange = function(e) {
      if(this.readyState == 4 && this.status >= 200 && this.status < 300) {
        callback(this);
      } else if(this.readyState == 4) {
        console.log("Request for weather failed status code is [" + this.status + "]");
        errCallback(this);
      }
    };
    
    // If after the timeout request hasn't finished we need to abort it
    setTimeout(function() {
      console.log("Checking up on HTTP request");
      if(this.readyState != 4) {
        this.abort();
        this.onerror();
        console.log("Aborting request since its still not done");
      }
    }.bind(req), Constants.HTTP.requestTimeout);
    
    req.open(method, url, true);
    req.send();
  };
  
  return {
    'makeRequest' : self.makeRequest
  };
})();

var ByteConversions = (function() {
  var showBinaryForByte = function(byte) {
    var str = byte.toString(2);
    return ('00000000' + str).substring(str.length);
  };
  
  var toInt8ByteArray = function(integer) {
    return [integer & 0x000000FF];
  };
  
  var toInt16ByteArray = function(integer) {
    return [
      (integer & 0x000000FF),
      (integer & 0x0000FF00) >> 8
    ];
  };
  
  var toInt32ByteArray = function(integer) {
    return [
      (integer & 0x000000FF),
      (integer & 0x0000FF00) >> 8,
      (integer & 0x00FF0000) >> 16,
      (integer & 0xFF000000) >> 24
    ];
  };
  
  return {
    showBinaryForByte : showBinaryForByte,
    toInt8ByteArray : toInt8ByteArray,
    toInt16ByteArray : toInt16ByteArray,
    toInt32ByteArray : toInt32ByteArray,
  };
})();

var Weather = (function() {
  var self = this;
  
  self.buildQueryParamsMap = function(gpsLocation) {
    return {
      'appId' : Constants.OpenWeatherMap.appId,
      'lat' : gpsLocation.latitude,
      'lon' : gpsLocation.longitude,
      'mode' : Constants.OpenWeatherMap.format,
      'cnt' : Constants.OpenWeatherMap.maxResults,
      'units' : Constants.OpenWeatherMap.units
    }
  };
  
  self.smallId = function(bigId) {
    throw "Not yet implemented";
  };
  
  self.weatherSuccess = function(weather) {
    console.log(weather.response);
    
    var response = JSON.parse(weather.response);
    console.log(response.list);
    
    var list = response.list;
    var highs = list.map(function(x) {return x.temp.max;});
    var lows = list.map(function(x) {return x.temp.min;});
    var ids = list.map(function(x) {return parseInt(x.weather.id)});
  };
  
  self.weatherFailed = function(error) {
    console.log(error);
  };
  
  self.retrieve = function(gpsResults) {
    var queryParams = self.buildQueryParamsMap(gpsResults.coords);
    var url = Urls.generateUrl(Constants.OpenWeatherMap.baseUrl, queryParams);
    console.log("Retrieving the weather from generated url of [" + url + "]");
    
    HTTPServices.makeRequest("GET", url, self.weatherSuccess, self.weatherFailed);
  };
  
  return {
    'retrieve' : self.retrieve
  };
})();

var MessageHandler = (function(handlers) {
  var self = {};
  
  // Check preconditions
  if(handlers === undefined) {
    throw "Handlers is undefined"
  }
  
  if(handlers.length < 1) {
    throw "No handlers were created, message handler will not be able to handle messages"
  }
  
  self.handlers = handlers;
  
  self.handleMessage = function(message) {
    var messageType = "???";
    throw "Not implemented";
    
    var handler = handlers[messageType];
    if(handler === undefined) {
      throw "No handler for message of type [" + messageType + "]";
    }
  };
  
  return {
    'handleMessage' : self.handleMessage
  };
});

Pebble.addEventListener('ready', function() {
    console.log('JavaScript app ready and running');
    Pebble.sendAppMessage({"MESSAGE_TYPE": Constants.MessageTypes.PHONE_READY});
    console.log('Phone ready message has been sent');
  }
);

var messageHandler = new MessageHandler([function(message) {console.log(message);}]);
Pebble.addEventListener('appmessage', function(e) {
    var msg = e.payload;
    messageHandler.handleMessage(msg);
  }
);

navigator.geolocation.getCurrentPosition(Weather.retrieve, console.log, Constants.LocationOptions);