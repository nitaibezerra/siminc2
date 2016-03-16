<?php

if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = " UPDATE proinfantil.valoraluno SET vaastatus='I' WHERE vaaid=".$_REQUEST['vaaid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelaapoio/periodovalor');
}

if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$valor = $_POST['vaavalor'];
	$valor = str_replace('.','',$valor);
	$valor = str_replace(',','.',$valor);
	
	$dtini = $_POST['dtini'];
	$dtfim = $_POST['dtfim'];
	
	$dtinip = formata_data_sql($dtini);
	$dtfimp = formata_data_sql($dtfim);
	
	
	if( $_POST['vaaid'] ){
		$sql = "UPDATE 
					proinfantil.valoraluno 
				SET 
					timid = '".$_POST['timid']."',
					vaadatainicial = '".$dtinip."',
					vaadatafinal = '".$dtfimp."',
					vaavalor = '".$valor."',
					vaatipo = '".$_POST['vaatipo']."'
				WHERE 
					vaaid = '".$_POST['vaaid']."';";
	
		$db->executar($sql);					
		$db->commit();
		$db->sucesso('sistema/tabelaapoio/periodovalor');
	} else {
		
		$sql = "SELECT count(vaaid) as total FROM proinfantil.valoraluno  
			WHERE vaastatus = 'A' 
			and timid = ".$_POST['timid']." 
			and vaatipo = '".$_POST['vaatipo']."'
			AND (
				     (
				     to_char(vaadatainicial, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
					or 
				     to_char(vaadatafinal, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
				     )
				     or 
				     (
				     '$dtinip' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
					or 
				     '$dtfimp' BETWEEN to_char(vaadatainicial, 'YYYY-MM-DD') and to_char(vaadatafinal, 'YYYY-MM-DD')
				     )
				   )";
			
		$total = $db->pegaUm($sql);
		if($total == 0){
			$sql = " INSERT INTO proinfantil.valoraluno(
					 	timid, 
					 	vaadatainicial,
					 	vaadatafinal,
					 	vaavalor,
					 	vaatipo,
					 	vaastatus
					 ) VALUES (
					 	'".$_POST['timid']."',
					 	'".$dtinip."',  
					 	'".$dtfimp."',
					 	'".$valor."',
					 	'".$_POST['vaatipo']."',
					 	'A'
					 );";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('sistema/tabelaapoio/periodovalor');
		} else{
			print '<script>
						alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
						window.location.href = "proinfantil.php?modulo=sistema/tabelaapoio/periodovalor&acao=A";
				  </script>';
			exit;
		}
	}
}

if( $_REQUEST['vaaid'] ){
	$sql = "SELECT 
				vaaid, 
				timid,
				vaatipo,
				to_char(vaadatainicial::timestamp,'DD/MM/YYYY') as dtini,
				to_char(vaadatafinal::timestamp,'DD/MM/YYYY') as dtfim,
				vaavalor
			FROM  
				proinfantil.valoraluno  
			WHERE 
				vaaid = ".$_REQUEST['vaaid'];
	
	$arrDados = $db->pegaLinha($sql);
	
	$timid = $arrDados['timid'] ? $arrDados['timid'] : $_REQUEST['timid'];
	//$modid = ($_REQUEST['modid'] ? $_REQUEST['modid'] : $db->pegaUm("SELECT modid FROM proinfantil.tipomodalidade WHERE timstatus = 'A' and timid = $timid"));
	//$arrDados['modid'] = $modid;
	
	extract($arrDados);
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Valor Referência Proinfância', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="vaaid" id="vaaid" value="<? echo $vaaid ?>" />
<input type="hidden" name="requisicao" id="requisicao" value="" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<!--  <tr>
		<td align='right' class="subtitulodireita">Módulo:</td>
		<td>
			<?php
				$sql = "SELECT 
						  modid as codigo,
						  moddsc as descricao
						FROM 
						  proinfantil.modulo 
						where modstatus = 'A'";
				
				$db->monta_combo("modid",$sql,'S',"-- Selecione --", 'carregarTipoModulo','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr> -->
	<tr >
		<td align='right' class="subtitulodireita">Etapa / Modalidade:</td>
		<td>
			<?php
				$sql = "SELECT
						 timid AS codigo,
						 timdescricao AS descricao
						FROM
						 proinfantil.tipomodalidade 
						 WHERE timstatus = 'A'
						 	and modid =  1
						ORDER BY
						 2;";
				
				$db->monta_combo("timid",$sql,'S',"-- Selecione --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr >
		<td align='right' class="subtitulodireita">Tipo:</td>
		<td>
			<?php
				$sql = "SELECT 'I' AS codigo, 'Integral' AS descricao
						UNION
						SELECT 'P' AS codigo, 'Parcial' AS descricao";
				
				$db->monta_combo("vaatipo",$sql,'S',"-- Selecione --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Período:</td>
	<td>
		
		<?= campo_data2( 'dtini', 'S', 'S', '', '' ); ?>
		&nbsp;
		à
		&nbsp;
		
		<?= campo_data2( 'dtfim', 'S', 'S', '', '' ); ?>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor</td>
	<td>
		<? 
			
			if($vaavalor) $vaavalor = number_format($vaavalor,2,",",".");  
		?>
		<?= campo_texto( 'vaavalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, ''); ?>
	</td>
	</tr>
	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<?php
		if (isset($arrDados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "select
				opcao,
				descricao||' '||tipo as desc,
				dtini,
				dtfim,
				valor
			from(
				SELECT
					'<center><a href=\"#\" onclick=\"alterarPeriodo(' || o.vaaid || ');\"><img border=0 src=\"../imagens/alterar.gif\" /></a> 
					 <a style=\"cursor:pointer\" onclick=\"excluirPeriodo(' || o.vaaid || ');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a></center>' as opcao, 
					t.timdescricao as descricao,
					CASE WHEN o.vaatipo = 'I' THEN
						'Integral'
					ELSE
						'Parcial'
					END as tipo,
					to_char(o.vaadatainicial::date,'DD/MM/YYYY') as dtini,
					to_char(o.vaadatafinal::date,'DD/MM/YYYY') as dtfim,		
					o.vaavalor as valor
				FROM 
					proinfantil.valoraluno o
					inner join proinfantil.tipomodalidade t on t.timid = o.timid
				WHERE
					vaastatus = 'A'
			  	ORDER BY 
					 5, 2
			) as foo";
	$cabecalho = array( "Opções","Etapa / Modalidade / Tipo", "Data Início","Data Fim","Valor");
	$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', 'S', '', '', '', true);
?>
<script type="text/javascript">

function carregarTipoModulo( modid ){
	if( modid != '' ){
		document.formulario.submit();
	}
}

function alterarPeriodo(vaaid){
	$('#vaaid').val(vaaid);
	$('#formulario').submit();
}

function excluirPeriodo(vaaid){
	$('#vaaid').val(vaaid);
	$('#requisicao').val('excluir');
	$('#formulario').submit();
}

function validaForm(){
	
	if(document.formulario.timid.value == ''){
		alert ('O campo Etapa / Modalidade deve ser preenchido.');
		document.formulario.timid.focus();
		return false;
	}
	if(document.formulario.vaatipo.value == ''){
		alert ('O campo Tipo deve ser preenchido.');
		document.formulario.vaatipo.focus();
		return false;
	}
	if(document.formulario.dtini.value == ''){
		alert ('O campo Período Início deve ser preenchido.');
		document.formulario.dtini.focus();
		return false;
	}
	if(document.formulario.dtfim.value == ''){
		alert ('O campo Período Fim deve ser preenchido.');
		document.formulario.dtfim.focus();
		return false;
	}
	if(document.formulario.dtini.value != '' && document.formulario.dtfim.value != ''){ 
		if (!validaDataMaior(document.formulario.dtini, document.formulario.dtfim)){
			alert("O Período Início não pode ser maior que o Período Fim.");
			document.formulario.dtfim.focus();
			return false;
		}
	}	
	if(document.formulario.vaavalor.value == ''){
		alert ('O campo Valor deve ser preenchido.');
		document.formulario.vaavalor.focus();
		return false;
	}
	$('#requisicao').val('salvar');
	document.formulario.submit();

}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>