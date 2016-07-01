<?php

include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_pagamentos.php";

function inserirRestricoes($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto3.restricaousuario(
            iuscpf, reurestricao, reucpf, reudata, reutipo)
    		VALUES ('".$dados['iuscpf']."', '".$dados['reurestricao']."', '".$_SESSION['usucpf']."', NOW(), 'I') RETURNING reuid;";
	
	$reuid = $db->pegaUm($sql);
	
	$sql = "UPDATE sispacto3.identificacaousuario SET reuid='".$reuid."' WHERE iuscpf='".$dados['iuscpf']."'";
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert" => "Restrição inserida com sucesso", "location" => "sispacto3.php?modulo=consultarcpfpacto&acao=A&iuscpf=".$dados['iuscpf']);
	alertlocation($al);
	

}

function removerRestricoes($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto3.restricaousuario(
            iuscpf, reurestricao, reucpf, reudata, reutipo)
    		VALUES ('".$dados['iuscpf']."', NULL, '".$_SESSION['usucpf']."', NOW(), 'R')";
	
	$db->executar($sql);
	
	$sql = "UPDATE sispacto3.identificacaousuario SET reuid=NULL WHERE iuscpf='".$dados['iuscpf']."'";
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert" => "Restrição removida com sucesso", "location" => "sispacto3.php?modulo=consultarcpfpacto&acao=A&iuscpf=".$dados['iuscpf']);
	alertlocation($al);
	
}

function excluirMensario($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto3.historicoreaberturanota WHERE mavid IN( SELECT mavid FROM sispacto3.mensarioavaliacoes WHERE menid='".$dados['menid']."')";
	$db->executar($sql);
	$sql = "DELETE FROM sispacto3.mensarioavaliacoes WHERE menid='".$dados['menid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM sispacto3.mensario WHERE menid='".$dados['menid']."'";
	$db->executar($sql);
	
	$db->commit();
	
	if(!$dados[noredirect]) {
		$al = array("alert" => "Avaliação removida com sucesso", "location" => "sispacto3.php?modulo=consultarcpfpacto&acao=A&iuscpf=".$dados['iuscpf']);
		alertlocation($al);
	}
	

}

function excluirPagamento($dados) {
	global $db;
	
	$pagamentobolsista = $db->pegaLinha("SELECT p.iusd, p.fpbid, i.iuscpf FROM sispacto3.pagamentobolsista p
										 INNER JOIN sispacto3.identificacaousuario i ON i.iusd = p.iusd 
										 WHERE pboid='".$dados['pboid']."'");
	
	$mensariosss = $db->carregarColuna("SELECT menid FROM sispacto3.mensario WHERE iusd='".$pagamentobolsista['iusd']."' AND fpbid='".$pagamentobolsista['fpbid']."'");
	
	if($mensariosss[0]) {
		foreach($mensariosss as $menid) {
			excluirMensario(array('menid' => $menid,'noredirect' => true));
		}
	}
	
	$db->executar("DELETE FROM sispacto3.pagamentobolsista WHERE pboid='".$dados['pboid']."'");
	$db->commit();
	
	$al = array("alert" => "Pagamento removida com sucesso", "location" => "sispacto3.php?modulo=consultarcpfpacto&acao=A&iuscpf=".$pagamentobolsista['iuscpf']);
	alertlocation($al);
	
	
}



if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	$db->close();
	exit;
}


include APPRAIZ . "includes/cabecalho.inc";
echo '<br />';

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script language="javascript" type="text/javascript" src="./js/sispacto3.js"></script>

<form method=post name="formbuscar" id="formbuscar">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td class="SubTituloDireita">CPF</td>
	<td><?=campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iusnome"', '', mascaraglobal(str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf']),"###.###.###-##")); ?></td>
</tr>
<tr>
    <td class="SubTituloDireita">Nome</td>
	<td><?=campo_texto('iusnome', "N", "S", "CPF", 50, 150, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">
		<input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();">
	</td>
</tr>
</table>
</form>

<div id="modalInfo" style="display:none;"></div>

<? if($_REQUEST['iusnome']) : ?>

	<?
	
	$sql = "SELECT '<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'sispacto3.php?modulo=consultarcpfpacto&acao=A&iuscpf='||iuscpf||'\';\">' as acao, CASE WHEN iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE iuscpf END as iuscpf, iusnome FROM sispacto3.identificacaousuario WHERE iusnome ilike '%".$_REQUEST['iusnome']."%'";
	
	
	$cabecalho = array("&nbsp;","CPF","Nome");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	
	?>

<? else : ?>


<? if($_REQUEST['iuscpf']) : ?>

<?

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	
	if($_SESSION['sispacto3']['universidade']['uncid']) {
		$fl_uni ="AND i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";	
	} else {
		$fl_uni ="AND 1=2";
	}
	
}


$_REQUEST['iuscpf'] = str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf']);

$identificacaousuario = $db->pegaLinha("SELECT *, 
											   CASE WHEN p.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao||' ( Municipal ) '
													WHEN p.estuf IS NOT NULL THEN e.estuf||' / '||e.estdescricao||' ( Estadual )'
											   END as rede,
											   ed.esddsc as esddscturma 
										FROM sispacto3.identificacaousuario i 
										LEFT JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
										LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
										LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
										LEFT JOIN workflow.documento d ON d.docid = p.docidturma 
										LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid  
										WHERE iuscpf='".$_REQUEST['iuscpf']."' {$fl_uni}");

if(!$identificacaousuario) $identificacaousuario = $db->pegaLinha("SELECT *, 
																		   CASE WHEN p.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao||' ( Municipal ) '
																				WHEN p.estuf IS NOT NULL THEN e.estuf||' / '||e.estdescricao||' ( Estadual )'
																		   END as rede,
																		   ed.esddsc as esddscturma 
																	FROM sispacto3.identificacaousuario i 
																	LEFT JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
																	LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
																	LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
																	LEFT JOIN workflow.documento d ON d.docid = p.docidturma 
																	LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid  
																	WHERE iuscpf ilike 'REM".substr($_REQUEST['iuscpf'],3)."' {$fl_uni}");


if($identificacaousuario) :

	$tipoperfil  = $db->pegaLinha("SELECT *
								   FROM sispacto3.tipoperfil t
								   INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
								   WHERE t.iusd='".$identificacaousuario['iusd']."'");

	if($tipoperfil['pflcod']) {
		$plpabreviacao = $db->pegaUm("SELECT plpabreviacao FROM sispacto3.pagamentoperfil WHERE pflcod='".$tipoperfil['pflcod']."'");
		if($_HIERARQUIA_PFL[$tipoperfil['pflcod']]) $plpabreviacaosub = $db->pegaUm("SELECT plpabreviacao FROM sispacto3.pagamentoperfil WHERE pflcod IN(".implode(",",$_HIERARQUIA_PFL[$tipoperfil['pflcod']]).")");
	}

	$perfilsimec = $db->carregarColuna("SELECT pp.pfldsc FROM seguranca.perfilusuario pu 
								  		INNER JOIN seguranca.perfil pp ON pp.pflcod = pu.pflcod AND pp.sisid='".SIS_SISPACTO."' 
								  		WHERE pu.usucpf='".$identificacaousuario['iuscpf']."'");
	
	if($plpabreviacao) {

		if($plpabreviacao)
			$turmaparticipa = $db->pegaUm("SELECT tt.turdesc ||' ('||i.iusnome||' => '||pp.pfldsc||':'||uu.unisigla||')' as turma FROM sispacto3.{$plpabreviacao}turma ot 
											  INNER JOIN sispacto3.turmas tt ON tt.turid = ot.turid 
											  INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tt.iusd 
											  LEFT JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
											  LEFT JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
											  LEFT JOIN sispacto3.universidadecadastro u ON u.uncid = i.uncid 
											  LEFT JOIN sispacto3.universidade uu ON uu.uniid = u.uniid 
											  WHERE ot.iusd='".$identificacaousuario['iusd']."'");
		if($plpabreviacaosub)
			$turmagerencia = $db->pegaUm("SELECT tt.turdesc ||':'||coalesce(uu.unisigla,'XX')||' ( <img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAlunosTurma('||tt.turid||')\"> '||count(ot.otuid)||' participantes)' as turma FROM sispacto3.turmas tt
													  INNER JOIN sispacto3.{$plpabreviacaosub}turma ot ON ot.turid = tt.turid
													  LEFT JOIN sispacto3.universidadecadastro u ON u.uncid = tt.uncid
													  LEFT JOIN sispacto3.universidade uu ON uu.uniid = u.uniid
													  WHERE tt.iusd='".$identificacaousuario['iusd']."'
													  GROUP BY tt.turid, tt.turdesc, uu.unisigla");

		if($plpabreviacao=='orientadorestudo') {

			$turmaparticipa2 = $db->pegaUm("SELECT tt.turdesc ||' ('||i.iusnome||' => '||pp.pfldsc||':'||uu.unisigla||')' as turma FROM sispacto3.{$plpabreviacao}turmacl ot
												  INNER JOIN sispacto3.turmas tt ON tt.turid = ot.turid
												  INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tt.iusd
												  LEFT JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd
												  LEFT JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
												  LEFT JOIN sispacto3.universidadecadastro u ON u.uncid = i.uncid
												  LEFT JOIN sispacto3.universidade uu ON uu.uniid = u.uniid
												  WHERE ot.iusd='".$identificacaousuario['iusd']."'");

		}
	}
	



	$acessosistema = $db->pegaLinha("SELECT CASE WHEN suscod='A' THEN 'Ativo'
												 WHEN suscod='P' THEN 'Pendente' 
												 WHEN suscod='B' THEN 'Bloqueado' END as situacao,
											to_char(susdataultacesso,'dd/mm/YYYY HH24:MI') as susdataultacesso
									 FROM seguranca.usuario_sistema WHERE usucpf='".$identificacaousuario['iuscpf']."' AND sisid='".SIS_SISPACTO."'");



	if($identificacaousuario['uncid']) {
	
		$universidade = $db->pegaLinha("SELECT u.unisigla, u.uninome, cadastrosgb, e.esddsc as esddscfor FROM sispacto3.universidade u 
										 INNER JOIN sispacto3.universidadecadastro uu ON uu.uniid = u.uniid 
										 INNER JOIN workflow.documento d ON d.docid = uu.docidformacaoinicial 
										 INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
										 WHERE uu.uncid='".$identificacaousuario['uncid']."'");

	}


?>
<script>
function verConsultar(menid) {

	ajaxatualizar('requisicao=consultarDetalhesAvaliacoes&menid='+menid,'modalInfo');
	
	jQuery("#modalInfo").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}


function carregarLogCadastroSGB(usucpf) {
	divCarregando();
	

	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: 'requisicao=carregarLogCadastroSGB&usucpf='+usucpf,
   		async: false,
   		success: function(html){
   			jQuery("#modalInfo").html(html);
   		}
	});
	
	jQuery("#modalInfo").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });
	                    
	divCarregado();
}

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function carregarAlunosTurma(turid) {
	divCarregando();

	ajaxatualizar('requisicao=carregarAlunosTurma&consulta=true&turid='+turid,'modalInfo');

	jQuery("#modalInfo").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 400,
        modal: true,
     	close: function(){} 
    });
    
	divCarregado();
	
}


function carregarAlunosTurmaOutros(turid) {
	divCarregando();

	ajaxatualizar('requisicao=carregarAlunosTurmaOutros&consulta=true&turid='+turid,'modalInfo');

	jQuery("#modalInfo").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 400,
        modal: true,
     	close: function(){} 
    });
    
	divCarregado();
	
}

function inserirRestricoes() {
	if(jQuery('#reurestricao').val()=='') {
		alert('Preencha o motivo da restrição');
		return false;
	}

	jQuery('#requisicao').val('inserirRestricoes');

	jQuery('#restricao').submit();
	
}

function removerRestricoes() {

	jQuery('#requisicao').val('removerRestricoes');

	jQuery('#restricao').submit();
	
}

function excluirMensario(menid) {
	var conf = confirm('Deseja realmente excluir toda estrutura de avaliação deste período de referência? Com isso, este deverá ser reavaliador para ficar apto a receber a bolsa.');

	if(conf) {
		window.location='sispacto3.php?modulo=consultarcpfpacto&acao=A&requisicao=excluirMensario&iuscpf=<?=$identificacaousuario['iuscpf'] ?>&menid='+menid;
	}
}

function excluirPagamento(pboid) {
	var conf = confirm('Deseja realmente excluir toda estrutura de avaliação deste pagamento? Com isso, este deverá ser reavaliador para ficar apto a receber a bolsa.');

	if(conf) {
		window.location='sispacto3.php?modulo=consultarcpfpacto&acao=A&requisicao=excluirPagamento&pboid='+pboid;
	}

	
}


</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top">
	<p align="center" style="font-size:x-small;"><b>Cadastramento Geral</b></p>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Nome:</td>
			<td style="font-size:x-small;"><?=$identificacaousuario['iusnome'] ?>&nbsp;&nbsp;&nbsp;<span style=font-size:xx-small;>( cadastrado no SGB: <?=(($identificacaousuario['cadastradosgb']=='t')?"Sim":"<b style=color:red;><img src=../imagens/atencao.png align=absmiddle style=cursor:pointer; onclick=\"carregarLogCadastroSGB('".$identificacaousuario['iuscpf']."');\"> Não</b>") ?> )</span></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Termo compromisso:</td>
			<td style="font-size:x-small;"><?=(($identificacaousuario['iustermocompromisso']=='t')?"Sim":"Não") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Perfil da Bolsa:</td>
			<td style="font-size:x-small;"><?=(($tipoperfil['pfldsc'])?$tipoperfil['pfldsc']:"Não possui") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Perfil no SIMEC:</td>
			<td style="font-size:x-small;"><?=(($perfilsimec)?implode(", ",$perfilsimec):"Não possui") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação de acesso:</td>
			<td style="font-size:x-small;"><?=(($acessosistema['situacao'])?$acessosistema['situacao']:"Não possui acesso") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Último acesso:</td>
			<td style="font-size:x-small;"><?=(($acessosistema['susdataultacesso'])?$acessosistema['susdataultacesso']:"Nunca acessou") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Universidade:</td>
			<td style="font-size:x-small;"><?=(($universidade)?$universidade['unisigla']." - ".$universidade['uninome']:"Não vinculado a universidade") ?> &nbsp;&nbsp;&nbsp;<span style=font-size:xx-small;>( cadastrado no SGB: <?=(($universidade['cadastrosgb']=='t')?"Sim":"<b style=color:red;>Não</b>") ?> )</span></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação da universidade:</td>
			<td style="font-size:x-small;"><?=(($universidade['esddsc'])?$universidade['esddsc']:"Não vinculado a universidade") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação da formação inicial (Universidade):</td>
			<td style="font-size:x-small;"><?=$universidade['esddscfor'] ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação cadastro (outras turmas):</td>
			<td style="font-size:x-small;"><?=$universidade['esddsctur'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Rede:</td>
			<td style="font-size:x-small;"><?=(($identificacaousuario['rede'])?$identificacaousuario['rede']:"Equipe IES") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação município/estado (cadastro PA) :</td>
			<td style="font-size:x-small;"><?=(($identificacaousuario['esddscturma'])?$identificacaousuario['esddscturma']:"Equipe IES") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Participa turma (FR/OE):</td>
			<td style="font-size:x-small;"><?=(($turmaparticipa)?$turmaparticipa:"Não foi alocado em turma") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Gerencia turma (FR/OE):</td>
			<td style="font-size:x-small;"><?=(($turmagerencia)?$turmagerencia:"Não gerencia turma") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Participa turma (SP/CL):</td>
			<td style="font-size:x-small;"><?=(($turmaparticipa2)?$turmaparticipa2:"Não foi alocado em turma") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Gerencia turma (SP/CL):</td>
			<td style="font-size:x-small;"><?=(($turmagerencia2)?$turmagerencia2:"Não gerencia turma") ?></td>
		</tr>
	</table>
	
	
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" valign="top">
	<table width="100%">
	<tr>
	<td width="40%" valign="top">
	<p align="center" style="font-size:x-small;"><b>Histórico de avaliações</b></p>
	
	<?
	
	$sql = "SELECT '<span style=\"white-space: nowrap;\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verConsultar('||m.menid||')\"> '||CASE WHEN e.esdid!=".ESD_APROVADO_MENSARIO." THEN '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirMensario('||m.menid||');\">' ELSE '' END||'</span>' as acao, e.esdid, m.iusd, m.fpbid, me.mesdsc || ' / ' || p.fpbanoreferencia as ref, AVG(ma.mavtotal) as total, e.esddsc||CASE WHEN d.hstid IS NOT NULL THEN ' ('||to_char(h.htddata,'dd/mm/YYYY HH24:MI')||')' ELSE '' END as esddsc FROM sispacto3.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN sispacto3.folhapagamento p ON p.fpbid = m.fpbid 
			LEFT JOIN sispacto3.mensarioavaliacoes ma ON ma.menid = m.menid  
			INNER JOIN public.meses me ON me.mescod::integer = p.fpbmesreferencia
		
			WHERE m.iusd='".$identificacaousuario['iusd']."' 
			GROUP BY me.mesdsc, p.fpbanoreferencia, e.esdid, e.esddsc, m.iusd, m.fpbid, e.esdid, m.menid, d.hstid, h.htddata  
			ORDER BY m.fpbid";
	
	$arrDadosP = $db->carregar($sql);
	
	if($arrDadosP[0]) {
		foreach($arrDadosP as $key => $ar) {
			$arrDadosF[$key]['acao']   = $ar['acao'];
			$arrDadosF[$key]['ref']    = '<span style="font-size:x-small;">'.$ar['ref'].'</span>';
			$arrDadosF[$key]['total']  = $ar['total'];
			$arrDadosF[$key]['esddsc'] = '<span style="font-size:x-small;">'.$ar['esddsc'].'</span>';
			
			$arrDadosF[$key]['restricao'] = (($ar['esdid']!=ESD_APROVADO_MENSARIO)?pegarRestricaoPagamento(array('iusd'=>$ar['iusd'],'fpbid'=>$ar['fpbid'])):"&nbsp;");
		}
	}
	
	if(!$arrDadosF[0]) $arrDadosF = array();
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Mês</span>","<span style=font-size:x-small;>Nota média</span>","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Restrição</span>");
	$db->monta_lista_simples($arrDadosF,$cabecalho,100000,5,'N','100%','N');
	
	
	?>
	</td>
	<td valign="top">
	
	<p align="center" style="font-size:x-small;"><b>Histórico de pagamentos</b></p>
	
	<?
	
	
	
	$sql = "SELECT '<img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( '|| pb.docid ||' )\"> '||CASE WHEN d.esdid IN(".ESD_PAGAMENTO_AUTORIZADO.",".ESD_PAGAMENTO_APTO.") THEN '<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirPagamento('||pboid||');\">' ELSE '' END as acao,
				   '<span style=\"font-size:x-small;\">'|| me.mesdsc || ' / ' || p.fpbanoreferencia||'</span>' as ref, 
				   '<span style=\"font-size:x-small;\">'||pp.pfldsc||'</span>' as pfldsc, 
					pb.pbovlrpagamento, 
					'<span style=\"font-size:x-small;\">'||u.unisigla||' - '||u.uninome||'</span>' as universidade, 
					'<span style=\"font-size:x-small;\">'||es.esddsc||'</span>' as esddsc, 
					CASE WHEN d.esdid=".ESD_PAGAMENTO_EFETIVADO." THEN TO_CHAR ( AGE(h.htddata, docdatainclusao) , 'MM \"mes(es)\" DD \"dia(s)\" ' )::text ELSE '-' END  
			FROM sispacto3.pagamentobolsista pb 
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = pb.iusd 
			INNER JOIN workflow.documento d ON d.docid = pb.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = pb.pflcod 
			INNER JOIN sispacto3.folhapagamento p ON p.fpbid = pb.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = p.fpbmesreferencia 
			INNER JOIN sispacto3.universidade u ON u.uniid = pb.uniid 		
			WHERE pb.iusd='".$identificacaousuario['iusd']."' 
			ORDER BY pb.fpbid";
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Mês</span>","<span style=font-size:x-small;>Perfil</span>","<span style=font-size:x-small;>Valor(R$)</span>","<span style=font-size:x-small;>Universidade</span>","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Dias para efetivação</span>");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	
	?>
	
	</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" valign="top">
	<form method=post name="restricao" id="restricao">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="iuscpf" value="<?=$_REQUEST['iuscpf'] ?>">
	<table width="100%">
		<tr>
			<td width="40%" valign="top">
			
			<div style="width: 95%;padding: 10px;border: 5px solid gray;margin: 0px;">
			
			<p>A inserção da restrição acarreta no bloqueio da aprovação da avaliação, ou seja, os pagamentos não serão gerados. O motivo da restrição irá aparecer para o bolsista entender a razão do sistema não autorizar as avaliações.</p> 
			
			</div>
			
			
			<p align="center" style="font-size:x-small;"><b>Restrições</b></p>
			
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
					<td class="SubTituloDireita" style="font-size:x-small;" nowrap>Possui alguma restrição?</td>
					<td style="font-size:x-small;"><?=(($identificacaousuario['reuid'])?"<b style=color:red;>Sim</b>":"<b style=color:blue;>Não</b>") ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="font-size:x-small;" width="25%">Motivo da restrição:</td>
					<td style="font-size:x-small;">
					<? if($identificacaousuario['reuid']) : ?>
					
					<? 
					$motivo = $db->pegaUm("SELECT reurestricao FROM sispacto3.restricaousuario WHERE reuid='".$identificacaousuario['reuid']."'");
					echo $motivo; 
					?>
					<br>
					<br>
					<input type="button" name="removerrestricao" value="Remover restrição" onclick="removerRestricoes();">
					
					<? else : ?>
					
					<? echo campo_textarea( 'reurestricao', 'S', 'S', '', '50', '5', '500'); ?>
					<br>
					<input type="button" name="inserirrestricao" value="Inserir restrição" onclick="inserirRestricoes();">
					
					<? endif; ?>
					</td>
				</tr>
				<tr>
					<td colspan="2">
					<p align="center" style="font-size:x-small;">Histórico de restrições</p>
					
					<? 
					$sql = "SELECT CASE WHEN reutipo='I' THEN '<span style=\"font-size:x-small;\">Inserção</span>' 
										WHEN reutipo='R' THEN '<span style=\"font-size:x-small;\">Remoção</span>' END as tipo,
								   '<div style=\"font-size:x-small;width:300px;height:40px;overflow:auto;\">'||reurestricao||'</div>' as motivo,
								   '<span style=\"font-size:x-small;\">'||replace(to_char(u.usucpf::numeric, '000:000:000-00'), ':', '.')||' - '||u.usunome||'</span>' as usu,
								   '<span style=\"font-size:x-small;\">'||to_char(reudata,'dd/mm/YYYY HH24:MI')||'</span>' as reudata
							FROM sispacto3.restricaousuario r 
							INNER JOIN seguranca.usuario u ON u.usucpf = r.reucpf 
							WHERE iuscpf='".$_REQUEST['iuscpf']."' 
							ORDER BY reudata DESC";
					
					$cabecalho = array("<span style=font-size:x-small;>Tipo</span>","<span style=font-size:x-small;>Motivo</span>","<span style=font-size:x-small;>Inserido por</span>","<span style=font-size:x-small;>Data</span>");
					$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true,false,false,true);
					?>
					
					</td>
				</tr>
			</table>
 
			</td>
			<td width="60%" valign="top">
			<p align="center" style="font-size:x-small;"><b>Gerenciamento de acessos</b></p>
			<? 
			carregarHistoricoUsuario(array('usucpf' => $_REQUEST['iuscpf']));
			?>
			<br>
			<table class="tabela">
			<tr>
				<td style="font-size:x-small;">P1</td>
				<td style="font-size:x-small;">Já sou bolsista do FNDE/MEC e não desejo substituir minha bolsa atual pela bolsa do PACTO</td>
			</tr>
			</table>
			<p align="center" style="font-size:x-small;"><b>Histórico de modificações dos dados cadastrais</b></p>
			<?php 
			$sql = "SELECT '<span style=\"font-size:x-small;\">'||u.usunome||'<span>' as re,
						   '<span style=\"font-size:x-small;\">'||to_char(lordata,'dd/mm/YYYY HH24:MI')||'</span>' as dta,
						   '<span style=\"font-size:x-small;\">'||CASE WHEN strpos(lordados, '\"iusnaodesejosubstituirbolsa\":\"TRUE\",')!=0 THEN '<img src=../imagens/checked.gif>' ELSE '<img src=../imagens/check.gif>' END||'</span>' as desejabolsa
					FROM sispacto3.logrequisicao i 
					LEFT JOIN seguranca.usuario u ON u.usucpf = i.usucpf 
					WHERE i.usucpf='".$identificacaousuario['iuscpf']."' AND i.lorrequisicao='atualizarDadosIdentificacaoUsuario'";
			
			$cabecalho = array("<span style=font-size:x-small;>Modificado por</span>","<span style=font-size:x-small;>Data</span>","<span style=font-size:x-small;>P1</span>");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			
			?>
			<br>
			<p align="center" style="font-size:x-small;"><b>Histórico de modificações no programa</b></p>
			<?php 
			$sql = "SELECT CASE WHEN h.hstacao='T' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi <b>SUBSTITUÍDO(A)</b> por '||i.iusnome||'</div>'
								WHEN h.hstacao='R' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi <b>REMOVIDO(A)</b> do SISPACTO 2014</div>'
								WHEN h.hstacao='I' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i.iusnome||' ( '||p.pfldsc||' ) foi <b>INSERIDO(A)</b> no SISPACTO 2014</div>'
								WHEN h.hstacao='F' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi trocado da turma <b>'||COALESCE(tu1.turdesc,'XX')||'</b> para <b>'||COALESCE(tu2.turdesc,'XX')||'</b></div>'
							ELSE  h.hstacao
						   END as modif,
						   '<span style=\"font-size:x-small;\">'||u.usunome||'<span>' as resp,
						   '<span style=\"font-size:x-small;\">'||to_char(hstdata,'dd/mm/YYYY HH24:MI')||'<span>' as data
					FROM sispacto3.historicotrocausuario h 
					INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
					LEFT JOIN seguranca.perfil p ON p.pflcod = h.pflcod 
					LEFT JOIN sispacto3.identificacaousuario i ON i.iusd = h.iusdnovo 
					LEFT JOIN sispacto3.identificacaousuario i2 ON i2.iusd = h.iusdantigo 
					LEFT JOIN sispacto3.turmas tu1 ON tu1.turid = h.turidantigo 
					LEFT JOIN sispacto3.turmas tu2 ON tu2.turid = h.turidnovo 
					WHERE (iusdnovo='".$identificacaousuario['iusd']."' OR iusdantigo='".$identificacaousuario['iusd']."') ORDER BY hstdata";
			
			$cabecalho = array("<span style=font-size:x-small;>Modificação</span>","<span style=font-size:x-small;>Responsável</span>","<span style=font-size:x-small;>Data</span>");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			
			?>
			</td>
		</tr>
	</table>
	</form>
	</td>
</tr>

</table>
<? else : ?>
<div style="width: 80%;padding: 10px;border: 5px solid gray;margin: 0px;">CPF não consta do banco de dados do SISPACTO 2014.</div>
<? endif; ?>

<? endif; ?>

<? endif; ?>