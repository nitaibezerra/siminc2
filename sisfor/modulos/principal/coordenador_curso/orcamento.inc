<?

$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if(($estado['esdid'] != ESD_PROJETO_EMCADASTRAMENTO && $estado['esdid'] != ESD_PROJETO_AJUSTES) || in_array(PFL_CONSULTAGERAL,$perfil)) {
	$consulta = true;
} 

$perfil = pegarPerfil($_SESSION['usucpf']);

$acessoalteracaoorcamento = true;

if(in_array(PFL_COORDENADOR_INST, $perfil)) {
	
	$iusd_curso = $db->pegaUm("SELECT i.iusd FROM sisfor.sisfor s INNER JOIN sisfor.identificacaousuario i ON i.iuscpf = s.usucpf WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");
	$iusd_cpf   = $db->pegaUm("SELECT iusd FROM sisfor.identificacaousuario WHERE iuscpf='".$_SESSION['usucpf']."'");
	
	if($iusd_curso != $iusd_cpf) {
		$acessoalteracaoorcamento = false;
	}
	
}

?>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirCustos_s(orcid) {
	var param='&alpid='+jQuery('#alpid').val();
	if(orcid!='') {
		param += '&orcid='+orcid;
	}


	
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function inserirCustos(orcid) {
	var param='';
	if(orcid!='') {
		param = '&orcid='+orcid;
	}
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarListaCustos_s(alpid) {
	ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listacustos_s');
}


function carregarListaCustos() {
	ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_listacustos');
	ajaxatualizar('requisicao=calcularCustoAlunoCusteio&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_custoaluno');
}

function carregarNaturezaDespesasCustos() {
	ajaxatualizar('requisicao=carregarNaturezaDespesasCustos&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','td_naturezadespesas');
}

function excluirCustos(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_listacustos');
	}

}

function excluirCustos_s(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+jQuery('#alpid').val(),'div_listacustos_s');
	}

}

function solicitarAlteracaoOrcamento() {

	var conf = confirm('Deseja realmente solicitar alteração no orçamento?');

	if(!conf) {
		return false;
	}

	jQuery('#inserircustos_s').css('display','');
	jQuery('#confirmarsolicitacao').css('display','');
	jQuery('#cancelarsolicitacao').css('display','');
	jQuery('#tbl_situacaoanterior').css('display','none;');
	

	var alpid = 0;
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=solicitarAlteracaoProjeto&sifid=<?=$_SESSION['sisfor']['sifid'] ?>',
   		async: false,
   		success: function(r){alpid=r;}
	});
	jQuery('#alpid').val(alpid);
	carregarListaCustos_s(alpid);

	jQuery("#modalSolicitacaoOrcamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });


}

function exibirSolicitacaoAlteracaoOrcamento(alpid, alpsituacao) {

	jQuery('#inserircustos_s').css('display','none');
	jQuery('#confirmarsolicitacao').css('display','none');
	jQuery('#cancelarsolicitacao').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpid='+alpid,'div_listacustos_anterior');
	jQuery('#tbl_situacaoanterior').css('display','');
	
	carregarListaCustos_s(alpid);

	jQuery("#modalSolicitacaoOrcamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function autorizarAlteracao(alpid, alpautorizado) {

	switch(alpautorizado) {
		case '2':
			var conf = confirm('Deseja realmente autorizar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpautorizado=2&alpid='+alpid,'');
			
			break;

		case '3':
			var conf = confirm('Deseja realmente recusar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid=<?=$_SESSION['sisfor']['sifid'] ?>&alpautorizado=3&alpid='+alpid,'');
			
			break;
		
	}

	window.location=window.location;
	
	
}

function confirmarSolicitacao() {
	var pt = prompt("Escreva uma justificativa para solicitação de alteração do orçamento:", "");

	if(pt=='') {
		return false;
	}
	
	ajaxatualizar('requisicao=confirmarSolicitacao&alpautorizado=1&alpid='+jQuery('#alpid').val()+'&alpjustificativa='+pt,'');
	window.location=window.location;
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='inserircustos']").remove();
});
<? endif; ?>

</script>

<div id="modalExibir" style="display:none;"></div>

<div id="modalSolicitacaoOrcamento" style="display:none;">

<input type="hidden" name="alpid" id="alpid" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos_s" onclick="inserirCustos_s('');"></p>
		<div id="div_listacustos_s"></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao" onclick="confirmarSolicitacao();"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao" onclick="jQuery('#modalSolicitacaoOrcamento').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<div id="div_listacustos_anterior"></div>
		</td>
	</tr>

</table>

</div>


<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Orientação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos" onclick="inserirCustos('');"></p>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta"=>$consulta,"sifid"=>$_SESSION['sisfor']['sifid']));
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Custo Aluno (custeio)</td>
		<td id="div_custoaluno">
		<? 
		calcularCustoAlunoCusteio(array("sifid"=>$_SESSION['sisfor']['sifid']));
		?>
		</td>
	</tr>

</table>
</form>