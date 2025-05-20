<html>
    <head>
        <title>ΣΦΔ</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
        <iframe name="hack" style="display:none;"></iframe>
        <h1>ΣΦΔ - ΒΝ</h1>
    	<form action="index.php" method="post" target="hack">
             <input type="submit" name="reset" value="Reset"/>
        </form>
        <h2>Red</h2>
    	<form action="index.php" method="post" target="hack">
             <input type="submit" name="rsub" value="-1 Point" class="red"/>
             <input type="submit" name="radd" value="+1 Point" class="red"/>
        </form>
        <h2>Gold</h2>
    	<form action="index.php" method="post" target="hack">
             <input type="submit" name="gsub" value="-1 Point" class="gold"/>
             <input type="submit" name="gadd" value="+1 Point" class="gold"/>
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
