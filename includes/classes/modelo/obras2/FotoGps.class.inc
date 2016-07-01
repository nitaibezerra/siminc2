<?php
	
class FotoGps extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.fotogps";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fgpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fgpid' => null, 
									  	'arqid' => null, 
									  	'fgplatitude' => null, 
									  	'fgplongitude' => null, 
									  	'fgpzoom' => null, 
									  	'fgpstatus' => null 
									  );

	public function extractInformacaoGps($arqid, $pathImg, $insertGps = true){
		$exif=exif_read_data($pathImg, 0, true);		
		
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
	
}	