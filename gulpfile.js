var gulp = require('gulp');
var source = require('vinyl-source-stream');
var browserify = require('browserify');
var reactify = require('reactify');
var less = require('gulp-less');
var livereload = require('gulp-livereload');
var $ = require('gulp-load-plugins')();
var webpack = require('webpack');
gulp.task('move', function() {
    gulp
        .src(['./app/index.html', './app/shim.js', './app/css/normalize.css', 'package.json'])
        .pipe(gulp.dest('build'))
});

gulp.task('less', function() {
    return gulp.src('./app/less/main.less')
        .pipe(less())
        .on('error', function(err){console.log('less error',err)})
        .pipe(gulp.dest('build'));
});


gulp.task('bundle', function(cb) {
    var started = false;
    var config = require('./webpack.config.js');
    var bundler = webpack(config);
    var watch = true;
    function bundle(err, stats) {
        if (err) {
            throw new $.util.PluginError('webpack', err);
        }
        //男人就需要看看eslint的错误提示
        //if (argv.verbose) {
        $.util.log('[webpack]', stats.toString({colors: true}));
        //}

        if (!started) {
            started = true;
            return cb();
        }
    }

    if (watch) {
        bundler.watch(200, bundle);
    } else {
        bundler.run(bundle);
    }
});

gulp.task('browserify', function () {
    return browserify('./app/jsx/index.jsx')
        .transform(reactify)
        .bundle()
        .on('error', function(err){console.log('browserify error', err)})
        .pipe(source('bundle.js'))
        .pipe(gulp.dest('build'))
});

gulp.task('watch', function() {
    livereload.listen({ basePath: 'build' });
    gulp.watch(['./app/jsx/components/*.jsx','./app/jsx/**'], ['bundle']);
    gulp.watch(['./app/less/**'], ['less']);
    gulp.watch(['./app/index.html','package.json'], ['move']);
});

gulp.task('default', ['move', 'less', 'bundle', 'watch']);