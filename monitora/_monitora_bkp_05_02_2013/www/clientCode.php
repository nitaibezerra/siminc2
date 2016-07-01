<?php
	include("upload.class.php");
	//
	$maxSize=1024*1000;//the max file size for images in bytes.
	$u=new uploader($maxSize, "");
	$imageName=$u->upload("file");
?>