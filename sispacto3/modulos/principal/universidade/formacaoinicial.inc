<?
$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['docidformacaoinicial'] );

if($estado['esdid'] != ESD_ABERTO_FORMACAOINICIAL) {
	$consulta = true;
}


?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>
function inserirOuvinte(fioid) {
	window.open('sispacto3.php?modulo=principal/universidade/inserirouvinte&acao=A&fioid='+fioid,'Ouvinte','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function marcarTodos(obj) {
	jQuery("[name^='iusd['][value='"+obj.value+"']").attr('checked',true);
}

function salvarFormacaoInicial() {
	var total_radio = jQuery("[name^='iusd['][type=radio]").length/2; 
	var num_n_marcados = jQuery("[name^='iusd['][type=radio]:checked").length;
	
	if(total_radio != num_n_marcados) {
		alert('É necessário preencher todas as informações sobre Presente / Ausente.');
		return false;
	} else {
		var conf = confirm('Confirma os registros de frequência?');
	}
	
	if(conf) {
		divCarregando();
		jQuery('#formulario').submit();
	}
}

function carregarOuvintes() {
	ajaxatualizar('requisicao=carregarOuvintesFormacaoInicial&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>','dv_ouvintes');
}

function excluirOuvinte(fioid) {
	var conf = confirm('Deseja realmente excluir ouvinte?');
	if(conf) {
		ajaxatualizar('requisicao=excluirOuvinte&fioid='+fioid,'');
		carregarOuvintes();
	}
}

jQuery(document).ready(function() {
	divCarregando();
	jQuery("[id^='btn_turma_']").each(function() {
		jQuery(this).click();
	});
	
	jQuery("[id^='btn_turma_']").each(function() {
		jQuery(this).css('display','none');
	});
	<? if($consulta) : ?>	
	jQuery("#dv_ouvintes img").remove();
	jQuery("#inserirouvintes").remove();
	jQuery("#salvarformacaoinicial").remove();
	jQuery("[name^='iusd[']").attr('disabled','disabled');
	jQuery("[name^='marcartodos']").attr('disabled','disabled');
	<? endif; ?>
	divCarregado();
});

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Formação Inicial</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>

	<tr>
		<td colspan="2">
		<form method="post" name="formulario" id="formulario">
		<input type="hidden" name="requisicao" value="salvarFormacaoInicial">
		<table width="100%">
			<tr>
				<td cospan="2" class="SubTituloCentro">Lista Orientadores Substitutos</td>
			</tr>
			<tr>
				<td>
				<p style="color:red;">Os participantes do curso Formação Inicial que não constam na lista dos pré-cadastrados (participaram como ouvintes) e participantes que irão substituir algum Orientador de Estudo cadastrado (Ausente), estes deverão ser cadastrados.</p>
				<p><input type="button" value="Inserir Orientadore Substituto" onclick="inserirOuvinte('');" id="inserirouvintes"></p>
				
				<div id="dv_ouvintes">
				<? carregarOuvintesFormacaoInicial(array('uncid'=>$_SESSION['sispacto3']['universidade']['uncid'])); ?>
				</div>
				
				</td>
				<td width="5%" valign="top">&nbsp;</td>
			</tr>
			<tr>
				<td cospan="2" class="SubTituloCentro">Lista Pré-Cadastrados</td>
			</tr>
			<tr>
				<td valign="top">
				<input type="hidden" name="formacaoinicial" id="formacaoinicial" value="true">
				<?=carregarTurmas(array("formacaoinicial"=>true,"consulta"=>true,"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'],"pflcodturma"=>PFL_FORMADORIES)) ?>
				<br>
				<b>Orientadores sem turmas</b>
				
				<?
				
				$sql = "SELECT '<center>'|| CASE WHEN SUBSTR(i.iuscpf,1,3)!='SIS' THEN '<input type=radio name=\"iusd['||i.iusd||']\" value=\"TRUE\" '||CASE WHEN i.iusformacaoinicialorientador=true THEN 'checked' ELSE '' END||'> Presente <input type=radio name=\"iusd['||i.iusd||']\" value=\"FALSE\" '||CASE WHEN i.iusformacaoinicialorientador=false THEN 'checked' ELSE '' END||'> Ausente' ELSE '' END ||'</center>' as acao, replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, i.iusemailprincipal, m.estuf || ' / ' || m.mundescricao as municipio, CASE WHEN pp.muncod IS NULL THEN 'Estadual' ELSE 'Municipal' END as esfera 
						FROM sispacto3.identificacaousuario i 
						INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = ".PFL_ORIENTADORESTUDO."
						LEFT JOIN sispacto3.orientadorestudoturma ot ON i.iusd = ot.iusd
						LEFT JOIN sispacto3.pactoidadecerta pp ON pp.picid = i.picid
						LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
						WHERE i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND ot.otuid IS NULL ORDER BY i.iusnome";
				
				$cabecalho = array("&nbsp;","CPF","Nome","Email","UF/Município","Esfera");
				$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
				
				
				
				?>
				
				</td>
				<td width="5%" valign="top"><?
				/* Barra de estado atual e ações e Historico */
				wf_desenhaBarraNavegacao( $_SESSION['sispacto3']['universidade']['docidformacaoinicial'], array() );
				?></td>
			</tr>
		</table>
		</form>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=orcamento';">
			<input type="button" value="Salvar" onclick="salvarFormacaoInicial();" id="salvarformacaoinicial">
			
		</td>
	</tr>
</table>
