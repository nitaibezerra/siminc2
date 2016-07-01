<?php
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "academico/classes/Liberacao.class.inc";
require_once APPRAIZ . "academico/classes/NivelCurso.class.inc";
require_once APPRAIZ . "academico/classes/LimiteGeral.class.inc";

$oNivelCurso  = new NivelCurso();
$oLimiteGeral = new LimiteGeral();

if($_POST['btnSalvar'])
{
	$vPeriodo 		= $_REQUEST['periodo'];
	$vLimiteGeral 	= $_REQUEST['ltg'];
	
	// Atualização de limite de Cotas
	if( is_array( $vLimiteGeral ) ){

		foreach($vLimiteGeral as $limitenvcid => $limiteltg){

			if( !$oLimiteGeral->existeNivelSemEntidade($limitenvcid) ){

				$ltgid = $oLimiteGeral->gravarLimiteCotas($limiteltg, $limitenvcid);
	
			}else{

				$ltgid = $oLimiteGeral->editarLimiteCotas($limiteltg, $limitenvcid);
	
			}
		}
/* 
		if( !empty($ltgid) ){
		
			alert('Salvo com Sucesso.');
		
		}else{
		
			alert('Não foi possível salvar os Limites Gerais, verifique se o valor de limite não é superior aos valores já lançados.');
		
		} */
	}
	
	if( is_array( $vPeriodo ) )
	{
		// Atualização de período
		foreach ( $vPeriodo as $chave ){
			
			$dtInicial  = "dtini_".$vPeriodo[$chave];
			$dtFinal	= "dtfim_".$vPeriodo[$chave];
			
			if( !empty( $_REQUEST[$dtInicial] ) && !empty( $_REQUEST[$dtFinal] )){
				//Atualiza novos
				$sqlPeriodo = "UPDATE academico.liberacao 
								SET libdtinicio='".formata_data_sql( $_REQUEST[$dtInicial] )."'
								, libdtfinal='".formata_data_sql( $_REQUEST[$dtFinal] )."' 
								WHERE mescod='" .$chave. "'";		
			}else{
				//Atualiza restante como NULL
				$sqlPeriodo = "UPDATE academico.liberacao SET libdtinicio=null, libdtfinal=null WHERE mescod='" .$chave. "'";
			}
			$retorno = $db->executar( $sqlPeriodo );
		}
		
		echo '<script> alert("Período cadastrado com sucesso."); </script>';
	}
}

// Recupera os niveis de curso cadastrados
$niveisCurso = $oNivelCurso->lista();
$ltg 	= $oLimiteGeral->listaLimiteDeCotasGeral();

?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/> 
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
<!--
function imagemObrigatorio( id, valor ){
	$( "#obrini_"+id ).css('display', valor );
	$( "#obrfim_"+id ).css('display', valor );
}

var opcoesCalendario = {
		dateFormat: 'dd/mm/yy',
		dayNames: [
		'Domingo','Segunda','Terça','Quarta','Quinta','Sexta','Sábado','Domingo'
		],
		dayNamesMin: [
		'D','S','T','Q','Q','S','S','D'
		],
		dayNamesShort: [
		'Dom','Seg','Ter','Qua','Qui','Sex','Sáb','Dom'
		],
		monthNames: [
		'Janeiro','Fevereiro','Março','Abril','Maio','Junho','Julho','Agosto','Setembro',
		'Outubro','Novembro','Dezembro'
		],
		monthNamesShort: [
		'Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set',
		'Out','Nov','Dez'
		],
		nextText: 'Próximo',
		prevText: 'Anterior'
	};

	function aplicaEventoCalendario( contador ){

		// Adiciona componente de calendário aos campos de TEXT abaixo
		$( "#dtini_"+contador+", #dtfim_"+contador ).datepicker( opcoesCalendario ).change(function(){
			if( !validaData( this ) && $(this).val() != "" ){
				alert( $(this).val() + ' não é uma data válida.' );
				$(this).val('');
			}
		});
		
		// Aplica o mesmo evento de click do INPUT à IMAGEM 
		$("#imgini_"+contador).click(function() {
			if( !$("#dtini_"+contador).attr('disabled') ){ 
			   $("#dtini_"+contador).datepicker( "show" );
			}
		});

		// Aplica o mesmo evento de click do INPUT à IMAGEM
		$("#imgfim_"+contador).click(function() {
			if( !$("#dtfim_"+contador).attr('disabled') ){
			   $("#dtfim_"+contador).datepicker( "show" );
			}
		});

		if( $("#acao_"+contador).attr('checked') ){
			imagemObrigatorio( contador, '');
		}
		
		// Habilita/Desabilita campo TEXT das datas, limpa o campo de texto quando desabilitado e altera cor de fundo do input
		$("#acao_"+contador).click(function() {
			if( $( "#acao_"+contador).attr('checked') ){
				$( "#dtini_"+contador+", #dtfim_"+contador ).attr('disabled','');
				$( "#dtini_"+contador+", #dtfim_"+contador ).css("background-color", "#ffffff");
				imagemObrigatorio( contador, '');
			}else{
				$( "#dtini_"+contador+", #dtfim_"+contador ).attr({disabled:'disabled', value:''});
				$( "#dtini_"+contador+", #dtfim_"+contador ).css("background-color", "#cccccc");
				imagemObrigatorio( contador, 'none');
			}
		});
	}

	$(document).ready(function(){

		$( '#tbPeriodoLiberacao tr:odd').css("background-color", "#f4f4f4");
		$( 'input:disabled').css("background-color", "#cccccc");
		$( '#tbPeriodoLiberacaoRodape').css('background-color' , '#CCCCCC').css('text-align', 'right');
		$( "img[id^='obrini']" ).css('display', 'none');
		$( "img[id^='obrfim']" ).css('display', 'none');

		$("#formulario").submit( function(){

			var retorno = true;
			
			$("input[id^='dtini']").each(function(index){

				var cpData    = $(this).attr('name').split('_');
				var cpDataIni = $( "#dtini_"+cpData[1] );
				var cpDataFim = $( "#dtfim_"+cpData[1] );

				if( cpDataIni.val() != "" && cpDataFim.val() != "" ){

				    var dataInicio = cpDataIni.val();
				    var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
					
				    var dataFim = cpDataFim.val();
				    var dataFimConvertida = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);
					
				    if (dataInicioConvertida > dataFimConvertida){
				        alert( 'Período "'+ dataInicio + ' a ' + dataFim + '" inválido' );
				        imagemObrigatorio( cpData[1], '');
				        retorno = false;
				    }

				}else if( (cpDataIni.val() != "" && cpDataFim.val() == "") || (cpDataIni.val() == "" && cpDataFim.val() != "") ){
					alert( "O(s) seguinte(s) campo(s) deve(m) ser preenchido(s):\nPeríodo de Liberação" );
					imagemObrigatorio( cpData[1], '');
					retorno = false;
					
				}else if( $("#acao_"+cpData[1]).attr('checked') ){
					if( cpDataIni.val() == "" || cpDataFim.val() == "" ){
						alert( "O(s) seguinte(s) campo(s) deve(m) ser preenchido(s):\nPeríodo de Liberação" );
						imagemObrigatorio( cpData[1], '');
						retorno = false;
					}
				}
			});

			return retorno;
		});	
	});
//-->
</script>



<br/>
<style type="text/css">
    
    .center {
        text-align: center;
    }
    
</style>
<form method="post" name="formulario" id="formulario">
	
	<?php monta_titulo("Limite de Cotas", ""); ?>
	
    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <th rowspan="2" style="width:300px;"> N&iacute;vel </th>
            <th colspan="5"> Ano </th>
        </tr>
        <tr>
            <th> 2008 </th>
            <th> 2009 </th>
            <th> 2010 </th>
            <th> 2011 </th>
            <th> 2012 </th>
        </tr>
        <?php foreach($niveisCurso as $nivel): ?>
        <tr>
            <td class="SubTituloDireita">
                <?php echo $nivel['nvcdsc']; ?>:
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2008]", 'N', 'S', '', 4, 4, '[####]', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2008']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2009]", 'N', 'S', '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2009']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2010]", 'N', 'S', '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2010']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2011]", 'N', 'S', '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2011']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2012]", 'N', 'S', '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2012']); ?>
            </td>
        </tr>
        <?php endforeach; ?>
    </table>
	<br/>	
    <?php monta_titulo("Liberação", ""); ?>
	<table align="center" border="0" cellpadding="3" cellspacing="1" style="width: 600px;">
		<thead>
			<tr>
				<th>Mês</th>
				<th>Período de Liberação</th>
				<th>Ação</th>
			</tr>
		</thead>
		<tbody id='tbPeriodoLiberacao'>
			<?php 
			$objLiberacao 	= new Liberacao();
			$listaPeriodos  = $objLiberacao->listarPeriodos();
			
			//Valida se existe registro
			if( $listaPeriodos ){
	
				foreach( $listaPeriodos as $periodo){
					
					$contador  	  	= $periodo['mescod'];
					$desabilitado 	= empty( $periodo['libdtinicio'] ) && empty($periodo['libdtfinal']) ? "disabled='disabled'" : "";
					$dataInicial	= !empty( $periodo['libdtinicio'] ) ? formata_data( $periodo['libdtinicio'] ) :  null;
					$dataFinal		= !empty( $periodo['libdtfinal']  ) ? formata_data( $periodo['libdtfinal'] )  :  null;
					$ligaCheckbox	= !empty( $periodo['libdtinicio'] ) && !empty( $periodo['libdtfinal'] ) ? "checked=checked"  :  null;
					
					echo "<tr>";
					echo "	<td>";
					echo 		$periodo['mesdsc'];
					echo "	</td>";
					echo "	<td align='center'>De ";
					echo " 		<input type='text' id='dtini_".$contador."' name='dtini_".$contador."' title='Data Inicial' class='normal' ".$desabilitado. " value='".$dataInicial."' maxlength='10' onKeyUp=\"this.value=mascaraglobal('##/##/####',this.value);\" />";
					echo "		<img src='../includes/JsLibrary/date/displaycalendar/images/calendario.gif' align='absmiddle' border='0' style='cursor:pointer' title='Escolha uma Data' id='imgini_".$contador."' ".$desabilitado." />";
					echo "		<img border=\"0\" src=\"../imagens/obrig.gif\" id='obrini_".$contador."' name='obrini_".$contador."' >";
					echo " 		a <input type='text' id='dtfim_".$contador."' name='dtfim_".$contador."' title='Data Final' class='normal' ".$desabilitado. " value='".$dataFinal."' maxlength='10' onKeyUp=\"this.value=mascaraglobal('##/##/####',this.value);\" />";
					echo "		<img src='../includes/JsLibrary/date/displaycalendar/images/calendario.gif' align='absmiddle' border='0' style='cursor:pointer' title='Escolha uma Data' id='imgfim_".$contador."' ".$desabilitado." />";
					echo "		<img border=\"0\" src=\"../imagens/obrig.gif\" id='obrfim_".$contador."' name='obrfim_".$contador."' >";
					echo "	</td>";
					echo "	<td align='center'>";
					echo "		<input type='hidden' id='periodo[".$contador."]' name='periodo[".$contador."]' value='".$contador."' />";
					echo " 		<input type='checkbox' id='acao_".$contador."' name='acao_".$contador."' ".$ligaCheckbox." />";
								?>
								<script type="text/javascript">
									aplicaEventoCalendario('<?php echo($contador);?>');							
								</script>
								<?php
					echo "	</td>";
					echo "</tr>";
				}
			}
			?>
		</tbody>
		<tfoot id='tbPeriodoLiberacaoRodape'>
			<tr>
				<td colspan="3">
					<?php if(!in_array(570, pegaArrayPerfil($_SESSION['usucpf']))): ?>
						<input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar" />
						<input type="button" value="Voltar" onclick="javascript: history.back(-1);"  />
					<?php endif; ?>
				</td>
			</tr>
		</tfoot>
	</table>
</form>