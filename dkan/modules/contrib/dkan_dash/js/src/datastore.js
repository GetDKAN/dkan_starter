import { isArray } from 'lodash';

const QUERY_LIMIT = 10000000;
const DKAN_AG_FIELDS = [
    'sort',
    'group_by',
    'avg',
    'sum',
    'min',
    'max',
    'count'
  ];

export default class Datastore {
  constructor(props) {
    this.baseUrl = props.baseUrl;
    this.dataResources = props.dataResources || {};
    this.queryLimit = props.queryLimit || QUERY_LIMIT;
  }

  fetchResource(queries) {
    return new Promise((resolve, reject) => {
      fetch(this.baseUrl + '/api/action/datastore/search.json', {
        method: 'post',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        contentType: 'application/json',
        body: JSON.stringify(queries)
      })
      .then(response => response.json())
      .then(data => resolve(data))
      .catch(e => reject(e));
    })
  }

  // takes current query and query key
  // returns true if query needs to be rewritten based on joins
  // or returns false
  checkRewriteAggregateField(q, field, revert = false) {
    if (DKAN_AG_FIELDS.indexOf(field) >= 0) {
      if (revert) {
        if (typeof q[field] === "object") {
          return true;
        }
      } else {
        if (typeof q[field] === "string") {
          return true;
        }
        if (isArray(q[field])) {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * @param filters {Array} An array of filters
   **/
  getJoinQuery(filters) {
    let dataResources = Object.assign({}, this.dataResources);
    let ids = {};
    let joinFields = {};
    let output = {};

    filters.forEach(filter => {
      let aliasCount = {}; // keep track of incremented aliases
      let dataKeys = Object.keys(filter.joins); // get dataKeys

      dataKeys.forEach(k => {
        // add table alias to ids obj
        let i;

        if (!ids[k]) {
          ids[k] = dataResources[k].uuid;
          joinFields[k] = filter.joins[k];
        } else {
          // re-alias k with, incrementing alias count
          if (!aliasCount[k]) {
            aliasCount[k] = 1;
            ids[k + aliasCount[k]] = dataResources[k].uuid;
            joinFields[k] = filter.joins[k]; // add joinField for this alias
          } else {
            aliasCount[k]++;
            ids[k + aliasCount[k]] = dataResources[k].uuid;
            joinFields[k + aliasCount[k]] = filter.joins[k];
          }
        }
      });
    });

    output.ids = ids;
    output.joinFields = joinFields;

    return output;
  }


  // Map appliedFilters onto API query object
  // for a single dataResource
  // a join represents a set of resourceIds (tables)
  // and a set of fields
  // resourceIds and fields are keyed to a table alias
  //
  // We use the dataResource[dataKey].uuid as the resource_id
  // By default we use the dataKey as the table alias
  // If the alias is already in use, we need to create
  // a new alias
  mapQueries(dataKey, qs, uuid, filters, filterQueries) {
    let joinFilters = [];
    let nonJoinFilters = [];

    // separate filters into join-filters, and non-
    filters.forEach(filter => {
      if (filter.table) {
        joinFilters.push(filter);
      } else {
        nonJoinFilters.push(filter);
      }
    });

    for (let k in qs) {
      let joinQuery = this.getJoinQuery(joinFilters);

      if (joinFilters.length > 0) {
        qs[k].resource_id = joinQuery.ids;
        qs[k].join = joinQuery.joinFields;

        // reformat query aggregations with tablename if JOINS are present
        Object.keys(qs[k]).forEach(qField => {
          if (this.checkRewriteAggregateField(qs[k], qField)) {
            let qFieldVal = qs[k][qField];

            qs[k][qField] = {};
            qs[k][qField][dataKey] = {};
            qs[k][qField][dataKey] = qFieldVal;
          }
        })
      } else {
        qs[k].resource_id = uuid;

        Object.keys(qs[k]).forEach(qField => {
          if (this.checkRewriteAggregateField(qs[k], qField, true)) {
            // rewrite aggregate field values to DKAN API's non-join syntax
            let agVal = this.dataResources[dataKey].queries[k][qField][dataKey] || this.dataResources[dataKey].queries[k][qField];
            qs[k][qField] = agVal;
          }
        });
      }

      qs[k].limit = this.queryLimit;
      qs[k].filters = filterQueries;
    }

    return qs;
  }
}