<?php

if( $_REQUEST['requisicao'] == 'delete' ){
	$sql = " UPDATE proinfantil.valorreferencianproinfancia SET vrpstatus='I' WHERE vrpid=".$_REQUEST['vrpid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelaapoio/valorReferenciaProinfantil');
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$vrpdatainicial = $_POST['vrpdatainicial'];
	$vrpdatafinal 	= $_POST['vrpdatafinal'];
	
	$vrpvalor = $_POST['vrpvalor'];
	$vrpvalor = str_replace('.','',$vrpvalor);
	$vrpvalor = str_replace(',','.',$vrpvalor);
	
	$ttudsc = $db->pegaUm("SELECT ttudsc FROM proinfantil.tipoturma WHERE ttuid = {$_POST['ttuid']}");
	$tatdsc = $db->pegaUm("SELECT tatdsc FROM proinfantil.tipoatendimentoturma WHERE tatid = {$_POST['tatid']}");
	
	$vrpdescricao = trim($ttudsc).' '.trim($tatdsc);
	
	if( $_POST['vrpid'] ){		
		$sql = "UPDATE proinfantil.valorreferencianproinfancia SET 
				  	vrpano 			= '{$_POST['vrpano']}',
				  	vrpdatainicial 	= '{$vrpdatainicial}',
				  	vrpdatafinal 	= '{$vrpdatafinal}',
				  	vrpvalor 		= '{$vrpvalor}',
				  	ttuid 			= {$_POST['ttuid']},
				  	tatid 			= {$_POST['tatid']},
				  	vrpdescricao 	= '{$vrpdescricao}'				 
				WHERE 
				  	vrpid = {$_POST['vrpid']}";
		
		$db->executar($sql);					
		$db->commit();
		$db->sucesso('sistema/tabelaapoio/valorReferenciaProinfantil');
	} else {
		
		$sql = "SELECT count(vrpid) as total FROM proinfantil.valorreferencianproinfancia  
				WHERE vrpstatus = 'A' 
				and ttuid = ".$_POST['ttuid']." 
				and tatid = '".$_POST['tatid']."'
				and vrpano = '".$_POST['vrpano']."'
				AND ( ( to_char(vrpdatainicial, 'YYYY-MM-DD') BETWEEN '$vrpdatainicial'and '$vrpdatafinal'
						or 
					     to_char(vrpdatafinal, 'YYYY-MM-DD')  BETWEEN '$vrpdatainicial' and '$vrpdatafinal'
					     )
					     or 
					     (
					     '$vrpdatainicial' BETWEEN to_char(vrpdatainicial, 'YYYY-MM-DD') and to_char(vrpdatafinal, 'YYYY-MM-DD')
						or 
					     '$vrpdatafinal' BETWEEN to_char(vrpdatainicial, 'YYYY-MM-DD') and to_char(vrpdatafinal, 'YYYY-MM-DD')
					     )
					   )";
			
		$total = $db->pegaUm($sql);
		if($total == 0){
			$sql = "INSERT INTO proinfantil.valorreferencianproinfancia(vrpano, vrpdatainicial, vrpdatafinal, vrpvalor, ttuid, tatid, vrpdescricao) 
					VALUES (
					  '{$_POST['vrpano']}',
					  '{$vrpdatainicial}',
					  '{$vrpdatafinal}',
					  '{$vrpvalor}',
					  {$_POST['ttuid']},
					  {$_POST['tatid']},
					  '{$vrpdescricao}'
					)";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('sistema/tabelaapoio/valorReferenciaProinfantil');
			exit;
		} else{
			print '<script>
						alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
						//window.location.href = "proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaProinfantil&acao=A";
				  </script>';
			//exit;
		}
	}
}

if( $_REQUEST['vrpid'] ){
	$sql = "SELECT 
				vrpid, 
				vrpano, 
				to_char(vrpdatainicial::timestamp,'DD/MM/YYYY') as vrpdatainicial, 
				to_char(vrpdatafinal::timestamp,'DD/MM/YYYY') as vrpdatafinal, 
				vrpvalor, 
				ttuid,
				tatid, 
				vrpdescricao
			FROM proinfantil.valorreferencianproinfancia 
			WHERE vrpid = {$_REQUEST['vrpid']}";
	
	$arrDados = $db->pegaLinha($sql);
	extract($arrDados);
	$vrpdatainicial = formata_data_sql($vrpdatainicial);
	$vrpdatafinal = formata_data_sql($vrpdatafinal);
} else {
	extract($_POST);
	$vrpdatainicial = formata_data_sql($vrpdatainicial);
	$vrpdatafinal = formata_data_sql($vrpdatafinal);
	$vrpvalor = $_POST['vrpvalor'];
	$vrpvalor = str_replace('.','',$vrpvalor);
	$vrpvalor = str_replace(',','.',$vrpvalor);
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Valor Referência Proinfância', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="vrpid" value="<? echo $vrpid; ?>" />
<input type="hidden" name="requisicao" id="requisicao" value="" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr >
		<td align='right' class="subtitulodireita">Etapa / Modalidade:</td>
		<td>
			<?php
				$sql = "SELECT
							ttuid as codigo, 
                            ttudsc as descricao
						FROM
						 	proinfantil.tipoturma t
						WHERE ttustatus = 'A' 
						ORDER BY
						 	ttudsc;";
				
				$db->monta_combo('ttuid', $sql, 'S', '-- Selecione --', '', '', '', '', 'S', 'ttuid', '', $ttuid);
			?>
		</td>
	</tr>
	<tr >
		<td align='right' class="subtitulodireita">Tipo Atendimento:</td>
		<td>
			<?php
				$sql = "SELECT 
						  tatid as codigo,
						  tatdsc as descricao
						FROM 
						  proinfantil.tipoatendimentoturma 
						WHERE tatstatus = 'A'";
				
				$db->monta_combo('tatid', $sql, 'S', '-- Selecione --', '', '', '', '', 'S', 'tatid', '', $tatid);
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Período:</td>
		<td>
			<?=campo_data2('vrpdatainicial', 'S', 'S', '', 'S', '', '', $vrpdatainicial, '', '', 'vrpdatainicial'); ?>&nbsp;&nbsp;à&nbsp;&nbsp;
			<?=campo_data2('vrpdatafinal', 'S', 'S', '', 'S', '', '', $vrpdatafinal, '', '', 'vrpdatafinal'); ?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor:</td>
		<td>
			<? 
			if($vrpvalor) $vrpvalor = number_format($vrpvalor,2,",",".");  
			echo  campo_texto('vrpvalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $vrpvalor); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" width="45%">Ano:</td>
		<td>
			<?
			$vrpano = ( $vrpano ? $vrpano : $_SESSION['exercicio']);
			echo  campo_texto('vrpano', 'S', 'S', '', 17, 4, '[#]', '', 'right', '', 0, '', '', $vrpano); ?>
		</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<input type="button" class="botao" name="btcancela" value="Cancelar" onclick="javascript: window.location.href='proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaProinfantil&acao=A'">
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "SELECT
				'<img border=0 style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarPeriodo('||o.vrpid||')\" /> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaProinfantil&acao=A&vrpid=' || o.vrpid || '&requisicao=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as opcao, 
				o.vrpdescricao,
                vrpano,
				t.ttudsc,
				ta.tatdsc as tipo,
				to_char(o.vrpdatainicial, 'DD/MM/YYYY') as dtini,
				to_char(o.vrpdatafinal, 'DD/MM/YYYY') as dtfim,		
				o.vrpvalor as valor
			FROM 
				proinfantil.valorreferencianproinfancia o
				inner join proinfantil.tipoturma t on t.ttuid = o.ttuid
                inner join proinfantil.tipoatendimentoturma ta on ta.tatid = o.tatid
			WHERE
				o.vrpstatus = 'A'
                and ta.tatstatus = 'A'
		  	ORDER BY 
				t.ttudsc";
	
	$cabecalho = array( "Opções", "Descrição", "Ano", "Etapa / Modalidade", "Tipo Atendimento", "Data Início","Data Fim","Valor");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
<script type="text/javascript">
function validaForm(){

	var erro = false;
	var nome = '';
	//var strNome = '';
	
	$('.obrigatorio, .CampoEstilo').each(function(){
		if( $(this).val() == '' ){
			nome = $(this).attr('name');
			//strNome += nome+'\n';
			erro = true;
			return false;
		}
	});
	
	if( erro  ){
		alert('Existe(m) campo(s) obrigátorio(s) em branco!');
		$('[name="ttuid"]').focus();
		return false;
	} else {
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	}
}

function alterarPeriodo(vrpid){
	window.location.href = 'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaProinfantil&acao=A&vrpid='+vrpid;
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>