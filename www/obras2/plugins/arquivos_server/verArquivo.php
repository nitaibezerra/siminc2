<?php

/**
 * Download a file and it's metadata
 * The object returned will contain the file name, MIME type, metadata
 * (obtained from x-dropbox-metadata HTTP header) and file contents
 * @link https://www.dropbox.com/developers/reference/api#files-GET
 * @link https://github.com/BenTheDesigner/Dropbox/blob/master/Dropbox/API.php#L129-168
 */
session_start();

// Require the bootstrap
require_once('bootstrap.php');

session_write_close();

// Set the file path
// You will need to modify $path or run putFile.php first
if ( isset($_REQUEST["id"]) ){
    $path = realpath(ARQUIVOS_RAIZ . ($_REQUEST["id"]));
    if($path && validaPath($path))
    {
    	// Download the file
    	$file = $path;
    	if ( $file )
    	{
    	    $info = pathinfo($file);
    	    $size = filesize($file);
    	    $mime = get_mimetype($info['extension']);

    	    header("HTTP/1.0 200 OK");
    		header("Content-Disposition:attachment; filename=\"". $info['basename'] ."\"");
    		header('Content-type: ' . $mime. ';');
    		header("Content-length: " . $size);
    		header("Cache-Control: no-cache, must-revalidate");
    		header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    		header("Pragma: no-cache");

    		// Dump the output
    		readfile($path);
    		die();
    	}
    }
}

function get_mimetype($value='') {

    $ct['htm'] = 'text/html';
    $ct['html'] = 'text/html';
    $ct['txt'] = 'text/plain';
    $ct['asc'] = 'text/plain';
    $ct['bmp'] = 'image/bmp';
    $ct['gif'] = 'image/gif';
    $ct['jpeg'] = 'image/jpeg';
    $ct['jpg'] = 'image/jpeg';
    $ct['jpe'] = 'image/jpeg';
    $ct['png'] = 'image/png';
    $ct['ico'] = 'image/vnd.microsoft.icon';
    $ct['mpeg'] = 'video/mpeg';
    $ct['mpg'] = 'video/mpeg';
    $ct['mpe'] = 'video/mpeg';
    $ct['qt'] = 'video/quicktime';
    $ct['mov'] = 'video/quicktime';
    $ct['avi'] = 'video/x-msvideo';
    $ct['wmv'] = 'video/x-ms-wmv';
    $ct['mp2'] = 'audio/mpeg';
    $ct['mp3'] = 'audio/mpeg';
    $ct['rm'] = 'audio/x-pn-realaudio';
    $ct['ram'] = 'audio/x-pn-realaudio';
    $ct['rpm'] = 'audio/x-pn-realaudio-plugin';
    $ct['ra'] = 'audio/x-realaudio';
    $ct['wav'] = 'audio/x-wav';
    $ct['css'] = 'text/css';
    $ct['zip'] = 'application/zip';
    $ct['pdf'] = 'application/pdf';
    $ct['doc'] = 'application/msword';
    $ct['bin'] = 'application/octet-stream';
    $ct['exe'] = 'application/octet-stream';
    $ct['class']= 'application/octet-stream';
    $ct['dll'] = 'application/octet-stream';
    $ct['xls'] = 'application/vnd.ms-excel';
    $ct['ppt'] = 'application/vnd.ms-powerpoint';
    $ct['wbxml']= 'application/vnd.wap.wbxml';
    $ct['wmlc'] = 'application/vnd.wap.wmlc';
    $ct['wmlsc']= 'application/vnd.wap.wmlscriptc';
    $ct['dvi'] = 'application/x-dvi';
    $ct['spl'] = 'application/x-futuresplash';
    $ct['gtar'] = 'application/x-gtar';
    $ct['gzip'] = 'application/x-gzip';
    $ct['js'] = 'application/x-javascript';
    $ct['swf'] = 'application/x-shockwave-flash';
    $ct['tar'] = 'application/x-tar';
    $ct['xhtml']= 'application/xhtml+xml';
    $ct['au'] = 'audio/basic';
    $ct['snd'] = 'audio/basic';
    $ct['midi'] = 'audio/midi';
    $ct['mid'] = 'audio/midi';
    $ct['m3u'] = 'audio/x-mpegurl';
    $ct['tiff'] = 'image/tiff';
    $ct['tif'] = 'image/tiff';
    $ct['rtf'] = 'text/rtf';
    $ct['wml'] = 'text/vnd.wap.wml';
    $ct['wmls'] = 'text/vnd.wap.wmlscript';
    $ct['xsl'] = 'text/xml';
    $ct['xml'] = 'text/xml';

    $extension = $value;

    if (!$type = $ct[strtolower($extension)]) {
        $type = 'text/html';
    }

    return $type;
}

header("HTTP/1.0 404 Not Found");