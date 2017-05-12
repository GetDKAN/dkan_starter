import React, { Component } from 'react';
import { Dashboard, Card, BaseComponent, Dataset, DataHandler, StateHandler, DataHandlers, EventDispatcher, Registry } from 'react-dash'
import Datastore from './datastore';
import {isArray, isEmpty,  mapValues, reduce, isEqual, pick, intersection} from 'lodash';

let baseUrl = '';

for (let name in Drupal.settings.dkanDash.dataHandlers) {
  DataHandler.set(name, Drupal.settings.dkanDash.dataHandlers[name]);
}

for (let name in Drupal.settings.dkanDash.stateHandlers) {
  StateHandler.set(name, Drupal.settings.dkanDash.stateHandlers[name]);
}

export default class DKANDash extends Dashboard {
  constructor(props) {
    super(props);
    this.Datastore = new Datastore({
      baseUrl: baseUrl,
      dataResources: this.props.dataResources
    });
    this.state.appliedFilters = this.getConstantAppliedFilters();
  }

  applyDataHandlers(datahandlers, componentData=[]) {
    let _handlers = datahandlers;
    let _appliedFilters = this.state.appliedFilters || {};
    let _data = DataHandler.handle.call(this, _handlers, componentData, this.state.data, {}, _appliedFilters);
    return _data;
  }

  getDashboardData(appliedFilters) {
    let dashData = Object.assign({}, this.state.data);
    let dataKeys = Object.keys(this.props.dataResources || {});
    let i = 0;
    appliedFilters = appliedFilters || Object.assign({}, this.state.appliedFilters);

    dataKeys.forEach( (dataKey) => {
      let qObj = Object.assign({}, this.props.dataResources[dataKey]);
      let filters = this.getAppliedFiltersByDataKey(dataKey, appliedFilters);
      let filterQueries = this.getFilterQueries(filters);
      let qs = this.Datastore.mapQueries(dataKey, qObj.queries, qObj.uuid, filters, filterQueries);

      this.Datastore.fetchResource(qs).then(response => {
        i++;
        dashData[dataKey] = response;
        this.setState({data: dashData, isFetching: false});
      }).catch(e => {
        console.error('Error fetching resource', dataKey, qObj, e);
      });
    });
  }

  /**
   * @param key {String} where settings.dataResources[key] is valid
   * @returns {Array} Returns an array of format:
   * [
   *    {
   *      fieldName1: ['val']
   *    },
   *    {
   *      fieldName2: ['val1', 'val2', 'val3']
   *    },
   *    {
   *      tableName: {
   *        fieldName3: [val]
   *      }
   *    }
   * ]
   **/
  getFilterQueries(filters) {
    let output = {};

    filters.forEach(cur => {
      let vals = this.normalizeFilterValues(cur.value);

      if (vals.value) vals = vals.value;

      // need to key filter obj to join table, if present in filter config
      if (cur.table) {
        output[cur.table] = {};
        output[cur.table][cur.field] = vals;
      } else {
        output[cur.field] = vals;
      }
    });

    return output;
  }

  /**
   * Return all filters which are defined as part of
   * settings.dataResources -> queries
   */
  getConstantAppliedFilters() {
    let dataResources = this.props.dataResources || {};
    let constantAppliedFilters = {};

    // Drill down through the dataResources and their queries
    // and find defined filters which will be returned and
    // added to appliedFilters
    Object.keys(dataResources).forEach(dataKey => {
      Object.keys(dataResources[dataKey].queries).forEach(qKey => {
        if (dataResources[dataKey].queries[qKey].filters) {
          dataResources[dataKey].queries[qKey].filters.forEach(filter => {
            filter.willFilter = dataKey;
            constantAppliedFilters[filter.field] = filter;
          })
        }
      });
    });

    return constantAppliedFilters;
  }

  // @@TODO this code is reproduced in the Dashboard component onAction
  normalizeFilterValues(values) {
    let output = [];
    values.forEach(val => {
      if (val.value && isArray(val.value)) {
        output = output.concat(val.value);
      } else if (val.value && !isArray(val.value)) {
        output.push(val.value);
      } else {
        output.push(val);
      }
    });

    return output;
  }

  getChildData(component) {
    let data = [];

    if (component.dataHandlers) {
      data = this.applyDataHandlers(component.dataHandlers, component.data);
    } else if (component.data) {
      if (component.data.length > 0) {
        data = component.data;
      }
    }

    return data;
  }

  /**
   * Return a collection with filters that are not disabled
   * by any other filter.
   */
  filterDisabled(filters) {
    let filterKeys = Object.keys(filters);
    return reduce(filters, (memo, filter, filterKey) => {
      if(!intersection(filterKeys, filter.disabledBy).length) {
        memo[filterKey] = filter;
      }
      return memo;
    }, {});
  }

  /**
   * For appliedFilters, use filter definition
   * to determine if the filter applies to the
   * given dataResource (key) using the
   * filter.willFilter array
   **/
  getAppliedFiltersByDataKey(key, appliedFilters) {
    // We can pass appliedFilters here in order to skip the Dashboard component lifecycles
    let toFilter = [];
    appliedFilters = this.filterDisabled(appliedFilters);

    Object.keys(appliedFilters).map(k => {
      let next = appliedFilters[k];
      if (next && next.willFilter && next.willFilter.length > 0 ) {
        if (next.willFilter.indexOf(key) >= 0) {
          toFilter.push(next);
        }
      }
    });

    return toFilter;
  }
}
