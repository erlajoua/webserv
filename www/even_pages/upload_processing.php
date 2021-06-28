<!DOCTYPE html>
<html>
  <head>
    <title>42 Webserv</title>
    <meta charset="utf-8" name="Webserv" content="Webserv">
    <link rel="stylesheet" href="../style.css">
  </head>
  <body>
    <?php
    $uploaddir = './upload/';
    $uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
    if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
        echo '<h1>SUCCESS</h1><p>The file has been uploaded!</p>';
    } else {
        echo '<h1 style="color:red";>FAILURE</h1><p>The file has NOT been uploaded...</p>';
    }
    ?>
  </body>
</html>
