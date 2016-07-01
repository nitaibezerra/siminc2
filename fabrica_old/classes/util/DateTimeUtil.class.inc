<?php 
class DateTimeUtil {

	public static function retiraMascara($dataComMascara){
		if (strpos($dataComMascara, "/")) {
			$dia = substr($dataComMascara, 0, 2);
			$mes = substr($dataComMascara, 3, 2);
			$ano = substr($dataComMascara, 6, 4);
			return "$ano-$mes-$dia";
		} else {
			return $dataComMascara;
		}
	}
	
	public static function retiraMascaraRetornandoObjetoDateTime($dataComMascara){
		if (strpos($dataComMascara, "/")) {
			$dia = substr($dataComMascara, 0, 2);
			$mes = substr($dataComMascara, 3, 2);
			$ano = substr($dataComMascara, 6, 4);
			return new DateTime("$ano-$mes-$dia");
		} else {
			return new DateTime($dataComMascara);
		}
	}
	
	public static function now($mascara){
		$now = date_create();
		return $now->format($mascara);
	}
	
}