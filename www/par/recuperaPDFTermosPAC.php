<?php 

require_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once "_funcoes.php";
require_once "_constantes.php";

$db = new cls_banco();

ob_clean();

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_REQUEST['terid'] && $db->testa_superuser() ){
	
	$sql = "SELECT 
				estuf,
				muncod,
				proid,
				usucpf,
				to_char(terdatainclusao,'YYYY') as ano,
				terdatainclusao
			FROM 
				par.termocompromissopac 
			WHERE
				terid = {$_REQUEST['terid']}";
	
	$dados = $db->pegalinha( $sql );

	$termo = $_REQUEST['terid'];
	
	$html = pegaTermoCompromissoArquivo( '', $_REQUEST['terid'] ); 
	
	$html = str_replace("'", '', $html);
	
	$pdf = pdf( utf8_encode( $html ) );

	if( $pdf && $termo ) {

		$descricaoArquivo = 'PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$dados['ano'];
	
		$campos	= Array("terdatainclusao" =>"'{$dados['terdatainclusao']}'",
						"usucpf"		  => "'{$dados['usucpf']}'");
	
		$file = New FilesSimec("termocompromissopac", $campos, 'par');
	
		$file->arquivo['name'] = $descricaoArquivo.".pdf";
	
		$arquivoSalvo = $file->setStream( $descricaoArquivo, $pdf, "text/pdf", ".pdf", false);
	
		$sql = "UPDATE par.termocompromissopac
				SET
					arqid = $arquivoSalvo
				WHERE
					terid = $termo";
	
		$db->executar($sql);
	
// 		if( is_array( $preids ) ){
			
// 			$sql = "";
			
// 			foreach( $preids as $preid ){
// 				$sql .= "INSERT INTO par.termoobra(preid, terid)
// 						 VALUES ( $preid, $termo);";
// 			}
			
// 			$db->executar($sql);
// 		}
	
		$db->commit();
	
		$tipo = $_REQUEST['regerar'] ? 'regerado' : 'novo';
	
		if($arquivoSalvo){
			
			pdf( utf8_encode($html) , 1 );
		}
	}
}else{
	
	ver('Faltou o terid.');
}

?>