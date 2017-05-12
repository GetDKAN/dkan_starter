/* eslint-disable no-var */
var webpack = require('webpack');
var path = require('path');
var ExtractTextPlugin = require('extract-text-webpack-plugin');
var MODE = process.env.MODE;
var plugins = [new ExtractTextPlugin('dkan_dash.min.css')];
var devtool = (MODE === 'production') ? 'source-map' : 'eval';

if(MODE === 'production') {
  plugins = plugins.concat([
    new webpack.optimize.OccurenceOrderPlugin(),
    new webpack.optimize.UglifyJsPlugin({
      compressor: {
        warnings: false
      },
      output: {comments: false},
      mangle: true
    }),
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: JSON.stringify('production')
      }
    }),
  ]);
}

module.exports = {
  devtool: devtool,
  entry: ['whatwg-fetch','./src/index'],
  output: {
    path: path.join(__dirname, '..' ,'dist'),
    filename: 'dkan_dash.min.js'
  },
  plugins: plugins,
  module: {
    loaders: [
      {
        test: /\.js?$/,
        loaders: ['babel'],
        include: path.join(__dirname, 'src')
      },
      { test: /\.css$/, loader: ExtractTextPlugin.extract('css-loader') },
      { test: /\.(ttf|eot|svg|woff(2)?)(\?[a-z0-9]+)?$/, loader: 'file-loader' },
      { test: /\.scss$/, loader: ExtractTextPlugin.extract('css-loader!sass-loader')  }
    ]
  }
};