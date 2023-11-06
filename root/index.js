
let exit = document.getElementById('this');
let overlay_exit = document.getElementById('that');
let movie_delete = document.getElementById('delete_movie');
let overlay = document.getElementById('show_menu_overlay');

let insert_movie_button = document.getElementById('insert_movie_button');
let insert_movie_show = document.getElementById('insert_movie_show');

let fav_movie_input = false;

exit.innerHTML = 'MENU';

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
