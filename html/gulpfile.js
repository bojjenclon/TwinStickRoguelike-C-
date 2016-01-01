"use strict";

const gulp = require('gulp');
const cache = require('gulp-cached');
const remember = require('gulp-remember');

gulp.task('default', ['prepare-js', 'prepare-css']);

/* Tasks to convert code via babel and sass */

const babel = require('gulp-babel');
gulp.task('babel', () => {
  return gulp.src('es6/**/*.js')
    .pipe(cache('babel'))
    .pipe(babel({
      presets: ['es2015']
    }))
    .pipe(remember('babel'))
    .pipe(gulp.dest('compiled/js'));
});

const sass = require('gulp-sass');
gulp.task('sass', function () {
  return gulp.src('sass/**/*.scss')
    .pipe(cache('sass'))
    .pipe(sass().on('error', sass.logError))
    .pipe(remember('sass'))
    .pipe(gulp.dest('compiled/css'));
});

/* Tasks to minify js and css */

const uglify = require('gulp-uglify'); 
gulp.task('minify-js', function() {
  return gulp.src(['js/**/*.js', 'compiled/js/**/*.js', '!js/jscrollpane.min.js'])
    .pipe(cache('minify-js'))
    .pipe(uglify())
    .pipe(remember('minify-js'))
    .pipe(gulp.dest('dist/js/min'));
});

const minifyCss = require('gulp-minify-css');
gulp.task('minify-css', function() {
  return gulp.src(['compiled/css/*.css'])
    .pipe(cache('minify-css'))
    .pipe(minifyCss())
    .pipe(remember('minify-css'))
    .pipe(gulp.dest('dist/css/min'));
});

/* Tasks to concat related files */

const concat = require('gulp-concat');
const merge = require('merge-stream');
gulp.task('concat-js', function() {
  let InGameHud = 
    gulp.src(['dist/js/min/jquery-2.1.4.js', 'dist/js/min/InGameHud.js'])
      .pipe(concat('InGameHud.min.js', {newLine: ' '}))
      .pipe(gulp.dest('dist/js/min'));

  return merge(InGameHud);
});

gulp.task('concat-css', function() {
  let InGameHud = 
    gulp.src(['dist/css/min/InGameHud.css', 'dist/css/min/shared.css', 'dist/css/min/statBars.css'])
      .pipe(concat('InGameHud.min.css', {newLine: ' '}))
      .pipe(gulp.dest('dist/css/min'));

  return merge(InGameHud);
});

/* Convenience tasks to run multiple related tasks together */

const runSequence = require('run-sequence');
gulp.task('prepare-js', function() {
  runSequence('babel', 'minify-js', 'concat-js');
});

gulp.task('prepare-css', function() {
  runSequence('sass', 'minify-css', 'concat-css');
});

/* Watch tasks */

gulp.task('watch-js', function () {
  let babelWatcher = gulp.watch('es6/**/*.js', ['prepare-js']);  // watch the same files in our babel task
  babelWatcher.on('change', function (event) {
    if (event.type === 'deleted') {                         // if a file is deleted, forget about it
      delete cached.caches.scripts[event.path];             // gulp-cached remove api
      remember.forget('babel', event.path);                 // gulp-remember remove api

      delete cached.caches['minify-js'][event.path]; 
      remember.forget('minify-js', event.path);

      delete cached.caches['concat-js'][event.path]; 
      remember.forget('concat-js', event.path);
    }
  });
});

gulp.task('watch-css', function () {
  let babelWatcher = gulp.watch('sass/**/*.scss', ['prepare-css']);
  babelWatcher.on('change', function (event) {
    if (event.type === 'deleted') { 
      delete cached.caches['sass'][event.path]; 
      remember.forget('sass', event.path);

      delete cached.caches['minify-css'][event.path]; 
      remember.forget('minify-css', event.path);

      delete cached.caches['concat-css'][event.path]; 
      remember.forget('concat-css', event.path);
    }
  });
});

gulp.task('watch', ['watch-js', 'watch-css']);
