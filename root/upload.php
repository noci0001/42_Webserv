<?php
ini_set('upload_max_filesize', '20M');
ini_set('post_max_size', '25M');
ini_set('memory_limit', '32M');
// phpinfo();
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $uploadDirectory = './uploads/'; // Directory where you want to save the uploaded posters

    if (!file_exists($uploadDirectory)) {
        mkdir($uploadDirectory, 0777, true);
    }

    // Define the new file name (e.g., "custom_upload")
    $newFileName = "custom_upload";

    // Get the file extension from the original file name
    $fileExtension = pathinfo($_FILES['poster']['name'], PATHINFO_EXTENSION);

    // Create the new file path with the standard name and original extension
    $posterFile = $uploadDirectory . $newFileName . '.' . $fileExtension;

    if (move_uploaded_file($_FILES['poster']['tmp_name'], $posterFile)) {
        // Display the uploaded image on the HTML form
        echo '<!DOCTYPE html>';
        echo '<html lang="en">';
        echo '<head>';
        echo '<meta charset="UTF-8">';
        echo '<meta name="viewport" content="width=device-width, initial-scale=1.0">';
        echo '<link rel="stylesheet" href="style.css">';
        echo '<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Cormorant+Upright:wght@300;400;500;600;700&family=Open+Sans:wght@300;400;500;600;700&display=swap">';
        echo '<title>42 WEBFLIX: POST YOUR FAVORITE MOVIE</title>';
        echo '</head>';
        echo '<body onload="navigate_to_uploads()">';
        echo '</body>';
        echo '<script src="index.js"></script>';
        echo '</html>';
    } else {
        echo 'File upload failed.';
    }
    // header("Location: ./post_movie.html");
    exit();
}
?>