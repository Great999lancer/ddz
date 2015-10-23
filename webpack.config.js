'use strict';

var _ = require('lodash');
var webpack = require('webpack');

var argv = require('minimist')(process.argv.slice(2));

var DEBUG = !argv.release;

var GLOBALS = {
    'process.env.NODE_ENV': DEBUG ? '"development"' : '"production"',
    '__DEV__': DEBUG
};


var strip = DEBUG?
{
    test: /nothing/,
    exclude: /node_modules/,
    loader: 'strip-loader?strip[]=console.log'
}
    :{
    test: /\.js$/,
    exclude: /node_modules/,
    loader: 'strip-loader?strip[]=console.log'
};

var config = {
    output: {
        path: './build/',
        publicPath: './',
        sourcePrefix: '  '
    },

    cache: DEBUG,
    debug: DEBUG,
    devtool: false,

    stats: {
        colors: true,
        reasons: DEBUG
    },

    plugins: [
        new webpack.optimize.OccurenceOrderPlugin()

    ],

    resolve: {
        extensions: ['', '.webpack.js', '.web.js', '.js', '.jsx']
    },
    module: {
        preLoaders: [
            {
                test: /\.js$/,
                exclude: /node_modules/,
                loader: 'eslint-loader'
            }
        ],

        loaders: [
            {
                test: /\.gif/,
                loader: 'url-loader?limit=10000&mimetype=image/gif'
            },
            {
                test: /\.jpg/,
                loader: 'url-loader?limit=10000&mimetype=image/jpg'
            },
            {
                test: /\.png/,
                loader: 'url-loader?limit=10000&mimetype=image/png'
            },
            {
                test: /\.svg/,
                loader: 'url-loader?limit=10000&mimetype=image/svg+xml'
            },
            strip,
            {
                test: /\.jsx?$/,
                exclude: /node_modules/,
                loader: 'babel-loader?stage=0'
            }
        ]
    }
};

//
// Configuration for the client-side bundle (app.js)
// -----------------------------------------------------------------------------

var appConfig = _.merge({}, config, {
    entry: {
        app:'./app/jsx/index.jsx'
        //vendor:['react','react-router','react-proxy-loader','flux','react/lib/Object.assign','react/lib/ReactCSSTransitionGroup','react/lib/ReactComponentWithPureRenderMixin']
    },
    output: {
        filename: "bundle.js"
    },
    plugins: config.plugins.concat([
            new webpack.DefinePlugin(_.merge(GLOBALS, {'__SERVER__': false})),
            /*
            new webpack.optimize.CommonsChunkPlugin({
                name:"vendor",
                minChunks: Infinity
            })
            */
        ].concat(DEBUG ? [] : [
                new webpack.optimize.DedupePlugin(),
                new webpack.optimize.UglifyJsPlugin()
            ])
    ),
    resolve: {
        extensions: ["", ".js", ".scss"],
        modulesDirectories: ["src", "node_modules"]
    }
});

module.exports = [appConfig]
