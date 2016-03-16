<?php 
header( 'Content-type: '. $_REQUEST['arqtipo'] );
header( 'Content-Disposition: attachment; filename='.$_REQUEST['filename']);
$url = str_replace(Array('{','}'),'',$_REQUEST['url']);
readfile( $url );
?>
<script>
window.close();
</script>
