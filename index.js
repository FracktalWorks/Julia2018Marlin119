const chalk = require('chalk');
const path = require('path');
const readlineSync = require('readline-sync');
const fs = require('fs-extra');
const exec = require('child_process').exec;
const chokidar = require('chokidar');
const moment = require('moment');


function i(s) { console.log(s); }
function e(s) { console.log( chalk.red(s) ); }
function s(s) { console.log( chalk.green(s) ); }
function m(s) { console.log( chalk.gray(s) ); }
function p() { readlineSync.keyInPause("Press any key to exit."); }
function fe(d) { return fs.existsSync(d); }
function mkdirne(d) { if ( !fe(d) ) { fs.mkdirSync(d); } }
function cdir(d) { fs.emptyDirSync(d); }

const  pwd = __dirname;
const dir_src = path.join( pwd, "src" );
const dir_intermediate = path.join( pwd, "intermediate" );
const dir_output = path.join( pwd, "output" );
const dir_src_common = path.join( dir_src, "common" );
const dir_src_marlin = path.join( dir_src, "Marlin", "Marlin" );
const variants = ["Julia18_GLCD", "Julia18_GLCD_HB", "Julia18_RPI", "Julia18_RPI_E"];

s( "Fracktal Works Marlin Build Automation" );
s( "PWD: " + pwd );

// Check for Marlin or Exit
if ( !fs.existsSync(dir_src_marlin) ) {
    e( "Marlin not found." );
    p();
    return;
}

// Check for variant sources
for (let i = 0; i < variants.length; i++) {
    temp = path.join(dir_src, variants[i]);
	
    if( !fe(temp) ) {
        e( "Variant source not found: " + variants[i] );
        p();
        return;
    }
}

// Make intermediate dir if not exists
mkdirne( dir_intermediate );

// Make output dir if not exists
mkdirne( dir_output );

// Show variant choice
const build_option = readlineSync.keyInSelect(variants, "Build variant");
if (build_option == -1) {
    m( "Quitting" );
	p();
    return;
}


// Generate variant path
const build_variant = variants[build_option];
const dir_src_variant = path.join(dir_src, build_variant);
const dir_intermediate_variant = path.join(dir_intermediate, build_variant);
const file_ino = build_variant + ".ino";
const path_ino = path.join(dir_intermediate_variant, file_ino);
const path_hex = path_ino + ".mega.hex";

m(dir_src_variant);
m(dir_intermediate_variant);
m(path_ino);
m(path_hex);


// Clear intermediate dir
cdir( dir_intermediate );

// Make intermediate dir for variant
mkdirne( dir_intermediate_variant );

// Copy Marlin files to intermediate
fs.copySync( dir_src_marlin, dir_intermediate_variant, { "overwrite": true } );

// Copy common files to intermediate, if exists
if ( fe (dir_src_common) ) {
	fs.copySync( dir_src_common, dir_intermediate_variant, { "overwrite": true } );
}

// Rename Marlin.ino to build_variant
fs.renameSync( path.join(dir_intermediate_variant, "Marlin.ino"), path_ino);

// Copy variant files to intermediate
fs.copySync( dir_src_variant, dir_intermediate_variant, { "overwrite": true } );

// Open ino file
exec( path_ino );
s( "Opening ino file" );

// Watch src variant dir for changes
s( "Watching common and variant dir.." );

function copyData(srcPath, destPath) {
    fs.readFile(srcPath, function (err, data) {
		if (err) throw err;
		//Do your processing, MD5, send a satellite to the moon, etc.
		fs.writeFile (destPath, data, function(err) {
			if (err) throw err;
			m(' Overwritten: ' + destPath );
		});
	});
}

function onFileEvent(filePath) {
	try {
		m(" " + moment().format("DD-MM-YYYY HH:mm:ss") + " Changed: " + filePath);
		
		if ( filePath.indexOf(".hex") < 0 ) {
			// fs.copySync( filePath, dir_intermediate_variant, { "overwrite": true } );
			copyData( filePath, path.join(dir_intermediate_variant, path.basename( filePath )) );
		} else {
			const path_hex_dest = path.join( dir_output, build_variant + "_mega_" + moment().format("DDMMYYYY_HHmmss") + ".hex");
			fs.copySync( filePath, path_hex_dest, { "overwrite": true } );
		}
	} catch(ex) {
		e(ex);
	}
}

const watcher = chokidar.watch( [dir_src_variant, dir_src_common, path_hex] , { "persistent": true, "ignoreInitial": true } );
watcher.on('add', onFileEvent);
watcher.on('change', onFileEvent);
// watcher.on('unlink', onFileEvent);


function exitHandler() {
	process.stdin.resume();//so the program will not close instantly
	
	if (watcher)
		watcher.close();
    process.exit();
}

//do something when app is closing
process.on('exit', exitHandler.bind(null));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null));

// catches "kill pid" (for example: nodemon restart)
process.on('SIGUSR1', exitHandler.bind(null));
process.on('SIGUSR2', exitHandler.bind(null));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null));