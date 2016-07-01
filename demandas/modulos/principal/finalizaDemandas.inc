<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

ini_set("memory_limit", "1024M");


//ON MOUSE OVER NOS CAMPOS DA LISTAGEM DA VALIDAÇÃO

	//Tempo de Classificação
	//$tempoclassif

//FIM ON MOUSE OVER NOS CAMPOS DA LISTAGEM DA VALIDAÇÃO


//filtra o andar
if ($_REQUEST['lcaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaAndarDemanda($_POST['lcaidAjax']);
	exit;
}


function montaAndarDemanda($lcaid){
	global $db;

	$sql = sprintf("SELECT
					 laa.laaid AS codigo,
					 aa.anddescricao AS descricao
					 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
					FROM 
					 	demandas.localandaratendimento laa
						inner join demandas.andaratendimento aa on laa.andid = aa.andid
					WHERE
					 laa.lcaid = %d 
					ORDER BY 
					 aa.anddescricao", 
	$lcaid);
	$dados = $db->carregar($sql);
	if(count($dados)==1) $laaid = $dados[0]['codigo'];
	$db->monta_combo( 'laaid', $sql, 'S', 'Filtro...', '', '', '', '', '', '', '', $laaid );
}


if($_POST['varaux'] == '1'){



	include_once APPRAIZ . 'includes/workflow.php';

	//tpdid=31="Fluxo - Suporte de Demandas"
	//esdid=91="Em Análise"
	//esdid=92="Em Atendimento"
	//esdid=93="Aguardando validação"
	//esdid=94="Aguardando avaliação"
	//esdid=95="Validada"
	//esdid=100="Cancelada"
	//esdid=122="Aguardando recursos"
	//esdid=170="Validada SEM PAUSA"

	//aedid=165
	//esdidorigem=93
	//esdiddestino=95
	//ou
	//esdiddestino=170


	//tpdid=35="Fluxo Genérico"
	//esdid=107="Em Análise"
	//esdid=108="Em Atendimento"
	//esdid=109="Finalizada"
	//esdid=110="Cancelada"
	//esdid=111="Aguardando validação"
	//esdid=112="Aguardando avaliação"
	//esdid=123="Aguardando Recursos"

	//aedid=186
	//esdidorigem=111
	//esdiddestino=109

	foreach ($_POST as $campo => $valor){
		//$$campo = $valor;

		if(substr($campo, 0, 2) == 'rd'){

			//echo 'Campo: '.$campo.' - Valor: '.$valor.'<br>';
			$dmdid = substr($campo, 2);

			$_SESSION['dmdid'] = $dmdid;

			$sql = "select d.docid, t.ordid
					from demandas.demanda d 
					inner join demandas.tiposervico t ON t.tipid = d.tipid 
					where d.dmdid=".$_SESSION['dmdid'];
			$dados = (array) $db->pegaLinha($sql);

			$docid = (integer) $dados['docid'];

			$cmddsc = $_POST['just'.$dmdid];


			if($dados['ordid'] == 1){ //sistemas de informação
				if($valor == 'S'){ //finalizar
					$aedid = 186;
				}
				elseif($valor == 'N'){ //invalidar
					$aedid = 279;
				}
			}
			else{
				if($valor == 'S'){ //validar
					$aedid = 165;
				}
				elseif($valor == 'SF'){ //validar SEM PAUSA
					$aedid = 368;
				}
				elseif($valor == 'N'){ //invalidar
					$aedid = 278;
				}
			}

			$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";

			$_SESSION['flagfinalizaembloco'] = "OK";

			wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );

		}



	}

	/*
	 for($i=0; $i<=$nlinhas;$i++){

		$_SESSION['dmdid'] = $_POST['chkfindem'][$i];

		$sql = "select d.docid, t.ordid
		from demandas.demanda d
		inner join demandas.tiposervico t ON t.tipid = d.tipid
		where d.dmdid=".$_SESSION['dmdid'];
		$dados = (array) $db->pegaLinha($sql);

		$docid = (integer) $dados['docid'];

		if($dados['ordid'] == 1) $aedid = 186;
		else $aedid = 165;

		$cmddsc = '';
		$dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";

		wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );

		}
		*/

	echo "<script>alert('Operação efetuada com sucesso.');</script>";

}

if ($_GET['dmdid']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdid'];
	header( "Location: demandas.php?modulo=principal/dadosDemanda&acao=A" );
	exit();
}


$codigo   = $_POST['codigo'];
$origem   = $_POST['ordid'];
$setor    = $_POST['unaid'];
$edificio = $_POST['lcaid'];
$andar    = $_POST['laaid'];
$sala     = $_POST['sala'];
$palavra_chave = $_POST['palavra_chave'];
$avaliacao    = $_POST['avaliacao'];
$ordenacao    = $_POST['ordenacao'];
$ordenacao2    = $_POST['ordenacao2'];
$dtini    = $_POST['dtini'];
$dtfim    = $_POST['dtfim'];

if($ordenacao == '1') $ordenacao = ' id ';
if($ordenacao == '2') $ordenacao = ' prioridade ';
if($ordenacao == '3') $ordenacao = ' qtdservico ';
//if($ordenacao == '4') $ordenacao = ' avaliacao ';
if($ordenacao == '5') $ordenacao = ' datainclusao ';
if($ordenacao == '6') $ordenacao = ' datainicio ';
if($ordenacao == '7') $ordenacao = ' datafim ';
if($ordenacao == '8') $ordenacao = ' dataconclusao ';
if($ordenacao == '9') $ordenacao = ' pontuacao ';
if($ordenacao == '10') $ordenacao = ' valordemanda ';


$ordenacaox = $ordenacao . ' ' . $ordenacao2;
//if(!$ordenacao) $ordenacaox = " dataconclusao desc, id DESC ";
if(!$ordenacao) $ordenacaox = " id ASC ";

$perfil   = arrayPerfil();

$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade 
									where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and ordid is not null
									union
									SELECT c.ordid from demandas.usuarioresponsabilidade ur
									inner join demandas.celula c on c.celid = ur.celid
									where rpustatus='A' and ur.celid is not null and usucpf = '".$_SESSION['usucpf']."'");

$celidx = $db->carregarColuna("SELECT distinct celid from demandas.usuarioresponsabilidade 
									where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and celid is not null");



if ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){
	$ordidx = $db->carregarColuna(" SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and pflcod in(".DEMANDA_PERFIL_ANALISTA_WEB.",".DEMANDA_PERFIL_GESTOR_MEC.",".DEMANDA_PERFIL_DBA.") and not ordid is null");
	$ordidxCombo = " OR t.ordid in (2)";
	$celidx = "";
	
	//trata ordid para query
	//se tiver ordid=3, ira limpar todos os codigos e deixar somente o ordid=3
	if ( in_array("3", $ordidx) ){
		$ordidxQuery = "t.ordid in (3)";
	}
}

if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) ){
	$ordidx = $db->carregarColuna("SELECT ordid from demandas.origemdemanda");
	$celidx = "";
	
	if(!$origem && !$_REQUEST['dmdidAjax']){
		$from = "";

		$and = " AND od.ordid = 1"; // sistemas de informação
	}
	else{
		$from = "";

		$and = "";
	}	
}


if($ordidx[0]) {
		$ordidx = implode(',',$ordidx);
		$ordidx = "t.ordid in (".$ordidx.")";
}
else{
	if(!$celidx){
		echo '<script>
				alert("É necessário vincular uma origem ao seu perfil.");
				history.back();
			  </script>';
	}
	$ordidx = "0=0";
}


if($celidx) {
	
	$celidx = implode(',',$celidx);
	
	//2=redes / 13=Solicitações DTI - CGI / 14=Serviços de Cabeamento e Elétrica
	if ( strpos($ordidx, '2') || strpos($ordidx, '13') || strpos($ordidx, '14') ){
		$celidx = "t.celid in (".$celidx.")";
	}
	else{
		$celidx = "cel.celid in (".$celidx.")";
	}
	
}
else{
	$celidx = "0=0";
}


//dbg($ordidx);
//dbg($celidx,1);

/*
if ( in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) ||  in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ){

	$ordidx = $db->carregarColuna(" SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and pflcod in(".DEMANDA_PERFIL_ANALISTA_SISTEMA.",".DEMANDA_PERFIL_GERENTE_PROJETO.") and not ordid is null");

	$from = "LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
			   LEFT JOIN demandas.usuarioresponsabilidade ur1 ON ur1.sidid = sd.sidid AND
			   													 ur1.rpustatus = 'A' AND
			   													 ur1.usucpf = '".$_SESSION['usucpf']."' AND
																 ur1.pflcod IN (".DEMANDA_PERFIL_GERENTE_PROJETO.",".DEMANDA_PERFIL_ANALISTA_SISTEMA.")";
	//$where1[] = "d.sidid IS NOT NULL";
	$and = " AND d.sidid in (select distinct sc4.sidid
							from demandas.sistemacelula sc4 
							inner JOIN demandas.usuarioresponsabilidade ur5 ON ur5.celid = sc4.celid 
							where ur5.rpustatus = 'A' and ur5.usucpf='{$_SESSION['usucpf']}'
							) ";

}

if ( in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) ){

	/*
	 if ( !in_array('2', $ordidx) ){
		$ordidx2 = $db->carregarColuna("SELECT distinct c.ordid from demandas.usuarioresponsabilidade u inner join demandas.celula c on c.celid = u.celid where u.rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and pflcod in(".DEMANDA_PERFIL_GESTOR_REDES.") and u.ordid is null");
		array_push($ordidx,$ordidx2);
		}
		*/
/*
	$from = "LEFT JOIN demandas.usuarioresponsabilidade ur7 ON ur7.celid = t.celid AND
			   													 ur7.rpustatus = 'A' AND
			   													 ur7.usucpf = '".$_SESSION['usucpf']."' AND
																 ur7.pflcod IN (".DEMANDA_PERFIL_GESTOR_REDES.")";

	$and = " AND t.celid in (select distinct ur8.celid
							 from demandas.usuarioresponsabilidade ur8 
							 inner join demandas.tiposervico t8 on t8.celid = ur8.celid
							 where t8.ordid=2 and ur8.rpustatus = 'A' and ur8.usucpf='{$_SESSION['usucpf']}'
							)";




}


if ( in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ||  in_array(DEMANDA_PERFIL_DBA, $perfil) ){

	$ordidx = $db->carregarColuna(" SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and pflcod in(".DEMANDA_PERFIL_ANALISTA_WEB.",".DEMANDA_PERFIL_GESTOR_MEC.",".DEMANDA_PERFIL_DBA.") and not ordid is null");

	$from = "INNER JOIN demandas.usuarioresponsabilidade ur ON (ur.ordid = od.ordid AND
																  ur.rpustatus = 'A' AND	
																  ur.usucpf = '".$_SESSION['usucpf']."' AND	
																  ur.pflcod IN (".DEMANDA_PERFIL_GESTOR_MEC.",
																 			    ".DEMANDA_PERFIL_ANALISTA_WEB.",
																 			    ".DEMANDA_PERFIL_DBA."
																 			    )
																 )";
	$and = "";

}


if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) ){
	$ordidx = $db->carregarColuna("SELECT ordid from demandas.origemdemanda");


	if(!$origem && !$_REQUEST['dmdidAjax']){
		$from = "";

		$and = " AND od.ordid = 1"; // sistemas de informação
	}
	else{
		$from = "";

		$and = "";
	}


}
*/




if ($codigo && !$_REQUEST['dmdidAjax']){
	$codigo = str_replace("#","",$codigo);
	$where[] = "d.dmdid = {$codigo}";
}

if ($origem && !$_REQUEST['dmdidAjax']){
	$where[] = "od.ordid = {$origem}";
}

if ($setor && !$_REQUEST['dmdidAjax']){
	$where[] = "d.unaid = {$setor}";
}

if ($edificio && !$_REQUEST['dmdidAjax']){
	$where[] = "loc.lcaid = {$edificio}";
}

if ($andar && !$_REQUEST['dmdidAjax']){
	$where[] = "aa.andid = {$andar}";
}

if ($sala && !$_REQUEST['dmdidAjax']){
	$where[] = "d.dmdsalaatendimento = '{$sala}'";
}

if ($palavra_chave && !$_REQUEST['dmdidAjax']){
	$where[] = " (d.dmdtitulo ILIKE '%{$palavra_chave}%' OR d.dmddsc ILIKE '%{$palavra_chave}%')";
}

if ($avaliacao && !$_REQUEST['dmdidAjax']){
	//$where[] = "avd.avdgeral = '{$avaliacao}'";
	$inneravaliacao = "INNER JOIN (select dmdid from demandas.avaliacaodemanda WHERE avdgeral = '{$avaliacao}' group by dmdid) avd ON avd.dmdid = d.dmdid";
}

if ($dtini && $dtfim && !$_REQUEST['dmdidAjax']){
	$dtini2 = formata_data_sql($dtini);
	$dtfim2 = formata_data_sql($dtfim);
	$where[] = " d.dmddatainclusao BETWEEN '{$dtini2} 00:00:00' AND '{$dtfim2} 23:59:59' ";
}


$where1 = ($where ? " AND ".implode(' AND ', $where) : ' ');





//filtra demandas filhas
if($_REQUEST['dmdidAjax']) {
	$dmdidorigem = " AND d.dmdidorigem = ".$_REQUEST['dmdidAjax']." AND doc.esdid is not null ";
	//$unionAll = "";
}
else{

	$whereSub = " where d.dmdstatus = 'A' and b.esdid in (93,111) and d.dmdidorigem is not null ";
	if($ordidx) $whereSub .= "and $ordidx";
	if($celidx) $whereSub .= "and $celidx";
	
	//somente para gestor suporte de atendimento
	if($origem == '2') $whereSub = str_replace('t.ordid in (3)','t.ordid in (2,3)',$whereSub);
		
	//if($codigo) $whereSub = " where d.dmdstatus = 'A' and d.dmdidorigem is not null ";
	
	if(!$codigo){
		// pega somente as dmdidorigem is not null
		$sqlsub = "select distinct d.dmdidorigem
						from 	demandas.demanda d
							inner join workflow.documento b on b.docid = d.docid and b.tpdid in (31,35)
							LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
							LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sidid = sis.sidid
							LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
							LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid	
							$inneravaliacao
							$from
							$whereSub
							$and
							$where1
					";
								
							//dbg($sqlsub,1);
	
							$sub = array();
							$dmdidPai = array();
							$sub = $db->carregar( $sqlsub );
							if($sub){
								foreach($sub as $subs){
									$dmdidPaiAdd = verificaDemandaPai($subs['dmdidorigem']);
									array_push($dmdidPai, $dmdidPaiAdd);
								}
							}
	
	
	
							$whereSub = " where d.dmdstatus = 'A' and b.esdid in (93,111) and d.dmdidorigem is null ";
							if($ordidx) $whereSub .= "and $ordidx";
							if($celidx) $whereSub .= "and $celidx";
							
							//somente para gestor suporte de atendimento
							if($origem == '2') $whereSub = str_replace('t.ordid in (3)','t.ordid in (2,3)',$whereSub);
							
							
							if($codigo) $whereSub = " where d.dmdstatus = 'A' and d.dmdidorigem is null ";
	
	
		// pega somente as dmdidorigem is null
		$sqlsub2 = "select distinct d.dmdid
						from 	demandas.demanda d
							inner join workflow.documento b on b.docid = d.docid and b.tpdid in (31,35)
							LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
							LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sidid = sis.sidid
							LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
							LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid	
							$inneravaliacao
							$from
							$whereSub
							$and
							$where1
					";
							
							//dbg($sqlsub2,1);
							
							
							$dmdidPai2 = array();
							$dmdidPai2 = $db->carregarColuna( $sqlsub2 );
	
	
	
							if($dmdidPai) $strDmdid = implode(',', $dmdidPai);
							if($dmdidPai2) $strDmdid2 = implode(',', $dmdidPai2);
	
							$strDmdidTodas = $strDmdid . ($strDmdid && $strDmdid2 ? "," : "") . $strDmdid2;
	
							$dmdidorigem = " AND d.dmdid in ($strDmdidTodas) ";
							//$dmdidorigem = " AND d.dmdid in (100568333) ";
	}
	else{
		$dmdidorigem = " AND d.dmdid in ($codigo) ";
	}
}




	// query principal
	$sql = "


		SELECT
			 DISTINCT
			 
			 CASE 
			 	--WHEN dm.contador > 0 and subdemandaaberta > 0 THEN
			 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 
			 	      and ( select count(dmdid)
			 				from demandas.demanda d1
			 				inner join demandas.tiposervico t1 on t1.tipid = d1.tipid ".str_replace('t.','and t1.',$ordidxQuery)."
			 				inner join workflow.documento d2 on d2.docid = d1.docid and d2.tpdid in (31,35)
			 				where (d2.esdid not in (95,109,100,110,135,136,170) or d2.esdid is null)
						 	and d1.dmdidorigem = d.dmdid) > 0 THEN
			 	  ''
			 	ELSE
			 		CASE
			 			WHEN doc.esdid in (93,111) and $ordidx and $celidx THEN 
			 	  			--'<input type=\'checkbox\' name=\'chkfindem[]\' id=\'chkfindem\' value=\''||d.dmdid||'\' >'
			 	  			'<div id=\'divdesmarc'||d.dmdid||'\' style=\'display:none\'><div style=\'cursor:hand\' onclick=\'desmcarcaItem('||d.dmdid||','||od.ordid||')\'><center>- Desmarcar</center></div></div> <BR>'
			 	  			|| '<input type=\'radio\' name=\'rd'||d.dmdid||'\' id=\'rd'||d.dmdid||'\' value=\'S\' onclick=\'mostraJust('||d.dmdid||')\'> <font color=blue><b>SIM</b></font> <br>'
			 	  			||
			 	  			CASE WHEN od.ordid not in (1) THEN 
			 	  					'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
			 	  					||
			 	  					'<input type=\'radio\' name=\'rd'||d.dmdid||'\' id=\'rd'||d.dmdid||'\' value=\'SF\' onclick=\'escondeJust('||d.dmdid||')\'> <font color=#DAA520><b>SIM, SEM PAUSA</b></font> <br>'
			 	  				 ELSE
			 	  				 	''
			 	  			END
			 	  			||
			 	  			'&nbsp;&nbsp;<input type=\'radio\' name=\'rd'||d.dmdid||'\' id=\'rd'||d.dmdid||'\' value=\'N\' onclick=\'escondeJust('||d.dmdid||')\'> <font color=red><b>NÃO</b></font>'
			 	  			|| '<div id=\'divtext'||d.dmdid||'\' style=\'display:none\'><br><center>Justificativa:<br><textarea rows=5 cols=30 name=\'just'||d.dmdid||'\'></textarea></center></div>' 
			 	  		END
			 END as fd,					 

			 d.dmdid as id,
							 
			 CASE 
			 	--WHEN dm.contador > 0 THEN
			 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
			 	 
							
			 			--CASE WHEN subdemandaaberta > 0 THEN
			 			CASE WHEN ( select count(dmdid)
					 				from demandas.demanda d1
					 				inner join demandas.tiposervico t1 on t1.tipid = d1.tipid ".str_replace('t.','and t1.',$ordidxQuery)."
					 				inner join workflow.documento d2 on d2.docid = d1.docid and d2.tpdid in (31,35)
					 				where (d2.esdid not in (95,109,100,110,135,136,170) or d2.esdid is null)
								 	and d1.dmdidorigem = d.dmdid) > 0 THEN
			 			
			 					CASE WHEN ( select count(dmdid)
										    from demandas.demanda d2
										    inner join demandas.tiposervico t2 on t2.tipid = d2.tipid ".str_replace('t.','and t2.',$ordidxQuery)."
										    inner join workflow.documento d3 on d3.docid = d2.docid and d3.tpdid in (31,35)
										    where d3.esdid in (93,111) -- aguardando validação
										    and d2.dmdidorigem = d.dmdid ) > 0 THEN 
			 							
			 							'<img src=\'../imagens/workflow/1.png\' > <a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 						ELSE
			 							'<img src=\'../imagens/workflow/2.png\' align=\'middle\'> <a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 					END
			 				ELSE
			 					'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 			END		 	
			 	
			 	ELSE 
			 	  '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 END AS tit,
			 
			 --CASE 
			 --	WHEN dm.contador > 0 THEN 
			 --	  '<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 --	ELSE 
			 --	  '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
			 --END AS tit,			 
			 					 
			 orddescricao AS origemdemanda,
			 t.tipnome as tipodemanda,
			 dmddsc as descricao,
			 
			 CASE 
			  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
			  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
			  END as solicitante,
			  
			 CASE
			  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
			  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
			 END AS responsavel,
			 
			 '' AS servicoexec2,
			 
			 --CASE 
			 -- 	WHEN avd.avdgeral = '1' THEN 'RUIM - ' 
			 -- 	WHEN avd.avdgeral = '2' THEN 'REGULAR - '
			 -- 	WHEN avd.avdgeral = '3' THEN 'BOM - ' 
			 -- 	WHEN avd.avdgeral = '4' THEN 'ÓTIMO - '
			 --END || avd.avsobs AS avaliacao,
			 
			 
			 
			 '' AS dataconclusao,
			 '' AS datadocfinalizada,
			 p.pridsc as prioridade,
			 d.dmdjustprioridade as justprioridade,
			 (CASE WHEN d.dmdqtde > 0 THEN 
		 		d.dmdqtde
		 	  ELSE
		 	   		'1'	 
		 	 END) AS qtdservico,
			 
			 d.dmddataclassificacao AS dataclassificacao,
			 to_char(d.dmddataclassificacao::timestamp,'DD/MM/YYYY HH24:MI') as dataclassificacao2,
			 d.dmddataclassificacaosi AS dataclassificacaosi,
			 to_char(d.dmddataclassificacaosi::timestamp,'DD/MM/YYYY HH24:MI') as dataclassificacaosi2,
			 
			 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI') AS datainclusao,
			 to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
			 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
			 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
			 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
			 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
		 	 d.dmdhorarioatendimento,
		 	 (CASE WHEN pt.tsppontuacao is not null THEN 
		 			pt.tsppontuacao
		 	   ELSE
		 	   		'0'	 
		 	  END) AS pontuacao,
		 	 lpad(pt.tsphora::varchar,2,'0') || ':' || lpad(pt.tspminuto::varchar,2,'0') as prazocatalogo,
		 	 (CASE WHEN od.ordid = 3 THEN 
		 			'00:15'
		 	   ELSE
		 	   		to_char(pt.tsptempoclassif, 'HH24:MI')	 
		 	 END) AS tempoclassifcatalogo,
		 	 '' as tempoclassif,
		 	 '' as tempoclassifsi,
		 	 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
		 	 d.dmdobstempoadicional as justtempoadicional,
		 	 esddsc as situacao,
		 	 upper(unasigla)||' - '||unadescricao as setor,
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 u.usufonenum AS tel,
			 d.docid, 
			 doc.esdid,
			 coalesce(
				(
			 	cast(pt.tsppontuacao as bigint) * (CASE WHEN d.dmdqtde > 0 THEN 
													 	d.dmdqtde
											       	 ELSE
														1          
			 										 END) * COALESCE(crtvlponto,0)
				) , '0.00') as valordemanda		 	 		 	 
			FROM
			 demandas.demanda d
		    
			 /*
			 LEFT JOIN ( select dmdidorigem,  count(dmdid) as contador from demandas.demanda group by dmdidorigem ) dm ON dm.dmdidorigem = d.dmdid
			 
			 LEFT JOIN ( select dmdidorigem,  count(dmdid) as contador2 
			 				from demandas.demanda d2
							inner join demandas.tiposervico t2 on t2.tipid = d2.tipid ".str_replace('t.','and t2.',$ordidxQuery)."
							inner join workflow.documento d3 on d3.docid = d2.docid and d3.tpdid in (31,35)
							where d3.esdid in (93,111) -- aguardando validação
							group by dmdidorigem ) dm2 ON dm2.dmdidorigem = d.dmdid 
			 				
			 LEFT JOIN ( select dmdidorigem,  count(dmdid) as subdemandaaberta 
			 				from demandas.demanda d1
			 				inner join demandas.tiposervico t1 on t1.tipid = d1.tipid ".str_replace('t.','and t1.',$ordidxQuery)."
			 				inner join workflow.documento d2 on d2.docid = d1.docid and d2.tpdid in (31,35)
			 				where (d2.esdid not in (95,109,100,110,135,136,170) or d2.esdid is null)
						 	--where (d2.esdid not in (95,109,100,110,135,136,170) or d2.esdid is null) 
						 	--where (d2.esdid not in (95,109,100,110,135,136,170) or d2.esdid is null) and t.ordid not in (1,8,11,15,16)
			 				group by dmdidorigem ) dm3 ON dm3.dmdidorigem = d.dmdid 
			 */
			 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
			 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
			 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
			 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sidid = sis.sidid
			 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid
			 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
			 LEFT JOIN demandas.anexos anx ON anx.dmdid = d.dmdid
			 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
			 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
			 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 	 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
			 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			 $inneravaliacao
			 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid
			 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and d.dmddatainclusao between con.crtdtinicio and con.crtdtfim			
				  						  
			$from

			WHERE 
			  d.dmdstatus = 'A'
			  $and
			  $where1			     					  
			  $dmdidorigem

			ORDER BY  $ordenacaox ";


			  //ver($sql,d);

			  //config paginação
			  if(!$_REQUEST['dmdidAjax']){
			  	if ($_REQUEST['numero']=='') $numero = 1;
			  	else $numero = intval($_REQUEST['numero']);

			  	$perpage = 30;
			  	$pages = 10;

			  	$sqlCount = "select count(1) from (" . $sql . ") rs";
			  	$totalRegistro = $db->pegaUm($sqlCount,0,600);

			  	$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

			  	if ($ordidx) {

			  		$RS = $db->carregar($sql,null,600);
			  		$nlinhas = count($RS);
			  		if (! $RS) $nl = 0; else $nl=$nlinhas;
			  		$reg_fim = $nlinhas;
			  		if ($nl>0) $total_reg = $totalRegistro;

			  		//	$dados = $db->carregar($sql);
			  	}
			  }
			  //fim config




  //Ajax
  if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
  	explodeSubDemandas($sql);
  	exit;
  }


  function verificaDemandaPai($dmdidx)
  {
	  global $db;
	
	  $sqlsub = "select dmdidorigem, dmdid from demandas.demanda where dmdid = ".$dmdidx;
	  $dado = $db->carregar( $sqlsub );
	
	  if ($dado[0]['dmdidorigem']){
		  return verificaDemandaPai($dado[0]['dmdidorigem']);
	  } else {
		  return $dado[0]['dmdid'];
	  }
  }


  // Inclui componente de relatórios
  include APPRAIZ. 'includes/classes/relatorio.class.inc';

  $classdata = new Data;


  include APPRAIZ . 'includes/cabecalho.inc';
  print '<br/>';
  $db->cria_aba( $abacod_tela, $url, '' );

  ?>
<html>
<head>


<script src="../includes/calendario.js"></script>

<script type="text/javascript">
		//varial global
		d = document;
	
		function finalizaDem() {
	
			var obj = document.getElementsByTagName("input");
			var ct = 0;
			for(i=0; i<obj.length; i++) {
				
				if(obj[i].type=='radio'){
					
					if(obj[i].checked == true){
						
						ct = 1;
						dmdid = obj[i].id.substr(2);
						
						if(obj[i].value=='N' || obj[i].value=='SF'){
							//if(document.getElementById('just'+dmdid).value==''){
							if(eval("document.formulario.just"+dmdid+".value == '';")){
								alert("Campo Obrigatório.");
								//document.getElementById('just'+dmdid).focus();
								eval("document.formulario.just"+dmdid+".focus();");
								return false;
								break;
							}
						}
					}
					
				}
				
			}
			if(ct == 0){
				alert("Selecione pelo menos um item para finalizar a demanda.");
				return false;
			}
			
			document.formulario.varaux.value='1';
			document.formulario.submit();
			
		}
		
		
		
		function montaSubLista(dmdid)
		{
			td 	   = document.getElementById('td_'+dmdid);
			img_mais   = document.getElementById('img_mais_'+dmdid);
			img_menos   = document.getElementById('img_menos_'+dmdid);
			
			
			// Faz uma requisição ajax, via POST
			$.ajax({type: "POST",
			   		url: "demandas.php?modulo=principal/finalizaDemandas&acao=A",
			   		data: "dmdidAjax="+dmdid,
			   		success: function(msg){
		   		
						td.style.display = '';
						//td.style.padding = '5px';
						img_mais.style.display = 'none';
						img_menos.style.display = '';
						td.innerHTML = msg;
			   		}
			 });

			 /*
			var req = new Ajax.Request('demandas.php?modulo=principal/finalizaDemandas&acao=A', {
					        method:     'post',
					        parameters: '&dmdidAjax=' + dmdid,
					        onComplete: function (res)
					        {	
								td.style.display = '';
								//td.style.padding = '5px';
								img_mais.style.display = 'none';
								img_menos.style.display = '';
								td.innerHTML = res.responseText;
					        }
					  });
			*/
		}
	
		function desmontaSubLista(dmdid)
		{
			td 	   = document.getElementById('td_'+dmdid);
			img_mais   = document.getElementById('img_mais_'+dmdid);
			img_menos   = document.getElementById('img_menos_'+dmdid);
			
			td.innerHTML = '';
			td.style.display = 'none';
			img_mais.style.display = '';
			img_menos.style.display = 'none';
		
		}
	
		
		function marcatodas(){
			
			document.getElementById('mdtodos').innerHTML = '<a href="javascript:desmarcatodas();"> - Desmarcar todos</a>';
	
			var obj = document.getElementsByTagName("input");
			var ct = 0;
			for(i=0; i<obj.length; i++) {
				
				if(obj[i].type=='radio'){
					
					if(obj[i].value=='S'){
						obj[i].checked = true;
						dmdid = obj[i].id.substr(2);
						document.getElementById('divtext'+dmdid).style.display = 'none';
						document.getElementById('divdesmarc'+dmdid).style.display = '';
					}
					
				}
				
			}
			
		}
		
		function desmarcatodas(){
			
			document.getElementById('mdtodos').innerHTML = '<a href="javascript:marcatodas();"> - Marcar todos</a>';
			
			var obj = document.getElementsByTagName("input");
			var ct = 0;
			for(i=0; i<obj.length; i++) {
				
				if(obj[i].type=='radio'){
					
					//if(obj[i].value=='S'){
						obj[i].checked = false;
						dmdid = obj[i].id.substr(2);
						document.getElementById('divtext'+dmdid).style.display = 'none';
						document.getElementById('divdesmarc'+dmdid).style.display = 'none';
					//}
					
				}
				
			}
		}	
		
		/*
		function abreTodasSub(){
	
			form = document.getElementsByTagName("img");
			for(i=0; i<form.length; i++) {
				if( form[i].id.substr(0,9) == "img_mais_") {
					dmdid = form[i].id;
					dmdid = dmdid.toString().replace("img_mais_","");
					//alert(dmdid);
					montaSubLista(dmdid);
				}
			}		
			
		}
		*/
		//abre todas as subdemandas	
		//abreTodasSub();
		
		function mostraJust(dmdid){
			divtext = document.getElementById('divtext'+dmdid);
			//just 	= document.getElementById('just'+dmdid);
			//just.value = '';
			eval("document.formulario.just"+dmdid+".value = '';");
			divtext.style.display = 'none';
	
			divdesmarc = document.getElementById('divdesmarc'+dmdid);
			divdesmarc.style.display = '';		
		}
	
		function escondeJust(dmdid){
			divtext = document.getElementById('divtext'+dmdid);
			divtext.style.display = '';
	
			divdesmarc = document.getElementById('divdesmarc'+dmdid);
			divdesmarc.style.display = '';
	
			
		}
	
		function desmcarcaItem(dmdid, ordid){

			//desmarcItem = document.getElementById('rd'+dmdid);
			//desmarcItem.checked = false;		
			//desmarcItem..checked = false;
			if(ordid != '1'){
				eval("document.formulario.rd"+dmdid+"[0].checked = false;");
				eval("document.formulario.rd"+dmdid+"[1].checked = false;");
				eval("document.formulario.rd"+dmdid+"[2].checked = false;");
			}else{
				eval("document.formulario.rd"+dmdid+"[0].checked = false;");
				eval("document.formulario.rd"+dmdid+"[1].checked = false;");
			}			
	
			divdesmarc = document.getElementById('divdesmarc'+dmdid);
			divdesmarc.style.display = 'none';
			divtext = document.getElementById('divtext'+dmdid);
			divtext.style.display = 'none';		
	
					
			
		}
		
		/*
		* Faz requisição via ajax
		* Filtra o andar, atravéz do parametro passado 'lcaid'
		*/
		function filtraAndar(lcaid) {
	
			if(!lcaid) lcaid = '999999';
			
			td     = d.getElementById('andardemanda');	
			select = td.getElementsByTagName('select')[0];
			//trSis  = d.getElementById('tr_sistema');
	
			//trSis.style.display = paramSystem == 1 ? (navigator.appName == 'Netscape' ? 'table-row' : 'block') : 'none';	
			
			// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
			if (select)
				select.disabled = true;


			// Faz uma requisição ajax, via POST
			$.ajax({type: "POST",
			   		url: "demandas.php?modulo=principal/finalizaDemandas&acao=A",
			   		data: "lcaidAjax="+lcaid,
			   		success: function(msg){
		   		
						td.innerHTML = msg;

			   		}
			 });

			/*			
			// Faz uma requisição ajax, passando o parametro 'ordid', via POST
			var req = new Ajax.Request('demandas.php?modulo=principal/finalizaDemandas&acao=A', {
									        method:     'post',
									        parameters: '&lcaidAjax=' + lcaid,
									        onComplete: function (res)
									        {			
												td.innerHTML = res.responseText;
									        }
									  });
			*/
		}

		function pesqAvanc()
		{
			pesquisatit  = d.getElementById('pesquisatit');
			pesquisacorpo  = d.getElementById('pesquisacorpo');

			//pesquisacorpo.style.display = 'none';

			//td.innerHTML

			if(pesquisacorpo.style.display){
				pesquisacorpo.style.display = '';
				pesquisatit.innerHTML = "[-] Fechar Pesquisa Avaçada";
			}
			else{
				pesquisacorpo.style.display = 'none';
				pesquisatit.innerHTML = "[+] Abrir Pesquisa Avaçada";
			}
			
		}
	
		
	</script>

</head>
<body>
<form action="" method="POST" name="formulario"><input type="hidden" name="varaux" value="" /> <input type="Hidden" name="numero" value="" />


<table width="100%" border="0" cellspacing="0" cellpadding="3" align="center"  >
	<tr>
		<td bgcolor="#DCDCDC" class="TituloTela">
			<div style="margin-left: 17%;">Validar Demandas</div>
		</td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" >
			<div style="margin-left: 15%;">Marque os campos Sim/Não para validar as demandas.</div>
			
			<div id="pesquisatit" onclick="pesqAvanc()" style="color: blue; font-weight: bold; cursor: hand;">
				[+] Abrir Pesquisa Avaçada
			</div>
		</td>
	</tr>
</table>

<div id="pesquisacorpo" style="display: none">

	<table width="95%"  border="0" cellpadding="3" cellspacing="0">
		<tr>
			<td style="background-color: #e9e9e9; color: #404040;">
			<table  border="0" cellpadding="0" cellspacing="0">
				<tr>
					<td style="background-color: #e9e9e9; color: #404040;">
					
					<div style="margin-left: 25px;">
					
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td width="10%" valign="top"><B>Código:</B> <br>
								<?= campo_texto( 'codigo', 'N', 'S', '', 10, 15, '', '' ); ?></td>
								<td><B>Setor:</B> <br>
								<?php
								$unaid = $_REQUEST['unaid'];
								$sql = " SELECT
													  unaid AS codigo, 
												  	  upper(unasigla)||' - '||unadescricao as descricao 
													 FROM
													  demandas.unidadeatendimento 
													 WHERE
													  unastatus = 'A'
													  order by unasigla, unadescricao";
								$db->monta_combo( "unaid", $sql, 'S', 'Filtro...', '', '','','320','','unaid');
								?></td>
							</tr>
		
							<tr>
								<td colspan=2 align="left">
								<div style="float: left;"><B>Edifício:</B> <br>
								<?php
								$sql = "SELECT
													 lcaid AS codigo,
													 lcadescricao AS descricao
													FROM 
													 demandas.localatendimento
													ORDER BY 
													 lcadescricao";
								$lcaid = $_REQUEST['lcaid'];
								$db->monta_combo( 'lcaid', $sql, 'S', 'Filtro...', 'filtraAndar', '' );
								?> &nbsp;&nbsp;</div>
		
								<div style="float: left; margin-left: 5px;"><B>Andar:</B> <br>
								<div id='andardemanda'><?
								if($lcaid){
									$sql = sprintf("SELECT
																		 laa.laaid AS codigo,
																		 aa.anddescricao AS descricao
																		 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
																		FROM 
																		 	demandas.localandaratendimento laa
																			inner join demandas.andaratendimento aa on laa.andid = aa.andid
																		WHERE
																		 laa.lcaid = %d 
																		ORDER BY 
																		 aa.anddescricao", 
									$lcaid);
									$laaid = $_REQUEST['laaid'];
									$db->monta_combo( 'laaid', $sql, 'S', 'Filtro...', '', '', '', '', '', '', '', $laaid );
								}
								else{
									echo "<select name=''><option>Filtro...</option></select>";
								}
								?></div>
								</div>
		
								<div style="float: left; margin-left: 8px;">&nbsp;&nbsp; <B>Sala:</B>
								<br>
								<?= campo_texto( 'sala', 'N', 'S', '', 15, 20, '', ''); ?></div>
		
								</td>
							</tr>
							<tr>
								<td colspan=2 align="left">
								<div style="float: left;"><B>Origem da demanda:</B> <br>
								<?
									
								if($ordidx){
									$sqlord = "SELECT
														 t.ordid AS codigo,
														 t.orddescricao AS descricao
														FROM
														 demandas.origemdemanda t
														WHERE
														 t.ordstatus = 'A'
														 and $ordidx
														 $ordidxCombo
														ORDER BY
														 t.orddescricao;";
		
									$ordid = $_REQUEST['ordid'];
		
									$db->monta_combo( "ordid", $sqlord, 'S', 'Filtro...', '', '','','','','ordid');
		
								}
		
								else{
									/*
									 $sql = "SELECT
									 ordid AS codigo,
									 orddescricao AS descricao
									 FROM
									 demandas.origemdemanda
									 WHERE
									 ordstatus = 'A'
									 and ordid in (1)
									 ORDER BY
									 orddescricao;";
									 */
									echo "<select name='ordid'><option value=''>Filtro...</option></select>";
								}
		
									
								?></div>
		
								<div style="float: left; margin-left: 5px;">&nbsp;&nbsp;<B>Palavra-Chave:</B>
								<br>
								<?= campo_texto( 'palavra_chave', 'N', 'S', '', 41, 60, '', '', '', '', '', 'id="palavra_chave"', '', ''); ?>
								</div>
		
								</td>
							</tr>
							<tr>
								<td colspan=2 align="left">
								<div style="float: left;"><B>Avaliação:</B> <br>
								<?
								$avaliacao = $_REQUEST['avaliacao'];
									
								$sql = " select '1' as codigo, 'Ruim' as descricao
						  						   union all
												   select '2' as codigo, 'Regular' as descricao
												   union all
												   select '3' as codigo, 'Bom' as descricao
												   union all
												   select '4' as codigo, 'Ótimo' as descricao
												   ";
		
								$db->monta_combo( "avaliacao", $sql, 'S', 'Filtro...', '', '','','','','avaliacao');
								?></div>
		
								<div style="float: left; margin-left: 5px;">&nbsp;&nbsp;<B>Ordernar
								por:</B> <br>
								<?
									
								$ordenacao = $_REQUEST['ordenacao'];
									
								$sql = " select '1' as codigo, 'Código' as descricao
						  						   union all
												   select '2' as codigo, 'Prioridade' as descricao
												   union all
												   select '3' as codigo, 'Quantidade de Serviço' as descricao
												   union all
												   --select '4' as codigo, 'Avaliação' as descricao
												   --union all
												   select '5' as codigo, 'Data Abertura' as descricao
												   union all
												   select '6' as codigo, 'Previsão de Início' as descricao
												   union all
												   select '7' as codigo, 'Previsão de Término' as descricao
												   union all
												   select '8' as codigo, 'Data Conclusão' as descricao
												   union all
												   select '9' as codigo, 'Pontuação da Demanda' as descricao
												   union all
												   select '10' as codigo, 'Valor da Demanda' as descricao
												   
												   ";
		
									
		
								echo "&nbsp;";
								$db->monta_combo( "ordenacao", $sql, 'S', 'Filtro...', '', '','','','','ordenacao');
		
		
		
								$ordenacao2 = $_REQUEST['ordenacao2'];
		
								$sql = " select 'ASC' as codigo, 'Crescente' as descricao
						  						   union all
												   select 'DESC' as codigo, 'Decrescente' as descricao
												   ";
		
									
		
								echo "&nbsp;";
								$db->monta_combo( "ordenacao2", $sql, 'S', 'Filtro...', '', '','','','','ordenacao2');
		
								?></div>
		
								</td>
							</tr>
							<tr>
								<td colspan=2 align="left">
								<div style="float: left;"><B>Período de Abertura:</B> <br>
									<?= campo_data( 'dtini', 'S', 'S', '', '' ); ?>
									&nbsp;&nbsp;
									a
									&nbsp;&nbsp;&nbsp;
									<?= campo_data( 'dtfim', 'S', 'S', '', '' ); ?>
								</div>
								</td>
							</tr>
		
						</table>
		
						<div style="float: left; margin-top: 15px;">
							<input style="cursor: pointer;" type="submit" name="pesquisar" value="Pesquisar">
						</div>
						
						</td>
					</tr>
				</table>
			</div>
	
			</td>
			<td style="background-color: #e9e9e9; color: #404040;"></td>
		</tr>
	
	</table>

</div>

<style type="text/css">

	table 
	{
		padding: 0;
		border: 0;
		outline: 0;
		vertical-align: baseline;
		background: transparent;
		border-collapse: collapse;
		border-spacing: 0;
	}
	
	table.tbl1
	{
		width: 100%;
	}
	
	table.tbl1 tr td,
	table.tbl1 tr th
	{
		border-right: solid 1px #888;
	}
	
	table.tbl1 tr td
	{
		text-align: center;
	}
	
	table.tbl1 td.tam300
	{
		width: 300px;
	}
	table.tbl1 td.tam200
	{
		width: 200px;
	}
	table.tbl1 td.tam100
	{
		width: 100px;
	}
	

</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script>
(function($) {
	$.fn.createScrollableTable = function(options) {

		var defaults = {
			width: '3810px',
			height: '600px',
			border: 'solid 1px #888'
		};
		var options = $.extend(defaults, options);

		return this.each(function() {
			var table = $(this);
			prepareTable(table);
		});

		function prepareTable(table) {
			var tableId = table.attr('id');

			// wrap the current table (will end up being just body table)
			var bodyWrap = table.wrap('<div></div>')
									.parent()
									.attr('id', tableId + '_body_wrap')
									.css({
										width: options.width,
										height: options.height,
										overflow: 'auto'
									});

			// wrap the body
			var tableWrap = bodyWrap.wrap('<div></div>')
									.parent()
									.attr('id', tableId + '_table_wrap')
									.css({
										overflow: 'hidden',
										display: 'inline-block',
										border: options.border
									});

			// clone the header
			var headWrap = $(document.createElement('div'))
									.attr('Id', tableId + '_head_wrap')
									.prependTo(tableWrap)
									.css({
										width: options.width,
										overflow: 'hidden'
									});

			var headTable = table.clone(true)
									.attr('Id', tableId + '_head')
									.appendTo(headWrap)
									.css({
										'table-layout': 'fixed'
									});

			var bufferCol = $(document.createElement('th'))
									.css({
										width: '100%'
									})
									.appendTo(headTable.find('thead tr'));

			// remove the extra html
			headTable.find('tbody').remove();
			table.find('thead').remove();

			// size the header columns to match the body
			var allBodyCols = table.find('tbody tr:first td');
			headTable.find('thead tr th').each(function(index) {
				var desiredWidth = getWidth($(allBodyCols[index]));
				$(this).css({ width: desiredWidth + 'px' });
			});
		}

		function getWidth(td) {
			if ($.browser.msie) { return $(td).outerWidth() - 1; }
			if ($.browser.mozilla) { return $(td).width(); }
			if ($.browser.safari) { return $(td).outerWidth(); }
			return $(td).outerWidth();
		};


	};

})(jQuery);






$(document).ready(function() {
	/* zebra stripe the tables (not necessary for scrolling) */
    var tbl = $("table.tbl1");
    addZebraStripe(tbl);
    addMouseOver(tbl);
	
	/* make the table scrollable with a fixed header */
    $("table.scroll").createScrollableTable({
        width: '3810px',
        height: '600px'
    });

});

function addZebraStripe(table) {
    table.find("tbody tr:odd").addClass("alt");
}

function addMouseOver(table) {
    table.find("tbody tr").hover(
            function() {
                $(this).addClass("over");
            },
            function() {
                $(this).removeClass("over");
            }
        );
}


</script>				


<center>

<div class="SubtituloEsquerda" style="padding: 0; margin: 0 auto; text-align: center; width: 100%"><br>Relatório de Atividades Técnicas - Demandas</div>
<div id="mdtodos" class="SubtituloEsquerda"	style="padding: 0; margin: 0 auto; text-align: left; width: 100%"><a href="javascript:marcatodas();"> - Marcar todos</a></div>
<div style="display: none; margin: 0 auto; width: 1px;"	id="alteraDemanda"></div>
<!-- 
<div style="margin: 0 auto; padding: 0; height: 400px; width: 1345px; background-color: #eee; border: none;" class="div_rolagem" id="exibeDemandas">
-->


<table align="center" border="0" cellspacing="0" cellpadding="0" class="tbl1 scroll">
	<thead>
		<tr>
			<th class="tam300" valign="top"><strong>VALIDAR?</strong></th>
			<th class="tam100" valign="top"><strong>Cód</strong></th>
			<th class="tam100" valign="top"><strong>Assunto</strong></th>
			<th class="tam100" valign="top"><strong>Analista / Técnico Responsável</strong></th>
			<th class="tam100" valign="top"><strong>Solicitante</strong></th>
			<th class="tam100" valign="top"><strong>Prioridade</strong></th>
			<th class="tam100" valign="top"><strong>Justificativa da Prioridade</strong></th>
			<th class="tam100" valign="top"><strong>Setor</strong></th>
			<th class="tam100" valign="top"><strong>Local</strong></th>
			<th class="tam100" valign="top"><strong>Origem / Tipo</strong></th>
			<th class="tam200" valign="top"><strong>Descrição</strong></th>
			<th class="tam200" valign="top"><strong>Serviço Executado</strong></th>
			<th class="tam100" valign="top"><strong>Quantidade de Serviço</strong></th>
			<th class="tam100" valign="top"><strong>Data Abertura</strong></th>
			<th class="tam100" valign="top"><strong>Tempo de Classificação</strong></th>
			<th class="tam100" valign="top"><strong>Data de Classificação SI</strong></th>
			<th class="tam100" valign="top"><strong>Tempo de Classificação Catálogo SI</strong></th>
			<th class="tam100" valign="top"><strong>Tempo de Classificação SI</strong></th>
			<th class="tam100" valign="top"><strong>Previsão	de Início</strong></th>
			<th class="tam100" valign="top"><strong>Previsão	de Término</strong></th>
			<th class="tam100" valign="top"><strong>Prazo Previsto Catálago (hh:mm)</strong></th>
			<th class="tam100" valign="top"><strong>Tempo Adicional Atendimento</strong></th>
			<th class="tam100" valign="top"><strong>Justificatica do Tempo Adicional</strong></th>
			<th class="tam100" valign="top"><strong>Prazo Previsto Calculado (+ Pausa)</strong></th>
			<th class="tam200" valign="top"><strong>Tempo da Pausa</strong></th>
			<th class="tam200" valign="top"><strong>Observações</strong></th>
			<th class="tam100" valign="top"><strong>Data Conclusão</strong></th>
			<th class="tam100" valign="top"><strong>Duração do Atendimento</strong></th>
			<th class="tam200" valign="top"><strong>Avaliação</strong></th>
			<th class="tam200" valign="top"><strong>Histórico Validação</strong></th>
			<th class="tam100" valign="top"><strong>Pontuação da Demanda</strong></th>
			<th class="tam100" valign="top"><strong>Total da Pontuação</strong></th>
			<th class="tam100" valign="top"><strong>Valor da Demanda (R$)</strong></th>
		</tr>
	</thead>
	<tbody>

			<?php if ($ordidx){?>
			<?php if ( $RS ) : ?>

			<?
			//pega o valor por demanda
			/*$sqlvl = "select vlpvalorponto from demandas.valorpontuacao ";
			$valorpordemanda = $db->PegaUm($sqlvl);
			if(!$valorpordemanda) $valorpordemanda = 0;*/
			?>
			<?$cor = ''; ?>
			<?$ct = 0; ?>
			<?foreach ( $RS as $dado ) :

			$total_minuto = 0;
			$textotempopausa = "";
			$prazoatendimento = "";
			$tempodecorrido = "";
			//$tempoclassif = "";
			$observacao = "";
			$avaliacaousu = "";
			$totalpontuacao = "";
			$valordemanda = 0;
			
			
			//pega valores do left join da sql
			if($dado['docid']){
				$sqld = "select a.docid, max(b.cmdid) as cmdid, to_char(max(htddata)::timestamp,'DD-MM-YYYY HH24:MI') as datahist, to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:SS') as datahist2, b.cmddsc as servicoexec
							 		from 	workflow.historicodocumento a
							 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
							 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
							 	where aedid in (146, 191) and c.esdid in (93,95,109,111,170)
								and a.docid = ".$dado['docid']."
							  group by a.docid, b.cmddsc";
				$dadoleft = $db->pegaLinha($sqld);
				
				if($dadoleft){
					$dado['dataconclusao'] = $dadoleft['datahist'];
					$dado['datadocfinalizada'] = $dadoleft['datahist2'];
					$dado['servicoexec2'] = $dadoleft['servicoexec'];
				} 
				
				
				/*ja esta recuperando na query principal
				$sqlc = "select to_char(min(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
						from 	workflow.historicodocumento 
						where docid = ".$dado['docid'];
				$dado['dataclassificacao'] = $db->pegaUm($sqlc);
				*/
			}
			

			//calcula prazo previsto
			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento']);


			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa
													from demandas.pausademanda p 
													inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
													where p.dmdid = ". $dado['id'];

			$dadosp = $db->carregar($sql);

			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
														from demandas.demanda 
														where dmdid = ". (int) $dado['id'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
					//$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$horasx.":".$minutosx."</font>";
				}


			}
			$textotempopausa .= "</div>";


			//$tempototalpausa = 0;
			//dbg($dado['id'] . ' = ' . $tempototalpausa."<br>");

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);
			//$seg_ini	= substr($dado['dmddatainiprevatendimento'],17,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			//echo "<br>data final = ".$dataFinalx;

			if($dado['dmddatainiprevatendimento'] && $dataFinalPrazoPrev){
				$prazoatendimento = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			}

			//echo (int) $dado['id'] ." - ". $dado['datadocfinalizada']."<br>";

			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);

				//$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatafimprevatendimento']))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				//$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dado['datadocfinalizada']))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				//$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'] , 'tempoEntreDadas', 'string','yyyy/mm/dd');

				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$tempodecorrido = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');


				if($total_tempodecorrido > $total_prazoatendimento){
					$tempodecorrido = "<font color=red>". $tempodecorrido . "</font>";
					$dataconclusao = "<font color=red>". $dataconclusao . "</font>";
				}
				else{
					$tempodecorrido = "<font color=blue>". $tempodecorrido . "</font>";
					$dataconclusao = "<font color=blue>". $dataconclusao . "</font>";
				}
			}

/*
			//pega o tempo de classificação da demanda
			$dtabertura = $dado['dataabertura'];
			$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
			//echo $dado['id'] . " - " . $dtabertura . " - " . $dado['dataclassificacao']."<br>";
			
			if($dado['dataclassificacao']){
				$tempoclassif = $classdata->diferencaEntreDatas(  $dtabertura , $dado['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			}
			if(!$tempoclassif) $tempoclassif = "0 minuto";
*/

			//pega o tempo da 1º classificação da demanda
			if($dado['dataabertura'] && $dado['dataclassificacao']){
				$dtabertura = $dado['dataabertura'];
				
				$hor_tc	= 0;
				$min_tc	= 15;
				
				$ano_ini	= substr($dtabertura,6,4);
				$mes_ini	= substr($dtabertura,3,2);
				$dia_ini	= substr($dtabertura,0,2);
				$hor_ini	= substr($dtabertura,11,2);
				$min_ini	= substr($dtabertura,14,2);
	
				$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
				$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);
			
				
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dataclassificacao'])));
				
				
				$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
				
				//echo $dado['nudemanda'] ." = ". $dtxini . " > " . $dtxfim.' = ('.($dtxini == $dtxfim).')<br>';
				
				//echo $dado['nudemanda'] . " - " . $dtabertura . " - " . $dado['dataclassificacao']."<br>";
				if($dtxfim > $dtxini){
					$dado['tempoclassif'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dado['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				else{
					$dado['tempoclassif'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dado['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				
			}
			if(!$dado['tempoclassif']) $dado['tempoclassif'] = "0 minuto";
	
			
			//pega o tempo da 2º classificação da origem: serviço de impressão
			if($dado['dataclassificacao'] && $dado['dataclassificacaosi']){
				$dtabertura = $dado['dataclassificacao'];
				
				
				$hor_tc	= substr($dado['tempoclassifcatalogo'],0,2);
				$min_tc	= substr($dado['tempoclassifcatalogo'],3,2);
				
				$ano_ini	= substr($dtabertura,0,4);
				$mes_ini	= substr($dtabertura,5,2);
				$dia_ini	= substr($dtabertura,8,2);
				$hor_ini	= substr($dtabertura,11,2);
				$min_ini	= substr($dtabertura,14,2);
					
				$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
				$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);

				
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dataclassificacaosi'])));
								
				
				$dtabertura = substr($dtabertura,0,4) ."-". substr($dtabertura,5,2) ."-". substr($dtabertura,8,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
				
				//echo $dado['nudemanda'] ." = ". $dtxini . " > " . $dtxfim.' = ('.($dtxini == $dtxfim).')<br>';
				
				//echo $dado['nudemanda'] . " - " . $dtabertura . " - " . $dado['dataclassificacao']."<br>";
				if($dtxfim > $dtxini){
					$dado['tempoclassifsi'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dado['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				else{
					$dado['tempoclassifsi'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dado['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
				}
				
			}
			if(!$dado['tempoclassifsi']) $dado['tempoclassifsi'] = "0 minuto";
			
			

			//pega o calculo total de pontuação
			if($dado['qtdservico'] && $dado['pontuacao']){
				$totalpontuacao = (int) $dado['qtdservico'] * (int) $dado['pontuacao'];
			}

			//pega o calculo valor da demanda
			/*if($totalpontuacao){
				//echo $dado['totalpontuacao'] . " * ". $valordemanda."<br>";
				$valordemanda = $totalpontuacao * (float) $valorpordemanda;
			}*/

			
			//pega observação
			$sqlob = "select u.usunome, obsdsc, to_char(obsdata::timestamp,'DD/MM/YYYY HH24:MI') AS obsdata
												  from demandas.observacoes o
												  inner join seguranca.usuario u on u.usucpf = o.usucpf 
												  where obslog IS NULL and obsstatus='A' and dmdid = ".$dado['id'];
			$ob = array();
			$ob = $db->carregar( $sqlob );
			$observacao = "";
			if($ob){
				$observacao = '<div align=left>';
				foreach($ob as $obx){
					$observacao .= $obx['obsdsc'] .'<br><b>Incluído por: '. $obx['usunome'] . '<br> Data: '. $obx['obsdata'] .'</b><br><br>';
				}
				$observacao .= '</div>';
			}
			
			//pega avaliação
			$sqlav = "SELECT to_char(avddata::timestamp,'DD/MM/YYYY HH24:MI:SS') as avddata,
														(CASE avdgeral
														 	WHEN '1' THEN 'RUIM'
														 	WHEN '2' THEN 'REGULAR'
														 	WHEN '3' THEN 'BOM'
														 	WHEN '4' THEN 'ÓTIMO'
														END) AS avdgeral,
										 				avsobs 
												FROM demandas.avaliacaodemanda 
												where avdstatus='A' and dmdid = ". (int) $dado['id'] . "
												order by avddata desc";	
			$av = array();
			$av = $db->carregar( $sqlav );
			$avaliacaousu = "";
			if($av){
				$avaliacaousu = '<div align=left>';
				foreach($av as $avx){
					$avaliacaousu .= '<b>Data: '. $avx['avddata'] . '<br> Avaliação: '. $avx['avdgeral'] .'</b>';
					if($avx['avsobs']) $avaliacaousu .= '<br><b>Justificativa: '. $avx['avsobs'] . '</b>';
					$avaliacaousu .= "<br><br>";
				}
				$avaliacaousu .= '</div>';
			}
			

			//verifica se é a 1º validação/invaliadação
			if($dado['docid']){
				$sqlval = "select a.hstid, a.docid, u.usunome as nomegestor, b.cmddsc as comentario,
														CASE WHEN a.aedid in (278,279) THEN 
																  '<font color=red>INVALIDADA</font>' 
															 WHEN a.aedid in (368) THEN 
																  '<font color=#DAA520>VALIDADA SEM PAUSA</font>'
															 ELSE 
															 	  '<font color=blue>VALIDADA</font>' 
														END as situacao, 
														to_char(a.htddata::timestamp,'DD/MM/YYYY HH24:MI:SS') as datagestor
															from 	workflow.historicodocumento a
																inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
																inner join seguranca.usuario u on u.usucpf = a.usucpf
																left join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid
														where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada SEM PAUSA
														and c.docid = ".$dado['docid']."
														order by a.htddata desc ";

				$val = array();
				$val = $db->carregar( $sqlval );
				$revalidacao = "";
				if($val){
					$revalidacao = '<div align=left>';
					foreach($val as $valx){
						$revalidacao .= '<b>Data:</b> '. $valx['datagestor'] . '<br><b>'. $valx['situacao'] .' POR '.$valx['nomegestor'].'</b>';
						if($valx['comentario']) $revalidacao .= '<br><b>Justificativa:</b> '. $valx['comentario'];
						$revalidacao .= "<br><br>";
					}
					$revalidacao .= '</div>';
				}
			}

			// pinta linha caso seja revalidação
			if( ($dado['esdid'] == '93' || $dado['esdid'] == '111') && $revalidacao){
				$cor = '#FFA07A' ;
			}
			else{
				$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
			}

			?>
				<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
					<td class="tam300" nowrap="nowrap"><?=$dado['fd']?>&nbsp;</span></td>
					<td class="tam100" align="center" ><a style='color: #0066CC' href='demandas.php?modulo=principal/lista&acao=A&dmdid=<?=$dado['id']?>'><?=$dado['id'] ?></a></td>
					<td class="tam100">&nbsp;<?=$dado['tit']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['responsavel'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['solicitante'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['prioridade'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['justprioridade'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['setor'] ?>&nbsp;</td>
					<td class="tam100"><b>Edifício:</b>
					<?=$dado['edificio'] ?> <br>
					<b>Andar:</b> <?=$dado['andar'] ?> <br>
					<b>Sala:</b> <?=$dado['sala'] ?> <br>
					<b>Telefone:</b> <?=$dado['tel'] ?></td>
					<td class="tam100">&nbsp;<?=$dado['origemdemanda'] ?>
					/ <br>
					<?=$dado['tipodemanda'] ?>&nbsp;</td>
					<td class="tam200">&nbsp;<?=$dado['descricao'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['servicoexec2'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['qtdservico'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['datainclusao'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['tempoclassif']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['dataclassificacaosi2']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['tempoclassifcatalogo'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['tempoclassifsi']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['datainicio'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['datafim'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['prazocatalogo']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['tempoadicional']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['justtempoadicional']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$prazoatendimento?>&nbsp;</td>
					<td class="tam200">&nbsp;<?=$textotempopausa?>&nbsp;</td>
					<td class="tam200">&nbsp;<?=$observacao?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['dataconclusao'] ?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$tempodecorrido?>&nbsp;</td>
					<td class="tam200">&nbsp;<?=$avaliacaousu?>&nbsp;</td>
					<td class="tam200">&nbsp;<?=$revalidacao?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$dado['pontuacao']?>&nbsp;</td>
					<td class="tam100">&nbsp;<?=$totalpontuacao?>&nbsp;</td>
					<td class="tam100" align="right">&nbsp;<?=number_format($dado['valordemanda'], 2, ',', '.')?>&nbsp;</td>
				</tr>
				<tr style='background-color: #F7F7F7'>
					<td colspan=29 style='padding-left: 20px;'
						id='td_<?=$dado['id'] ?>'></td>
				</tr>
				<?//explodeSubDemandas($dado['id']);?>
				<?$ct++;?>
				<?endforeach;?>
				<?php else : ?>
				<tr>
					<td colspan=29
						style="text-align: center; padding: 15px; background-color: #fafafa; color: red; font-weight: bold; font-size: 10px;">
					Não existem demandas para finalizar.</td>
				</tr>
				<?php endif; ?>
				<?php }else{?>
				<tr>
					<td colspan=29
						style="text-align: center; padding: 15px; background-color: #fafafa; color: red; font-weight: bold; font-size: 10px;">
					Você não possui permissão para finalizar demandas, pois não existe
					atribuição de áreas ao seu perfil.</td>
				</tr>
				<?php }?>

			
	</tbody>

</table>



				<?php //paginação
					
				print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
				include APPRAIZ."includes/paginacao.inc";
				print '</td></tr></table>';
				print '<script language="JavaScript">function pagina(numero) {document.formulario.numero.value=numero;document.formulario.submit();}</script>';
				?>


<table width="96%" align="center" border=0>
	<tr>
		<td><br>
		<img src="/imagens/seta_filho.gif"> <input style="cursor: pointer;"
			type="button" onclick="finalizaDem();" name="finalizademanda"
			value="Validar Demandas" /></td>
	</tr>
</table>
<!--  
</div>
--></center>


</form>
</body>
</html>


		<?

		function explodeSubDemandas($sqlx)
		{

			global $db;

			// Inclui componente de relatórios
			include APPRAIZ. 'includes/classes/relatorio.class.inc';

			$classdata = new Data;

			$dadossub = $db->carregar($sqlx);

			?>
<table width="95%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr>
		<td align="CENTER" valign="top" class="title" bgcolor="#dfdfdf"	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>VALIDAR?</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Cód</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Assunto</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Situação</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Analista / Técnico Responsável</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Solicitante</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Prioridade</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Justificativa da Prioridade</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Setor</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Local</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Origem / Tipo</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Descrição</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Serviço Executado</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Quantidade	de Serviço</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data Abertura</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Tempo de Classificação</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data de Classificação SI</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Tempo de Classificação Catálogo SI</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Tempo de Classificação SI</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Previsão de Início</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Previsão de Término</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Prazo Previsto Catálago (hh:mm)</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Tempo Adicional Atendimento</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Justificatica do Tempo Adicional</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Prazo Previsto Calculado (+ Pausa)</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Tempo da Pausa</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Observações</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data Conclusão</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Duração do Atendimento</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Avaliação</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Histórico Validação</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Pontuação da Demanda</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Total da Pontuação</strong></td>
		<td align="" valign="top" class="title" bgcolor="#dfdfdf" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Valor da Demanda (R$)</strong></td>
	
	</tr>
	<?php if (  $dadossub  ) : ?>

	<?
	//pega o valor por demanda
	/*$sqlvl = "select vlpvalorponto from demandas.valorpontuacao ";
	$valorpordemanda = $db->PegaUm($sqlvl);
	if(!$valorpordemanda) $valorpordemanda = 0;*/
	?>

	<?$cor = ''; ?>
	<?foreach ( $dadossub as $dadosub ) :

	$total_minuto = 0;
	$textotempopausa = "";
	$prazoatendimento = "";
	$tempodecorrido = "";
	//$tempoclassif = "";
	$observacao = "";
	$avaliacaousu = "";
	$totalpontuacao = "";
	$valordemanda = 0;
	
	
	//pega valores do left join da sql
	if($dadosub['docid']){
		$sqld = "select a.docid, max(b.cmdid) as cmdid, to_char(max(htddata)::timestamp,'DD-MM-YYYY HH24:MI') as datahist, to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:SS') as datahist2, b.cmddsc as servicoexec
					 		from 	workflow.historicodocumento a
					 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
					 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
					 	where aedid in (146, 191) and c.esdid in (93,95,109,111,170)
						and a.docid = ".$dadosub['docid']."
					  group by a.docid, b.cmddsc";
		$dadoleft = $db->pegaLinha($sqld);
		
		if($dadoleft){
			$dadosub['dataconclusao'] = $dadoleft['datahist'];
			$dadosub['datadocfinalizada'] = $dadoleft['datahist2'];
			$dadosub['servicoexec2'] = $dadoleft['servicoexec'];
		} 
		
		
		$sqlc = "select to_char(min(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
				from 	workflow.historicodocumento 
				where docid = ".$dadosub['docid'];
		$dadosub['dataclassificacao'] = $db->pegaUm($sqlc);
	}	


	//calcula prazo previsto
	$total_minuto = calculaTempoMinuto($dadosub['dmddatainiprevatendimento'], $dadosub['dmddatafimprevatendimento'], $dadosub['dmdhorarioatendimento']);


	//verifica pausa da demanda
	$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa
								from demandas.pausademanda p 
								inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
								where p.dmdid = ". $dadosub['id'];

	$dadosubsp = $db->carregar($sql);

	$flagIndeterminado = '';
	$tempototalpausa = 0;
	$textotempopausa = "<div align='left'>";

	if($dadosubsp){
		foreach($dadosubsp as $dadosubp){

			if($dadosubp['pdmdatainiciopausa'] && $dadosubp['pdmdatafimpausa']){
					
				$ano_inip	= substr($dadosubp['pdmdatainiciopausa'],0,4);
				$mes_inip	= substr($dadosubp['pdmdatainiciopausa'],5,2);
				$dia_inip	= substr($dadosubp['pdmdatainiciopausa'],8,2);
				$hor_inip	= substr($dadosubp['pdmdatainiciopausa'],11,2);
				$min_inip	= substr($dadosubp['pdmdatainiciopausa'],14,2);

				$ano_fimp	= substr($dadosubp['pdmdatafimpausa'],0,4);
				$mes_fimp	= substr($dadosubp['pdmdatafimpausa'],5,2);
				$dia_fimp	= substr($dadosubp['pdmdatafimpausa'],8,2);
				$hor_fimp	= substr($dadosubp['pdmdatafimpausa'],11,2);
				$min_fimp	= substr($dadosubp['pdmdatafimpausa'],14,2);
					
				$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
				$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final
					
				// pega o tempo total da pausa
				$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);
					
					
				$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
				$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';
					
			}

			//monta o texto da tempopausa
			$textotempopausa .= "<b>Tipo:</b> ". $dadosubp['tpadsc'];
			$textotempopausa .= "<br><b>Justificativa:</b> ". $dadosubp['pdmjustificativa']."";

			if($dadosubp['pdmdatafimpausa']){
				$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				if(!$tempop) $tempop = '0 minuto';
				$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
			}else{
				$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
				$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
			}

			$textotempopausa .= "<BR><BR>";

		}
			
			
			
		//if($flagIndeterminado == '1')
		//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
		//else{
		$datainiaux = date('Y-m-d H:i').':00';
		$ano_aux	= substr($datainiaux,0,4);
		$mes_aux	= substr($datainiaux,5,2);
		$dia_aux	= substr($datainiaux,8,2);
		$hor_aux	= substr($datainiaux,11,2);
		$min_aux	= substr($datainiaux,14,2);

		$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
		$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
		$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
		$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
		//}
			

		//pega prioridade e data termino
		$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
									from demandas.demanda 
									where dmdid = ". (int) $dadosub['id'];
		$dadosdmd = $db->carregar($sql);
			
		$resto = $tempototalpausa;
		$horas 			= $resto/3600; //quantidade de horas
		$intHoras 		= floor($horas);
		if($intHoras >= 1){	//se houver horas
			$horasx = $intHoras;
			$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
		}
			
		$minutos 		= $resto/60; //quantidade de minutos
		$intMinutos 	= floor($minutos);
		if($intMinutos >= 1){ //se houver minutos
			$minutosx = $intMinutos;
			$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
		}
			
		if(!$horasx) $horasx = "00";
		if(strlen($horasx) == 1) $horasx = "0".$horasx;
		if(!$minutosx) $minutosx = "00";
		if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;

		$hormin = $horasx.":".$minutosx;
			
		$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dadosub['dataconclusao']);
			
		if($flagIndeterminado){
			$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
		}
		else{
			$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
			//$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$horasx.":".$minutosx."</font>";
		}

			
			
	}
	$textotempopausa .= "</div>";


	//$tempototalpausa = 0;
	//dbg($dadosub['id'] . ' = ' . $tempototalpausa."<br>");

	$ano_ini	= substr($dadosub['dmddatainiprevatendimento'],0,4);
	$mes_ini	= substr($dadosub['dmddatainiprevatendimento'],5,2);
	$dia_ini	= substr($dadosub['dmddatainiprevatendimento'],8,2);
	$hor_ini	= substr($dadosub['dmddatainiprevatendimento'],11,2);
	$min_ini	= substr($dadosub['dmddatainiprevatendimento'],14,2);
	//$seg_ini	= substr($dadosub['dmddatainiprevatendimento'],17,2);

	$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
	$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

	//echo "<br>data final = ".$dataFinalx;

	if($dadosub['dmddatainiprevatendimento'] && $dataFinalPrazoPrev){
		$prazoatendimento = $classdata->diferencaEntreDatas(  $dadosub['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
	}
	//echo (int) $dadosub['id'] ." - ". $dadosub['datadocfinalizada']."<br>";

	if($dadosub['datadocfinalizada']){

		//calcula Duração do atendimento
		$total_minuto_conclusao = calculaTempoMinuto($dadosub['dmddatainiprevatendimento'], $dadosub['datadocfinalizada'], $dadosub['dmdhorarioatendimento']);
		$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
		$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
			
		//$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dmddatafimprevatendimento']))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dmddatainiprevatendimento']))) );
		//$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['datadocfinalizada']))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dmddatainiprevatendimento']))) );
			
		//$dadosub['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dadosub['dmddatainiprevatendimento'], $dadosub['datadocfinalizada'] , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			
		$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dmddatainiprevatendimento']))) );
		$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dmddatainiprevatendimento']))) );
			
		$tempodecorrido = $classdata->diferencaEntreDatas(  $dadosub['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			
			
		if($total_tempodecorrido > $total_prazoatendimento){
			$tempodecorrido = "<font color=red>". $tempodecorrido . "</font>";
			$dataconclusao = "<font color=red>". $dataconclusao . "</font>";
		}
		else{
			$tempodecorrido = "<font color=blue>". $tempodecorrido . "</font>";
			$dataconclusao = "<font color=blue>". $dataconclusao . "</font>";
		}
	}

/*
	//pega o tempo de classificação da demanda
	$dtabertura = $dadosub['dataabertura'];
	$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
	//echo $dadosub['id'] . " - " . $dtabertura . " - " . $dadosub['dataclassificacao']."<br>";
	if($dadosub['dataclassificacao']){
		$tempoclassif = $classdata->diferencaEntreDatas(  $dtabertura , $dadosub['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd');
	}
	if(!$tempoclassif) $tempoclassif = "0 minuto";
*/
	
	
	//pega o tempo da 1º classificação da demanda
	if($dadosub['dataabertura'] && $dadosub['dataclassificacao']){
		$dtabertura = $dadosub['dataabertura'];
		
		$hor_tc	= 0;
		$min_tc	= 15;
		
		$ano_ini	= substr($dtabertura,6,4);
		$mes_ini	= substr($dtabertura,3,2);
		$dia_ini	= substr($dtabertura,0,2);
		$hor_ini	= substr($dtabertura,11,2);
		$min_ini	= substr($dtabertura,14,2);

		$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
		$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);
	
		
		$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
		$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dataclassificacao'])));
		
		
		$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
		
		//echo $dadosub['nudemanda'] ." = ". $dtxini . " > " . $dtxfim.' = ('.($dtxini == $dtxfim).')<br>';
		
		//echo $dadosub['nudemanda'] . " - " . $dtabertura . " - " . $dadosub['dataclassificacao']."<br>";
		if($dtxfim > $dtxini){
			$dadosub['tempoclassif'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dadosub['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
		}
		else{
			$dadosub['tempoclassif'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dadosub['dataclassificacao'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
		}
		
	}
	if(!$dadosub['tempoclassif']) $dadosub['tempoclassif'] = "0 minuto";

	
	//pega o tempo da 2º classificação da origem: serviço de impressão
	if($dadosub['dataclassificacao'] && $dadosub['dataclassificacaosi']){
		$dtabertura = $dadosub['dataclassificacao'];
		
		
		$hor_tc	= substr($dadosub['tempoclassifcatalogo'],0,2);
		$min_tc	= substr($dadosub['tempoclassifcatalogo'],3,2);
		
		$ano_ini	= substr($dtabertura,6,4);
		$mes_ini	= substr($dtabertura,3,2);
		$dia_ini	= substr($dtabertura,0,2);
		$hor_ini	= substr($dtabertura,11,2);
		$min_ini	= substr($dtabertura,14,2);
			
		$dataini = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
		$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $dataini);

		
		$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
		$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dadosub['dataclassificacaosi'])));
						
		
		$dtabertura = substr($dtabertura,6,4) ."-". substr($dtabertura,3,2) ."-". substr($dtabertura,0,2) ." ". substr($dtabertura,11,2) .":". substr($dtabertura,14,2) . ":00";
		
		//echo $dadosub['nudemanda'] ." = ". $dtxini . " > " . $dtxfim.' = ('.($dtxini == $dtxfim).')<br>';
		
		//echo $dadosub['nudemanda'] . " - " . $dtabertura . " - " . $dadosub['dataclassificacao']."<br>";
		if($dtxfim > $dtxini){
			$dadosub['tempoclassifsi'] = "<font color=red>".$classdata->diferencaEntreDatas(  $dtabertura , $dadosub['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
		}
		else{
			$dadosub['tempoclassifsi'] = "<font color=blue>".$classdata->diferencaEntreDatas(  $dtabertura , $dadosub['dataclassificacaosi'] , 'tempoEntreDadas', 'string','yyyy/mm/dd'). "</font>";
		}
		
	}
	if(!$dadosub['tempoclassifsi']) $dadosub['tempoclassifsi'] = "0 minuto";
	
	
	
	//pega o calculo total de pontuação
	if($dadosub['qtdservico'] && $dadosub['pontuacao']){
		$totalpontuacao = (int) $dadosub['qtdservico'] * (int) $dadosub['pontuacao'];
	}

	//pega o calculo valor da demanda
	/*if($totalpontuacao){
		//echo $dados[$i]['totalpontuacao'] . " * ". $valordemanda."<br>";
		$valordemanda = $totalpontuacao * (float) $valorpordemanda;
	}*/


	//pega observação
	$sqlob = "select u.usunome, obsdsc, to_char(obsdata::timestamp,'DD/MM/YYYY HH24:MI') AS obsdata
							  from demandas.observacoes o
							  inner join seguranca.usuario u on u.usucpf = o.usucpf 
							  where obslog IS NULL and obsstatus='A' and dmdid = ".$dadosub['id'];
	$ob = array();
	$ob = $db->carregar( $sqlob );
	if($ob){
		$observacao = '<div align=left>';
		foreach($ob as $obx){
			$observacao .= $obx['obsdsc'] .'<br><b>Incluído por: '. $obx['usunome'] . '<br> Data: '. $obx['obsdata'] .'</b><br><br>';
		}
		$observacao .= '</div>';
	}


	//pega avaliação
	$sqlav = "SELECT to_char(avddata::timestamp,'DD/MM/YYYY HH24:MI:SS') as avddata,
									(CASE avdgeral
									 	WHEN '1' THEN 'RUIM'
									 	WHEN '2' THEN 'REGULAR'
									 	WHEN '3' THEN 'BOM'
									 	WHEN '4' THEN 'ÓTIMO'
									END) AS avdgeral,
					 				avsobs 
							FROM demandas.avaliacaodemanda 
							where avdstatus='A' and dmdid = ". (int) $dadosub['id'] . "
							order by avddata desc";	
	$av = array();
	$av = $db->carregar( $sqlav );
	if($av){
		$avaliacaousu = '<div align=left>';
		foreach($av as $avx){
			$avaliacaousu .= '<b>Data: '. $avx['avddata'] . '<br> Avaliação: '. $avx['avdgeral'] .'</b>';
			if($avx['avsobs']) $avaliacaousu .= '<br><b>Justificativa: '. $avx['avsobs'] . '</b>';
			$avaliacaousu .= "<br><br>";
		}
		$avaliacaousu .= '</div>';
	}


	//verifica se é a 1º validação/invaliadação
	if($dadosub['docid']){
		$sqlval = "select a.hstid, a.docid, u.usunome as nomegestor, b.cmddsc as comentario,
										CASE WHEN a.aedid in (278,279) THEN 
												  '<font color=red>INVALIDADA</font>' 
											 WHEN a.aedid in (368) THEN 
												  '<font color=#DAA520>VALIDADA SEM PAUSA</font>'
											 ELSE 
											 	  '<font color=blue>VALIDADA</font>' 
										END as situacao, 
										to_char(a.htddata::timestamp,'DD/MM/YYYY HH24:MI:SS') as datagestor
										from 	workflow.historicodocumento a
											inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
											inner join seguranca.usuario u on u.usucpf = a.usucpf
											left join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid
									where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada SEM PAUSA
									and c.docid = ".$dadosub['docid']."
									order by a.htddata desc ";

		$val = array();
		$val = $db->carregar( $sqlval );
		$revalidacao = "";
		if($val){
			$revalidacao = '<div align=left>';
			foreach($val as $valx){
				$revalidacao .= '<b>Data:</b> '. $valx['datagestor'] . '<br><b>'. $valx['situacao'] .' POR '.$valx['nomegestor'].'</b>';
				if($valx['comentario']) $revalidacao .= '<br><b>Justificativa:</b> '. $valx['comentario'];
				$revalidacao .= "<br><br>";
			}
			$revalidacao .= '</div>';
		}
	}

	// pinta linha caso seja revalidação
	if( ($dadosub['esdid'] == '93' || $dadosub['esdid'] == '111') && $revalidacao){
		$cor = '#FFA07A' ;
	}
	else{
		$cor = $cor == '#F7F7F7' ? '#ffffff' : '#F7F7F7' ;
	}

	?>

	<tr bgcolor="<?=$cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor ?>';">
		<td align="left" nowrap="nowrap"><?=$dadosub['fd']?></td>
		<td align="center"><a style='color: #0066CC'
			href='demandas.php?modulo=principal/lista&acao=A&dmdid=<?=$dadosub['id']?>'><?=$dadosub['id'] ?></a></td>
		<td><?=$dadosub['tit']?></td>
		<td><?=$dadosub['situacao']?></td>
		<td><?=$dadosub['responsavel'] ?></td>
		<td><?=$dadosub['solicitante'] ?></td>
		<td><?=$dadosub['prioridade'] ?></td>
		<td><?=$dadosub['justprioridade'] ?></td>
		<td><?=$dadosub['setor'] ?></td>
		<td><b>Edifício:</b> <?=$dadosub['edificio'] ?> <br>
		<b>Andar:</b> <?=$dadosub['andar'] ?> <br>
		<b>Sala:</b> <?=$dadosub['sala'] ?> <br>
		<b>Telefone:</b> <?=$dadosub['tel'] ?></td>
		<td><?=$dadosub['origemdemanda'] ?> / <br>
		<?=$dadosub['tipodemanda'] ?></td>
		<td><?=$dadosub['descricao'] ?></td>
		<td><?=$dadosub['servicoexec2'] ?></td>
		<td><?=$dadosub['qtdservico'] ?></td>
		<td><?=$dadosub['datainclusao'] ?></td>
		<td><?=$dadosub['tempoclassif']?></td>
		<td><?=$dadosub['dataclassificacaosi2']?></td>
		<td><?=$dadosub['tempoclassifcatalogo']?></td>
		<td><?=$dadosub['tempoclassifsi']?></td>
		<td><?=$dadosub['datainicio'] ?></td>
		<td><?=$dadosub['datafim'] ?></td>
		<td><?=$dadosub['prazocatalogo']?></td>
		<td><?=$dadosub['tempoadicional']?></td>
		<td><?=$dadosub['justtempoadicional']?></td>
		<td><?=$prazoatendimento?></td>
		<td><?=$textotempopausa?></td>
		<td><?=$observacao?></td>
		<td><?=$dadosub['dataconclusao'] ?></td>
		<td><?=$tempodecorrido?></td>
		<td><?=$avaliacaousu?></td>
		<td><?=$revalidacao?></td>
		<td><?=$dadosub['pontuacao']?></td>
		<td><?=$totalpontuacao?></td>
		<td><?=number_format($dadosub['valordemanda'], 2, ',', '.')?></td>
	</tr>
	<tr style='background-color: #F7F7F7'>
		<td colspan=29 style='padding-left: 20px;'
			id='td_<?=$dadosub['id'] ?>'></td>
	</tr>
	<?//explodeSubDemandas($dadosub['id']);?>
	<?endforeach;?>
	<?php else : ?>
	<tr>
		<td colspan=29
			style="text-align: center; padding: 15px; background-color: #fafafa; color: red; font-weight: bold; font-size: 10px;">
		Não existem subdemandas para finalizar.</td>
	</tr>
	<?php endif; ?>
</table>

<?}?>

<?php 

//INICIO controla a classificação da demanda
//Caso o gestor esteja classificando uma pagina, exibir mensagem de aviso para os demais gestores
$perfilUsu = arrayPerfil();
if( in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUsu) ){		
	
		
		$sql = "DELETE from demandas.controlevalidacao 
				where usucpf not in (select usucpf 
									 from seguranca.usuariosonline 
									 where mnudsc = 'Validar Demandas')";
		$db->executar($sql);
		
		
		$sql = "DELETE from demandas.controlevalidacao where usucpf = '".$_SESSION['usucpf']."'";
		$db->executar($sql);
		$db->commit();
		
		$sql = "select c.covid, u.usunome, c.usucpf 
				from demandas.controlevalidacao c
				inner join seguranca.usuario u on u.usucpf = c.usucpf
				where c.numeropg = $numero ";
		
		//dbg($sql);
	  	$dados = $db->pegaLinha($sql);
	  	
	  	if(!$dados['covid']){
	  		$sql = sprintf("INSERT INTO demandas.controlevalidacao
					(
						usucpf,
						numeropg		
					)VALUES(
						'%s',
						%s
					)",
					$_SESSION['usucpf'],
					$numero
					);
			$db->executar($sql);
			$db->commit();
	  	}
	  	else{
	  		if($dados['usucpf'] <> $_SESSION['usucpf']){
		  		?>
		  			<script>
		  				alert("O Gestor: <?=$dados['usunome']?>,\njá está classificando esta página!!!");
		  			</script>
		  		<? 
	  		}
	  	}
	  	 
	
	
}


//FIM controla a classificação da demanda


?>