"use strict";

const gulp = require('gulp');
const cache = require('gulp-cached');
const remember = require('gulp-remember');

gulp.task('default', ['prepare-js', 'prepare-css']);

const babel = require('gulp-babel');
gulp.task('babel', () => {
  return gulp.src('es6/*.js')
    .pipe(cache('babel'))
    .pipe(babel({
      presets: ['es2015']
    }))
    .pipe(remember('babel'))
    .pipe(gulp.dest('js'));
});

const sass = require('gulp-sass');
gulp.task('sass', function () {
  return gulp.src('./sass/**/*.scss')
    .pipe(cache('sass'))
    .pipe(sass().on('error', sass.logError))
    .pipe(remember('sass'))
    .pipe(gulp.dest('dist/css'));
});

const uglify = require('gulp-uglify'); 
gulp.task('minify-js', function() {
  return gulp.src(['js/*.js', '!js/jscrollpane.min.js'])
    .pipe(cache('minify-js'))
    .pipe(uglify())
    .pipe(remember('minify-js'))
    .pipe(gulp.dest('dist/js'));
});

const minifyCss = require('gulp-minify-css');
gulp.task('minify-css', function() {
  return gulp.src(['sass/InGameHud.css', 'css/shared.css', 'css/statBars.css'])
    .pipe(cache('minify-css'))
    .pipe(minifyCss())
    .pipe(remember('minify-css'))
    .pipe(gulp.dest('dist/css'));
});

const concat = require('gulp-concat');
const merge = require('merge-stream');
gulp.task('concat-js', function() {
  let InGameHud = 
    gulp.src(['dist/js/jquery-2.1.4.js', 'dist/js/InGameHud.js'])
      .pipe(concat('InGameHud.min.js', {newLine: ' '}))
      .pipe(gulp.dest('dist/js'));

  return merge(InGameHud);
});

gulp.task('concat-css', function() {
  let InGameHud = 
    gulp.src(['dist/css/InGameHud.css', 'dist/css/shared.css', 'dist/css/statBars.css'])
      .pipe(concat('InGameHud.min.css', {newLine: ' '}))
      .pipe(gulp.dest('dist/css'));

  return merge(InGameHud);
});

const runSequence = require('run-sequence');
gulp.task('prepare-js', function() {
  runSequence('babel', 'minify-js', 'concat-js');
});

gulp.task('prepare-css', function() {
  runSequence('sass', 'minify-css', 'concat-css');
});
