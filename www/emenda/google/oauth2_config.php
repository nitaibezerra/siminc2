<?php
define('AUTHORIZATION_ENDPOINT', 'https://winsrv2008/RM7/APIIntegration/AuthorizeFeatures');
define('ACCESS_TOKEN_ENDPOINT', 'https://winsrv2008/RM7/APIIntegration/Token');
define('CLIENT_ID', 'fc8326b764bd490b9b5af1eb4939e5e9');
define('CLIENT_SECRET', '0ce23d03961f4f1fb0b9493c61e974da');
define('CALLBACK_URI', 'http://10.211.55.2/client_rm/google/oauth2_callback_google.php');

/***************************************************************************
 * Function: Run CURL
 * Description: Executes a CURL request
 * Parameters: url (string) - URL to make request to
 *             method (string) - HTTP transfer method
 *             headers - HTTP transfer headers
 *             postvals - post values
 **************************************************************************/
function runCurl($url, $method = 'GET', $postvals = null) {
    $ch = curl_init($url);

    //GET request: send headers and return data transfer
    if ($method == 'GET'){
        $options = array(
            CURLOPT_URL => $url,
            CURLOPT_RETURNTRANSFER => 1,
        	CURLOPT_SSL_VERIFYPEER => false
        );
        $result = curl_setopt_array($ch, $options);
    //POST / PUT request: send post object and return data transfer
    } else {
        $options = array(
            CURLOPT_URL => $url,
            CURLOPT_POST => 1,
            CURLOPT_POSTFIELDS => $postvals,
            CURLOPT_RETURNTRANSFER => 1,
        	CURLOPT_SSL_VERIFYPEER => false
        );
        $result = curl_setopt_array($ch, $options);
    }

    $response = curl_exec($ch); 
    
    curl_close($ch);

    return $response;
}

/***************************************************************************
 * Function: Refresh Access Token
 * Description: Refreshes an expired access token
 * Parameters: key (string) - application consumer key
 *             secret (string) - application consumer secret
 *             refresh_token (string) - refresh_token parameter passed in
 *                to fetch access token request.
 **************************************************************************/
function refreshToken($refresh_token) {
    //construct POST object required for refresh token fetch
    $postvals = array('grant_type' => 'refresh_token',
                      'client_id' => CLIENT_ID,
                      'client_secret' => CLIENT_SECRET,
                      'refresh_token' => $refresh_token);

    //return JSON refreshed access token object
    return json_decode(runCurl(ACCESS_TOKEN_ENDPOINT, 'POST', $postvals));
}
?>