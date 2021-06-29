<?php
if (isset($_POST["name"]))
{
	setcookie("name", $_POST["name"], time() + 365*24*3600, null, null, false, true);
}
?>
<!DOCTYPE html>
<html>
	<head>
		<title>42 Webserv - Cookie processing</title>
		<meta charset="utf-8" name="Webserv" content="Webserv">
	</head>
	<body>
		<h2>$_COOKIE :</h2>
		<pre><?php var_dump($_COOKIE) ?></pre>
		<?php if (isset($_COOKIE["name"])) echo "<h2>\$_COOKIE[\"name\"] : ", $_COOKIE["name"], "</h2>"; ?>
	</body>
</html>
