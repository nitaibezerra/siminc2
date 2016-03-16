<?php

if( $_REQUEST['requisicao'] == 'delete' ){
	$sql = " UPDATE proinfantil.valorreferencianovasturmas SET vrtstatus='I' WHERE vrtid=".$_REQUEST['vrtid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelaapoio/valorReferenciaNovasTurmas');
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$vrtdatainicial = $_POST['vrtdatainicial'];
	$vrtdatafinal 	= $_POST['vrtdatafinal'];
	
	$vrtvalor = $_POST['vrtvalor'];
	$vrtvalor = str_replace('.','',$vrtvalor);
	$vrtvalor = str_replace(',','.',$vrtvalor);
	
	$ttudsc = $db->pegaUm("SELECT ttudsc FROM proinfantil.tipoturma WHERE ttuid = {$_POST['ttuid']}");
	$tatdsc = $db->pegaUm("SELECT tatdsc FROM proinfantil.tipoatendimentoturma WHERE tatid = {$_POST['tatid']}");
	$tirdescricao = $db->pegaUm("SELECT tirdescricao FROM proinfantil.tiporede WHERE tirid = {$_POST['tirid']}");
	
	$vrtdescricao = trim($ttudsc).' '.trim($tirdescricao).' '.trim($tatdsc);
	
	if( $_POST['vrtid'] ){		
		$sql = "UPDATE proinfantil.valorreferencianovasturmas SET 
				  	vrtano 			= '{$_POST['vrtano']}',
				  	vrtdatainicial 	= '{$vrtdatainicial}',
				  	vrtdatafinal 	= '{$vrtdatafinal}',
				  	vrtvalor 		= '{$vrtvalor}',
				  	ttuid 			= {$_POST['ttuid']},
				  	tirid 			= {$_POST['tirid']},
				  	tatid 			= {$_POST['tatid']},
				  	vrtdescricao 	= '{$vrtdescricao}'				 
				WHERE 
				  	vrtid = {$_POST['vrtid']}";
		
		$db->executar($sql);					
		$db->commit();
		$db->sucesso('sistema/tabelaapoio/valorReferenciaNovasTurmas');
	} else {
		
		$sql = "SELECT count(vrtid) as total FROM proinfantil.valorreferencianovasturmas  
				WHERE vrtstatus = 'A' 
				and ttuid = ".$_POST['ttuid']." 
				and tirid = '".$_POST['tirid']."'
				and tatid = '".$_POST['tatid']."'
				and vrtano = '".$_POST['vrtano']."'
				AND ( ( to_char(vrtdatainicial, 'YYYY-MM-DD') BETWEEN '$vrtdatainicial'and '$vrtdatafinal'
						or 
					     to_char(vrtdatafinal, 'YYYY-MM-DD')  BETWEEN '$vrtdatainicial' and '$vrtdatafinal'
					     )
					     or 
					     (
					     '$vrtdatainicial' BETWEEN to_char(vrtdatainicial, 'YYYY-MM-DD') and to_char(vrtdatafinal, 'YYYY-MM-DD')
						or 
					     '$vrtdatafinal' BETWEEN to_char(vrtdatainicial, 'YYYY-MM-DD') and to_char(vrtdatafinal, 'YYYY-MM-DD')
					     )
					   )";
			
		$total = $db->pegaUm($sql);
		if($total == 0){
			$sql = "INSERT INTO proinfantil.valorreferencianovasturmas(vrtano, vrtdatainicial, vrtdatafinal, vrtvalor, ttuid, tirid, tatid, vrtdescricao) 
					VALUES (
					  '{$_POST['vrtano']}',
					  '{$vrtdatainicial}',
					  '{$vrtdatafinal}',
					  '{$vrtvalor}',
					  {$_POST['ttuid']},
					  {$_POST['tirid']},
					  {$_POST['tatid']},
					  '{$vrtdescricao}'
					)";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('sistema/tabelaapoio/valorReferenciaNovasTurmas');
			exit;
		} else{
			print '<script>
						alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
						//window.location.href = "proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaNovasTurmas&acao=A";
				  </script>';
			//exit;
		}
	}
}

if( $_REQUEST['vrtid'] ){
	$sql = "SELECT 
				vrtid, 
				vrtano, 
				to_char(vrtdatainicial::timestamp,'DD/MM/YYYY') as vrtdatainicial, 
				to_char(vrtdatafinal::timestamp,'DD/MM/YYYY') as vrtdatafinal, 
				vrtvalor, 
				ttuid, 
				tirid, 
				tatid, 
				vrtdescricao
			FROM proinfantil.valorreferencianovasturmas 
			WHERE vrtid = {$_REQUEST['vrtid']}";
	
	$arrDados = $db->pegaLinha($sql);
	extract($arrDados);
	$vrtdatainicial = formata_data_sql($vrtdatainicial);
	$vrtdatafinal = formata_data_sql($vrtdatafinal);
} else {
	extract($_POST);
	$vrtdatainicial = formata_data_sql($vrtdatainicial);
	$vrtdatafinal = formata_data_sql($vrtdatafinal);
	$vrtvalor = $_POST['vrtvalor'];
	$vrtvalor = str_replace('.','',$vrtvalor);
	$vrtvalor = str_replace(',','.',$vrtvalor);
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Valor Referência Novas Turmas', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="vrtid" value="<? echo $vrtid; ?>" />
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
	<tr >
		<td align='right' class="subtitulodireita">Tipo Rede:</td>
		<td>
			<?php
				$sql = "SELECT 
						  tirid as codigo,
						  tirdescricao as descricao
						FROM 
						  proinfantil.tiporede";
				
				$db->monta_combo('tirid', $sql, 'S', '-- Selecione --', '', '', '', '', 'S', 'tirid', '', $tirid);
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Período:</td>
		<td>
			<?
			$vrtdatainicial = $vrtdatainicial ? $vrtdatainicial : formata_data_sql('01/01/'.$_SESSION['exercicio']);
			$vrtdatafinal = $vrtdatafinal ? $vrtdatafinal : formata_data_sql('31/12/'.$_SESSION['exercicio']);
			echo campo_data2('vrtdatainicial', 'S', 'S', '', 'S', '', '', $vrtdatainicial, '', '', 'vrtdatainicial'); ?>&nbsp;&nbsp;à&nbsp;&nbsp;
			<?=campo_data2('vrtdatafinal', 'S', 'S', '', 'S', '', '', $vrtdatafinal, '', '', 'vrtdatafinal'); ?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor:</td>
		<td>
			<? 
			if($vrtvalor) $vrtvalor = number_format($vrtvalor,2,",",".");  
			echo  campo_texto('vrtvalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $vrtvalor); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" width="45%">Ano:</td>
		<td>
			<?
			$vrtano = ( $vrtano ? $vrtano : $_SESSION['exercicio']);
			echo  campo_texto('vrtano', 'S', 'S', '', 17, 4, '[#]', '', 'right', '', 0, '', '', $vrtano); ?>
		</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<input type="button" class="botao" name="btcancela" value="Cancelar" onclick="javascript: window.location.href='proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaNovasTurmas&acao=A'">
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "SELECT
				'<img border=0 style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarPeriodo('||o.vrtid||')\" /> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaNovasTurmas&acao=A&vrtid=' || o.vrtid || '&requisicao=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as opcao, 
				o.vrtdescricao,
                vrtano,
				t.ttudsc,
				ta.tatdsc as tipo,
                tr.tirdescricao,
				to_char(o.vrtdatainicial, 'DD/MM/YYYY') as dtini,
				to_char(o.vrtdatafinal, 'DD/MM/YYYY') as dtfim,		
				o.vrtvalor as valor
			FROM 
				proinfantil.valorreferencianovasturmas o
				inner join proinfantil.tipoturma t on t.ttuid = o.ttuid
                inner join proinfantil.tiporede tr on tr.tirid = o.tirid
                inner join proinfantil.tipoatendimentoturma ta on ta.tatid = o.tatid
			WHERE
				o.vrtstatus = 'A'
                and ta.tatstatus = 'A'
                and o.vrtano = '{$_SESSION['exercicio']}'
		  	ORDER BY 
				t.ttudsc";
	
	$cabecalho = array( "Opções", "Descrição", "Ano", "Etapa / Modalidade", "Tipo Atendimento", "Tipo Rede", "Data Início","Data Fim","Valor");
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
		$('[name="'+nome+'"]').focus();
		return false;
	} else {
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	}
}

function alterarPeriodo(vrtid){
	window.location.href = 'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaNovasTurmas&acao=A&vrtid='+vrtid;
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>