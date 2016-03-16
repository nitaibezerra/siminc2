<?php if($_REQUEST["polo"] == "N"){ ?>

	<iframe width="600" height="600" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" 
	src="http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll=<?php echo $_REQUEST["latitude"]; ?>,-<?php echo $_REQUEST["longitude"]; ?>&amp;spn=0.009417,0.013304&amp;z=16&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw">
	</iframe>
	
<?php }else { ?>

	<iframe width="600" height="600" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" 
	src="http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll=-<?php echo $_REQUEST["latitude"]; ?>,-<?php echo $_REQUEST["longitude"]; ?>&amp;spn=0.009417,0.013304&amp;z=16&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw">
	</iframe>
	
<?php } ?>
