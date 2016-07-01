<?php
if( $_POST['requisicao'] == 'limparPlanoTrabalho' ){
	limparPlanoTrabalho($_POST);
	exit();
}

function limparPlanoTrabalho($dados) {
	global $db;
	
	if($dados['ptrid']) {
		//foreach($dados['ptrid'] as $ptrid) {
			//deletaFilhosPTA( $ptrid );
			$retorno = deletaReformulacaoPTA($dados['ptrid'], $dados['refjustificativaexclusao']);
		//}
		if( $retorno ){
			echo 'Plano de trabalho foi deletado com sucesso.';
		} else {
			echo 'Nenhum plano de trabalho foi deletado.';
		}
		/*if( $retorno ){
			echo "<script>
						alert('Plano de trabalho foi deletado com sucesso.');
						window.location.href = window.location; 
				</script>";
		} else {
			echo "<script>
					alert('Nenhum plano de trabalho foi deletado.');
					window.location.href = window.location; 
			</script>";
		}*/
	} else {
		echo 'Nenhum plano de trabalho selecionado.';
		/*echo "<script>
					alert('Nenhum plano de trabalho selecionado.');
					window.location.href = window.location; 
			</script>";*/
	}
	exit();
}

if( $_REQUEST['popup'] == 'S' ){
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	
	$_POST['ptrcod'] = $db->pegaUm( "select ptrcod from emenda.planotrabalho where ptrid =".$_REQUEST['ptrid']);
} else {
	include APPRAIZ."includes/cabecalho.inc";
}

monta_titulo( $titulo_modulo, 'Tela de limpeza das reformulações com problemas' );
?>
<div id="erro"></div>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

function limparPlanoTrabalho() {
	
	$('#requisicao').val('limpar');
 	$('#formulario').submit();
	
	/*$.ajax({
   		type: "POST",
   		url: "emenda.php?modulo=principal/limparReformulacao&acao=A",
   		data: "requisicao=limparPlanoTrabalho&"+$('#formulario').serialize(),
   		async: false,
   		success: function(msg){
   			document.getElementById('erro').innerHTML = msg;
   			alert(msg);
   		}
 		});*/
 		
 	//buscarPlanoTrabalho();

}
function excluirReformulacao(ptrid) {
	
	if( $('[name="refjustificativaexclusao"]').val() == '' ){
		alert('O campo Justificativa é de preenchimento obrigatório.');
		$('[name="refjustificativaexclusao"]').focus();
		return false;
	}
	
	$.ajax({
   		type: "POST",
   		url: "emenda.php?modulo=principal/limparReformulacao&acao=A",
   		data: "requisicao=limparPlanoTrabalho&ptrid="+ptrid+'&refjustificativaexclusao='+$('[name="refjustificativaexclusao"]').val(),
   		async: false,
   		success: function(msg){
   			//document.getElementById('erro').innerHTML = msg;
   			alert(msg);
   		}
 		});
 		
 	buscarPlanoTrabalho();

}

function buscarPlanoTrabalho() {
	/*divCarregando();
	$.ajax({
   		type: "POST",
   		url: "emenda.php?modulo=principal/limparReformulacao&acao=A",
   		data: "requisicao=buscarPlanoTrabalho&ptrcod="+document.getElementById('ptrcod').value,
   		async: false,
   		success: function(msg){
   			document.getElementById('lista_planotrabalho').innerHTML = msg;
   		}
 		});
 	divCarregado();*/
 	$('#requisicao').val('buscar');
 	$('#formulario').submit();
}

function validaChk(){
	var habilita = 'false';
	$('input[type=checkbox]').each(function(){
		if($(this).attr('checked') ){
			habilita = 'true';			
		}
	});
	
	if( habilita == 'true' ){
		$('#Excluir').attr('disabled', false);
	} else {
		$('#Excluir').attr('disabled', true);
	}
}

</script>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value="">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloDireita" width="25%">Informe o Número do PTA:</td>
		<td><? 
		$ptrcod = $_POST['ptrcod'];
		echo campo_texto( 'ptrcod', 'N', 'S', '', 11, 10, '#########', '', '', '', '', 'id="ptrcod"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Justificativa:</td>
		<td><?=campo_textarea('refjustificativaexclusao', 'S', 'S', 'Justificativa', 100, 5, 2000, '', '', '', '', 'Justificativa');?></td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro">
			<input type="button" name="buscar" value="Buscar" onclick="buscarPlanoTrabalho();">
		</td>
	</tr>
	</table>
<?php

if( ($_POST['requisicao'] == 'buscar' || $_REQUEST['popup'] == 'S') && $_POST['ptrcod'] ){
	
	$sql = "SELECT 
				CASE WHEN ptt.ptrid = (SELECT max(ptrid) FROM emenda.planotrabalho WHERE ptrcod = ptt.ptrcod and ptrstatus = 'A')
					THEN '<center><img src=\"/imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer;\" onclick=\"excluirReformulacao('||ptt.ptrid||')\"></center>'
					ELSE '<center><img src=\"/imagens/excluir_01.gif\" title=\"Excluir\" style=\"cursor: pointer;\"></center>'
				END as acao, 
				array_to_string(array(SELECT tr.trefnome
			                          FROM emenda.reformulatipos rt 
			                              inner join emenda.tiporeformulacao tr on tr.trefid = rt.trefid and tr.trefstatus = 'A'
			                          WHERE 
			                              rt.refid = pt.refid), ', <br>') as descricao,
				to_char(pt.refdatainclusao,'dd/mm/YYYY HH24:MI') AS refdatainclusao,
				case when pt.refsituacaoreformulacao = 'C' then 'Em andamento'
			    when pt.refsituacaoreformulacao = 'F' then 'Finalizado' else 'Excluído' end as situacao
			FROM emenda.ptminreformulacao pt 
			INNER JOIN emenda.planotrabalho ptt ON ptt.ptrid = pt.ptrid 
			WHERE 
				ptt.ptridpai IS NOT NULL 
				AND ptt.ptrcod = {$_POST['ptrcod']} 
				and pt.refsituacaoreformulacao <> 'E'
			GROUP BY ptt.ptrid, ptt.ptrcod, pt.refid, pt.refsituacaoreformulacao, pt.refdatainclusao
			ORDER BY ptt.ptrid ASC";
	
	//$arr = $db->carregar($sql);
	
	/*if($arr[0]) {
		foreach($arr as $key => $ar) {
			$arrLista[$key]['acao'] = $ar['acao'];
			$arrLista[$key]['ptrid'] = $ar['ptrid'];
			$arrLista[$key]['ptridpai'] = $ar['ptridpai'];
			$arrLista[$key]['ptrexercicio'] = $ar['ptrexercicio'];
			$arrLista[$key]['descricao'] = $ar['descricao'];
			
			$tipoRef = pegaTipoReformulacao($ar['ptrid']);
			unset($arrRef);
			if($tipoRef) {
				foreach($tipoRef as $ref) {
					$arrRef[] = $ref['descricao'];
				}
			}
			
			$arrLista[$key]['tiporef'] = (($arrRef)?implode(", ",$arrRef):"Não informado");
			
			
		}
	} else {
		$arrLista = array();
	}*/
	
	
	$cabecalho = array("Ação", "Descrição", "Data", "Situação");
	echo monta_titulo2('Lista de Reformulações', '');
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '95%', $par2, true, '', '', true);
}
?>

</form>