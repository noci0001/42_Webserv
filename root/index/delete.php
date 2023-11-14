<?php
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Directory path where the uploaded images are stored
    $directoryPath = './uploads/';

    if (is_dir($directoryPath)) {
        // Get an array of all files in the directory
        $files = scandir($directoryPath);

        // Iterate through the files and delete them
        foreach ($files as $file) {
            // Ignore "." and ".." entries and subdirectories
            if ($file != "." && $file != ".." && !is_dir($directoryPath . $file)) {
                // Delete the file
                unlink($directoryPath . $file);
            }
        }

        echo 'All files in the "uploads" directory have been deleted.';
    } else {
        echo 'Directory not found or inaccessible.';
    }
}
?>