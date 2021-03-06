var Constants = {
  "OpenWeatherMap" : {
    "appId" : "%OPEN_WEATHER_MAP_KEY%",
    "baseUrl" : "http://api.openweathermap.org/data/2.5/forecast/daily",
    "format" : "json",
    "maxResults" : 11,
    "units" : "metric"
  },
  "WeatherUnderground" : {
    "urlTemplate" : "http://api.wunderground.com/api/{KEY}/forecast10day/q/{LON},{LAT}.json"
  },
  "MessageTypes" : {
    "PHONE_READY" : 0,
    "WEATHER_REPORT" : 1,
    "FETCH_WEATHER" : 2,
    "WEATHER_FAILED" : 3,
    "LOCATION_FAILED" : 4,
    "CONFIGURATION_CHANGED" : 5
  },
  "LocationOptions" : {
    "enableHighAccuracy" : false, 
    "maximumAge" : 240000,
    "timeout" : 15000
  },
  "HTTP" : {
    "requestTimeout" : 30000
  },
  "Analytics" : {
    "key" : "%QUICK_WEATHER_GOOGLE_ANALYTICS_KEY%",
    "URL" : "http://www.google-analytics.com/collect",
    "appName" : "Pebble Quick Weather",
    "appVersion" : "1.0.0"
  },
  "Config" : {
    "URL" : "http://mcclellanmj.github.io/pebble-quick-weather/"
  },
  // LongToShortMap is generated using the generate_mapping_js.py file, not advised to manually edit this
  "OpenWeatherLongToShortMap": {"761": "45", "622": "38", "621": "37", "620": "36", "212": "5", "600": "29", "210": "3", "211": "4", "313": "16", "312": "15", "311": "14", "310": "13", "701": "39", "314": "17", "601": "30", "230": "7", "231": "8", "232": "9", "959": "69", "958": "68", "962": "72", "762": "46", "951": "61", "953": "63", "602": "31", "955": "65", "954": "64", "957": "67", "956": "66", "731": "42", "612": "33", "321": "18", "520": "25", "521": "26", "522": "27", "504": "23", "502": "21", "503": "22", "500": "19", "501": "20", "201": "1", "200": "0", "751": "44", "202": "2", "771": "47", "300": "10", "301": "11", "302": "12", "611": "32", "616": "35", "952": "62", "711": "40", "615": "34", "803": "52", "960": "70", "221": "6", "961": "71", "902": "56", "903": "57", "900": "54", "901": "55", "906": "60", "781": "48", "904": "58", "905": "59", "531": "28", "721": "41", "511": "24", "802": "51", "801": "50", "800": "49", "741": "43", "804": "53"},
  "WeatherUndergroundLongToShortMap": {"flurries": "29", "chanceflurries": "73", "mostlycloudy": "52", "partlycloudy": "51", "partlysunny": "51", "snow": "30", "clear": "49", "sunny": "49", "mostlysunny": "50", "cloudy": "53", "chancesnow": "76", "sleet": "32", "tstorms": "4", "fog": "43", "hazy": "41", "unknown": "24", "rain": "78", "chancesleet": "75", "chancerain": "74", "chancetstorms": "77"}
};

var ArrayUtils = (function() {
  var self = {};
  
  /**
  * Collapse a sequence of arrays into a single array
  * For example [[a,b], [c,d]] will become a,c,b,d
  */
  self.sequence = function(arrays) {
    var toReturn = [];
    var lengths = arrays[0].length;

    for(var i = 0; i < lengths; i++) {
      for(var j = 0; j < arrays.length; j++) {
        toReturn.push(arrays[j][i]);
      }
    }

    return toReturn;
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

var Debug = (function() {
  var self = {};

  self.printArray = function(array) {
    console.log(array.join(','));
  };

  return {
    'printArray' : self.printArray
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

var Analytics = (function() {
  var self = {};

  self.track = function(action) {
    var req = new XMLHttpRequest();
    var queryParams = {
      "v" : "1",
      "tid" : Constants.Analytics.key,
      "cid" : Pebble.getAccountToken(),
      "t" : "event",
      "an" : Constants.Analytics.appName,
      "av" : Constants.Analytics.appVersion,
      "ec" : "app",
      "ea" : action
    };

    var url = Urls.generateUrl(Constants.Analytics.URL, queryParams);
    console.log("Requesting analytics from [" + url + "]")

    HTTPServices.makeRequest('POST', url, function() {console.log("success")}, function() {console.log("error")});
  };

  return {
    'track' : self.track
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

var Configuration = (function() {
  var self = {};

  self.convertConfigurationToBytes = function(phoneModel) {
    var initialDisplay = phoneModel.initialDisplay === "TEXT" ? 1 : 0;
    var unit = phoneModel.unit === "CELSIUS" ? 1 : 0;

    var result = [].concat.apply([], [initialDisplay, unit].map(ByteConversions.toInt8ByteArray));
    return result;
  };

  self.loadConfiguration = function() {
    var initialDisplay = localStorage.getItem("initialDisplay");
    var unit = localStorage.getItem("unit");
    var showCurrentDay = localStorage.getItem("showCurrentDay");
    var weatherUndergroundKey = localStorage.getItem("weatherUndergroundKey");

    return {
      "unit" : unit,
      "initialDisplay" : initialDisplay,
      "showCurrentDay" : showCurrentDay,
      "weatherUndergroundKey": weatherUndergroundKey === null ? "" : weatherUndergroundKey
    };
  };

  return {
    "convertConfigurationToBytes" : self.convertConfigurationToBytes,
    "loadConfiguration" : self.loadConfiguration
  };
})();

var WeatherUnderground = (function() {
  var self = {};

  self.buildRetrieve = function(apiKey) {
    console.log("Building the retrieve function " + apiKey);

    return function(gpsLocation) {
      self.retrieve(apiKey, gpsLocation);
    }.bind(this);
  };

  self.generateUrl = function(apiKey, gpsLocation) {
    var template = Constants.WeatherUnderground.urlTemplate;
    template = template.replace("{KEY}", apiKey);
    template = template.replace("{LON}", gpsLocation.latitude);
    template = template.replace("{LAT}", gpsLocation.longitude);

    return template;
  };

  self.retrieve = function(apiKey, gpsLocation) {
    var url = self.generateUrl(apiKey, gpsLocation.coords);
    console.log("url is [" + url + "]");

    HTTPServices.makeRequest("GET", url, self.weatherSuccess, self.weatherFailed);
  };

  self.smallId = function(condition) {
    if(!Constants.WeatherUndergroundLongToShortMap.hasOwnProperty(condition)) {
      var message = "The forecast code [" + condition + "] had no mapping in our forecast mappings";
      console.log(message);
      throw message;
    }

    var smallId = Constants.WeatherUndergroundLongToShortMap[condition];
    return smallId;
  };

  self.createPebbleModel = function(forecast) {
    var highs = forecast.map(function(x) {return parseInt(x.high.celsius)});
    Debug.printArray(highs);
    var lows = forecast.map(function(x) {return parseInt(x.low.celsius)});
    Debug.printArray(lows);
    var conditions = forecast.map(function(x) {return x.icon});
    Debug.printArray(conditions);

    var highBytes = [].concat.apply([], highs.map(ByteConversions.toInt8ByteArray));
    var lowBytes = [].concat.apply([], lows.map(ByteConversions.toInt8ByteArray));
    var forecastIds = [].concat.apply([], conditions.map(function(x) { return ByteConversions.toInt8ByteArray(self.smallId(x));}));
    var weatherStructs = ArrayUtils.sequence([forecastIds, highBytes, lowBytes]);

    var forecastStart = parseInt(forecast[0].date.epoch);

    return {
      "MESSAGE_TYPE" : ByteConversions.toInt8ByteArray(Constants.MessageTypes.WEATHER_REPORT),
      "WEATHER_START" : ByteConversions.toInt32ByteArray(forecastStart),
      "WEATHER_FORECASTS" : weatherStructs
    };
  };

  self.weatherSuccess = function(data) {
    var response = JSON.parse(data.response);
    var forecasts = response.forecast.simpleforecast.forecastday;
    var pebbleModel = self.createPebbleModel(forecasts);

    Pebble.sendAppMessage(pebbleModel);
  };

  self.weatherFailed = function(error) {
    console.log(error);
    Pebble.sendAppMessage({
      "MESSAGE_TYPE" : ByteConversions.toInt8ByteArray(Constants.MessageTypes.WEATHER_FAILED)
    });
  };

  return {
    "buildRetrieve" : self.buildRetrieve
  };
})();

var OpenWeatherMap = (function() {
  var self = {};
  
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
    if(!Constants.OpenWeatherLongToShortMap.hasOwnProperty(bigId)) {
      var message = "The forecast code [" + bigId + "] had no mapping in our forecast mappings";
      console.log(message);
      throw message;
    }

    var smallId = Constants.OpenWeatherLongToShortMap[bigId];
    return smallId;
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

  self.getFirstDay = function(showCurrentDay, responseList) {
    console.log("Show Today = [" + showCurrentDay + "]");
    if(showCurrentDay === "YES") {
      return responseList;
    } else {
      return responseList.slice(1);
    }
  }

  self.weatherSuccess = function(weather) {
    var response = JSON.parse(weather.response);
    var configuration = Configuration.loadConfiguration();
    
    var list = self.getFirstDay(configuration.showCurrentDay, response.list);

    var highs = list.map(function(x) {return x.temp.max;});
    var lows = list.map(function(x) {return x.temp.min;});
    var ids = list.map(function(x) {return x.weather[0]['id']});
    var startDate = list[0].dt;

    var weatherModel = self.createPebbleModel(startDate, highs, lows, ids);
    Pebble.sendAppMessage(weatherModel);
  };
  
  self.weatherFailed = function(error) {
    console.log(error);
    Pebble.sendAppMessage({
      "MESSAGE_TYPE" : ByteConversions.toInt8ByteArray(Constants.MessageTypes.WEATHER_FAILED)
    });
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

var GPS = (function() {
  var self = {};

  self.failed = function(error) {
    console.log(error);
    Pebble.sendAppMessage({
      "MESSAGE_TYPE" : ByteConversions.toInt8ByteArray(Constants.MessageTypes.LOCATION_FAILED)
    });
  };

  return {
    'failed' : self.failed
  };
})();

var MessageHandler = (function() {
  var self = {};
  
  self.handleMessage = function(message) {
    var messageType = message.MESSAGE_TYPE;
    console.log("Got message of type [" + messageType + "]");
    
    if(messageType === Constants.MessageTypes.FETCH_WEATHER) {
      console.log("Start fetching the weather");

      var weatherUndergroundKey = Configuration.loadConfiguration().weatherUndergroundKey;
      console.log("Weather underground key is [" + weatherUndergroundKey + "]");

      var retrieveFunction = weatherUndergroundKey !== "" ? WeatherUnderground.buildRetrieve(weatherUndergroundKey) : OpenWeatherMap.retrieve;
      navigator.geolocation.getCurrentPosition(retrieveFunction, GPS.failed, Constants.LocationOptions);
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
    Pebble.sendAppMessage({
      "MESSAGE_TYPE" : Constants.MessageTypes.PHONE_READY,
      "CONFIGURATION" : Configuration.convertConfigurationToBytes(Configuration.loadConfiguration())
    });
    Analytics.track("started");
    console.log('Phone ready message has been sent');
  }
);

Pebble.addEventListener('appmessage', function(e) {
    var msg = e.payload;
    console.log("Got message from the watch");
    MessageHandler.handleMessage(msg);
  }
);

Pebble.addEventListener("showConfiguration", function(e) {
  console.log("Showing the configuration");

  var config = Configuration.loadConfiguration();
  Pebble.openURL(Constants.Config.URL + "#" + encodeURIComponent(JSON.stringify(config)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("Showing the webview closed");
  var configData = JSON.parse(decodeURIComponent(e.response));

  localStorage.setItem("initialDisplay", configData.initialDisplay);
  localStorage.setItem("unit", configData.unit);
  localStorage.setItem("showCurrentDay", configData.showCurrentDay);
  localStorage.setItem("weatherUndergroundKey", configData.weatherUndergroundKey);

  Pebble.sendAppMessage({
    "MESSAGE_TYPE": Constants.MessageTypes.CONFIGURATION,
    "CONFIGURATION" : Configuration.convertConfigurationToBytes(configData),
  });
});