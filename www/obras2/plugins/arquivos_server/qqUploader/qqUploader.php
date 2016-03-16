<?php

/**
 * Handle file uploads via XMLHttpRequest
 */
class qqUploadedFileXhr {
    /**
     * Save the file to the specified path
     * @return boolean TRUE on success
     */
    function save($path) {
        $input = fopen("php://input", "r");
        $temp = tmpfile();
        $realSize = stream_copy_to_stream($input, $temp);
        fclose($input);

        if ($realSize != $this->getSize()){
            return false;
        }

        $target = fopen($path, "w");
        fseek($temp, 0, SEEK_SET);
        stream_copy_to_stream($temp, $target);
        fclose($target);

        return true;
    }
    function getName() {
        return $_GET['qqfile'];
    }
    function getSize() {
        if (isset($_SERVER["CONTENT_LENGTH"])){
            return (int)$_SERVER["CONTENT_LENGTH"];
        } else {
//            throw new Exception('Getting content length is not supported.');
            throw new Exception('Não foi possível obter o tamanho do arquivo.');
        }
    }
}

/**
 * Handle file uploads via regular form post (uses the $_FILES array)
 */
class qqUploadedFileForm {
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

    function __construct(array $allowedExtensions = array(), $sizeLimit = 10485760){
        $allowedExtensions = array_map("strtolower", $allowedExtensions);
        $this->allowedExtensions = $allowedExtensions;
        $this->sizeLimit = $sizeLimit;

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
//            die("{'error':'increase post_max_size and upload_max_filesize to $size'}");
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
        if (!is_writable($uploadDirectory)){
//            return array('error' => "Server error. Upload directory isn't writable.");
            return array('error' => "Erro de servidor. Diret�rio de upload não está acessível.");
        }

        if (!$this->file){
//            return array('error' => 'No files were uploaded.');
            return array('error' => 'Nenhum arquivo para fazer upload.');
        }

        $size = $this->file->getSize();

        if ($size == 0) {
//            return array('error' => 'File is empty');
            return array('error' => 'O arquivo está vazio.');
        }

        if ($size > $this->sizeLimit) {
//            return array('error' => 'File is too large');
            return array('error' => 'O arquivo é muito grande.');
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

        if(!$replaceOldFile){
            /// don't overwrite previous files that were uploaded
            while (file_exists($uploadDirectory . $filename . '.' . $ext)) {
                $filename .= rand(10, 99);
            }
        }

        $this->fileName = utf8_decode($filename) . '.' . $ext;

        if ($this->file->save($uploadDirectory . $this->fileName)){
            return array('success'=>true);
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
