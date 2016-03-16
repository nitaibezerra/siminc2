<?php
$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];
$pdoid = trim($_REQUEST['pdoid']);

if($_REQUEST['carregarData']){
	header('content-type: text/html; charset=ISO-8859-1');	
	echo getDataFinalConvenio($_POST['dias'], $_POST['datainicio']);			
	exit;
}

function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0];
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	echo $dataFormatada;
}

if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = "delete from obras2.desbloqueioobra where desid = {$_REQUEST['desid']}";
	$db->executar($sql);
	$db->commit();
	
	$pdoid = $_REQUEST['pdoid'];
	$db->sucesso('principal/dadosDesbloqueio', '&obrid='.$_REQUEST['obrid'].'&pdoid='.$pdoid);
	die;
}

if( $_POST['requisicao'] == 'salvar' ){
	
	$pdoid = $_REQUEST['pdoid'];
	$destipodesbloqueio = "'".$_POST['destipodesbloqueio']."'";
	$descomentario 		= $_POST['descomentario'] ? "'".$_POST['descomentario']."'" : 'null';
	$desdatainicio 		= $_POST['desdatainicio'] ? "'".formata_data_sql( $_POST['desdatainicio'] )."'" : 'null';
	$desdias 			= $_POST['desdias'] ? "'".$_POST['desdias']."'" : 'null';
	$destermino 		= $_POST['destermino'] ? "'".formata_data_sql( $_POST['destermino'] )."'" : 'null';
	
	$sql = "INSERT INTO obras2.desbloqueioobra(pdoid, destipodesbloqueio, descomentario, desdatainicio, desdias, destermino, usucpf) 
			VALUES ($pdoid, $destipodesbloqueio, $descomentario, $desdatainicio, $desdias, $destermino, '{$_SESSION['usucpf']}')";
	$db->executar($sql);
	$db->commit();
    enviarEmailAnaliseDesbloqueio($pdoid);
	$db->sucesso('principal/dadosDesbloqueio', '&obrid='.$_REQUEST['obrid'].'&pdoid='.$pdoid);
	die;
}

$urlP = "/obras2/obras2.php?modulo=principal/dadosDesbloqueio&acao=A&obrid=".$_REQUEST['obrid']."&pdoid=".$_REQUEST['pdoid'];
print carregaAbaDesbloqueio($urlP, $_REQUEST['obrid'], $_REQUEST['pdoid']);

monta_titulo( "Desbloqueio", "" );

if ( $empid ){
	$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
	$empreendimento->montaCabecalho();
}

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR);

$habilita = 'N';
if(!possui_perfil(PFLCOD_CALL_CENTER) && possui_perfil( $pflcods )){
	$habilita = 'S';
}

?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
	$(document).ready(function(){
		$('#tr_obras1').css('display', 'none');
		
		if( $('#habilita').val() == 'N' ){ 
			$('input, select, textarea').attr('disabled', true);
			$('.img_add_foto').hide();
		}
		
		if( $('[name="desdias"]').val() != '' ) calcularData( $('[name="desdias"]').val() );
	});
	
	function salvarDesbloqueio(){
		var erro = 1;
		$('[name=destipodesbloqueio]').each(function(){
			if($(this).attr('checked') == true){
				erro = '0';
			}
		});
		
		if( erro == 1 ){
			alert('O campo "Tipo Desbloqueio" é obrigatório!');
			return false;
		}
		
		if( $('[name="descomentario"]').val() == '' ){
			alert('O campo "Comentário" é obrigatório!');
			$('[name="descomentario"]').focus();
			return false;
		}
		
		if ( $('[name=destipodesbloqueio]:checked').val() == 'D' ){
		
			if( $('#desdatainicio').val() == '' ){
				alert('O campo "Data de Início" é obrigatório!');
				$('#desdatainicio').focus();
				return false;
			}
			
			if( $('[name="desdias"]').val() == '' ){
				alert('O campo "Dias" é obrigatório!');
				$('[name="desdias"]').focus();
				return false;
			}
		}
		
		$('#requisicao').val('salvar');
 		$('[name=formulario]').submit();
	}
	
	function calcularData(dias){
		if( Number(dias) ){
			$.ajax({
					type: "POST",
					url: 'obras2.php?modulo=principal/dadosDesbloqueio&acao=A',
					data: { carregarData: "true", dias: dias, datainicio: $('[name="desdatainicio"]').val()},
					async: false,
					success: function(retornoajax) {
						
						$('[name="destermino"]').val(retornoajax);
  				}
  			});
		} else {
			alert('Formado do dia invalido');
			$('[name="desdias"]').val('');
			return false;
		}
	}
	
	function excluirPedido( desid ){
		window.location.href = window.location+'&requisicao=excluir&desid='+desid;
	}
</script>
<body>
<form id="formulario" name="formulario" method=post enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="pdoid" id="pdoid" value="<?=$pdoid; ?>" />
<input type="hidden" name="desid" id="desid" value="<?=$desid; ?>" />
<input type="hidden" name="habilita" id="habilita" value="<?=$habilita; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo Desbloqueio:</td>
		<td>
			<input type="radio" name="destipodesbloqueio" id="destipodesbloqueio_d" value="D"> Deferido
			<input type="radio" name="destipodesbloqueio" id="destipodesbloqueio_i" value="I"> Indeferido
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Comentário:</td>
		<td><?=campo_textarea('descomentario', 'S', 'S', 'Comentário', 98, 5, 4000, '', '', '', '', 'Comentário');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Data de Início:</td>
		<td><?
		$desdatainicio = formata_data(date('Y-m-d'));
		echo campo_data2('desdatainicio', 'S', 'S', '', 'N', '', '', $desdatainicio, '', '', 'desdatainicio');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Dias:</td>
		<td><?
			$desdias = $desdias ? $desdias : 180;
			echo campo_texto('desdias', 'S', 'S', 'Dias', 10, 15, '[#]', '','right','','','id="desdias"', '', $desdias, 'calcularData(this.value)');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Data de Término:</td>
		<td><?=campo_data2('destermino', 'N', 'N', '', 'N', '', '', $destermino, '', '', 'destermino');?></td>
	</tr>
	<tr bgcolor="#DEDEDE">
		<td colspan="2" align="center">
			<?php 
			if ( $_SESSION['usucpf'] == '' ):
			?>
			<input name="btn_salvar" value="Salvar Desbloqueio" type="button" onclick="salvarDesbloqueio();">
			<?php
			endif;
			?>		
		</td>
	</tr>
</table>
</form>
<?

if( $habilita == 'S' ){
	$imagem = "'<img src=\"../imagens/excluir.gif\" onclick=\"excluirPedido('||d.desid||')\" style=\"border:0; cursor:pointer;\" title=\"Excluir Desbloqueio\">'";
} else {
	$imagem = "'<img src=\"../imagens/excluir_01.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir Desbloqueio\">'";
}

$sql = "SELECT 
		    $imagem as acoes,
		  	case when d.destipodesbloqueio = 'D' then 'Deferido ' else 'Indeferido' end as tipos,
		  	d.descomentario,
		  	to_char(d.desdatainicio, 'DD/MM/YYYY') as inicio,
		  	d.desdias,
		  	to_char(d.destermino, 'DD/MM/YYYY') as termino,
		  	us.usunome
		FROM 
			obras2.desbloqueioobra d
		  	inner join seguranca.usuario us on us.usucpf = d.usucpf
		WHERE
			d.pdoid = {$_REQUEST['pdoid']}";

$cabecalho = array('Ações', 'Tipo Desbloqueio', 'Comentário', 'Data Início', 'Dias', 'Data Término', 'Usuário');
$arrHeighTds = array('05%', '10%', '50%', '10%', '02%', '10%', '20%');
$db->monta_lista($sql, $cabecalho, 1000000, 5, 'N', 'center', '', '', $arrHeighTds);
?>
</body>
</html>