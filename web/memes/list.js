var loader;
var lister;
var error;
var result;
var memeList = {};
var ul;
var p;
var query;

//main page ready event handler
$(document).ready(function(e) {

    loader = $(this).find(".loader");
    lister = $(this).find(".lister");
    error = $(this).find(".error");
    result = $(this).find(".numResults");

    
    ul = lister.find("#memeList");
    p = result.find("#resultString");
    
    lister.on("dataReady", populateList);
    result.on("dataReady", displayNumResults);

    var vars = [], hash;
    var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
    for(var i = 0; i < hashes.length; i++)
    {
        hash = hashes[i].split('=');
        vars.push(hash[0]);
        vars[hash[0]] = hash[1];
    }
    query = vars["q"];
    console.log(query);
    
    //GET meme list
    if (query == null || query == "") {
        getMemeList("/memes", false);
    }
    else {
        getMemeList("/memes/search/" + query, true);
    }
    
});

//retrieve meme list from the server
function getMemeList (uri, search) {
    if (search === false) {
        $.ajax({
            url: uri,
            type: 'GET',
            success: function(data){ 
                memeList = data;
                lister.trigger("dataReady");
            },
            error: function(data) {
                displayError();
            }
        });
    }
    else {
        $.ajax({
            url: uri,
            type: 'GET',
            success: function(data){ 
                memeList = data;
                lister.trigger("dataReady");
                result.trigger("dataReady");
            },
            error: function(data) {
                displayError();
            }
        });
    }
}

//populate meme list
function populateList () {
    $.each(memeList, function(idx, id){
        ul.append('<li><a target="_blank" href="/memes/view/' + id + '">View Meme</a><br><a target="_blank" href="/memes/manage/' + id + '">Manage Meme</a><br><iframe id="frame" src="/memes/view/' + id + '"/></li>')
    });
    loader.addClass('hide');
    lister.removeClass('hide');
}

//method to display error to the user
function displayError () {
    loader.addClass('hide');
    error.removeClass('hide');
}

function displayNumResults() {
    var num_entries = 0;
    $.each(memeList, function(idx, id){
        num_entries += 1;
    });
    // p.text(num_entries);
    if (num_entries > 0) {
       p.text("Found " + num_entries + " results for " + query + "."); 
    }
    else {
       p.text("No results found for " + query + ".");
    }
    result.removeClass('hide');
}
