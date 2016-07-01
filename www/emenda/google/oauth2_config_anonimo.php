<?php
define('AUTHORIZATION_ENDPOINT', 'https://winsrv2008/RM7/APIIntegration/AuthorizeFeatures');
define('ACCESS_TOKEN_ENDPOINT', 'https://winsrv2008/RM7/APIIntegration/Token');
define('WF_CREATE_EVENT', 'https://winsrv2008/wf/api/events');
define('CLIENT_ID', '72f71e1115b84ab2b81d9678330b7554');
define('CLIENT_SECRET', '0128c0dbbfe144bba901dd216ad5f944');
define('CALLBACK_URI', 'http://10.211.55.2/client_rm/google/oauth2_callback_google_anonimo.php');



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
    var_dump($ch);
    die;
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
* Function: Run CURL
* Description: Executes a CURL request
* Parameters: url (string) - URL to make request to
*             method (string) - HTTP transfer method
*             headers - HTTP transfer headers
*             postvals - post values
**************************************************************************/
function runCurlJson($url, $method = 'GET', $postvals = null, $access_token = "") {
	$ch = curl_init($url);

	//GET request: send headers and return data transfer
	if ($method == 'GET'){
		$options = array(
		CURLOPT_HTTPHEADER => array('Content-Type: application/json', 'Authorization: OAuth2 '.$access_token),
		CURLOPT_URL => $url,
		CURLOPT_RETURNTRANSFER => 1,
		CURLOPT_SSL_VERIFYPEER => false
		);
		$result = curl_setopt_array($ch, $options);
		//POST / PUT request: send post object and return data transfer
	} else {
		$options = array(
		CURLOPT_HTTPHEADER => array('Content-Type: application/json', 'Authorization: OAuth2 '.$access_token),
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