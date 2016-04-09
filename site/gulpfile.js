var gulp = require('gulp');
var sass = require('gulp-sass');
var autoprefixer = require('gulp-autoprefixer');

var sassPaths = ['bower_components/foundation-sites/scss'];

gulp.task('sass', function () {
    return gulp.src('_scss/main.scss')
        .pipe(sass({ includePaths: sassPaths }).on('error', sass.logError))
        .pipe(autoprefixer({ browsers: ['last 2 versions', 'ie >= 9', 'and_chr >= 2.3'] }))
        .pipe(gulp.dest('css'));
});

gulp.task('default', ['sass']);
