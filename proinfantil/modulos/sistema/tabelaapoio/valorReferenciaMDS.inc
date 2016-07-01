<?php

if( $_REQUEST['requisicao'] == 'delete' ){
	$sql = " UPDATE proinfantil.valorreferenciamds SET vrmstatus='I' WHERE vrmid=".$_REQUEST['vrmid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelaapoio/valorReferenciaMDS');
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$vrmdatainicial = $_POST['vrmdatainicial'];
	$vrmdatafinal 	= $_POST['vrmdatafinal'];
	
	$vrmvalor = $_POST['vrmvalor'];
	$vrmvalor = str_replace('.','',$vrmvalor);
	$vrmvalor = str_replace(',','.',$vrmvalor);
	
	$ttudsc = $db->pegaUm("SELECT ttudsc FROM proinfantil.tipoturma WHERE ttuid = {$_POST['ttuid']}");
	$tatdsc = $db->pegaUm("SELECT tatdsc FROM proinfantil.tipoatendimentoturma WHERE tatid = {$_POST['tatid']}");
	$tirdescricao = $db->pegaUm("SELECT tirdescricao FROM proinfantil.tiporede WHERE tirid = {$_POST['tirid']}");
	
	$vrmdescricao = trim($ttudsc).' '.trim($tatdsc).' '.trim($tirdescricao);
	
	if( $_POST['vrmid'] ){		
		$sql = "UPDATE proinfantil.valorreferenciamds SET 
				  	vrmano 			= '{$_POST['vrmano']}',
				  	vrmdatainicial 	= '{$vrmdatainicial}',
				  	vrmdatafinal 	= '{$vrmdatafinal}',
				  	vrmvalor 		= '{$vrmvalor}',
				  	ttuid 			= {$_POST['ttuid']},
				  	tatid 			= {$_POST['tatid']},
				  	tirid 			= {$_POST['tirid']},
				  	vrmdescricao 	= '{$vrmdescricao}'				 
				WHERE 
				  	vrmid = {$_POST['vrmid']}";
		
		$db->executar($sql);					
		$db->commit();
		$db->sucesso('sistema/tabelaapoio/valorReferenciaMDS');
	} else {
		
		$sql = "SELECT count(vrmid) as total FROM proinfantil.valorreferenciamds  
				WHERE vrmstatus = 'A' 
				and ttuid = ".$_POST['ttuid']." 
				and tatid = '".$_POST['tatid']."'
				and tirid = '".$_POST['tirid']."'
				and vrmano = '".$_POST['vrmano']."'
				AND ( ( to_char(vrmdatainicial, 'YYYY-MM-DD') BETWEEN '$vrmdatainicial'and '$vrmdatafinal'
						or 
					     to_char(vrmdatafinal, 'YYYY-MM-DD')  BETWEEN '$vrmdatainicial' and '$vrmdatafinal'
					     )
					     or 
					     (
					     '$vrmdatainicial' BETWEEN to_char(vrmdatainicial, 'YYYY-MM-DD') and to_char(vrmdatafinal, 'YYYY-MM-DD')
						or 
					     '$vrmdatafinal' BETWEEN to_char(vrmdatainicial, 'YYYY-MM-DD') and to_char(vrmdatafinal, 'YYYY-MM-DD')
					     )
					   )";
			
		$total = $db->pegaUm($sql);
		if($total == 0){
			$sql = "INSERT INTO proinfantil.valorreferenciamds(vrmano, vrmdatainicial, vrmdatafinal, vrmvalor, ttuid, tatid, tirid, vrmdescricao) 
					VALUES (
					  '{$_POST['vrmano']}',
					  '{$vrmdatainicial}',
					  '{$vrmdatafinal}',
					  '{$vrmvalor}',
					  {$_POST['ttuid']},
					  {$_POST['tatid']},
					  {$_POST['tirid']},
					  '{$vrmdescricao}'
					)";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('sistema/tabelaapoio/valorReferenciaMDS');
			exit;
		} else{
			print '<script>
						alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
						//window.location.href = "proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaMDS&acao=A";
				  </script>';
			//exit;
		}
	}
}

if( $_REQUEST['vrmid'] ){
	$sql = "SELECT 
				vrmid, 
				vrmano, 
				to_char(vrmdatainicial::timestamp,'DD/MM/YYYY') as vrmdatainicial, 
				to_char(vrmdatafinal::timestamp,'DD/MM/YYYY') as vrmdatafinal, 
				vrmvalor, 
				ttuid,
				tatid, 
				tirid,
				vrmdescricao
			FROM proinfantil.valorreferenciamds 
			WHERE vrmid = {$_REQUEST['vrmid']}";
	
	$arrDados = $db->pegaLinha($sql);
	extract($arrDados);
	$vrmdatainicial = formata_data_sql($vrmdatainicial);
	$vrmdatafinal = formata_data_sql($vrmdatafinal);
} else {
	extract($_POST);
	$vrmdatainicial = formata_data_sql($vrmdatainicial);
	$vrmdatafinal = formata_data_sql($vrmdatafinal);
	$vrmvalor = $_POST['vrmvalor'];
	$vrmvalor = str_replace('.','',$vrmvalor);
	$vrmvalor = str_replace(',','.',$vrmvalor);
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Valor Referência Suplementação MDS', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="vrmid" value="<? echo $vrmid; ?>" />
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
			$vrmdatainicial = $vrmdatainicial ? $vrmdatainicial : formata_data_sql('01/01/'.$_SESSION['exercicio']);
			$vrmdatafinal = $vrmdatafinal ? $vrmdatafinal : formata_data_sql('31/12/'.$_SESSION['exercicio']);
			echo campo_data2('vrmdatainicial', 'S', 'S', '', 'S', '', '', $vrmdatainicial, '', '', 'vrmdatainicial'); ?>&nbsp;&nbsp;à&nbsp;&nbsp;
			<?=campo_data2('vrmdatafinal', 'S', 'S', '', 'S', '', '', $vrmdatafinal, '', '', 'vrmdatafinal'); ?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor:</td>
		<td>
			<? 
			if($vrmvalor) $vrmvalor = number_format($vrmvalor,2,",",".");  
			echo  campo_texto('vrmvalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $vrmvalor); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" width="45%">Ano:</td>
		<td>
			<?
			$vrmano = ( $vrmano ? $vrmano : $_SESSION['exercicio']);
			echo  campo_texto('vrmano', 'S', 'S', '', 17, 4, '[#]', '', 'right', '', 0, '', '', $vrmano); ?>
		</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<input type="button" class="botao" name="btcancela" value="Cancelar" onclick="javascript: window.location.href='proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaMDS&acao=A'">
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "SELECT
				'<img border=0 style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarPeriodo('||o.vrmid||')\" /> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaMDS&acao=A&vrmid=' || o.vrmid || '&requisicao=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as opcao, 
				o.vrmdescricao,
                vrmano,
				t.ttudsc,
				ta.tatdsc as tipo,
				tr.tirdescricao as tiporede,
				to_char(o.vrmdatainicial, 'DD/MM/YYYY') as dtini,
				to_char(o.vrmdatafinal, 'DD/MM/YYYY') as dtfim,		
				o.vrmvalor as valor
			FROM 
				proinfantil.valorreferenciamds o
				inner join proinfantil.tipoturma t on t.ttuid = o.ttuid
                inner join proinfantil.tipoatendimentoturma ta on ta.tatid = o.tatid
                left join proinfantil.tiporede tr on tr.tirid = o.tirid
			WHERE
				o.vrmstatus = 'A'
                and ta.tatstatus = 'A'
                and o.vrmano = '{$_SESSION['exercicio']}'
		  	ORDER BY 
				t.ttudsc";
	
	$cabecalho = array( "Opções", "Descrição", "Ano", "Tipo Turma", "Tipo Atendimento", "Tipo Rede", "Data Início","Data Fim","Valor");
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

function alterarPeriodo(vrmid){
	window.location.href = 'proinfantil.php?modulo=sistema/tabelaapoio/valorReferenciaMDS&acao=A&vrmid='+vrmid;
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>