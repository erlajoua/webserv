<?php
if (isset($_POST["name"])) {
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
		<?php
		if (isset($_POST["name"])) {
			echo "<p>Cookie \"name\" set to \"" . $_POST["name"] . "\".</p>";
		}
		else if (isset($_COOKIE["name"])) {
			echo "<p>Cookie \"name\" is \"" . $_COOKIE["name"] . "\".</p>";
		}
		else {
			echo "<p>No cookie \"name\" yet.";
		}
		?>
		<p>Link to change the "name" cookie : <a href="./cookie_form.php">Cookie form</a></p>
	</body>
</html>
