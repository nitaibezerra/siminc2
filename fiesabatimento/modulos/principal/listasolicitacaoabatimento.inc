<?
/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}
if( $_REQUEST['req2'] ){
	$_REQUEST['req2']($_REQUEST);
	die();
}

// verificaSolicitacoesDecursoPrazo();

$bloq = false;

$prazo = testa_prazo_aprovacao();

if( $prazo ){
	$bloq = true;
}

$perfis = pegaPerfilGeral();

if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) || in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) ){
	$sql = "SELECT muncod FROM fiesabatimento.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."'";
	$dados['muncod'] = $db->pegaUm($sql);
}
if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) || in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) ) {
	$sql = "SELECT estuf FROM fiesabatimento.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."'";
	$dados['estuf'] = $db->pegaUm($sql);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
// $db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Solicitações de Abatimento', $linha2);
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
function editarSolicitacaoAbatimento(idoid,sbaid) {
	window.location='fiesabatimento.php?modulo=principal/validacaoabatimento&acao=A&idoid='+idoid+'&sbaid='+sbaid;
}
function atualizaComboMunicipio( estuf ){

	$.ajax({
		type: "POST",
		url: window.location,
		data: "req=atualizaComboMunicipio&estuf="+estuf,
		async: false,
		success: function(msg){
			jQuery('#td_muncod').html(msg);
		}
	});
	return true;
}
$(document).ready(function(){

	<?php if( $bloq ){?>
		$('[type="checkbox"]').attr('disabled',true);
		$('[type="radio"]').attr('disabled',true);
		$('.re_abre').attr('disabled',true);
		$('[name="confirmar"]').attr('disabled',true);
		$('.aprovar').attr('disabled',true);
	<?php } ?>
	
	$('.pesquisar').click(function(){
		$('#formListaSolicitacao').submit();
	});
	$('.todos').click(function(){
		if( $(this).attr('checked') ){
			$('[name="sbaid[]"]').attr('checked',true);
		}else{
			$('[name="sbaid[]"]').attr('checked',false);
		}
	});
	$('.aprovar').click(function(){
		if( $('[name="sbaid[]"]:checked').length > 0 ){
			if( confirm('Tem certeza que deseja aprovar os '+$('[name="sbaid[]"]:checked').length+' registros automaticamente?') ){
				$('#req2').val('tramitarLote');
				$('#formListaAtp').submit();
			}
		}else{
			alert('Para aprovação é necessário no mínimo uma solicitação selecionada.');
		}
	});
	if( $('[name="sbaid[]"]').length == 0 ){
		$('.aprovar').hide();
	}
});
</script>
<form method="post" name="formListaSolicitacao" id="formListaSolicitacao">
<input type="hidden" name="req" id="req" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
		<p>
			Nesta tela são apresentadas as solicitações e renovações de abatimento FIES para aprovação, reabertura ou rejeição pelo Secretário de Educação. 
			<br><br>
			A pesquisa poderá ser iniciada mediante informação de dados pessoais do solicitante (CPF e nome), data da solicitação (início e fim) e as seguintes situações: Em solicitação pelo professor, Pendente de aprovação, Aprovada, Rejeitada, Reaberta e Rejeitada por decurso de prazo (a cargo do Secretário). 
			<br><br>
			Assim, preencha o(s) campo(s)  e acione a opção "Pesquisar" ou selecione uma das situações permitidas e acione a opção "Pesquisar".
			Acione o ícone 'Detalhar <img src="../imagens/consultar.gif">' ao lado de cada registro de solicitação para visualizar os detalhes de cada caso, e realizar a reabertura ou rejeição.
			Selecione o(s) registro(s) que queira aprovar, e em seguida acione a opção "Aprovar".
		</p>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">CPF :</td>
		<td><? echo campo_texto("idocpf", "N", "S", '', 14, 15, "###.###.###-##", "", "", "", 0, ""); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do solicitante :</td>
		<td><? echo campo_texto("idonome", "N", "S", '', 60, 150, "", "", "", "", 0, ""); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data da solicitação:</td>
		<td>Início: <?php echo campo_data2("sbadatasolicitacaoini","N","S","","","","",$sbadatasolicitacaoini); ?> Fim: <?php echo campo_data2("sbadatasolicitacaofim","N","S","","","","",$sbadatasolicitacaofim); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Situação :</td>
		<td>
			<? 
			
			if( $_REQUEST['esdid'] ){
				$esdid = $_REQUEST['esdid'];
			}else{
				$esdid = ESDID_AGUARDANDO_ANALISE;
			}
		
			$sql = "SELECT
						'999' as codigo,
						'Todos' as descricao
					UNION ALL
					SELECT
						esdid as codigo,
						esddsc as descricao
					FROM
						workflow.estadodocumento
					WHERE
						tpdid = ".TPDID_ANALISE_SITUACAO." AND esdstatus = 'A'
					ORDER BY 2";
			
			$db->monta_combo('esdid', $sql, 'S', '', '', '', '', '', 'N', 'esdid'); 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td><input type="button" class="pesquisar" value="Pesquisar"></td>
	</tr>
</table>
</form>
<!-- <form method="post" name="formListaAtp" id="formListaAtp"> -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<!-- 	<input type="hidden" name="req2" id="req2" value="" /> -->
	<tr>
		<td colspan="2">
		<?
		if( date('Ymd') >= DT_INICIO_APROVACAO && date('Ymd') <= DT_FIM_APROVACAO  ){
			
			$perfis = pegaPerfilGeral();
			
			$where = Array("1=1");
			
			$flatFiltro = false;
			if( $_REQUEST['idocpf'] ){
				$flatFiltro = true;
				$where[] = "ido.idocpf ilike '%".str_replace(Array('.','-'),'',$_REQUEST['idocpf'])."%'";
			}
			if( $_REQUEST['idonome'] ){
				$flatFiltro = true;
				$where[] = "upper(ido.idonome) ilike '%'||upper('".$_REQUEST['idonome']."')||'%'";
			}
			if( $_REQUEST['sbadatasolicitacaoini'] ){
				$flatFiltro = true;
				$where[] = "htddata::date >= '".ajusta_data($_REQUEST['sbadatasolicitacaoini'])."'::date";
			}
			if( $_REQUEST['sbadatasolicitacaofim'] ){
				$flatFiltro = true;
				$where[] = "htddata::date <= '".ajusta_data($_REQUEST['sbadatasolicitacaofim'])."'::date";
			}
			if( $_REQUEST['esdid'] && $_REQUEST['esdid'] != '999' ){
				$flatFiltro = true;
				$where[] = "esd.esdid = ".$_REQUEST['esdid'];
			}elseif( $_REQUEST['esdid'] == '999' ){
			}else{
				$flatFiltro = true;
				$where[] = "esd.esdid = ".ESDID_AGUARDANDO_ANALISE;
			}
			if( $_REQUEST['estuf'] ){
				$flatFiltro = true;
				$where[] = "mun.estuf ilike '".$_REQUEST['estuf']."'";
			}
			if( $_REQUEST['muncod'] ){
				$flatFiltro = true;
				$where[] = "mun.muncod ilike '".$_REQUEST['muncod']."'";
			}
			if( in_array(PFL_SECRETARIO_MUNICIPAL,$perfis) //|| in_array(PFL_SUB_SECRETARIO_MUNICIPAL,$perfis) 
					){
				$param = Array('campo'=>'muncod','valor'=> MUNCOD_BRASILIA, 'perfil' => PFL_SECRETARIO_MUNICIPAL );
				if( !temResp( $param ) ){
					$esfera = 'M';
					$where[] = "atp.esferaprofessor = 'M'";
				}
				$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.muncod = atp.muncodprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
			}
			if( in_array(PFL_SECRETARIO_ESTADUAL,$perfis) //|| in_array(PFL_SUB_SECRETARIO_ESTADUAL,$perfis) 
					) {
				$param = Array('campo'=>'estuf','valor'=> 'DF','perfil'=>PFL_SECRETARIO_ESTADUAL);
				if( !temResp( $param ) ){
					$esfera = "E";
					$esfera2 = "F";
					$where[] = "(atp.esferaprofessor = 'E' OR atp.esferaprofessor = 'F') ";
				}
				$inner_resp = "INNER JOIN fiesabatimento.usuarioresponsabilidade urs ON urs.estuf = atp.estufprofessor AND urs.usucpf = '".$_SESSION['usucpf']."'";
			}			
			
			if( $_REQUEST['esdid'] == WF_FIES1_REJEITADO  ){
				$status = '';
			}else{
// 				$status = "AND ido.idostatus = 'A' 
// 						   AND sba.sbastatus = 'A'
				$status = "AND atp.atpstatus = 'A'";
// 						   AND idostatus='A'";
			}
			$sql = "SELECT DISTINCT
						'<center><img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"editarSolicitacaoAbatimento(\''||ido.idoid||'\',\''||sba.sbaid||'\')\"></center>' as acao,
						--CASE WHEN esd.esdid = ".ESDID_AGUARDANDO_ANALISE." AND atpidusuconfirmacao IS NULL
						--	THEN '<center><input type=\"checkbox\" name=\"sbaid[]\" value=\"'||sba.sbaid||'\"></center>'
						--	ELSE '<center> - </center>'
						--END as docid,
						fiesabatimento.agrupaUFMunEscola( sba.sbaid, '".$_SESSION['usucpf']."', '".$esfera."' )".
						($esfera2 != '' ? " || fiesabatimento.agrupaUFMunEscola( sba.sbaid, '".$_SESSION['usucpf']."', '".$esfera2."' )" : '' )." as escolas,
						idonome,
						' ' || idocpf,
						/*
						case when to_char(htddata,'DD/MM/YYYY') is not null then
							  to_char(htddata,'DD/MM/YYYY')
						     else
							to_char(sba.sbadatasolicitacao,'DD/MM/YYYY')
						end as data,
						*/
						to_char(sba.sbadatasolicitacao,'DD/MM/YYYY') as data,
						esd.esddsc ||' / '|| esd2.esddsc as situacao

						,
						(CASE WHEN sba.sbarenovacao = false THEN
								'Solicitação - 01/'||sba.sbaanoinicio||' à 12/'||sba.sbaanofim
							  ELSE
							  	'Renovação - 01/'||sba.sbaanoinicio||' à 12/'||sba.sbaanofim
						 END)  as renovacao

					FROM 
						fiesabatimento.identificacaodocente ido
					INNER JOIN fiesabatimento.solicitacaoabatimento sba ON sba.idoid = ido.idoid
					LEFT  JOIN fiesabatimento.atuacaoprofissional 	atp	ON atp.idoid = ido.idoid AND atp.sbaid = sba.sbaid
					INNER JOIN workflow.documento 				   	doc ON doc.docid = atp.docid
					INNER JOIN workflow.documento 				   	doc2 ON doc2.docid = sba.docid
					INNER JOIN workflow.estadodocumento 			esd ON esd.esdid = doc.esdid 
					INNER JOIN workflow.estadodocumento 			esd2 ON esd2.esdid = doc2.esdid 
					LEFT  JOIN (SELECT max(h.htddata) as htddata, h.docid
								FROM workflow.historicodocumento h
								INNER JOIN workflow.documento doc1 ON h.docid = doc1.docid AND doc1.tpdid = ".TPDID_ANALISE_SITUACAO."
								GROUP BY h.docid ) foo ON foo.docid = doc.docid
					LEFT  JOIN territorios.municipio 			   	mun ON mun.muncod::integer = atp.muncodprofessor::integer
					--LEFT  JOIN fiesabatimento.parametrosrenovacao 	par	ON par.preid = sba.preid
					$inner_resp
					WHERE 
						".implode(' AND ',$where)."
						$status
					ORDER BY
						idonome,
						escolas";
						
			$dados = $db->carregar($sql);
			if( $dados[0] == '' && $flatFiltro ){
				//echo "<script>alert('Nenhum registro encontrado com os parâmetros informados.');window.location = 'fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';</script>";
			}
			//echo '<div style="margin-left:30px;"><input type="checkbox" class="todos" style="cursor:pointer"><strong> Selecionar Todos</strong></div>';
	// 		$cabecalho = array("Detalhar","Aprovar","UF","Município","Escola","Nome do Solicitante","CPF","Data da Solicitação","Situação");
			$cabecalho = array("Detalhar<input type=\"hidden\" name=\"req2\" id=\"req2\" />","UF/Município - Escola","Nome do Solicitante","CPF","Data da Solicitação","Situação","Tipo");
		//$cabecalho = array("Detalhar<input type=\"hidden\" name=\"req2\" id=\"req2\" />","UF/Município - Escola","Nome do Solicitante","CPF","Data da Solicitação","Situação Atuação / Situação Solicitação");
			//dbg($sql,1);
			$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,'formListaAtp');
		}else{
		?>
		<b>Fora do periodo de aprovação.</b>
		<?php 	
		}
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">&nbsp;</td>
		<td><!-- <input type="button" class="aprovar" value="Aprovar"> --></td>
	</tr>
</table>
<!-- </form> -->