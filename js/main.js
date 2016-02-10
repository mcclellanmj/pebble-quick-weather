(function() {
  var radios = ['unit', 'initialDisplay', 'showCurrentDay'];

  function submitHandler() {
    var submitButton = $('#submitButton');

    submitButton.on('click', function() {
      console.log('Submit');

      var return_to = getQueryParam('return_to', 'pebblejs://close#');
      document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
    });
  }

  function setRadioValue(name, selectedValue) {
    var query = 'input[name="' + name + '"][value="' + selectedValue + '"]'
    $(query).prop('checked', true)
  }

  function loadOptions() {
    var defaults = {
      'unit' : 'FAHRENHEIT', 
      'initialDisplay' : 'ICONS',
      'showCurrentDay' : 'NO'
    };

    radios.forEach(function(x) {
      if(localStorage[x] !== undefined) {
        setRadioValue(x, localStorage[x]);
      }

      if( $("input[name='" + x + "'][value='" + localStorage[x] + "']").val() === undefined ) {
        setRadioValue(x, defaults[x]);
      }
    });
  }

  function getAndStoreConfigData() {
    var radios = ['unit', 'initialDisplay', 'showCurrentDay'];
    var options = {};

    radios.forEach(function(x) {
      var value = $("input[name='" + x + "']:checked").val()
      options[x] = value;
      localStorage[x] = value;
    });

    console.log('Got options: ' + JSON.stringify(options));
    return options;
  }

  function getQueryParam(variable, defaultValue) {
    var query = location.search.substring(1);
    var vars = query.split('&');
    for (var i = 0; i < vars.length; i++) {
      var pair = vars[i].split('=');
      if (pair[0] === variable) {
        return decodeURIComponent(pair[1]);
      }
    }
    return defaultValue || false;
  }
  
  loadOptions();
  submitHandler();
})();
