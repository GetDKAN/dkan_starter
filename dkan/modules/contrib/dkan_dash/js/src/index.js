/**
 * Remove the css files you don't need because
 * they are loaded by the environment.
 */
import 'nvd3/build/nv.d3.min.css';
import 'react-dash/dist/react-dashboard.min.css';
import 'react-select/dist/react-select.min.css';
import 'fixed-data-table/dist/fixed-data-table.min.css';

/**
 * Don't remove this. Required dependencies.
 */
import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import { Router, Route, browserHistory } from 'react-router';
import DKANDash from './dkan_dash';
import { DataHandler } from 'react-dash';

let settings;
let settingsPath = window.location.pathname.substr(1).replace('/', '__');

if (Drupal.settings.dkanDash.devSettings[settingsPath]) {
  settings = Drupal.settings.dkanDash.devSettings[settingsPath];
} else {
  settings = Drupal.settings.dkanDash.dashboard;
}

const DKANDashWrapper = (props) => <DKANDash {...props} {...settings} />;

// Wrap Dashboard component in router
class App extends Component {
  render() {
    return (
      <div id="router-container">
        <Router history={browserHistory}>
          <Route path='*' component={DKANDashWrapper}/>
        </Router>
      </div>
    )
  }
}

/**
 * This renders the App
 */
document.addEventListener('DOMContentLoaded', function(event) {
  ReactDOM.render(<App />, document.getElementById('root'));
});

setInterval(() => {
  if(parent.postMessage) {
    parent.postMessage(document.body.scrollHeight, '*');
  }
}, 500);
