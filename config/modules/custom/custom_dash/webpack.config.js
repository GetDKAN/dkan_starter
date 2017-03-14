var path = require('path');
var webpack = require('webpack');
var ExtractTextPlugin = require("extract-text-webpack-plugin");
var fs = require('fs');

var nodeModules = {};
fs.readdirSync('node_modules')
.filter(function(x) {
  return ['.bin'].indexOf(x) === -1;
})
.forEach(function(mod) {
  nodeModules[mod] = 'commonjs ' + mod;
});


module.exports = {
  entry: [
    './js/datahandlers.js',
    './js/stateHandlers.js',
  ],
  output: {
    path: path.join(__dirname, 'js'),
    filename: 'customDash.js',
    libraryTarget: 'umd'
  },
  plugins: [
    new webpack.optimize.DedupePlugin(),
    new webpack.optimize.OccurenceOrderPlugin(),
    new webpack.optimize.UglifyJsPlugin({
      compressor: {
        warnings: false
      },
      output: { comments: false },
      mangle: true
    }),
  ],
  module: {
    loaders: [
      {
        test: /\.js$/,
        loaders: ['babel-loader'],
        exclude: ['customDash.js'],
        include: [
          path.join(__dirname, 'js'),
        ]
      }
    ]
  },
  externals: nodeModules,
};