<?php
include APPRAIZ.'www/sisplan/_funcoes.php';
include APPRAIZ.'www/sisplan/_constantes.php';
include APPRAIZ.'includes/workflow.php';

$_xml = str_replace('\"','"',$_POST['xml']);

$xml = simplexml_load_string($_xml);

$sql = "UPDATE pde.atividade SET ";
foreach ( $xml as $k=>$v )
	foreach ( $v as $x=>$y ) 
		if( $x != "pddid" || $x != "idtiporecusapi" )
			if ( $x != "atiid" && $x != "atiidexterno" && $x != "idtiporecusapi" )
				$sql .= $x."='".$xml->Atividade->$x."', ";
		
$sql .= "WHERE atiid = ".$xml->Atividade->atiidexterno;

$sql = str_replace( ", WHERE"," WHERE", $sql );


$_atiid = $xml->Atividade->atiidexterno;
$_atistatuspi = (string)$xml->Atividade->atistatuspi;

switch ($_atistatuspi){
	case 'A':
		$_eadid = PLANO_INTERNO_APROVADO;
		$assunto = 'Solicitaηγo de PA aprovada';
	break;
	case 'R':
		$_eadid = PLANO_INTERNO_REPROVADO;
		$assunto = 'Solicitaηγo de PA reprovada';
	break;
}


try {
	if ( $db->executar($sql) )
	{
		$db->commit();
		
		// envia email para os Super Usuαrios
		$message = montaTabelaEmail($_atiid, $assunto);		
		$sql = "SELECT
					usuemail,
					usunome
				FROM
					seguranca.usuario u
					JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
				WHERE
					pu.pflcod = " . PERFIL_SUPERUSER;
	
		$dados = $db->carregar($sql);		
		$dados = $dados ? $dados : array();
		
		foreach($dados as $d):
			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');			
		endforeach;		
		
		// envia email para o usuario cadastrador do PI
		$sql = "SELECT
					usuemail,
					usunome
				FROM
					seguranca.usuario u
				JOIN pde.atividade a ON u.usucpf = a.usucpf
				WHERE
					a.atiid = '" . $_atiid . "'";
	
		$dados = $db->carregar($sql);		
		$dados = $dados ? $dados : array();
	
		foreach($dados as $d):
			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');			
		endforeach;
	
		//
		// envia email para os representante do PI						
		$sql = "SELECT
					atirepresentante,
					atiemailrepresentante
				FROM
					pde.atividade
				WHERE
					atiid= '" . $_atiid . "' AND atisnenviaemailtramitacao = 't'";
		
		$dados = $db->carregar($sql);		
		$dados = $dados ? $dados : array();
		
		foreach($dados as $d):
			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');			
		endforeach;
	}
	
}
catch ( Exception $e ){
	dbg($e,1);
}

$_docid = sisplan_pegarDocid( $_atiid );
$_cmddsc = '';
$_dadosVerificacao = array('atiid'=>(int)$_atiid );
$resultado = wf_alterarEstado((int)$_docid, (int)$_eadid, $_cmddsc, $_dadosVerificacao);
?>