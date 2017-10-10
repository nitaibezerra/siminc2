<?php

include APPRAIZ ."includes/workflow.php";
include_once "_funcoes.php";
include_once "_funcoes_pagamentos.php";

function excluirMensario($dados) {
	global $db;

	$sql = "DELETE FROM sismedio.historicoreaberturanota WHERE mavid IN( SELECT mavid FROM sismedio.mensarioavaliacoes WHERE menid='".$dados['menid']."')";
	$db->executar($sql);
	$sql = "DELETE FROM sismedio.mensarioavaliacoes WHERE menid='".$dados['menid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM sismedio.mensario WHERE menid='".$dados['menid']."'";
	$db->executar($sql);

	$db->commit();

	$al = array("alert" => "Restrição removida com sucesso", "location" => "sismedio.php?modulo=consultarcpfpacto&acao=A&iuscpf=".$dados['iuscpf']);
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

<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>

<form method=post name="formbuscar" id="formbuscar">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td class="SubTituloDireita">CPF</td>
	<td><?=campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', mascaraglobal(str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf']),"###.###.###-##")); ?></td>
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
	
	$sql = "SELECT '<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'sismedio.php?modulo=consultarcpfpacto&acao=A&iuscpf='||iuscpf||'\';\">' as acao, CASE WHEN iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE iuscpf END as iuscpf, iusnome FROM sismedio.identificacaousuario WHERE iusnome ilike '%".$_REQUEST['iusnome']."%'";
	
	
	$cabecalho = array("&nbsp;","CPF","Nome");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	
	?>

<? else : ?>

<? if($_REQUEST['iuscpf']) : ?>

<?

$_REQUEST['iuscpf'] = str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf']);

$identificacaousuario = $db->pegaLinha("SELECT *, 
											   CASE WHEN p.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao||' ( Municipal ) '
													WHEN p.estuf IS NOT NULL THEN e.estuf||' / '||e.estdescricao||' ( Estadual )'
											   END as rede,
											   ed.esddsc as esddscturma 
										FROM sismedio.identificacaousuario i 
										LEFT JOIN sismedio.pactoidadecerta p ON p.picid = i.picid 
										LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
										LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
										LEFT JOIN workflow.documento d ON d.docid = p.docidturma 
										LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid 
										WHERE iuscpf='".$_REQUEST['iuscpf']."'");

if($identificacaousuario) :

	if($identificacaousuario['iuscodigoinep']) {
		$sql = "SELECT * FROM sismedio.listaescolasensinomedio le
				INNER JOIN workflow.documento d ON d.docid = le.docid 
				INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid  
				WHERE le.lemcodigoinep='".$identificacaousuario['iuscodigoinep']."'";
		$escola = $db->pegaLinha($sql);
	}


	$tipoperfil  = $db->pegaLinha("SELECT *
								   FROM sismedio.tipoperfil t
								   INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
								   WHERE t.iusd='".$identificacaousuario['iusd']."'");

	$perfilsimec = $db->carregarColuna("SELECT pp.pfldsc FROM seguranca.perfilusuario pu 
								  		INNER JOIN seguranca.perfil pp ON pp.pflcod = pu.pflcod AND pp.sisid='".SIS_MEDIO."' 
								  		WHERE pu.usucpf='".$identificacaousuario['iuscpf']."'");
	
	$turmaparticipa = $db->pegaUm("SELECT tt.turdesc ||' ('||i.iusnome||' => '||pp.pfldsc||')' as turma FROM sismedio.orientadorturma ot 
									  INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
									  INNER JOIN sismedio.identificacaousuario i ON i.iusd = tt.iusd 
									  LEFT JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
									  LEFT JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
									  WHERE ot.iusd='".$identificacaousuario['iusd']."'");
	
	
	$turmagerencia = $db->pegaUm("SELECT tt.turdesc ||' ( <img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarAlunosTurma('||tt.turid||')\"> '||count(ot.otuid)||' participantes)' as turma FROM sismedio.turmas tt 
								  INNER JOIN sismedio.orientadorturma ot ON ot.turid = tt.turid 
								  WHERE tt.iusd='".$identificacaousuario['iusd']."' 
								  GROUP BY tt.turid, tt.turdesc");


	$acessosistema = $db->pegaLinha("SELECT CASE WHEN suscod='A' THEN 'Ativo'
												 WHEN suscod='P' THEN 'Pendente' 
												 WHEN suscod='B' THEN 'Bloqueado' END as situacao,
											to_char(susdataultacesso,'dd/mm/YYYY HH24:MI') as susdataultacesso
									 FROM seguranca.usuario_sistema WHERE usucpf='".$identificacaousuario['iuscpf']."' AND sisid='".SIS_MEDIO."'");



	if($identificacaousuario['uncid']) {

		if($tipoperfil['pflcod']) {

			$periodoref = $db->pegaUm("SELECT 
	
									(SELECT m.mesdsc || ' / ' || fpbanoreferencia as descricao 
												FROM sismedio.folhapagamento f 
												INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia WHERE fpbid=foo.mi)|| ' até '||
									(SELECT m.mesdsc || ' / ' || fpbanoreferencia as descricao 
												FROM sismedio.folhapagamento f 
												INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia WHERE fpbid=foo.ma) as periodo
									
									from (
									SELECT min(fpbid) as mi, max(fpbid) as ma FROM sismedio.folhapagamentouniversidade WHERE pflcod=".$tipoperfil['pflcod']." and uncid=".$identificacaousuario['uncid']."
									) foo");

		}

	
		$universidade = $db->pegaLinha("SELECT u.unisigla, u.uninome, e.esddsc, e2.esddsc as esddscfr, e3.esddsc as esddscoe, cadastrosgb FROM sismedio.universidade u 
										 INNER JOIN sismedio.universidadecadastro uu ON uu.uniid = u.uniid 
										 INNER JOIN workflow.documento d ON d.docid = uu.docid 
										 INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
										 INNER JOIN workflow.documento d2 ON d2.docid = uu.docidturmaformadoresregionais 
										 INNER JOIN workflow.estadodocumento e2 ON e2.esdid = d2.esdid
										 INNER JOIN workflow.documento d3 ON d3.docid = uu.docidturmaorientadoresestudo 
										 INNER JOIN workflow.estadodocumento e3 ON e3.esdid = d3.esdid

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

function excluirMensario(menid) {
	var conf = confirm('Deseja realmente excluir toda estrutura de avaliação deste período de referência? Com isso, este deverá ser reavaliador para ficar apto a receber a bolsa.');

	if(conf) {
		window.location='sismedio.php?modulo=consultarcpfpacto&acao=A&requisicao=excluirMensario&iuscpf=<?=$identificacaousuario['iuscpf'] ?>&menid='+menid;
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
			<td class="SubTituloDireita" style="font-size:x-small;">Perfil no <?php echo SIGLA_SISTEMA; ?>:</td>
			<td style="font-size:x-small;"><?=(($perfilsimec)?implode(", ",$perfilsimec):"Não possui") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Período de referência:</td>
			<td style="font-size:x-small;"><?=(($periodoref)?$periodoref:"Não selecionado") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">&nbsp;</td>
			<td style="font-size:x-small;">&nbsp;</td>
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
			<td class="SubTituloDireita" style="font-size:x-small;">Escola:</td>
			<td style="font-size:x-small;"><?=(($escola['lemcodigoinep'])?$escola['lemcodigoinep']." - ".$escola['lemnomeescola']:"Não vinculado a escola") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação da escola:</td>
			<td style="font-size:x-small;"><?=(($escola['esddsc'])?$escola['esddsc']:"Não vinculado a escola") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação turmas (FR):</td>
			<td style="font-size:x-small;"><?=$universidade['esddscfr'] ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Situação turmas (OE):</td>
			<td style="font-size:x-small;"><?=$universidade['esddscoe'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="font-size:x-small;">Participa turma (FR/OE):</td>
			<td style="font-size:x-small;"><?=(($turmaparticipa)?$turmaparticipa:"Não foi alocado em turma") ?></td>
			<td class="SubTituloDireita" style="font-size:x-small;">Gerencia turma (FR/OE):</td>
			<td style="font-size:x-small;"><?=(($turmagerencia)?$turmagerencia:"Não gerencia turma") ?></td>
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
	
	$sql = "SELECT '<span style=\"white-space: nowrap;\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verConsultar('||m.menid||')\"> '||CASE WHEN e.esdid!=".ESD_APROVADO_MENSARIO." THEN '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirMensario('||m.menid||');\">' ELSE '' END||'</span>' as acao, e.esdid, m.iusd, m.fpbid, me.mesdsc || ' / ' || p.fpbanoreferencia as ref, AVG(ma.mavtotal) as total, e.esddsc||CASE WHEN d.hstid IS NOT NULL THEN ' ('||to_char(h.htddata,'dd/mm/YYYY HH24:MI')||')' ELSE '' END as esddsc FROM sismedio.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN sismedio.folhapagamento p ON p.fpbid = m.fpbid 
			LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid  
			INNER JOIN public.meses me ON me.mescod::integer = p.fpbmesreferencia
		
			WHERE m.iusd='".$identificacaousuario['iusd']."' 
			GROUP BY me.mesdsc, p.fpbanoreferencia, e.esddsc, m.iusd, m.fpbid, e.esdid, m.menid, d.hstid, h.htddata 
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
	
	$sql = "SELECT '<img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( '|| pb.docid ||' )\">' as acao,
				   '<span style=\"font-size:x-small;\">'|| me.mesdsc || ' / ' || p.fpbanoreferencia||'</span>' as ref, 
				   '<span style=\"font-size:x-small;\">'||pp.pfldsc||'</span>' as pfldsc, 
					pb.pbovlrpagamento, 
					'<span style=\"font-size:x-small;\">'||u.unisigla||' - '||u.uninome||'</span>' as universidade, 
					'<span style=\"font-size:x-small;\">'||es.esddsc||'</span>' as esddsc, 
					CASE WHEN d.esdid=".ESD_PAGAMENTO_EFETIVADO." THEN TO_CHAR ( AGE(h.htddata, docdatainclusao) , 'MM \"mes(es)\" DD \"dia(s)\" ' )::text ELSE '-' END  
			FROM sismedio.pagamentobolsista pb 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = pb.iusd 
			INNER JOIN workflow.documento d ON d.docid = pb.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = pb.pflcod 
			INNER JOIN sismedio.folhapagamento p ON p.fpbid = pb.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = p.fpbmesreferencia 
			INNER JOIN sismedio.universidade u ON u.uniid = pb.uniid 		
			WHERE pb.iusd='".$identificacaousuario['iusd']."' 
			ORDER BY pb.fpbid";
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Mês</span>","<span style=font-size:x-small;>Perfil</span>","<span style=font-size:x-small;>Valor(R$)</span>","<span style=font-size:x-small;>Universidade</span>","<span style=font-size:x-small;>Situação</span>","<span style=font-size:x-small;>Dias para efetivação</span>");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	
	?>
	
	</td>
	</tr>
	
	<tr>
	<td valign="top">
	
	<p align="center" style="font-size:x-small;"><b>Histórico de modificações no programa</b></p>
	
	<?php 
			$sql = "SELECT CASE WHEN h.hstacao='T' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi <b>SUBSTITUÍDO(A)</b> por '||i.iusnome||'</div>'
								WHEN h.hstacao='R' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi <b>REMOVIDO(A)</b> do SISMédio</div>'
								WHEN h.hstacao='I' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i.iusnome||' ( '||p.pfldsc||' ) foi <b>INSERIDO(A)</b> no SISMédio</div>'
								WHEN h.hstacao='F' THEN '<div style=\"font-size:x-small;width:400px;height:40px;overflow:auto;\">'||i2.iusnome||' ( '||p.pfldsc||' ) foi trocado da turma <b>'||COALESCE(tu1.turdesc,'XX')||'</b> para <b>'||COALESCE(tu2.turdesc,'XX')||'</b></div>'
							ELSE  h.hstacao
						   END as modif,
						   '<span style=\"font-size:x-small;\">'||u.usunome||'<span>' as resp,
						   '<span style=\"font-size:x-small;\">'||to_char(hstdata,'dd/mm/YYYY HH24:MI')||'<span>' as data
					FROM sismedio.historicotrocausuario h 
					INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
					LEFT JOIN seguranca.perfil p ON p.pflcod = h.pflcod 
					LEFT JOIN sismedio.identificacaousuario i ON i.iusd = h.iusdnovo 
					LEFT JOIN sismedio.identificacaousuario i2 ON i2.iusd = h.iusdantigo 
					LEFT JOIN sismedio.turmas tu1 ON tu1.turid = h.turidantigo 
					LEFT JOIN sismedio.turmas tu2 ON tu2.turid = h.turidnovo 
					WHERE (iusdnovo='".$identificacaousuario['iusd']."' OR iusdantigo='".$identificacaousuario['iusd']."') ORDER BY hstdata";
	
	
	$cabecalho = array("<span style=font-size:x-small;>Modificação</span>","<span style=font-size:x-small;>Responsável</span>","<span style=font-size:x-small;>Data</span>");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	?>
	
	</td>
	<td valign="top">&nbsp;</td>
	</tr>
	</table>
	</td>
</tr>

</table>
<? else : ?>
<div style="width: 80%;padding: 10px;border: 5px solid gray;margin: 0px;">CPF não consta do banco de dados do SISMédio.</div>
<? endif; ?>

<? endif; ?>


<? endif; ?>