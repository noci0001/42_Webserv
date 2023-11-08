let exit = document.getElementById('this');
let overlay_exit = document.getElementById('that');
let movie_delete = document.getElementById('delete_movie');
let overlay = document.getElementById('show_menu_overlay');

let insert_movie_button = document.getElementById('insert_movie_button');
let insert_movie_show = document.getElementById('insert_movie_show');

let fav_movie_input = false;

let movie_has_been_posted = false;

exit.innerHTML = 'MENU';
exit.style.cursor = 'pointer';

//add event listener to exit button
exit.addEventListener('click', function(){
        overlay.style.display = 'block';
});

overlay_exit.addEventListener('click', function(){
    overlay.style.display = 'none';
});

if (insert_movie_button) {
    insert_movie_button.addEventListener('click', function(){
        if (fav_movie_input == false) {
            insert_movie_show.style.display = "block";
            fav_movie_input = true;
        } else {
            insert_movie_show.style.display = "none";
            fav_movie_input = false;
        }
    });
}

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
    if (movie_has_been_posted == true) {
        delete_movie.style.display = 'block';
    } else {
        delete_movie.style.display = 'none';
    }
}

function deleteMovie() {
    // Make an AJAX request to delete.php or another server-side script
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "./delete.php", true);

    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // Handle the response from the server, e.g., show a success message
            alert("Movie deleted successfully.");
            movie_has_been_posted = false;
            window.location.href = "./post_movie.html"; // Adjust the URL as needed
        }
    };

    xhr.send();
}

function navigate_to_uploads() {
    window.location.href = "./post_movie.html"; // Adjust the URL as needed
    movie_has_been_posted = true;
}