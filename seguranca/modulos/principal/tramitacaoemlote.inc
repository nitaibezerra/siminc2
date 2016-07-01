<?php


/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once APPRAIZ . "includes/workflow.php";

function tramitaremlote($dados) {
	global $db;
	echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/><link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = str_replace(array(".","-"),array("",""),$dados['cpfresponsavel']);
	$_SESSION['usucpf'] = str_replace(array(".","-"),array("",""),$dados['cpfresponsavel']);
	$sql = $dados['consulta']; 

	if(!$dados['consulta']) $erro[] = " - Consulta vazia";
	if(!$dados['aedid']) 	$erro[] = " - Ação não identificada";
	
	$lista = $db->carregar(str_replace(array("\'"),array("'"),$sql));
	
	if(!$lista[0])			$erro[] = " - SQL não esta trazendo registros";
	elseif(!$lista[0]['docid'])	$erro[] = " - Coluna docid não esta na consulta";
	
	echo "<h2>Resultado da tramitação em lote</h2>";
	echo "<p><a href=seguranca.php?modulo=principal/tramitacaoemlote&acao=A>Clique aqui para voltar para tela de tramitação</a></p>";
	if($erro) {
		
		echo "<table width=100%>";
		echo "<tr><td><b>Identificou-se os seguintes erros:</b></td></tr>";
		echo "<tr><td>".implode("<br>",$erro)."</td></tr>";
		echo "</table>";
		
	} else {

		echo "<table width=100%>";
		if($lista[0]) {
	
			echo "<tr><td><b>###Documento###</b></td><td><b>###Resultado###</b></td></tr>";
			foreach($lista as $l) {
				$docid = $l['docid'];
				$aedid = $dados['aedid'];
				$dados_wf = array();
				$result = wf_alterarEstado( $docid, $aedid, $dados['cmddsc'], $dados_wf);
				echo "<tr><td>".$docid."</td><td>".(($result)?"Tramitado sucesso":"Não tramitado")."</td></tr>";
			
			}
		} else {
			echo "<tr><td colspan=2>Consulta não retornou registros</td></tr>";
		}
		echo "</table>";
	
	}
}

function carregarResponsavel($dados) {
	global $db;
	$nomeresponsavel = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array("",""),$dados['cpfresponsavel'])."'");

	if($nomeresponsavel) {
		echo $nomeresponsavel;
	} else {
		echo "naopossuisimec";
	}
	
}

function carregarEstadoDocumento($dados) {
	global $db;
	
	echo "<table width=100%>";
	echo "<tr>";
	echo "<td>Estado Origem:</td>";
	
	echo "<td>";
	
	$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".$dados['tpdid']."' ORDER BY esdordem";
	$db->monta_combo('esdidorigem', $sql, 'S', 'Selecione', 'selecionarEstadoDocumento', '', '', '', 'S', 'esdidorigem');
	
	echo "</td>";
	
	echo "</tr>";
	
	echo "<tr>";
	echo "<td>Estado Destino:</td>";
	
	echo "<td>";
	
	$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".$dados['tpdid']."' ORDER BY esdordem";
	$db->monta_combo('esdiddestino', $sql, 'S', 'Selecione', 'selecionarEstadoDocumento', '', '', '', 'S', 'esdiddestino');
	
	echo "</td>";
	
	echo "</tr>";
	
	echo "<tr>";
	echo "<td>Ação de tramitação:</td>";
	
	echo "<td id=td_acaotramitacao>-</td>";
	
	echo "</tr>";
	
	echo "</table>";
	
}

function carregarAcaoTramitacao($dados) {
	global $db;
	
	$acaoestadodoc = $db->pegaLinha("SELECT aedid, aeddscrealizar, esdsncomentario FROM workflow.acaoestadodoc WHERE esdidorigem='".$dados['esdidorigem']."' AND esdiddestino='".$dados['esdiddestino']."'");
	
	if($acaoestadodoc) {
		echo $acaoestadodoc['aeddscrealizar']."<input type=hidden name=aedid value=".$acaoestadodoc['aedid']."><br> ".(($acaoestadodoc['esdsncomentario']=="t")?"<b>Necessita de comentário :</b> <input type=text class=normal name=cmddsc size=30>":"");
	} else {
		echo "-";
	}

}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Tramitação em Lote", "Tramitar documentos em lote");


?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script>

function carregarResponsavel() {

	if(jQuery('#cpfresponsavel').val().length!=14) {
		jQuery('#nomeresponsavel').val('');
		jQuery('#img_cpf_validado').html('');
		return false;
	}

	if(!validar_cpf(jQuery('#cpfresponsavel').val())) {
		jQuery('#nomeresponsavel').val('');
		jQuery('#img_cpf_validado').html('<img src=../imagens/valida2.gif border=0> CPF é inválido');
		return false;
	}

	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/tramitacaoemlote&acao=A",
   		data: "requisicao=carregarResponsavel&cpfresponsavel="+jQuery('#cpfresponsavel').val(),
   		async: false,
   		success: function(msg){
   		if(msg=="naopossuisimec") {
			jQuery('#nomeresponsavel').val('');
			jQuery('#img_cpf_validado').html('<img src=../imagens/valida2.gif border=0> CPF não consta no SIMEC');
   		} else {
			jQuery('#nomeresponsavel').val(msg);
			jQuery('#img_cpf_validado').html('<img src=../imagens/valida1.gif border=0> CPF validado');
   		}
		}
 		});

}

function selecionarTipoDocumento(tpdid) {

	if(!tpdid) {
		jQuery('#td_estadodocumento').html('Selecione o Tipo Documento');
	} else {
		jQuery.ajax({
	   		type: "POST",
	   		url: "seguranca.php?modulo=principal/tramitacaoemlote&acao=A",
	   		data: "requisicao=carregarEstadoDocumento&tpdid="+tpdid,
	   		async: false,
	   		success: function(msg){
	   			jQuery('#td_estadodocumento').html(msg);
			}
	 		});

	}

}

function selecionarEstadoDocumento(v) {

	if(jQuery('#esdidorigem').val() != '' && jQuery('#esdiddestino').val() != '') {
	
		jQuery.ajax({
	   		type: "POST",
	   		url: "seguranca.php?modulo=principal/tramitacaoemlote&acao=A",
	   		data: "requisicao=carregarAcaoTramitacao&esdidorigem="+jQuery('#esdidorigem').val()+"&esdiddestino="+jQuery('#esdiddestino').val(),
	   		async: false,
	   		success: function(msg){
	   			jQuery('#td_acaotramitacao').html(msg);
			}
	 		});
		
	} else {
		jQuery('#td_acaotramitacao').html('-');
	}
}

</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">

<input type="hidden" name="requisicao" value="tramitaremlote">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloCentro' colspan='2'>Responsável pela tramitação</td>
</tr>
<tr>
	<td class='SubTituloDireita'>CPF do Responsável:</td>
	<td><? echo campo_texto('cpfresponsavel', 'S', 'S', 'CPF do Responsável', 16, 14, "###.###.###-##", "", '', '', 0, 'id="cpfresponsavel"', 'carregarResponsavel();', '', 'carregarResponsavel();' ); ?> <span id="img_cpf_validado"></span></td>
</tr>
<tr>
	<td class='SubTituloDireita'>Nome do Responsável:</td>
	<td><? echo campo_texto('nomeresponsavel', 'N', 'N', 'Nome do Responsável', 60, 100, '', '', '', '', 0, 'id="nomeresponsavel"' ); ?></td>
</tr>
<tr>
	<td class='SubTituloCentro' colspan='2'>Ação da tramitação</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Tipo Documento:</td>
	<td><?
	$sql = "SELECT tpdid as codigo, s.sisdsc || ' - ' || t.tpddsc as descricao FROM workflow.tipodocumento t 
			LEFT JOIN seguranca.sistema s ON s.sisid = t.sisid 
			WHERE s.sisstatus='A' AND t.tpdstatus='A' 
			ORDER BY s.sisdsc, t.tpddsc";
	$db->monta_combo('tpdid', $sql, 'S', 'Selecione', 'selecionarTipoDocumento', '', '', '', 'S', 'tpdid');
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Estados:</td>
	<td id="td_estadodocumento">Selecione o Tipo Documento</td>
</tr>
<tr>
	<td class="SubTituloDireita">Consulta:</td>
	<td><textarea name="consulta" rows="20" cols="80"></textarea></td>
</tr>
<tr>
	<td colspan="2">
	<ul>
	<li>Nesta consulta necessáriamente deve existir a coluna "docid"</li>
	</ul>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita' colspan="2"><input type="submit" name="tramitaremlote" value="Salvar"></td>
</tr>
</table>
</form>

<? if($ags['agsid']) : ?>
<script>
document.observe("dom:loaded", function() {
	document.getElementById('tr_agsperdetalhes').style.display = "";
	document.getElementById('td_agsperdetalhes').innerHTML = "Carregando...";
	ajaxatualizar('requisicao=pegarDetalhesPeriodicidade&agsperiodicidade=<? echo $ags['agsperiodicidade']; ?>&agsid=<? echo $ags['agsid']; ?>','td_agsperdetalhes');
});
</script>
<? endif; ?>