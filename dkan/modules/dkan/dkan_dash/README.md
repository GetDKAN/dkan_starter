# React Dashboard Drupal Module 0.3.x

This is a boilerplate to create dashboards in Drupal by using the **React Dashboard** library. Library documentation is available at https://github.com/NuCivic/react-dashboard

Feel free to modify this code to fit your project requirements. If you think there is something that can be provided out-of-the-box please create a PR.


## What's provided?

* A working example
* A Drupal page with the needed markup to render a dashboard
* An endpoint to expose the data to be consumed by your dashboard
* An example of the autocomplete endpoint


## Getting started

```bash
$ git clone https://github.com/NuCivic/react_dashboard.git
$ bash init.sh
$ cd app
$ npm run dev_dkan
$ open http://localhost:5000/
```


## Autocomplete endpoint

A placeholder function to retrieve options ready to be consumed by the *React autocomplete* component.

`/dashboard_autocomplete/%node/%field/%value`

And it returns the data ready to be consumed by the autocomplete component.

```javascript
[
    {
        label: 'Label to be displayed',
        value: 'machine_name_to_be_iused'
    },
    ...
]
```


## Data endpoint

Provides an endpoint to query resources using filters and aggregations for a given node.

**Example:**
`/dashboard_data/12?year=2014&sum=agency`

**Options:**
*column*=*value*: Any column in the database can be used as a filter.
*agregation_function=column*: Any available agregation function available in mysql can be used. Computed fields are automatically aliased as aggregation_column (e.g. sum_arrests)
*groupBy*: a comma separated list of fields to group by.
*limit*: the range of results to retrieve (e.g. 0, 100).

## Dashboard template

The **Dashboard template** contains the markup to render the dashboard. It has the root div and load the required css and js files.

## Build

Before commit changes you need to perform a build by running:

```bash
$ npm run build_dkan
```
