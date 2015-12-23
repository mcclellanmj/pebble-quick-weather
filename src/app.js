var Constants = {
  "OpenWeatherMap" : {
    "appId" : "%OPEN_WEATHER_MAP_KEY%",
    "baseUrl" : "http://api.openweathermap.org/data/2.5/forecast/daily",
    "format" : "json",
    "maxResults" : 11,
    "units" : "metric"
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

var ArrayUtils = (function() {
  var self = {};
  
  /**
  * Collapse a sequence of arrays into a single array
  * For example [[a,b], [c,d]] will become a,c,b,d
  */
  self.sequence = function(arrays) {
    var final = [];

    arrays.forEach(function(element) {
      element.forEach(function(item) { final.push(item) });
    });
  };
  
  return {
    'sequence' : self.sequence
  };
})();

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
    return "1";
  };

  self.createPebbleModel = function(forecastStart, highs, lows, ids) {
    var expectedLength = highs.length;
    
    // Check our assumptions
    if(lows.length != expectedLength) {
      throw "Length of lows was different than highs, expected [" + expectedLength + "] but got [" + lows.length + "]";
    } 

    if(ids.length != expectedLength) {
      throw "Length of ids was different than highs, expected [" + expectedLength + "] but got [" + ids.length + "]";
    }
    
    var highBytes = [].concat.apply([], highs.map(ByteConversions.toInt8ByteArray));
    var lowBytes = [].concat.apply([], lows.map(ByteConversions.toInt8ByteArray));
    var forecastIds = [].concat.apply([], ids.map(function(id) { return ByteConversions.toInt8ByteArray(self.smallId(id));}));
    var weatherStructs = ArrayUtils.sequence([forecastIds, highBytes, lowBytes]);

    return {
      "MESSAGE_TYPE" : ByteConversions.toInt8ByteArray(Constants.MessageTypes.WEATHER_REPORT),
      "WEATHER_START" : ByteConversions.toInt32ByteArray(forecastStart),
      "WEATHER_FORECASTS" : weatherStructs
    };
  };

  self.weatherSuccess = function(weather) {
    var response = JSON.parse(weather.response);
    
    var list = response.list;
    var highs = list.map(function(x) {return x.temp.max;});
    var lows = list.map(function(x) {return x.temp.min;});
    var ids = list.map(function(x) {return parseInt(x.weather.id)});
    var startDate = list[0].dt;

    var weatherModel = self.createPebbleModel(startDate, highs, lows, ids);
    Pebble.sendAppMessage(weatherModel);
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

var MessageHandler = (function() {
  var self = {};
  
  self.handleMessage = function(message) {
    var messageType = message.MESSAGE_TYPE;
    console.log("Got message of type [" + messageType + "]");
    
    if(messageType === Constants.MessageTypes.FETCH_WEATHER) {
      console.log("Start fetching the weather");
      navigator.geolocation.getCurrentPosition(Weather.retrieve, console.log, Constants.LocationOptions);
    } else {
      console.log("Received unknown message type of [" + messageType + "]");
    }

  };
  
  return {
    'handleMessage' : self.handleMessage
  };
})();

Pebble.addEventListener('ready', function() {
    console.log('JavaScript app ready and running');
    Pebble.sendAppMessage({"MESSAGE_TYPE": Constants.MessageTypes.PHONE_READY});
    console.log('Phone ready message has been sent');
  }
);

Pebble.addEventListener('appmessage', function(e) {
    var msg = e.payload;
    console.log("Got message from the watch");
    MessageHandler.handleMessage(msg);
  }
);

// navigator.geolocation.getCurrentPosition(Weather.retrieve, console.log, Constants.LocationOptions);