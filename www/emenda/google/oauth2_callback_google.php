<?php 

/**
* This is the google callback script.
* After user approves / decline with the Google application,
* Google will redirect to this script (as define in Google APIs settings)
*/

require_once('oauth2_config.php');

$code = $_GET['code'];
//$error = $_GET['error'];

//if (isset($error)) {
//	exit();
//}

if (!isset($code)) {
	exit();
}

//construct POST object for access token fetch request
$post = array('code' => $code,
              'client_id' => CLIENT_ID,
              'client_secret' => CLIENT_SECRET,
              'redirect_uri' => CALLBACK_URI,
              'grant_type' => 'authorization_code');

//get JSON access token object (with refresh_token parameter)
$token = json_decode(runCurl(ACCESS_TOKEN_ENDPOINT, 'POST', $post));

//set request headers for signed OAuth request
$headers = array("Accept: application/json");
echo "token".$token->access_token;
die();

//construct URI to fetch contact information for current user
$contact_url = "https://www.google.com/m8/feeds/contacts/default/full?oauth_token=".$token->access_token;

//fetch profile of current user
$contacts = runCurl($contact_url, 'GET', $headers);
var_dump($contacts);

//echo "<h1>REFRESHING TOKEN</h1>";
var_dump(refreshToken($token->refresh_token));

?>