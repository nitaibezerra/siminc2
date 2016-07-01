<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Luiz Fernando Mendonça
 * Programador: Gustavo Fernandes da Guarda
 */

include_once APPRAIZ . "includes/workflow.php";

if($_POST['acao'] && $_POST['aMes'][0]){
	extract($_POST);
	
	# Monitoramento
	$oMonitoramento = new Monitoramento();
	$oMonitoramento->monid = null;
	$oMonitoramento->montipo = $_SESSION['escolaativa']['montipo'];
	if($tecid) $oMonitoramento->tecid = $tecid;
	if($espid) $oMonitoramento->espid = $espid;
	$oMonitoramento->monreferencia = $aMes[0];
	$monid = $oMonitoramento->salvar();
	if($monid){
		$docid = eaCriarDocumento( $monid );		
	}
	
	if($oMonitoramento->commit()){
		echo "<script type=\"text/javascript\">alert('Operação Realizada com Sucesso!'); ";
		echo "window.location.href = '{$_SERVER['HTTP_REFERER']}' </script>";
		die;
	}
	unset($oMonitoramento);
}

?>
<html>   
<body>
	<div class="fechar">
		<span style="float:right; font-weight: bold">Fechar<a href="#" class="jqmClose" style="vertical-align:middle;"></a></span>
	</div>
	<br />
<?php 
if($_GET['monperiodicidade'] == 'M'){
	$titulo_modulo = "Relatório Mensal";
} else {
	$titulo_modulo = "Relatório Bimestral";	
}
monta_titulo( $titulo_modulo, '' ); 

?>
<form name="formulario" id="formulario" method="post" action="escolaativa.php?modulo=principal/popupMensalBimestral&acao=A">
<!--<input type="hidden" name="monid" id="monid" value="<?php echo $_GET['monid']; ?>">-->
<input type="hidden" name="espid" id="espid" value="<?php echo $_GET['espid']; ?>">
<input type="hidden" name="tecid" id="tecid" value="<?php echo $_GET['tecid']; ?>">
<input type="hidden" name="acao" id="acao" value="1">
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<th colspan="2">Meses</th>
	</tr>
	<?php 
	$aMesChecado = array();
	if($tecid = $_GET['tecid']){
		//$sql = "select monmensal as mes1, monbimensal as mes2 from escolaativa.monitoramento where tecid = $tecid and monperiodicidade = '{$_GET['monperiodicidade']}' ";
		$sql = "select monreferencia as mes1 from escolaativa.monitoramento where tecid = $tecid and espid = {$_GET['espid']} ";
		//ver($sql);
		$aPeriodicidade = $db->carregar($sql);
		$aPeriodicidade = ($aPeriodicidade) ? $aPeriodicidade : array();
		
		if(count($aPeriodicidade) && $aPeriodicidade[0]){
			foreach($aPeriodicidade as $periodicidade){
				if($periodicidade['mes1']){
					array_push($aMesChecado, $periodicidade['mes1']);
				}
				/*if($periodicidade['mes2']){
					array_push($aMesChecado, $periodicidade['mes2']);					
				}*/				
			}
		}
	}
	
	$aMeses = array(1=>'Janeiro',
					2=>'Fevereiro',
					3=>'Março',
					4=>'Abril',
					5=>'Maio',
					6=>'Junho',
					7=>'Julho',
					8=>'Agosto',
					9=>'Setembro',
					10=>'Outubro',
					11=>'Novembro',
					12=>'Dezembro');
					
	foreach($aMeses as $chave => $mes){
		$ckecked = (in_array($chave, $aMesChecado)) ? "checked=\"checked\"" : "";		
		$disabled = (in_array($chave, $aMesChecado)) ? "disabled=\"disabled\"" : "";		
		echo "
		<tr>
			<td width=\"9px\">
				<input name=\"aMes[]\" id=\"mes_$chave\" value=\"$chave\" type=\"checkbox\" class=\"verificaCheck\" $ckecked $disabled  />
			</td>
			<td>
				$mes
			</td>
		</tr>
		";
	}
	?>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
		<td width="9px">
			<!--<input type="checkbox" id="checktodos"> Marcar todos </div>-->
		</td>
		<td><input type="submit" class="botao" id="botaoOk" name="bta" value="OK"></td>
	</tr>
</table>

<script type="text/javascript">
<!--

//$('#monperiodicidade').val('M')

//if($('#monperiodicidade').val() == 'M'){
	var limite = $('input[type=checkbox]:checked').length + 1;
	// limite para habilitar novamente
	var limiteD = limite - 1;
/*} else {
	var limite = $('input[type=checkbox]:checked').length + 2;
	// limite para habilitar novamente
	var limiteD = limite - 2;
}
*/

$(function() { 
	$('.verificaCheck').click(function() {
		// Verifica se é do tipo Bimestral, se for marcar o proximo ckeck(mes) para formar o bimestre
		/*if($('#monperiodicidade').val() == 'B' && $(this).val() != 12){
			if($(this).attr("checked") == true){
				$('#mes_'+(parseInt($(this).val()) + 1)).attr("checked", true);
			} else {
				$('#mes_'+(parseInt($(this).val()) + 1)).attr("checked", false);
			}
		}*/
    	
    	// verifica quantos check estão marcados
    	if($('input[type=checkbox]:checked').length == limite){
			$('input[type=checkbox]').each(function() {
				// Verifica todos que não estão marcados e desabilita
				if($(this).attr("checked") == false){
					$(this).attr('disabled', true);
				}
		    });
		} else if($('input[type=checkbox]:checked').length == limiteD){ // Senão habilita tudo novamente
			$('input[type=checkbox]').attr('disabled', false);
		}
    })
});

//-->
</script>
</body>
</html>