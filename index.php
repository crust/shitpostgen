<head>
<title>Shitpost Generator</title>
<meta property="og:image" content="http://gash.ca/guys/header.png" />
<meta property="og:title" content="Shitpost Generator" />
<meta property="og:url" content="http://gash.ca/guys/" />
<meta property="og:description" content="Streamlining dead memes since 2016. Refresh the page to become a professional shitposter." />
<meta property="og:site_name" content="Chris Bush's Cool Website" />
</head>
<body style="width:100%;height:100%" >
<?php

$path = exec("../../guys/make_image");

?>
<center>
<h1><a href="?browserHistoryToken=<?php echo substr(md5(microtime()), 0, 6); ?>" >Roll again</a></h1>
<a href="<?php echo $path; ?>" ><img style="max-height:90%;width:auto" src="<?php echo $path; ?>" /></a><br/>
</center>
