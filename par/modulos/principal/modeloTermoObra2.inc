<?php 
ob_clean();

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";


$html = $db->pegaUm("select terdocumento from par.termocompromissopac where terid = 3675");
$ano = 2012;
$termo = 3675;
$usucpf = '';
$preids = array( 9796 );

$pdf = pdf( utf8_encode($html) );

if( $pdf ) {
	
	$descricaoArquivo = 'PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$ano;
	
	$campos	= array("terdatainclusao" => "'".date("Y-m-d G:i:s")."'",
					"usucpf"		  => "'".$usucpf."'");
	
	$file = new FilesSimec("termocompromissopac", $campos, 'par');
	
	$file->arquivo['name'] = $descricaoArquivo.".pdf";
	
	$arquivoSalvo = $file->setStream( $descricaoArquivo, $pdf, "text/pdf", ".pdf", false);
	
	$sql = "UPDATE par.termocompromissopac
			SET
				arqid = ".$arquivoSalvo."
			WHERE
				terid = ".$termo;
	
	$terid = $db->executar($sql);
	
	if( is_array( $preids ) ){
		$sql = "";
		foreach( $preids as $preid ){
			$sql .= "INSERT INTO par.termoobra(
						preid, 
						terid)
					 VALUES (
						{$preid}, 
						{$termo});";
		}
		$db->executar($sql);
	}
	
	$db->commit();
	
	if($arquivoSalvo){
		pdf( utf8_encode($html) , 1 );
	}
}

//pdf( utf8_encode($html) , 1 );


?>