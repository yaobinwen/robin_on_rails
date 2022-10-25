(function () {
  // cookie functions
  var trustArcHelper = {};
  var temp = location.host.split('.').reverse();
  var rootDomain = '.' + temp[1] + '.' + temp[0];
  var domain =
    document.currentScript.getAttribute('data-domain') || 'redhat.com';
  var cmId = document.currentScript.getAttribute('data-cmid') || null;
  // Convert debug to boolean.
  var debug = document.currentScript.getAttribute('data-debug') || 'false';
  debug = debug === 'true';
  trustArcHelper.cookieDomain = rootDomain;

  if (debug) {
    console.log('Global variables:', {
      domain,
    });
  }

  /**
   * @param s
   */
  trustArcHelper.sanitizeUntrusted = function (s) {
    var str = '';
    if (typeof s === 'string') {
      str = s.replace(/[;'<\/>=\)\(]/gm, '!');
    } else if (typeof s === 'number' || typeof s === 'boolean') {
      str = s;
    }
    return str;
  };
  /**
   * @param cookieName
   */
  trustArcHelper.getCookie = function (cookieName) {
    var name = encodeURI(cookieName) + '=';
    var cookieValue = '';
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
      var c = ca[i];
      while (c.charAt(0) === ' ') c = c.substring(1);
      if (c.indexOf(name) === 0) {
        cookieValue = c.substring(name.length, c.length);
        return decodeURIComponent(cookieValue);
      }
    }
    return cookieValue;
  };
  /**
   * @param cookieName
   * @param cookieValue
   * @param exdays
   */
  trustArcHelper.setCookie = function (cookieName, cookieValue, exdays) {
    // check for html markup in cookieValue
    // if html markup present, remove the markup portion
    var cookieVal = cookieValue || '';
    cookieVal = trustArcHelper.sanitizeUntrusted(cookieVal);
    // set expiration time
    var expires = '';
    if (typeof exdays !== 'undefined' && exdays > 0) {
      var d = new Date();
      d.setTime(d.getTime() + exdays * 24 * 60 * 60 * 1000);
      expires = '; expires=' + d.toUTCString();
    }
    document.cookie =
      encodeURI(cookieName) +
      '=' +
      encodeURIComponent(cookieVal) +
      expires +
      '; path=/; domain=' +
      trustArcHelper.cookieDomain;
  };
  /**
   * @param c
   * @param a
   */
  trustArcHelper.loadScript = function (c, a) {
    if (debug) {
      console.log('loadScript', c, a);
    }
    var b = document.createElement('script');
    b.type = 'text/javascript';
    if (a && typeof a === 'function') {
      if (b.readyState) {
        /**
         *
         */
        b.onreadystatechange = function () {
          if (b.readyState === 'loaded' || b.readyState === 'complete') {
            b.onreadystatechange = null;
            a();
          }
        };
      } else {
        /**
         *
         */
        b.onload = function () {
          a();
        };
      }
    }
    b.src = c;
    document.getElementsByTagName('head')[0].appendChild(b);
  };
  // load the trust arc script
  var path = '';
  if (cmId !== null) {
    path = 'https://consent.trustarc.com/v2/notice/' + cmId;
  } else {
    path = 'https://consent.trustarc.com/notice';
    var params = {
      c: 'teconsent',
      domain: domain,
      gtm: '1',
      js: 'nj',
      noticeType: 'bb',
      privacypolicylink: 'https://www.redhat.com/en/about/privacy-policy',
      text: 'true',
    };
    var paramsFormatted = [];
    if (trustArcHelper.getCookie('trustArcTesting') === 'yes') {
      params['country'] = trustArcHelper.getCookie('trustArcCountry') || 'us';
      params['country'] = params['country'].toLowerCase();
      if (params['country'] === 'us') {
        params['state'] = trustArcHelper.getCookie('trustArcState');
      }
    }
    if ('state' in params && params['state'] === 'ca') {
      // just used for ccpa testing
      params['domain'] = 'redhatstage.com';
    }
    for (var key in params) {
      paramsFormatted.push(key + '=' + encodeURIComponent(params[key]));
    }
    path += '?' + paramsFormatted.join('&');
  }
  if (
    document.location.pathname.indexOf('/forms') !== 0 &&
    document.location.hostname !== 'redhat.lookbookhq.com'
  ) {
    trustArcHelper.loadScript(path);
  }

  var _STATE = {};

  /**
   *
   */
  function runOnce() {
    if (
      !_STATE.hasRunOnce &&
      window.truste &&
      truste.eu &&
      truste.eu.prefclose
    ) {
      if (debug) {
        console.log('runOnce');
      }
      _STATE.oldValue = parseInt(truste.eu.bindMap.prefCookie);
      _STATE.oldMethod = truste.eu.prefclose;
      /**
       *
       */
      truste.eu.prefclose = function () {
        if (debug) {
          console.log('prefclose');
        }
        _STATE.oldMethod();
        _STATE.newValue = parseInt(truste.eu.bindMap.prefCookie);
        // NaN is not equal to NaN, so skip when the new value is not set.
        if (_STATE.oldValue !== _STATE.newValue && !isNaN(_STATE.newValue)) {
          if (debug) {
            console.log('cookie values diff', _STATE);
          }
          setTimeout(function () {
            window.location.reload();
          }, 20);
        }
      };
      _STATE.hasRunOnce = true;
      _STATE.i && clearInterval(_STATE.i);
    }
  }
  _STATE.i = setInterval(runOnce, 10);
  // end trust arc script
})();
