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


const DKANDashWrapper = (props) => <DKANDash {...Drupal.settings.dkanDash.dashboard} />;

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