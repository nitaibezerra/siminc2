<?php

if( $_REQUEST['requisicao'] == 'delete' ){
	$sql = " UPDATE proinfantil.periodovaloretapa SET pvestatus='I' WHERE pveid=".$_REQUEST['pveid'];
	$db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/tabelaapoio/periodoValorEtapa');
}
if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$pvedatainicial = $_POST['pvedatainicial'];
	$pvedatafinal 	= $_POST['pvedatafinal'];
	
	$pvevalor = $_POST['pvevalor'];
	$pvevalor = str_replace('.','',$pvevalor);
	$pvevalor = str_replace(',','.',$pvevalor);
	
	if( $_POST['pveid'] ){
		$sql = "UPDATE 
					proinfantil.periodovaloretapa 
				SET 
					timid = '".$_POST['timid']."',
					pvedatainicial = '".$pvedatainicial."',
					pvedatafinal = '".$pvedatafinal."',
					pvevalor = '".$pvevalor."',
					tatid = '".$_POST['tatid']."',
					tirid = '".$_POST['tirid']."'
				WHERE 
					pveid = '".$_POST['pveid']."';";
	
		$db->executar($sql);					
		$db->commit();
		$db->sucesso('sistema/tabelaapoio/periodoValorEtapa');
	} else {
		
		$sql = "SELECT count(pveid) as total FROM proinfantil.periodovaloretapa  
				WHERE pvestatus = 'A' 
				and timid = ".$_POST['timid']." 
				and tatid = '".$_POST['tatid']."'
				and tirid = '".$_POST['tirid']."'
				AND ( ( to_char(pvedatainicial, 'YYYY-MM-DD') BETWEEN '$pvedatainicial'and '$pvedatafinal'
						or 
					     to_char(pvedatafinal, 'YYYY-MM-DD')  BETWEEN '$pvedatainicial' and '$pvedatafinal'
					     )
					     or 
					     (
					     '$pvedatainicial' BETWEEN to_char(pvedatainicial, 'YYYY-MM-DD') and to_char(pvedatafinal, 'YYYY-MM-DD')
						or 
					     '$pvedatafinal' BETWEEN to_char(pvedatainicial, 'YYYY-MM-DD') and to_char(pvedatafinal, 'YYYY-MM-DD')
					     )
					   )";
			
		$total = $db->pegaUm($sql);
		if($total == 0){
			$sql = "INSERT INTO proinfantil.periodovaloretapa(timid, pvedatainicial, pvedatafinal, pvevalor, tatid, tirid) 
					VALUES (
					 	'".$_POST['timid']."',
					 	'".$pvedatainicial."',  
					 	'".$pvedatafinal."',
					 	".$pvevalor.",
					 	".$_POST['tatid'].",
					 	".$_POST['tirid']."
					 );";
			$db->executar($sql);
			$db->commit();
			$db->sucesso('sistema/tabelaapoio/periodoValorEtapa');
		} else{
			print '<script>
						alert("Já existe este período para esta Etapa / Modalidade. \nFavor verifique o período.");
						window.location.href = "proinfantil.php?modulo=sistema/tabelaapoio/periodoValorEtapa&acao=A";
				  </script>';
			exit;
		}
	}
}

if( $_REQUEST['pveid'] ){
	$sql = "SELECT 
				pveid, 
				timid,
				tatid,
				tirid,
				to_char(pvedatainicial::timestamp,'DD/MM/YYYY') as pvedatainicial,
				to_char(pvedatafinal::timestamp,'DD/MM/YYYY') as pvedatafinal,
				pvevalor
			FROM  
				proinfantil.periodovaloretapa  
			WHERE 
				pveid = '".$_REQUEST['pveid']."'";
	
	$arrDados = $db->pegaLinha($sql);
	extract($arrDados);
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Cadastro de Período Valor Etapa', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="pveid" value="<? echo $pveid; ?>" />
<input type="hidden" name="requisicao" id="requisicao" value="" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr >
		<td align='right' class="subtitulodireita">Etapa / Modalidade:</td>
		<td>
			<?php
				$sql = "SELECT
						 timid AS codigo,
						 timdescricao AS descricao
						FROM
						 proinfantil.tipomodalidade 
						 WHERE timstatus = 'A' and modid = 2
						ORDER BY
						 2;";
				
				$db->monta_combo('timid', $sql, 'S', '-- Selecione --', '', '', '', '', 'S', 'timid', '', $timid);
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
		<?=campo_data2('pvedatainicial', 'S', 'S', '', 'S', '', '', $pvedatainicial, '', '', 'pvedatainicial'); ?>
		&nbsp;
		à
		&nbsp;
		<?=campo_data2('pvedatafinal', 'S', 'S', '', 'S', '', '', $pvedatafinal, '', '', 'pvedatafinal'); ?>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita" width="45%">Valor:</td>
	<td>
		<? 
		if($pvevalor) $pvevalor = number_format($pvevalor,2,",",".");  
		echo  campo_texto('pvevalor', 'S', 'S', '', 17, 15, '###.###.###,##', '', 'right', '', 0, '', '', $pvevalor); ?>
	</td>
	</tr>
	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<input type="button" class="botao" name="btcancela" value="Cancelar" onclick="javascript: window.location.href='proinfantil.php?modulo=sistema/tabelaapoio/periodoValorEtapa&acao=A'">
		</td>
	</tr>
</table>
</form>
<?php
	$sql = "SELECT
				'<img border=0 style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"alterarPeriodo('||o.pveid||')\" /> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=sistema/tabelaapoio/periodoValorEtapa&acao=A&pveid=' || o.pveid || '&requisicao=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as opcao, 
				t.timdescricao,
				ta.tatdsc as tipo,
                tr.tirdescricao,
				to_char(o.pvedatainicial, 'DD/MM/YYYY') as dtini,
				to_char(o.pvedatafinal, 'DD/MM/YYYY') as dtfim,		
				o.pvevalor as valor
			FROM 
				proinfantil.periodovaloretapa o
				inner join proinfantil.tipomodalidade t on t.timid = o.timid
                inner join proinfantil.tiporede tr on tr.tirid = o.tirid
                inner join proinfantil.tipoatendimentoturma ta on ta.tatid = o.tatid
			WHERE
				o.pvestatus = 'A'
                and ta.tatstatus = 'A'
		  	ORDER BY 
				 2";
	
	$cabecalho = array( "Opções","Etapa / Modalidade", "Tipo Atendimento", "Tipo Rede", "Data Início","Data Fim","Valor");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
<script type="text/javascript">
function validaForm(){

	var erro = false;
	var nome = '';
	
	$('.obrigatorio, .CampoEstilo').each(function(){
		if( $(this).val() == '' ){
			nome = $(this).attr('name');
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

function alterarPeriodo(pveid){
	window.location.href = 'proinfantil.php?modulo=sistema/tabelaapoio/periodoValorEtapa&acao=A&pveid='+pveid;
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>