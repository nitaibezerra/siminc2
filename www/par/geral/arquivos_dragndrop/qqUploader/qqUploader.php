<?php

/**
 * Handle file uploads via XMLHttpRequest
 */
class qqUploadedFileXhr {
	private $imageString = null;
	
   public function extractGps()
   {
        $input = fopen("php://input", "r");
   		$this->imageString = stream_get_contents($input);
   		fclose($input);

		$exif=exif_read_data("data://image/jpeg;base64," . base64_encode($this->imageString), 0, true);		
		
		if( !$exif || !isset($exif['GPS']) || ($exif['GPS']['GPSLatitude'] == '') ) {
			return false;
		} else {
			$lat_ref = $exif['GPS']['GPSLatitudeRef'];
			$lat_ref = ( $lat_ref == 'N' ? 1 : -1 ); 
			$lat = $exif['GPS']['GPSLatitude'];
			list($num, $dec) = explode('/', $lat[0]);
			$lat_s = $num / $dec;
			list($num, $dec) = explode('/', $lat[1]);
			$lat_m = $num / $dec;
			list($num, $dec) = explode('/', $lat[2]);
			$lat_v = $num / $dec;
	 
			$lon_ref = $exif['GPS']['GPSLongitudeRef'];
			$lon_ref = ( $lon_ref == 'E' ? 1 : -1 );
			$lon = $exif['GPS']['GPSLongitude'];
			list($num, $dec) = explode('/', $lon[0]);
			$lon_s = $num / $dec;
			list($num, $dec) = explode('/', $lon[1]);
			$lon_m = $num / $dec;
			list($num, $dec) = explode('/', $lon[2]);
			$lon_v = $num / $dec;
	 
			$gps_int = array('lat' => ($lat_s + $lat_m / 60.0 + $lat_v / 3600.0) * $lat_ref, 
							 'long' => ($lon_s + $lon_m / 60.0 + $lon_v / 3600.0) * $lon_ref);

			return $gps_int;		
		}
	}
		
    /**
     * Save the file to the specified path
     * @return boolean TRUE on success
     */
    function save($path) {
        $input = fopen("php://input", "r");
//        $temp = tmpfile();
//        $realSize = stream_copy_to_stream($input, $temp);
		$this->imageString = stream_get_contents($input);
        fclose($input);
		$realSize = strlen($this->imageString);
        
        if ($realSize != $this->getSize()){
            return false;
        }
      
        
		// armazena o arquivo na estrutura do simec 
		$campos	= array(
				"dmdid" 	    => $_SESSION['par']['dmdid']
		);	
		$file = new FilesSimec("demandaanexo", $campos, "par");
        $file->setPasta('par');
        $arqid = $file->setStream($this->getName(), $this->imageString, "image/jpeg", ".jpg", true, $this->getName());
        if ( $arqid > 0 )
        {
        	return $arqid;
        }
        else {
        	return false;
        }

    }
    function getName() {
        return $_GET['qqfile'];
    }
    function getSize() {
        if (isset($_SERVER["CONTENT_LENGTH"])){
            return (int)$_SERVER["CONTENT_LENGTH"];
        } else {
//            throw new Exception('Getting content length is not supported.');
            throw new Exception('NÃ£o foi possível obter o tamanho do arquivo.');
        }
    }
}

/**
 * Handle file uploads via regular form post (uses the $_FILES array)
 */
class qqUploadedFileForm {
	
	
   public function extractGps()
   {
		$exif=exif_read_data($_FILES['qqfile']['tmp_name'], 0, true);		
		
		if( (!$exif || $exif['GPS']['GPSLatitude'] == '')) {
			return false;
		} else {
			$lat_ref = $exif['GPS']['GPSLatitudeRef'];
			$lat_ref = ( $lat_ref == 'N' ? 1 : -1 ); 
			$lat = $exif['GPS']['GPSLatitude'];
			list($num, $dec) = explode('/', $lat[0]);
			$lat_s = $num / $dec;
			list($num, $dec) = explode('/', $lat[1]);
			$lat_m = $num / $dec;
			list($num, $dec) = explode('/', $lat[2]);
			$lat_v = $num / $dec;
	 
			$lon_ref = $exif['GPS']['GPSLongitudeRef'];
			$lon_ref = ( $lon_ref == 'E' ? 1 : -1 );
			$lon = $exif['GPS']['GPSLongitude'];
			list($num, $dec) = explode('/', $lon[0]);
			$lon_s = $num / $dec;
			list($num, $dec) = explode('/', $lon[1]);
			$lon_m = $num / $dec;
			list($num, $dec) = explode('/', $lon[2]);
			$lon_v = $num / $dec;
	 
			$gps_int = array('lat' => ($lat_s + $lat_m / 60.0 + $lat_v / 3600.0) * $lat_ref, 
							 'long' => ($lon_s + $lon_m / 60.0 + $lon_v / 3600.0) * $lon_ref);

			return $gps_int;		
		}
	}
		
    /**
     * Save the file to the specified path
     * @return boolean TRUE on success
     */
    function save($path) {
        if(!move_uploaded_file($_FILES['qqfile']['tmp_name'], $path)){
            return false;
        }
        return true;
    }
    function getName() {
        return $_FILES['qqfile']['name'];
    }
    function getSize() {
        return $_FILES['qqfile']['size'];
    }
}

class qqFileUploader {
    private $allowedExtensions = array();
    private $sizeLimit = 10485760;
    private $file;
    private $fileName;
    private $blockUploadNoGPSData;

    function __construct(array $allowedExtensions = array(), $sizeLimit = 10485760, $blockGps = false ){
        $allowedExtensions = array_map("strtolower", $allowedExtensions);

        $this->allowedExtensions = $allowedExtensions;
        $this->sizeLimit = $sizeLimit;
        $this->blockUploadNoGPSData= $blockGps;

        $this->checkServerSettings();

        if (isset($_GET['qqfile'])) {
            $this->file = new qqUploadedFileXhr();
        } elseif (isset($_FILES['qqfile'])) {
            $this->file = new qqUploadedFileForm();
        } else {
            $this->file = false;
        }
    }

    private function checkServerSettings(){
        $postSize = $this->toBytes(ini_get('post_max_size'));
        $uploadSize = $this->toBytes(ini_get('upload_max_filesize'));

        if ($postSize < $this->sizeLimit || $uploadSize < $this->sizeLimit){
            $size = max(1, $this->sizeLimit / 1024 / 1024) . 'M';
            die("{'error':'Aumente post_max_size e upload_max_filesize para $size'}");
        }
    }

    private function toBytes($str){
        $val = trim($str);
        $last = strtolower($str[strlen($str)-1]);
        switch($last) {
            case 'g': $val *= 1024;
            case 'm': $val *= 1024;
            case 'k': $val *= 1024;
        }
        return $val;
    }

    /**
     * Returns array('success'=>true) or array('error'=>'error message')
     */
    function handleUpload($uploadDirectory, $replaceOldFile = FALSE){

        if (!$this->file){
//            return array('error' => 'No files were uploaded.');
            return array('error' => 'Nenhum arquivo para fazer upload.');
        }

        $size = $this->file->getSize();

        if ($size == 0) {
//            return array('error' => 'File is empty');
            return array('error' => 'O arquivo está vazio.');
        }

        $sizeLimitSaida = max(1, $this->sizeLimit / 1024 / 1024) . 'M';
        if ($size > $this->sizeLimit) {
//            return array('error' => 'File is too large');
            return array('error' => 'O arquivo é muito grande. (Limite ' . $sizeLimitSaida . ')');
        }

        $pathinfo = pathinfo($this->file->getName());
        $filename = $pathinfo['filename'];
        //$filename = md5(uniqid());
        $ext = $pathinfo['extension'];

        if($this->allowedExtensions && !in_array(strtolower($ext), $this->allowedExtensions)){
            $these = implode(', ', $this->allowedExtensions);
//            return array('error' => 'File has an invalid extension, it should be one of '. $these . '.');
            return array('error' => 'O arquivo possui uma extensão inválida. As extensões permitidas são: '. $these . '.');
        }

        if($this->blockUploadNoGPSData)
        {
        	$dadosGPS = $this->file->extractGPS();
        	if ($dadosGPS === false)
        		return array('error' => 'O arquivo não possui coordendas GPS.');
        }
        
        $this->fileName = utf8_decode($filename) . '.' . $ext;

        if ($result = $this->file->save($this->fileName)){
            return array('success'=>true,'id_arquivo'=>$result);
        } else {
//            return array('error'=> 'Could not save uploaded file.' .
//                'The upload was cancelled, or server error encountered');
            return array('error'=> 'Não foi possível fazer o upload.' .
                'O upload foi cancelado, ou ocorreu um erro de servidor.');
        }

    }

    public function getFile(){
        return $this->file;
    }

    public function getFileName(){
        return $this->fileName;
    }
}
