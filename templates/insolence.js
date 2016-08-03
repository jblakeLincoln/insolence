var project_name = "";

var progress_counts = [ 0, 0, 0 ]
var progress_totals = [ 0, 0, 0 ]
var progress_reg = new RegExp("([^(]+)\((\d+(\.\d+)?)\/(\d+)\)");

var insolence_progress = document.getElementById("insolence_progress_bar");
var insolence_spinner = document.getElementById("insolence_spinner");
var insolence_tag = document.getElementById("insolence_progress_tag");
var insolence_canvas = document.getElementById('insolence_canvas');
var insolence_output = document.getElementById('insolence_output') || null;
var insolence_logo = document.getElementById("insolence_logo");
var insolence_loading = document.getElementById("insolence_loading_element");

var Module = {
	preRun: [],
	postRun: function()
	{
		insolence_progress.className += " fade_out";
		insolence_spinner.className += " fade_out";
		insolence_tag.className += " fade_out";
		insolence_logo.className += " fade_out";

		setTimeout(function() {
			insolence_loading.style.display = "none";
			insolence_canvas.style.display = "block";
		}, 600);
	},
	print: (function() {
		var e = document.getElementById("insolence_output") || null;
		if(e == null)
			return;
		e.value = '';
		return function(text) {
			if(arguments.length > 1)
				text = Array.prototype.slice.call(arguments).join(' ');
			e.value += text + "\u000A";
			e.scrollTop = insolence_output.scrollHeight;
		};
	})(),
	canvas: (function() {
		return insolence_canvas;
	})(),
	memoryInitializerRequest: function() {
		removeRunDependency('memory initializer');
	},
	totalDependencies: 0,
	setStatus: function(text) {
		var m;
		if(m = text.match(progress_reg))
			update_progress(2, parseInt(m[2]), parseInt(m[4]));
	}
};

function insolence_run(name) {
	if(typeof(name) !== "undefined")
		project_name = name;

	load_game_data_header();
	load_game_memory();
	load_game();
}

/* Get size of game data file.
 *
 * If the project does not use a data file, delete and replace with:
 *	update_progress(2, 1, 1);
 * */
function load_game_data_header() {
	var xhr_data = new XMLHttpRequest();
	xhr_data.open("HEAD", project_name + ".data", true);
	xhr_data.onreadystatechange = function() {
		if(this.readyState !== this.DONE)
			return;
		var val = parseInt(this.getResponseHeader("Content-Length"));
		update_progress(2, 0, val);
	}
	xhr_data.send(null);
};

function load_game_memory() {
	var memory_initialiser = project_name + ".js.mem";
	if(typeof Module['locateFile'] === 'function')
		memory_initialiser = Module['locateFile'](memory_initialiser);
	else if (Module['memoryInitializerPrefixURL'])
		memory_initialiser = Module['memoryInitializerPrefixURL'] + memory_initialiser;

	var xhr_memory = Module['memoryInitializerRequest'] = new XMLHttpRequest();
	xhr_memory.open('GET', memory_initialiser, true);
	xhr_memory.responseType = 'arraybuffer';
	xhr_memory.onprogress = function(e) {
		if(e.lengthComputable)
			update_progress(0, e.loaded, e.total);
	}
	xhr_memory.send(null);
};

function load_game() {
	var xhr_game = new XMLHttpRequest();
	xhr_game.responseType = "blob";
	xhr_game.open("GET", project_name + ".js", true);
	xhr_game.onload = function() {
		var script = document.createElement('script');
		var binaryData = [];
		binaryData.push(xhr_game.response);
		script.src = URL.createObjectURL(new Blob(binaryData, {type: "application/zip"}))

		document.body.appendChild(script);
		insolence_tag.textContent = "Loading game data ...";
	}
	xhr_game.onprogress = function(e) {
		if(e.lengthComputable)
			update_progress(1, e.loaded, e.total);
	}
	xhr_game.send(null);
};

/* Grab all XHRs and find out out far along we are. */
function update_progress(a, c, t)
{
	var count = 0,
		total = 0,
		tag_val,
		tag_max,
		i;

	progress_counts[a] = c;
	progress_totals[a] = t;

	for(i = 0; i < 2; ++i) {
		if(progress_totals[i] === 0)
			return;

		count += progress_counts[i];
		total += progress_totals[i];
	}

	insolence_progress.value = count / 1024;
	insolence_progress.max = total / 1024;

	if(count === total)
		return;

	tag_val = (count / 1024 / 1024).toFixed(2);
	tag_max = (total / 1024 / 1024).toFixed(2);
	insolence_tag.textContent = tag_val + "MB / " + tag_max + "MB";
}

window.onerror = function(e) {
	alert("Error: " + e);
	insolence_spinner.style.display = 'none';
};
