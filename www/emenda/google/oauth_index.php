<?php 
/**
* This script will be the first to be initiated
* It will call Google
*/

require_once('oauth2_config.php');

$accessTokenUri = AUTHORIZATION_ENDPOINT
."?client_id=".CLIENT_ID
."&redirect_uri=".CALLBACK_URI
//."&scope=https://www.google.com/m8/feeds/"
."&response_type=code";

// Redirect user to Google to get access token
header("Location:".$accessTokenUri);
exit();
?>