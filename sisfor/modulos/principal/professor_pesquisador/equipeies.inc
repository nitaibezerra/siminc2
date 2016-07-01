<?php 
$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if($estado['esdid'] != ESD_PROJETO_EMCADASTRAMENTO || in_array(PFL_CONSULTAGERAL,$perfil)) {
	$consulta = true;
} ?>

<script>
function inserirQtdEquipeIES() {

	jQuery('#formulario').submit();
	
}

function preencherTotais() {

	var totalqtd = 0;
	jQuery("[name^='epiqtd[']").each(function() {
		if(jQuery(this).val()!='') {
			totalqtd += parseFloat(jQuery(this).val());
		}
	});
	
	jQuery('#epiqtdtotal').val(totalqtd);

	jQuery("[name^='dadosperfil[]']").each(function() {
		var totalporperfil = 0;
		var valorun = parseFloat(jQuery(this).val());

		var qtd = 0;
		if(jQuery('#epiqtd_'+jQuery(this).attr('id')).val()!='') {
			var qtd = parseFloat(jQuery('#epiqtd_'+jQuery(this).attr('id')).val());
		}

		totalporperfil = qtd*valorun;

		jQuery('#epivalor_'+jQuery(this).attr('id')).val(mascaraglobal('###.###.###,##',totalporperfil.toFixed(2)));

	});

	var totalvalor = 0;
	jQuery("[name^='epivalor[']").each(function() {
		if(jQuery(this).val()!='') {
			var valor = replaceAll(replaceAll(jQuery(this).val(),".",""),",",".");
			totalvalor += parseFloat(valor);
		}
	});

	jQuery('#epivalortotal').val(mascaraglobal('###.###.###,##',totalvalor.toFixed(2)));
	
}

function inserirBolsasCalculadora() {

	var conf = confirm('Deseja realmente utilizar os valores da calculadora?');
	
	if(conf) {

		jQuery('#epiqtd_<?=PFL_SUPERVISOR_IES?>').val(jQuery('#qtdcalc_<?=PFL_SUPERVISOR_IES?>').val());
		jQuery('#epiqtd_<?=PFL_COORDENADOR_ADJUNTO_IES?>').val(jQuery('#qtdcalc_<?=PFL_COORDENADOR_ADJUNTO_IES?>').val());
		jQuery('#epiqtd_<?=PFL_COORDENADOR_CURSO?>').val(jQuery('#qtdcalc_<?=PFL_COORDENADOR_CURSO?>').val());
	
		jQuery('#epiqtd_<?=PFL_FORMADOR_IES?>').val(jQuery('#qtdcalc_<?=PFL_FORMADOR_IES?>').val());
		jQuery('#epiqtd_<?=PFL_PROFESSOR_PESQUISADOR?>').val(jQuery('#qtdcalc_<?=PFL_PROFESSOR_PESQUISADOR?>').val());
		jQuery('#epiqtd_<?=PFL_TUTOR?>').val(jQuery('#qtdcalc_<?=PFL_TUTOR?>').val());
	
		preencherTotais();

	}
}

function downloadArquivo(arqid) {
	window.location=window.location+'&requisicao=downloadArquivo&arqid='+arqid;
}

</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirQtdEquipeIES">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Nesta tela, deve-se cadastrar toda a equipe pedagógica responsável pelo curso, considerando o período de vigência informado na tela “Dados gerais do projeto”. Para calcular o número de bolsas para cada função, no caso dos cursos da SECADI, utilize a calculadora de bolsas disponibilizada no endereço <a href="http://cursosdh.wordpress.com/planilhas-secadi-2014" target="_blank"><b>http://cursosdh.wordpress.com/planilhas-secadi-2014</b></a>. No caso dos cursos da SEB, siga as orientações encaminhadas pela coordenação do curso no MEC. Em todos os casos, acrescente a memória de cálculo clicando no botão “Upload memória de cálculo”.</td>
	</tr>
	<!-- 
	<tr>
		<td class="SubTituloDireita" width="20%">Calculadoras do número de vagas</td>
		<td>
		<?php 
		/*
		$sql = "SELECT '<img src=../imagens/anexo.gif>' as acao,
			    a.arqnome||'.'||a.arqextensao as arquivo,
				CASE WHEN t.tcasecretaria='A' THEN 'SECADI'
					 WHEN t.tcasecretaria='B' THEN 'SEB' END as secretaria,
				t.tcadsc
		FROM sisfor.tabelacalculadora t
		INNER JOIN public.arquivo a ON a.arqid = t.arqid
		WHERE tcastatus='A'";
		
		$cabecalho = array("&nbsp;","Arquivo","Secretaria","Descrição");
		
		$db->monta_lista_simples($sql,$cabecalho,50,10,'N','100%',$par2);
		*/
		?>
		</td>
	</tr>
	 -->
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td>
		<? 
		
		carregarQuantitativoPorPerfil(array('sifid' => $_SESSION['sisfor']['sifid'], 'consulta' => $consulta));
		
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Upload Memória de Cálculo<br><font size=1>*Não obrigatório</font></td>
		<td>
		<input type="file" name="arquivo" id="arqidmemoria">
		<br>
		<?
		$arquivo = $db->pegaLinha("SELECT arqnome||'.'||arqextensao as arquivo, arqid FROM sisfor.sisfor s INNER JOIN public.arquivo p ON s.arqidmemoria = p.arqid WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
		
		if($arquivo) {
			echo "<img src=../imagens/anexo.gif style=cursor:pointer; onclick=downloadArquivo('".$arquivo['arqid']."');> ".$arquivo['arquivo'];
		}
		
		?>
		</td>
	</tr>
	
	<? if(!$consulta) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<input type="button" name="salvar" value="Salvar" onclick="inserirQtdEquipeIES();"> 
		</td>
	</tr>
	<? endif; ?>
</table>
</form>