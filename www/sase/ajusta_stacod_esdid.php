<?php

// carrega as funções gerais
require_once '../../global/config.inc';

if( !IS_PRODUCAO ){
	$_SESSION['baselogin'] = "simec_espelho_producao";
}



// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
	$_SESSION['usucpforigem'] = '';
	$auxusucpf = '';
	$auxusucpforigem = '';
}else{
	$auxusucpf = $_SESSION['usucpf'];
	$auxusucpforigem = $_SESSION['usucpforigem'];
}


//exit('teste');

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


$db = new cls_banco();

$sql = "
	SELECT a.assid, s.stacod, s.esdid, a.docid
	FROM sase.assessoramento a
	LEFT JOIN sase.situacaoassessoramento s ON s.stacod = a.stacod
	WHERE docid IS NULL
";
$listaAssessoramentos = $db->carregar( $sql );

foreach ($listaAssessoramentos as $key => $value) {
	$docid = wf_cadastrarDocumento( TPDID_SASE_ASSESSORAMENTO, 'Documento Assistência Técnica' );

	if(!empty($docid)){
		$sql = "
			UPDATE sase.assessoramento
			SET docid = {$docid}
			WHERE assid = {$value['assid']};
		";
		$db->executar( $sql );

		$sql = "
			UPDATE workflow.documento
			SET esdid = {$value['esdid']}
			WHERE docid = {$docid};
		";
		$db->executar( $sql );
	}
}

$db->commit();

