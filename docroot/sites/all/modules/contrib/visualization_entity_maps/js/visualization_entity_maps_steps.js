this.recline = this.recline || {};
this.recline.View = this.recline.View || {};


;(function ($, my, global) {
  'use strict';

  /**
  * Load data view
  */
  global.LoadDataView = Backbone.View.extend({
    template: '<div class="form-group">' +
    '<label for="control-map-source">Source</label>' +
    '<input value="{{source.url}}" type="text" id="control-map-source" class="form-control" />' +
    '</div>' +
    '<div class="form-group">' +
    '<select id="control-map-backend" class="form-control">' +
    '<option value="csv">CSV</option>' +
    '<option value="gdocs">Google Spreadsheet</option>' +
    '<option value="ckan">DataProxy</option>' +
    '</select>' +
    '</div>' +
    '<div id="controls">' +
    '<div id="next" class="btn btn-primary pull-right">Next</div>' +
    '</div>',
    events: {
      'change #control-map-source': 'changeSource',
      'change #control-map-backend': 'changeBackend',
    },
    initialize: function(options) {
      var self = this;
      self.options = _.defaults(options || {}, self.options);
      self.state = self.options.state;
      self.model = self.options.model;
      self.stepInfo = {
        title: 'Load Data',
        name: 'loadData'
      };
    },
    render: function() {
      var self = this;
      self.$el.html(Mustache.render(self.template, self.state.toJSON()));
    },
    changeSource: function(){
      this.updateField({fieldName: 'url', id: 'control-map-source'});
    },
    changeBackend: function(){
      this.updateField({fieldName: 'backend', id: 'control-map-backend'});
    },
    updateField: function(options){
      var self = this;
      var source = {};
      source[options.fieldName] = self.$('#' + options.id).val();
      self.state.set('source', _.extend(self.state.get('source') || {}, source));
      self.state.trigger('change');
    },
    updateState: function(state, cb) {
      var self = this;
      var url = self.$('#control-map-source').val();
      var backend = self.$('#control-map-backend').val();
      var source = {
        backend: backend,
        url: url
      };
      state.set('model', new recline.Model.Dataset(source));
      state.set('source', source);
      $('<div class="alert alert-info loader">Loading <span class="spin"></span></div>').insertAfter('#steps');
      state.get('model').fetch().done(function(){
        $('.loader').empty().hide();
        cb(state);
      });
    }
  });

  /**
  * Map settings view
  */
  global.MapSettingsView = Backbone.View.extend({
    template: '<div class="form-group">' +
    '<p><input type="radio" id="type-geopoint" name="control-map-type" value="geopoint" {{#sourceGeopoint}}checked{{/sourceGeopoint}}>' +
    '<label for="type-geopoint">Geo Point field</label></p>' +
    '<p><input type="radio" id="type-latlon" name="control-map-type" value="latlon" {{^sourceGeopoint}}checked{{/sourceGeopoint}}>' +
    '<label for="type-geopoint">Latitude and Longitude fields</label></p></div>' +
    '<div class="form-group form-group-latlon {{#sourceGeopoint}}form-group-hidden{{/sourceGeopoint}}">' +
      '<label for="control-map-latfield">Latitude Field</label>' +
      '<select id="control-map-latfield" class="form-control">' +
      '{{#fields}}' +
        '<option value="{{value}}" {{#latSelected}}selected{{/latSelected}}>{{name}}</option>' +
      '{{/fields}}' +
      '</select>' +
      '<label for="control-map-lonfield">Longitude Field</label>' +
      '<select id="control-map-lonfield" class="form-control">' +
      '{{#fields}}' +
      '<option value="{{value}}" {{#lonSelected}}selected{{/lonSelected}}>{{name}}</option>' +
      '{{/fields}}' +
      '</select>' +
    '</div>' +
    '<div class="form-group form-group-geopoint {{^sourceGeopoint}}form-group-hidden{{/sourceGeopoint}}"">' +
      '<label for="control-map-geopoint">Geopoint Field</label>' +
      '<select id="control-map-geopoint" class="form-control">' +
      '{{#fields}}' +
        '<option value="{{value}}" {{#geomSelected}}selected{{/geomSelected}}>{{name}}</option>' +
      '{{/fields}}' +
      '</select>' +
    '</div>' +
    '<div class="form-group">' +
      '<label for="control-map-tooltipfield">Tooltip fields</label>' +
      '<select id="control-map-tooltipfield" class="form-control chosen-select form-select" multiple>' +
      '{{#fields}}' +
        '<option value="{{value}}" {{#tooltipSelected}}selected{{/tooltipSelected}}>{{name}}</option>' +
      '{{/fields}}' +
      '</select>' +
    '</div>' +
    '<div class="form-group">' +
      '<input type="checkbox" id="control-map-cluster" value="{{cluster}}" {{#cluster}}checked{{/cluster}}>' +
      '<label for="control-map-cluster">Enable clustering</label>' +
    '</div>' +
    '<div class="form-group">' +
      '<input type="checkbox" id="control-map-show-title" value="1" {{#showTitle}}checked{{/showTitle}}>' +
      '<label for="control-map-show-title">Show title</label>' +
    '</div>' +
    '<div id="controls">' +
      '<div id="prev" class="btn btn-default pull-left">Back</div>' +
      '<button type="submit" class="form-submit btn btn-success pull-right">Finish</button>' +
    '</div>',
    events: {
      'change [name="control-map-type"]': 'toggleDepFields',
      'change #control-map-latfield': 'changeLatitude',
      'change #control-map-lonfield': 'changeLongitude',
      'change #control-map-geopoint': 'changeGeopoint',
      'change #control-map-cluster': 'changeCluster',
      'change #control-map-show-title': 'changeTitle',
      'change #control-map-type': 'changeMapType',
      'change #control-map-tooltipfield': 'changeTooltip',
    },
    initialize: function(options) {
      var self = this;
      self.options = _.defaults(options || {}, self.options);
      self.state = self.options.state;
      self.model = self.options.model;
      self.stepInfo = {
        title: 'Map Settings',
        name: 'mapSettings'
      };
    },
    changeLongitude: function(){
      this.updateField({fieldName: 'lonField', id: 'control-map-lonfield'});
    },
    changeLatitude: function(){
      this.updateField({fieldName: 'latField', id: 'control-map-latfield'});
    },
    changeGeopoint: function(){
      this.updateField({fieldName: 'geomField', id: 'control-map-geopoint'});
    },
    changeTooltip: function(){
      this.updateField({fieldName: 'tooltipField', id: 'control-map-tooltipfield'});
    },
    changeCluster: function(){
      var self = this;
      var mapState = {};
      mapState.cluster = self.$('#control-map-cluster').prop('checked');
      self.state.set('mapState', _.extend(self.state.get('mapState') || {}, mapState));
      self.state.trigger('change');
    },
    changeTitle: function(){
      var self = this;
      var mapState = {};
      mapState.showTitle = self.$('#control-map-show-title').prop('checked');
      self.state.set('mapState', _.extend(self.state.get('mapState') || {}, mapState));
      self.state.trigger('change');
    },
    updateField: function(options){
      var self = this;
      var mapState = {};
      mapState[options.fieldName] = self.$('#' + options.id).val();
      self.state.set('mapState', _.extend(self.state.get('mapState') || {}, mapState));
      self.state.trigger('change');
    },
    render: function() {
      var self = this;

      var mapForm = {
        geomField: null,
        latField: null,
        lonField: null,
        cluster: false,
        tooltipField: null,
        showTitle: true
      }
      var mapState = self.state.get('mapState');
      if (mapState) {
        mapForm = _.extend(mapForm, mapState);
      }

      var fields = [];

      self.state.get('model')
        .fields
        .each(function(field) {
          fields.push({
            value: field.id,
            name: field.id,
            latSelected: field.id === mapForm.latField,
            lonSelected: field.id === mapForm.lonField,
            geomSelected: field.id === mapForm.geomField,
            tooltipSelected: _.contains(mapForm.tooltipField, field.id),
          });
        });
      mapForm.fields = fields;
      mapForm.sourceGeopoint = mapForm.geomField || !mapState;

      self.$el.html(Mustache.render(self.template, mapForm));
      self.$('.chosen-select').chosen();
    },
    updateState: function(state, cb) {
      var self = this;
      var mapState = {
        lonField: null,
        latField: null,
        geomField: null,
        tooltipField: null,
        cluster: null,
        showTitle: true
      };
      var sourceType = null;
      if(self.$('#type-geopoint').prop('checked')) {
        sourceType = self.$('#type-geopoint').val();
      } else if(self.$('#type-latlon').prop('checked')) {
        sourceType = self.$('#type-latlon').val();
      }
      mapState.cluster = self.$('#control-map-cluster').prop('checked');
      mapState.showTitle = self.$('#control-map-show-title').prop('checked');
      if (sourceType == 'geopoint') {
        mapState.geomField = self.$('#control-map-geopoint').val();
      } else if (sourceType == 'latlon') {
        mapState.lonField = self.$('#control-map-lonfield').val();
        mapState.latField = self.$('#control-map-latfield').val();
      }
      mapState.tooltipField = self.$('#control-map-tooltipfield').val();

      state.set('mapState', mapState);
      cb(state);
      $('#eck-entity-form-add-visualization-ve-map').submit();
    },
    toggleDepFields: function(e) {

      var self = this;
      var mapState = {};
      mapState.type = e.target.value;
      self.state.set('mapState', _.extend(self.state.get('mapState') || {}, mapState));
      self.state.trigger('change');
      if (e.target.value == 'geopoint') {
        $('.form-group-latlon').addClass('form-group-hidden');
        $('.form-group-geopoint').removeClass('form-group-hidden');
      } else if(e.target.value == 'latlon') {
        $('.form-group-latlon').removeClass('form-group-hidden');
        $('.form-group-geopoint').addClass('form-group-hidden');
      }
    },
  });

  /**
  * Multi stage view
  */
  global.MultiStageView = Backbone.View.extend({
    template: '<h3>{{title}}</h3>' +
    '<input type="hidden" value="{{state}}"/>' +
    '<div id="step"></div>',
    events: {
      'click #next': 'nextStep',
      'click #prev': 'prevStep'
    },
    initialize: function(options) {
      var self = this;
      self.options = _.defaults(options || {}, self.options);
      self.state = self.options.state;
      self.currentView = null;
      self.currentStep = self.state.get('step') || 0;
      self.steps = [];

      self.state.set('step', self.currentStep);
    },
    render: function() {
      var self = this;
      self.currentView = self.getStep(self.currentStep);
      _.extend(self.currentView.stepInfo, {state: JSON.stringify(self.state.toJSON())});
      self.$el.html(Mustache.render(self.template, self.currentView.stepInfo));

      self.assign(self.currentView, '#step');
      return self;
    },
    assign: function(view, selector) {
      var self = this;
      view.setElement(self.$(selector)).render();
    },
    addStep: function(view) {
      var self = this;
      self.steps.push(view);
    },
    getStep: function(index) {
      var self = this;
      return self.steps[index];
    },
    nextStep: function() {
      var self = this;
      var toNext = self.updateStep(self.getNext(self.steps, self.currentStep));
      self.currentView.updateState(self.state, toNext);
    },
    prevStep: function() {
      var self = this;
      var toPrev = self.updateStep(self.getPrev(self.steps, self.currentStep));
      self.currentView.updateState(self.state, toPrev);
    },
    getNext: function(steps, current) {
      var limit = steps.length - 1;
      if(limit === current){
        return current;
      }
      return ++current;
    },
    getPrev: function(steps, current) {
      if(current) {
        return --current;
      }
      return current;
    },
    updateStep: function(n) {
      var self = this;
      return function(state) {
        self.state = state;
        self.gotoStep(n);
        self.trigger('multistep:change', {step:n});
      };
    },
    gotoStep: function(n) {
      var self = this;
      self.currentStep = n;
      self.state.set('step', self.currentStep);
      self.render();
    }
  });

})(jQuery, recline.View, window);
