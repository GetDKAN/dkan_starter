var COUNTY_FIELD = 'WL1 County';

Drupal.settings.dkanDash.dataHandlers = {

  /**
   * CHARTS AND TABLES
   **/

  groupByRange: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let data = pipelineData || componentData;
    let finalOutput = []; // array of series
    data.forEach(series => {
      let outputSeries = []; // an array of objects
      for (let i in handler.ranges) {
        let lowerBound = handler.ranges[i][0];
        let upperBound = handler.ranges[i][1];
        let xVal = lowerBound + ' - ' + upperBound;
        let yVal = 0;
        let groupedRow = {};
        // loop through vals for current range, if it's in the range, add to the running yVal sum;
        series.forEach(row => {
          // if it's in the range, add it to this range's sum
          if (row[handler.xField] >= lowerBound && row[handler.xField] <= upperBound) {
            yVal += parseInt(row[handler.yField]);
          }
        })

        groupedRow[handler.xField] = xVal;
        groupedRow[handler.yField] = yVal;
        outputSeries.push(groupedRow);
      }
      // now add our transformed series to the array of series for output
      finalOutput.push(outputSeries);
    });

    return finalOutput;
  },

  getCountyMetric: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    if (_hasAllPhysData(dashboardData)) {
      if (appliedFilters[COUNTY_FIELD] && appliedFilters[COUNTY_FIELD].willFilter.indexOf('physicianData') > -1) {
        let vals = appliedFilters[COUNTY_FIELD].vals;
        let countyData = dashboardData.countyData.all.result.records;
        let selectedCounty = countyData.filter(row => {
          return parseInt(row.County_ID) === vals[0];
        })

        return [selectedCounty[0][handler.metric]]
      }

    }

    return ['...'];
  },

  getSingleCountyMetricsHeaderContent: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let countyName = appliedFilters[COUNTY_FIELD].value[0].label;
    return '<h2>Metrics for ' + countyName + ' County</h2>';
  },

  // for instance, we reassign undefined as '' (empty string)
  reassignFalseyVals: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let data = pipelineData || componentData;
    let _data = [];
    data.forEach(series => {
      let _series = series.map(row => {
        if (row[handler.xField].indexOf(handler.FalsyFields) >= 0) {
          row[handler.xField] = handler.falseVal;
        }
        return row;
      });
      _data.push(_series);
    });
  },


  countyMetricMultiSelect: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let val;

    return _dashMode(appliedFilters);
  },

  // This is a hot mess - don't expect to understand this
  // We are getting percentage of total physicians queried who
  // accept medicare/medicaid vs who accept NEW medicare/medicaid
  getAcceptMedChartData: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let type = handler.serviceType;
    let total = 0;
    let accept_service, accept_service_amt, accept_service_pct, accept_new, accept_new_amt, accept_new_pct;
    let acceptField = (type === 'medicaid') ? 'Accept Medicaid' : 'Accept Medicare';
    let newField = (type === 'medicaid') ? 'New Medicaid' : 'New Medicare';

    // check for fetched data and preprocess
    if (dashboardData && dashboardData.physicianData && dashboardData.physicianData['accept_' + type]) {
      accept_service = dashboardData.physicianData['accept_' + type].result.records;
      accept_new = dashboardData.physicianData['accept_new_' + type].result.records;

      accept_service.forEach(row => {
        let t = (isNaN(row['count_accept_' + type]) ? 0 : parseInt(row['count_accept_' + type]));
        if (row[acceptField] === "Y") accept_service_amt = t;
        total += t;
      });

      accept_new.forEach(row => {
        if (row[newField] === "Y") accept_new_amt = parseInt(row['count_new_' + type]);
      });

      accept_service_pct = accept_service_amt / total || 0;
      accept_new_pct = accept_new_amt / total || 0;


      return [
        [ {x: acceptField, y: accept_service_pct }, {x: newField, y: accept_new_pct } ]
      ]

    }

    // If no data yet...
    return [];

  },

  // @@TODO finish this
  percentRetiringByAgeRange: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let data = componentData || pipelineData;
    if (data.length > 0) {
      return data;
    }
  },

  /**
   * METRICS
   **/

  // @@TODO getFieldTotal could easily be generalized to a common .dataHandler
  getCountyTotals: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let countyData, counties, records, val;

    if (_hasAllPhysData(dashboardData)) {
      countyData = dashboardData.countyData.all;
      records = countyData.result.records;
      records = _filterDataByCounty(records, appliedFilters);
      val = _getCountyTotal(records, handler.field);
      return [val]
    }
    return ['...']
  },

  // Total of all hours worked by GA Physicians / 40
  getPHW: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let hours = 0;
    let PHW;

    if (_hasAllPhysData(dashboardData)) {
      let data = dashboardData.physicianData.by_county.result.records;
      data.forEach(row => {
        hours += parseFloat(row.sum_county1);
        hours += parseFloat(row.sum_county2);
        hours += parseFloat(row.sum_county3);
      });
    }

    PHW = Math.round(hours/40);

    if (hours > 0 ) return [PHW];
    return ['.....'];
  },

  // @@DEPRECATED
  // Leaving this for historical purposes
  // We went back and forth a lot on this
  /*
  getTWB: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let wb;
    if (dashboardData.physicianData) {
      let data = dashboardData.physicianData.by_county.result.records;
      wb = _getWarmBodies(data);
    }
    return [wb];
  },
  */

  /**
   * Get Total Physician Count (TPC)
   */
  getTPC: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let TPC;

    if (_hasAllPhysData(dashboardData)) {
      let data = {};
      data.county1 = dashboardData.physicianData.by_county.result.records;
      data.county2 = dashboardData.physDataCounty2.by_county_2.result.records;
      TPC = _getTPC(data, dashboardData, _dashMode(appliedFilters), appliedFilters);

      return [TPC];
    }

    return ['...'];
  },

  getRate: function (componentData, dashboardData, handler, e, appliedFilters, pipelineData) {
    let physData = {};
    let countyData, records, pop, TPC, rate;

    if (_hasAllPhysData(dashboardData)) {
      countyData = dashboardData.countyData.all
      records = countyData.result.records;
      records = _filterDataByCounty(records, appliedFilters);
      pop = _getCountyTotal(records, 'County_Population');

      physData.county1 = dashboardData.physicianData.by_county.result.records;
      physData.county2 = dashboardData.physDataCounty2.by_county_2.result.records;
      TPC = _getTPC(physData, dashboardData, _dashMode(appliedFilters), appliedFilters);

      if (TPC > 0) {
        rate = ( TPC / pop ) * 100000;
        rate = d3.format(".0f")(rate);
        return [rate];
      }
    }

    return ['...'];
  },
}

function _filterDataByCounty(records, appliedFilters) {
    let filteredData = records;

    // IF COUNTY FILTER APPLIED
    if (_hasCountyFilterApplied(appliedFilters)) {
      filteredData = records.filter(row => {
          return (appliedFilters[COUNTY_FIELD].vals.indexOf(parseInt(row.County_ID)) >= 0);
      });
    }

    // if one MSA filter is applied, filter countyData
    // if both are applied - use state data
    if (appliedFilters.MSA) {
      filteredData = records.filter(row => {
        return appliedFilters.MSA.value.indexOf(row.MSA) > -1
      })
    }

   return filteredData;
}

function _getCountyTotal(countyData, field) {
  let x = 0;

  countyData.forEach(row => {
    x += parseInt(row[field]);
  });

  return x;
}

/**
 * Calculate TPC (Total Physician Count)
 * Filter phys data for county 1 and county 2
 * and return number total number physicians
 * in county
 */
function _getTPC(data, dashboardData, dashMode, appliedFilters) {
  let TPC = 0;

  // If it's the state or MSA level use basic count query to return total number of applicants
  if (dashMode === 'state' || appliedFilters.MSA) {
    return dashboardData.physicianData.state_total.result.records[0].count_applc_nbr || NULL;
  } else {      // if county filters are applied, filter by county and county
    Object.keys(data).forEach(d => {
      data[d].forEach(row => {
        if (row.count_county1 && row['WL1 County'] >0 && !isNaN(row.count_county1)) TPC += parseInt(row.count_county1);
        // if (row.count_county2 && row['WL2 County'] >0 && !isNaN(row.count_county2)) TPC += parseInt(row.count_county2);
      });
    });
  }

  return TPC;
}

/**
 * Dash mode is determined by which filters are applied
 * It is either state, singleCounty or multipleCounties
 */
function _dashMode(appliedFilters) {
    let mode;
    let hasCountyFilter = _hasCountyFilterApplied(appliedFilters);

    if (!hasCountyFilter)
    {
      mode = 'state';
    }

    else if (hasCountyFilter && appliedFilters[COUNTY_FIELD].value.length === 1)
    {
      mode = 'singleCounty';
    }

    else if (hasCountyFilter && appliedFilters[COUNTY_FIELD].value.length > 1)
    {
      mode = 'multipleCounties';
    }

    if (appliedFilters.MSA) {
      mode = 'multipleCounties';
    }

    return mode;
}

/**
 * Make sure data has returned for all data before updating filter
 * This way, all metrics update at the same time.
 *
 * @@ TODO this should use state.loading when that is implemented
 * @@ TODO also I don't think it's working entirely correctly
 */
function _hasAllPhysData(data) {
  let dashboardData = data || {};

  // return result of stupidly arge conditional
  return (
    (
      dashboardData.physicianData &&
      dashboardData.physicianData.by_county &&
      dashboardData.physicianData.by_county.result &&
      dashboardData.physicianData.by_county.result.records
    ) &&
    (
      dashboardData.physDataCounty2 &&
      dashboardData.physDataCounty2.by_county_2 &&
      dashboardData.physDataCounty2.by_county_2.result &&
      dashboardData.physDataCounty2.by_county_2.result.records
    )
  );
}

function _hasCountyFilterApplied(appliedFilters) {
  return (appliedFilters && appliedFilters[COUNTY_FIELD] && appliedFilters[COUNTY_FIELD].willFilter.indexOf('physicianData') > -1);
}