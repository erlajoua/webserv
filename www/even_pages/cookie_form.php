<!DOCTYPE html>
<html>
	<head>
		<title>42 Webserv - Cookie form</title>
		<meta charset="utf-8" name="Webserv" content="Webserv">
	</head>
	<body>
		<h1>Cookie form <?php
if (isset($_COOKIE["name"])) {
	echo "(current value : " . $_COOKIE["name"] . ")";
}
else {
	echo "(no current value)";
}
?></h1>
		<form action="/even_pages/cookie_processing.php" method="post">
			<label for="name">Name</label>
			<input type="name" name="name" id="name" required autofocus>
			<input type="submit" value="Submit">
		</form>
	</body>
</html>
