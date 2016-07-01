<?php

if( $_POST['ajaxConcluiPrimeirosPassos'] )
{
	$db->executar("UPDATE pdeinterativo.grupotrabalho SET grtconcluido = 't' WHERE grtid = ".$_POST['grtid']);
	$db->commit();
	
	salvarAbaResposta("primeiros_passos_passo_3");
	
	die('ok');
}

if( $_POST['ajaxExcluirMembro'] )
{
	if(is_numeric($_REQUEST['pesid'])) {
		$db->executar("UPDATE pdeinterativo.pessoagruptrab SET pgtstatus = 'I' WHERE pesid = ".$_REQUEST['pesid']." AND grtid = ".$_REQUEST['grtid']);
		$db->commit();
		salvarAbaResposta("primeiros_passos_passo_1");
		die('ok');
	} else {
		die('nok');
	}
}

if( $_POST['ajaxExisteDados'] )
{
	$ret 			= array();
	$ret['retorno'] = true;

	if( $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$_POST['grtid']." AND pgtstatus = 'A' AND pgtdiretor = 'f'") < 1 )
	{
		if( $db->pegaUm("SELECT count(1) FROM pdeinterativo.gruptrabanexo WHERE grtid = ".$_POST['grtid']." AND gtastatus = 'A'") < 1 )
		{
			$ret['retorno'] = false;
		}
	}

	die( simec_json_encode($ret) );
}

if( $_POST['ajaxExisteMembro'] )
{
	$ret 			= array();
	$ret['retorno'] = false;
	
	$cpf 	= str_replace('.', '', str_replace('-', '', $_POST['dpecpf']));
	
	$existe = $db->pegaUm("SELECT 
								count(pg.pgtid) 
							FROM 
								pdeinterativo.pessoagruptrab pg 
							INNER JOIN 
								pdeinterativo.pessoa p ON p.pesid = pg.pesid 
													  AND p.pesstatus = 'A'
													  AND p.usucpf = '".$cpf."'
							WHERE 
								pg.grtid = ".$_POST['grtid']." 
								AND pg.pgtstatus = 'A'");
	
	if( $existe > 0 ) $ret['retorno'] = true;
	
	// retorna a quantidade de membros cadastrados
	$ret['nummembros'] = $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$_POST['grtid']." AND pgtstatus = 'A' AND pgtdiretor = 'f'"); 
	
	die( simec_json_encode($ret) );
}

if( $_POST['ajaxApagaArquivo'] )
{
	$anxid = $db->pegaUm("SELECT anxid FROM pdeinterativo.anexo WHERE arqid = ".$_POST['arqid']);

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array('tpdid' => TPDID_GRUPO_TRABALHO);
	$file = new FilesSimec("anexo", $campos, "pdeinterativo");

	$db->executar("UPDATE pdeinterativo.gruptrabanexo SET gtastatus = 'I' WHERE anxid = ".$anxid);
	$db->commit();
	$db->executar("UPDATE pdeinterativo.anexo SET anxstatus = 'I' WHERE arqid = ".$_POST['arqid']);
	$db->commit();
	$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$_POST['arqid']);
	$db->commit();
	$file->excluiArquivoFisico($_POST['arqid']);

	salvarAbaResposta("primeiros_passos_passo_1");
	
	die('ok');
}

if( $_POST['ajaxApagaDadosGT'] )
{
	/*** Excluir os membros ***/
	$db->executar("UPDATE pdeinterativo.pessoagruptrab SET pgtstatus = 'I' WHERE grtid = ".$_POST['grtid']." AND pgtdiretor = 'f'");
	$db->commit();

	/*** Recupera os anexos ***/
	$gruptrabanexo = $db->carregar("SELECT * FROM pdeinterativo.gruptrabanexo WHERE grtid = ".$_POST['grtid']." AND gtastatus = 'A'");

	if( $gruptrabanexo )
	{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos = array('tpdid' => TPDID_GRUPO_TRABALHO);
		$file = new FilesSimec("anexo", $campos, "pdeinterativo");

		/*** Exclui os arquivos***/
		foreach($gruptrabanexo as $grupoanexo)
		{
			$arqid = $db->pegaUm("SELECT arqid FROM pdeinterativo.anexo WHERE anxid = ".$grupoanexo['anxid']);
				
			$db->executar("UPDATE pdeinterativo.gruptrabanexo SET gtastatus = 'I' WHERE gtaid = ".$grupoanexo['gtaid']);
			$db->commit();
			$db->executar("UPDATE pdeinterativo.anexo SET anxstatus = 'I' WHERE arqid = ".$arqid);
			$db->commit();
			$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid);
			$db->commit();
			$file->excluiArquivoFisico($arqid);
		}
	}

	$db->executar("UPDATE pdeinterativo.grupotrabalho SET grtconcluido = 'f' WHERE grtid = ".$_POST['grtid']);
	$db->commit();
	
	salvarAbaResposta("primeiros_passos_passo_1");
	
	die('ok');
}

if( $_POST['ajaxOpcaoGrupo'] )
{
	$db->executar("UPDATE pdeinterativo.grupotrabalho SET grtopcao = '".$_POST['opcao']."' WHERE grtid = ".$_POST['grtid']);
	$db->commit();
	salvarAbaResposta("primeiros_passos_passo_1");
	
	die('ok');
}

if( $_POST['ajaxInsereMembro'] )
{
	$cpf 	= str_replace(array(".","-"," ","/"), "", $_POST['dpecpf']);
	
	if(strlen($cpf)!=11) die('erro');
	
	$pesid	= $_POST['pesid'];
	$grtid	= $_POST['grtid'];
	
	if(!$_POST['grtid']) die('erro');

	if( $pesid && $pesid != '' )
	{
		$sql = "UPDATE pdeinterativo.pessoa SET pesnome = '".pg_escape_string($_POST['pesnome'])."' WHERE pesid = ".$pesid;
		$db->executar($sql);
		$db->commit();
	}
	else
	{
		$sql = "select pesid from pdeinterativo.pessoa where usucpf = '".$cpf."'";
		$pesid = $db->pegaUm($sql);
		if($pesid){
			$sql = "UPDATE pdeinterativo.pessoa SET pesnome = '".pg_escape_string($_POST['pesnome'])."' WHERE pesid = ".$pesid;
			$db->executar($sql);
		}else{
			$sql = "INSERT INTO pdeinterativo.pessoa(usucpf,pesnome) VALUES('".$cpf."','".pg_escape_string($_POST['pesnome'])."') RETURNING pesid";
			$pesid = $db->pegaUm($sql);
		}
		$db->commit();
	}

	$dpeid = $db->pegaUm("SELECT dpeid FROM pdeinterativo.detalhepessoa WHERE pesid = ".$pesid);

	$_POST['dpetelefone'] = str_replace("-", "", $_POST['dpetelefone']);

	if( $dpeid )
	{
		$sql = "UPDATE pdeinterativo.detalhepessoa SET fgtid = ".(($_POST['fgtid'])?"'".$_POST['fgtid']."'":"NULL").",dpetelefone = ".(($_POST['dpeddd']||$_POST['dpetelefone'])?"'".$_POST['dpeddd'].$_POST['dpetelefone']."'":"NULL").",dpeemail = '".pg_escape_string($_POST['dpeemail'])."' WHERE dpeid = ".$dpeid;
		$db->executar($sql);
	}
	else
	{
		$sql = "INSERT INTO pdeinterativo.detalhepessoa(pesid,fgtid,dpetelefone,dpeemail) VALUES(".$pesid.", ".(($_POST['fgtid'])?"'".$_POST['fgtid']."'":"NULL").", '".$_POST['dpeddd'].$_POST['dpetelefone']."', '".pg_escape_string($_POST['dpeemail'])."')";
		$db->executar($sql);
	}
	$db->commit();

	$gruptrab = $db->pegaUm("SELECT count(1) FROM pdeinterativo.pessoagruptrab WHERE grtid = ".$grtid." AND pesid = ".$pesid);

	if( $gruptrab == 0 ) {
		if($_SESSION['pdeinterativo_vars']['pdeid']) {
			$sql = "INSERT INTO pdeinterativo.pessoagruptrab(grtid,pesid,pdeid) VALUES(".$grtid.", ".$pesid.", ".$_SESSION['pdeinterativo_vars']['pdeid'].")";
			$db->executar($sql);
		}
	} else {
		$sql = "UPDATE pdeinterativo.pessoagruptrab SET pgtstatus = 'A' WHERE grtid=".$grtid." AND pesid=".$pesid;
		$db->executar($sql);
	}
	$db->commit();

	salvarAbaResposta("primeiros_passos_passo_1");
	
	die($pesid);
}

?>