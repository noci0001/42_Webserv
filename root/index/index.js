// app.use(express.json({limit: '50mb'}));
// app.use(express.urlencoded({limit: '50mb'}));

let exit = document.getElementById('this');
let overlay_exit = document.getElementById('that');
let movie_delete = document.getElementById('delete_movie');
let overlay = document.getElementById('show_menu_overlay');

let insert_movie_button = document.getElementById('insert_movie_button');
let insert_movie_show = document.getElementById('insert_movie_show');

let fav_movie_input = false;

let movie_has_been_posted = false;

movie_delete.style.display = 'none';

exit.innerHTML = 'MENU';
exit.style.cursor = 'pointer';

//add event listener to exit button
exit.addEventListener('click', function(){
        overlay.style.display = 'block';
});

overlay_exit.addEventListener('click', function(){
    overlay.style.display = 'none';
});

insert_movie_button.addEventListener('click', function(){
    if (fav_movie_input == false) {
        movie_delete.style.display = 'block';
        fav_movie_input = true;
    } else {
        movie_delete.style.display = "none";
        fav_movie_input = false;
    }
});

//if ESC is pressed, close the menu. Do not use keyCode as it is deprecated
document.addEventListener('keydown', function(event){
    if (event.key == 'Escape') {
        overlay.style.display = 'none';
    }
    if (event.key == 'Enter') {
        overlay.style.display = 'block';
    }
});

function check_if_movie_has_been_posted() {
    movie_delete.style.display = 'block';
}

// if (movie_has_been_posted == true) {
//     delete_movie.style.display = 'block';
// } else {
//     delete_movie.style.display = 'none';
// }

function deleteMovie() {
    // Make an AJAX request to delete.php or another server-side script
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/cig-bin/delete.py", true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // Handle the response from the server, e.g., show a success message
            alert("Movie deleted successfully.");
            window.location.href = "post_movie.html"; // Adjust the URL as needed
        }
    movie_delete.style.display = 'none';
    };

    xhr.send();
}

function navigate_to_uploads() {
    movie_has_been_posted = true;
    window.location.href = "post_movie.html"; // Adjust the URL as needed
}

