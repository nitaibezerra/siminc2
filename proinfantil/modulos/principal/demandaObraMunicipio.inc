<?php
include APPRAIZ ."includes/workflow.php";

if( empty($_SESSION['proinfantil']['popup']) ){
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br/>';
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}


if( $_SESSION['proinfantil']['obrid'] ){
	$obrid = $_SESSION['proinfantil']['obrid'];
} else {
	$obrid = $_REQUEST['obrid'];
}

$pinid = $_SESSION['proinfantil']['pinid'];

if ( !$obrid ){
	echo '<script>
			alert("A sessão foi perdida!");
			window.location.href = "proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A";
		  </script>';
	die;
}

include_once "_funcoes_proinfancia.php";

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

$perfis = pegaPerfil($_SESSION["usucpf"]);

montaAbasProinfantil( $abacod_tela, $url, '', $quest );

cabecalhoProInfantil($obrid);
$habilitado = verificaLiberacao();
?>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle;border:0px}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
$(function() {
	 $('[name=btn_limpar]').click(function() {
	 	$("[name^='qtde_']").val("");
	 });
	 $('[name=btn_sair]').click(function() {
	 	var url = "proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A";
		window.location.href = url;
	 });
	  $('[name=btn_salvar]').click(function() {
	 	var total = $('[name=qtde_total_geral]').val();
	 	if(total.search(".") >= 0){
			var arrTotal = total.split(".");
			var newTotal = arrTotal.join("")*1;
		}else{
			var newTotal = total*1;
		}
	    
		var max = <?php echo recuperaMaxAlunosPorObra($obrid) ?>;
	 	if(newTotal){
	 		if(newTotal <= max){

	 			var campo33 = $("[name$='3][3]']").val() ? $("[name$='3][3]']").val() : 0;
	 			if(campo33){
		 			campo33 = campo33.split("."); 
		 			campo33 = campo33.join("")*1;
	 			}
	 			var campo13 = $("[name$='1][3]']").val() ? $("[name$='1][3]']").val() : 0;
	 			if(campo13){
		 			campo13 = campo13.split("."); 
		 			campo13 = campo13.join("")*1;
	 			}
	 			var campo43 = $("[name$='4][3]']").val() ? $("[name$='4][3]']").val() : 0;
	 			if(campo43){
		 			campo43 = campo43.split("."); 
		 			campo43 = campo43.join("")*1;
	 			}
	 			var campo23 = $("[name$='2][3]']").val() ? $("[name$='2][3]']").val() : 0;
	 			if(campo23){
		 			campo23 = campo23.split("."); 
		 			campo23 = campo23.join("")*1;
	 			}
	 			

	 			var campo31 = $("[name$='3][1]']").val() ? $("[name$='3][1]']").val() : 0;
	 			if(campo31){
		 			campo31 = campo31.split("."); 
		 			campo31 = campo31.join("")*1;
	 			}
	 			var campo11 = $("[name$='1][1]']").val() ? $("[name$='1][1]']").val() : 0;
	 			if(campo11){
		 			campo11 = campo11.split("."); 
		 			campo11 = campo11.join("")*1;
	 			}
	 			var campo41 = $("[name$='4][1]']").val() ? $("[name$='4][1]']").val() : 0;
	 			if(campo41){
		 			campo41 = campo41.split("."); 
		 			campo41 = campo41.join("")*1;
	 			}
	 			var campo21 = $("[name$='2][1]']").val() ? $("[name$='2][1]']").val() : 0;
	 			if(campo21){
		 			campo21 = campo21.split("."); 
		 			campo21 = campo21.join("")*1;
	 			}
	 			

	 		 	if( parseFloat(campo33) < parseFloat(campo13) ){
	 		 		alert('A Quantidade da coluna "Total de Crianças Atendidas na Escola - Tempo Integral" deve ser igual ou maior que a quantidade da coluna "Crianças não Computadas para Recebimento do Fundeb - Tempo Integral" da Etapa "1 - Creche".');
	 		 		$("[name$='3][3]']").focus();
	 		 		return false;
	 		 	}
	 		 	if( parseFloat(campo43) < parseFloat(campo23) ){
	 		 		alert('A Quantidade da coluna "Total de Crianças Atendidas na Escola - Tempo Parcial" deve ser igual ou maior que a quantidade da coluna "Crianças não Computadas para Recebimento do Fundeb - Tempo Parcial" da Etapa "1 - Creche".');
	 		 		$("[name$='4][3]']").focus();
	 		 		return false;
	 		 	}

	 		 	if( parseFloat(campo31) < parseFloat(campo11) ){
	 		 		alert('A Quantidade da coluna "Total de Crianças Atendidas na Escola - Tempo Integral" deve ser igual ou maior que a quantidade da coluna "Crianças não Computadas para Recebimento do Fundeb - Tempo Integral" da Etapa "2 - Pré-Escola".');
	 		 		$("[name$='3][1]']").focus();
	 		 		return false;
	 		 	}
	 		 	if( parseFloat(campo41) < parseFloat(campo21) ){
	 		 		alert('A Quantidade da coluna "Total de Crianças Atendidas na Escola - Tempo Parcial" deve ser igual ou maior que a quantidade da coluna "Crianças não Computadas para Recebimento do Fundeb - Tempo Parcial" da Etapa "2 - Pré-Escola".');
	 		 		$("[name$='4][1]']").focus();
	 		 		return false;
	 		 	}
		 		
	 			$('[name=formulario_demanda]').submit();
	 			
		 	}else{
		 		alert('O número de alunos deve ser menor ou igual a ' + max + '!');
		 	}
	 	}else{
	 		alert('Informe o número de alunos!');
	 	}
	 });
	 
	 <?php if($_SESSION['proinfantil']['mgs']): ?>
	 	alert('<?php echo $_SESSION['proinfantil']['mgs'] ?>');
	 	<?php unset($_SESSION['proinfantil']['mgs']) ?>
	 <?php endif; ?>
});

function editarProInfantil(obrid){
	var url = "proinfantil.php?modulo=principal/proInfantilObra=A&obrid=" + obrid;
	window.location.href = url;
}

function formataTotal( total ){

	total = total+'';
	if(total.length > 3)
	{
		 var tmp = total+'';
	     tmp = tmp.replace(/([0-9]{3})$/g, ".$1");
	     return tmp;
	}
	else
	{
		return total;
	}
}

function calculaTotal(tur,mod){
	var total = 0;
	$("[name$='1][" + mod + "]']").each(function() {
		if($(this).attr("readonly") == false){
			if(this.value.search(".") >= 0){
				var arrvalor = this.value.split(".");
				var newTotal = arrvalor.join("")*1;
			}else{
				var newTotal = this.value*1;
			}
			total+=newTotal;
		}
	});
        
	$("[name='qtde_mod_total[" + mod + "]']").val(formataTotal( total ));

	$("[name$='2][" + mod + "]']").each(function() {
		if($(this).attr("readonly") == false){
			if(this.value.search(".") >= 0){
				var arrvalor = this.value.split(".");
				var newTotal = arrvalor.join("")*1;
			}else{
				var newTotal = this.value*1;
			}
			total+=newTotal;
		}
	});
	$("[name='qtde_mod_total[" + mod + "]']").val(formataTotal( total ));


	var total = 0;
	$("[name^='qtde_mod_turno[" + tur + "]']").each(function() { 
		if($(this).attr("readonly") == false){
			if(this.value.search(".") >= 0){
				var arrvalor = this.value.split(".");
				var newTotal = arrvalor.join("")*1;
			}else{
				var newTotal = this.value*1;
			}
			total+=newTotal;
		}
	});
	
	$("[name='qtde_turn_total[" + tur + "]']").val(formataTotal( total ));
	
	calculaTotalGeral();

}

function calculaTotalGeral(){
	var totalPar = 0;
	$("[name^='qtde_turn_total[1']").each(function() { 
		if(this.value.search(".") >= 0){
			var arrvalor = this.value.split(".");
			var newPar = arrvalor.join("")*1;
		}else{
			var newPar = this.value*1;
		}
		totalPar+=newPar;
	});
	$("[name='qtde_total_geral']").val(formataTotal(totalPar));
	
	$("[name^='qtde_turn_total[2']").each(function() { 
		if(this.value.search(".") >= 0){
			var arrvalor = this.value.split(".");
			var newPar = arrvalor.join("")*1;
		}else{
			var newPar = this.value*1;
		}
		totalPar+=newPar;
	});
	$("[name='qtde_total_geral']").val(formataTotal(totalPar));
}

$( document ).ready(function() {
	$("[name='qtde_total_geral']").attr("disabled", "disabled");
	$("[name='qtde_mod_total[1]']").attr("disabled", "disabled");
	$("[name='qtde_mod_total[3]']").attr("disabled", "disabled");
});

</script>

<form id="formulario_demanda" name="formulario_demanda" method="post" action="">
	<input type="hidden" value="salvarDemandaProInfantil" name="requisicao" />
	<input type="hidden" value="<?php echo $pinid; ?>" name="pinid" />
	<?php $arrTurnos = recuperaTipoTurno(); ?>
	<?php $arrAlunos = $pinid ? recuperaAlunoAtendido($pinid) : array(); ?>
	<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="92%">
			<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			    <tr>
			    	<td align="left" style="font-size: 12px">
			    	<p align="justify">Nas duas primeiras colunas, deve-se informar as novas matrículas não computadas para recebimento do Fundeb. 
					Nas duas seguintes, o total de crianças matriculadas no estabelecimento – esse número pode ser maior ou igual 
					ao das duas colunas anteriores, mas não pode ser menor. O preenchimento da última coluna é feito automaticamente, 
					com base nas informações declaradas nas duas primeiras colunas. Devem ser declaradas todas as matrículas do 
					estabelecimento informando em separado quantas matrículas em jornada de tempo integral e quantas de tempo parcial, 
					distinguindo as de creche e as de pré-escola. O “total geral” de cada coluna será automaticamente somado, uma vez 
					preenchidos os campos.</p>
			    	</td>
			    </tr>
			  </table>						
			</td>
			<?
			if( empty($_SESSION['proinfantil']['popup']) ){?>
				<td align="center" rowspan="2"><?php wf_desenhaBarraNavegacao( recuperaDocumentoProInfantil($pinid) , array( 'pinid' => $pinid ) ); ?></td>
			<?} ?>
		</tr>
		<tr>
			<td>
				<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				    <tr class="SubTitulotabela">
				        <td class="center bold">Etapa</td>
				        <?php if($arrTurnos): ?>
				        	<?php foreach($arrTurnos as $turno): ?>
				        		<td class="direita bold"><?php echo $turno['titdescricao'] ?></td>
				        	<?php endforeach; ?>
				        <?php endif; ?>
				        <td class="direita bold">Total de Matrículas não Computadas para Recebimento do Fundeb</td>
				        <?php $arrModalidades = recuperaTipoModalidade() ?>
				    </tr>
				    <?php if($arrModalidades): ?>
				    	<?php $i=0 ?>
				    	<?php foreach($arrModalidades as $modalidade): ?>
						    <?php $cor = $i%2 == 0 ? "#FFFFFF" : "#f7f7f7" ?>
						    <tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
						        <td><?php echo $modalidade['timdescricao'] ?></td>
						        <?php if($arrTurnos): ?>
						        	<?php foreach($arrTurnos as $turno): ?>
						        		<td class="direita bold">
						        			<?php echo campo_texto("qtde_mod_turno[{$turno['titid']}][{$modalidade['timid']}]","N",$habilitado,"",20,6,"[.###]","","right","","","","calculaTotal('{$turno['titid']}','{$modalidade['timid']}')",$arrAlunos[$turno['titid']][$modalidade['timid']]) ?>
						        			<?php $arrJS[] = array("titid" => $turno['titid'] , "timid" => $modalidade['timid']) ?>
						        		</td>
						        	<?php endforeach; ?>
						        <?php endif; ?>
						        <td class="direita bold"><?php echo campo_texto("qtde_mod_total[{$modalidade['timid']}]","N",$habilitado,"",20,20,"[.###]","","right","","","") ?></td>
						    </tr>
						    <?php $i++ ?>
					    <?php endforeach; ?>
					    <tr class="SubTitulotabela" >
					        <td class="bold" >Total Geral</td>
					        <?php if($arrTurnos): ?>
					        	<?php foreach($arrTurnos as $turno): ?>
					        		<td class="direita bold" ><?php echo campo_texto("qtde_turn_total[{$turno['titid']}]","N",$habilitado,"",20,20,"[.###]","","right","","","") ?></td>
					        	<?php endforeach; ?>
					        <?php endif; ?>
					        <td class="direita bold"><?php echo campo_texto("qtde_total_geral","N",$habilitado,"",20,20,"[.###]","","right","","","") ?></td>
					    </tr>
					    <tr>
				        	<td colspan="<?php echo is_array($arrTurnos) ? count($arrTurnos) + 2 : 2 ?>" class="SubTituloDireita center bold">
				        	<?if( empty($_SESSION['proinfantil']['popup']) ){ ?>
				        		<?php if( $habilitado == 'S' ){ ?>
					        		<input type="button" name="btn_salvar" value="Salvar" /> 
					        		<input type="button" name="btn_limpar" value="Limpar" /> 
				        		<?php } ?>
				        		<input type="button" name="btn_sair" value="Sair" />
				        	<?} else { ?>
				        		<input type="button" name="close" onclick="window.close();" value="Fechar" />
				        	<?} ?>
				        	</td>
				    	</tr>
				    <?php endif; ?>
				</table>			
			</td>	
		</tr>
	</table>		
</form>
<?php if($arrJS): ?>
	<script>
		<?php foreach($arrJS as $js): ?>
			calculaTotal('<?php echo $js['titid'] ?>','<?php echo $js['timid'] ?>')
		<?php endforeach; ?>
	</script>
<?php endif; ?>