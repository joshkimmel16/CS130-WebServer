var loader;
var lister;
var error;
var memeList = {};
var ul;

//main page ready event handler
$(document).ready(function(e) {

    loader = $(this).find(".loader");
    lister = $(this).find(".lister");
    error = $(this).find(".error");
    
    ul = lister.find("#memeList");
    
    lister.on("dataReady", populateList);
    
    //GET meme list
    getMemeList("/memes");
    
});

//retrieve meme list from the server
function getMemeList (uri) {
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

//populate meme list
function populateList () {
    $.each(memeList, function(idx, id){
        ul.append('<li><a target="_blank" href="/memes/view/' + id + '">' + id +'</a><br><iframe id="frame" src="/memes/view/' + id + '"/></li>')
    });
    loader.addClass('hide');
    lister.removeClass('hide');
}

//method to display error to the user
function displayError () {
    loader.addClass('hide');
    error.removeClass('hide');
}