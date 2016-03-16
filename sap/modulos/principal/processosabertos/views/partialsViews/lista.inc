
<?php
$bens = $oBens->processosAbertos();
$dados = array();

if(is_array($bens) && count($bens) >= 1){
	foreach($bens as $key => $value){
		$auxDataHora = explode(' ',$value['data']);
		$auxData = explode('-',$auxDataHora[0]);
		$value['data'] = '<center>'.$auxData[2].'/'.$auxData[1].'/'.$auxData[0].'</center>';
		
		if(!empty($value['dataprocesso'])){
			$auxData = explode('-',$value['dataprocesso']);
			$value['dataprocesso'] = '<center>'.$auxData[2].'/'.$auxData[1].'/'.$auxData[0].'</center>';
		}
		
		//centraliza os dados para apresentação na grid
		$value['tipo'] = '<center>'.$value['tipo'].'</center>';
		$value['numprocesso'] = '<center>'.$value['numprocesso'].'</center>';
		$value['usunome'] = '<center>'.$value['usunome'].'</center>';
		
		$dados[] = $value;
	}
}


$oBens->listaProcessosAbertos($dados);
?>
