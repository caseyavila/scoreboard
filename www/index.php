<html>
    <head>
        <title>ΣΦΔ</title>
    </head>
    <body>
        <h1>ΣΦΔ - ΒΝ</h1>
    	<form action="index.php" method="post">
             <input type="submit" name="reset" value="Reset"/>
        </form>
        <h2>Red</h2>
    	<form action="index.php" method="post">
             <input type="submit" name="radd" value="+1 Point"/>
        </form>
    	<form action="index.php" method="post">
             <input type="submit" name="rsub" value="-1 Point"/>
        </form>
        <h2>Gold</h2>
    	<form action="index.php" method="post">
             <input type="submit" name="gadd" value="+1 Point"/>
        </form>
    	<form action="index.php" method="post">
             <input type="submit" name="gsub" value="-1 Point"/>
        </form>
    </body>
</html>

<?php
    $input = fopen("/run/dye.stdin", "w");

    if ($_SERVER['REQUEST_METHOD'] == "POST") {
        if (isset($_POST['reset'])) {
            fwrite($input, "reset\n");
        } else if (isset($_POST['radd'])) {
            fwrite($input, "radd\n");
        } else if (isset($_POST['gadd'])) {
            fwrite($input, "gadd\n");
        } else if (isset($_POST['rsub'])) {
            fwrite($input, "rsub\n");
        } else if (isset($_POST['gsub'])) {
            fwrite($input, "gsub\n");
        }
    }

    fclose($input);
?>
