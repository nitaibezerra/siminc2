<?php
/****************** INCLUDES *************************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');
include_once(APPRAIZ.'includes/classes/dateTime.inc');


/***************** DECLARAÇÃO DE VARIAVEIS & INSTANCIA OBJETOS ***************/
$prsid 				= $_REQUEST['prsid'];
$obDadosBancarios 	= new HistoricoMonitoramentoDadosBancarios();
$obdata				= new Data();
$ano 				= $_REQUEST['hmdano'];
$sqlData			= " SELECT 
							hmdano as maiorano, hmdmes as maiormes
						FROM cte.historicomonitoramentodadosbancarios
						WHERE prsid = '".$_REQUEST['prsid']."' 
						AND hmdmes in (
							SELECT 
								MAX(hmdmes) as maiormes
							FROM cte.historicomonitoramentodadosbancarios
							WHERE prsid = '".$_REQUEST['prsid']."' AND hmdano = (SELECT MAX(hmdano) as maiorano 
											FROM cte.historicomonitoramentodadosbancarios
											WHERE prsid = '".$_REQUEST['prsid']."')
						) 
						AND hmdano in (SELECT MAX(hmdano) as maiorano 
								FROM cte.historicomonitoramentodadosbancarios
								WHERE prsid = '".$_REQUEST['prsid']."')
					  ";
//dbg($sqlData);

$ultimaData			= $db->pegaLinha($sqlData);
if($ultimaData == false){
	$sql = "SELECT prsano FROM cte.projetosape WHERE prsid = '".$_REQUEST['prsid']."'";
	$anoConvenio = $db->pegaUm($sql);
}

if($ultimaData){	
	if($ultimaData['maiormes'] == '12'){
		$proxmes = "01";
		$proxano = $ultimaData['maiorano']+1;
	}else{
		$proxmes = $ultimaData['maiormes']+1;
		$proxano = $ultimaData['maiorano'];
	}
	
	if($proxano == NULL && $proxmes == "01" ){
		$proxano = $anoConvenio + 1;
	}else if($proxano == NULL){
		$proxano = $anoConvenio;
	}
} else {
	$sql = "select prsiniciovigencia from cte.projetosape where prsid = '".$_REQUEST['prsid']."'";
	$dataVigencia = $db->pegaUm($sql);
	$v = explode( '-', $dataVigencia );
	$proxano = $v[0];
	$proxmes = $v[1];
}

$sql = "SELECT * FROM cte.projetosape WHERE prsid = ".$prsid;
$dadosProjeto = $obDadosBancarios->pegaLinha($sql);
$dadosProjetoData = $obdata->formataData($dadosProjeto['prsiniciovigencia'], "DD de mesTextual de YYYY");

$sql = "SELECT * FROM cte.historicomonitoramentodadosbancarios where prsid = ".$prsid;
//dbg($dadosProjeto['prsiniciovigencia']);

if($_REQUEST['requisicao']  == 'deletarMes'){
	$sql = "select hmdid from cte.historicomonitoramentoconvenio where hmdid = ".$_REQUEST['id'];
	$existeMonitoramentoIniciado = $db->pegaUm($sql);
	if($existeMonitoramentoIniciado == false){
		$sql = "DELETE FROM cte.historicomonitoramentodadosbancarios
				WHERE hmdid = ".$_REQUEST['id'];
		if($db->executar($sql)){
			$db->commit();
			echo "Mês deletado com sucesso.";
		}else{
			$db->rollback();
			echo "Ocorreu um erro ao tentar deletar este Mês.";
		}
		die();
	}else{
		echo "O monitoramento deste mês já foi iniciado. \n Não e possivel excluir.";
		die();
	}
}



//if($_REQUEST['requisicao']  == 'atualizaListaMeses'){
//	$obDadosBancarios->listaDadosBancarios($_REQUEST['prsid']);
//	die();
//}

//if($_REQUEST['requisicao']  == 'carregames' ){
//	$combo = '';
//	$sql="SELECT hmdmes FROM cte.historicomonitoramentodadosbancarios WHERE prsid = ".$_REQUEST['prsid']." AND hmdano = ".$_REQUEST['anor']." ORDER BY hmdmes DESC";
//	$mesValida = $obDadosBancarios->carregar($sql);
//	$mesesTela = array('0'=>'00','1'=>'01','2'=>'02','3'=>'03','4'=>'04','5'=>'05','6'=>'06','7'=>'07','8'=>'08','9'=>'09','10'=>'10','11'=>'11','12'=>'12');
//	//dbg($_REQUEST['prsid']." - ".$_REQUEST['ano'],1);
//	if(is_array($mesValida)){
//		foreach($mesValida as $meses){
//			if($meses['hmdmes'] == $mesesTela[$meses['hmdmes']] ){
//				
//				unset($mesesTela[$meses['hmdmes']]);
//			}
//		}
//		
//	}
//	array_shift($mesesTela);
//	$combo = '<select name="hmdmes" id="hmdmes" title="mês">
//					<option value="">selecione</option>';
//	if(is_array($mesesTela)){ 
//		foreach($mesesTela as $meses){
//			$combo .= '<option  value="'.$meses.'">'.$meses.'</option>';		
//		}
//	}
//	$combo .=	'</select>';
//	echo $combo;
//	die();
//}



/***************** SALVA DADOS DO FORM ***************************************/
if( $_REQUEST['salvar'] ){
	// Regra de négocios - cálculos dados e retira vírgulas para salvar certo no BD //
	$hmdsaldo1diames 		= str_replace(".", "", $_REQUEST['hmdsaldo1diames']);
	$hmdsaldo1diames 		= str_replace(",", ".", $hmdsaldo1diames);
	$hmdsaldoultimodiames 	= str_replace(".", "", $_REQUEST['hmdsaldoultimodiames']);
	$hmdsaldoultimodiames 	= str_replace(",", ".", $hmdsaldoultimodiames);
	$hmdvalorcontrapartida 	= str_replace(".", "", $_REQUEST['hmdvalorcontrapartida']);
	$hmdvalorcontrapartida 	= str_replace(",", ".", $hmdvalorcontrapartida);
	$hmdrendimento 			= str_replace(".", "", $_REQUEST['hmdrendimento']);
	$hmdrendimento 			= str_replace(",", ".", $hmdrendimento);
	
	$arCampos = array("hmdmes", "prsid");
	
	$obDadosBancarios->popularObjeto( $arCampos );
	$obDadosBancarios->hmdano 					= $ano;
	$obDadosBancarios->hmdsaldo1diames 			= $hmdsaldo1diames;
	$obDadosBancarios->hmdsaldoultimodiames 	= $hmdsaldoultimodiames;
	$obDadosBancarios->hmdvalorcontrapartida 	= $hmdvalorcontrapartida;
	$obDadosBancarios->hmdrendimento 			= $hmdrendimento;
	
	if($obDadosBancarios->salvar()){//Sava dados
		$obDadosBancarios->commit();
		 echo '<script> 
					alert( "Operação realizada com sucesso. " );
					parent.closeLyteboxBancarios();
			  </script>';
	}else{
		$obDadosBancarios->rollback();
		 echo '<script> 
					alert( "Ocorreu um erro ao tentar salvar os dados. " );
					parent.closeLyteboxBancarios();
			  </script>';
	}
}

?>

<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>
	<script type="text/javascript" src="../../includes/prototype.js"></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type=hidden name="salvar" value="false" />
	<input type=hidden name="prsid" value="<?php echo $prsid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" >Informações do Convênio.</th>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Data do convênio:</td>
			<td> <?=$dadosProjetoData; ?></td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Valor:</td>
			<td><?=number_format($dadosProjeto['prsvalorconvenio'],2, ',', '.'); ?> </td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Número do processo:</td>
			<td id="numProcesso"><?=$dadosProjeto['prsnumeroprocesso']; ?> </td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" >Cadastro de dados Financeiros do Convênio.</th>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Ano:</td>
			<td> 
				<input type="hidden" name="hmdano" id="hmdano" title="ano" value="<?= $proxano; ?>"></input>
				<label><?= $proxano; ?></label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mês:</td>
			<td>
			<div id="areaMes" >
				<input type="hidden" name="hmdmes" id="hmdmes" title="mês" value="<?= $proxmes; ?>"></input>
				<label><?= $proxmes; ?></label>
			</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor saldo do primeiro dia do mês:</td>
			<td><?= campo_texto('hmdsaldo1diames', "N", "S", "Saldo primeiro dia do mês", 30, 50, "", "", '', '', '', 'id="hmdsaldo1diames" onkeyup="mascaraValor(this)"', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor saldo do último dia do mês:</td>
			<td><?= campo_texto('hmdsaldoultimodiames', "N", "S", "Saldo do útimo dia do mês", 30, 50, "", "", '', '', '', 'id="hmdsaldoultimodiames" onkeyup="mascaraValor(this)"', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor da contrapartida:</td>
			<td><?= campo_texto('hmdvalorcontrapartida', "N", "S", "Valor da contra partida", 30, 50, "", "", '', '', '', 'id="hmdvalorcontrapartida" onkeyup="mascaraValor(this)"', '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Rendimento:</td>
			<td><?= campo_texto('hmdrendimento', "N", "S", "Rendimento", 30, 50, "", "", '', '', '', 'id="hmdrendimento" onkeyup="mascaraValor(this)"', '' ); ?></td>
		</tr>
		<!-- 
		<tr>
			<td class="SubTituloDireita">Total:</td>
			<td><?= campo_texto('total', "N", "N", "Total", 30, 50, "", "", '', '', '', 'id="total" onkeyup="mascaraValor(this)"', '' ); ?></td>
		</tr>
		 -->
		<tr>
			<td></td>
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados();" /></td>
		</tr>
	</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<div id="listaHistoricoBanc">
				<?php $obDadosBancarios->listaDadosBancarios($prsid); ?>
			</div>
		</td>
	</tr>
	<tr>
		<td style="text-align: center;">
			<a style="cursor: pointer" onclick="return false;"> 
				<img src="/imagens/print.png" alt="Versão para impressão" onclick="janelaImpressao('<?php echo $prsid; ?>', '<?php echo $ano; ?>')"> 
			</a>
		</td>
	</tr>
</table>
<script type="text/javascript" src="../../includes/funcoes.js"></script>
<script type="text/javascript">
	
	function removerMes(id){
		if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			 return false;
		}
		return new Ajax.Request(window.location.href,{	
			method: 'post',
			parameters: '&requisicao=deletarMes&id='+id,
			asynchronous: false,
			onComplete: function(resposta)
			{	
				alert(resposta.responseText);
				atualizalistaMesesBancario();
				
			}
		});
	}
	
	function atualizalistaMesesBancario(){
		return new Ajax.Request(window.location.href,{	
			method: 'post',
			parameters: '&requisicao=atualizaListaMeses&prsid='+<?=$prsid; ?>,
			asynchronous: false,
			onComplete: function(resposta)
			{	
				$('listaHistoricoBanc').innerHTML = resposta.responseText
				
			}
		});
	}
	
	/**
	 * function salvarDados();
	 * descscricao 	: Valida dados e submete o formulario;
	 */
	function salvarDados(){
		var mes 		= document.getElementById('hmdmes').value;
		var ultimo 		= document.getElementById('hmdsaldoultimodiames').value;
		var primeiro 	= document.getElementById('hmdsaldo1diames').value;
		var contra 		= document.getElementById('hmdvalorcontrapartida').value;
		var rendimento 	= document.getElementById('hmdrendimento').value;
		
		var nomeform 		= 'formulario';
		var submeterForm 	= false;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "hmdsaldo1diames";
		campos[1] 			= "hmdsaldoultimodiames";
		campos[2] 			= "hmdvalorcontrapartida";
		campos[3] 			= "hmdrendimento";
		campos[4] 			= "hmdmes";
		campos[5] 			= "hmdano";
		tiposDeCampos[0] 	= "valor";
		tiposDeCampos[1] 	= "valor";
		tiposDeCampos[2] 	= "valor";
		tiposDeCampos[3] 	= "valor";
		tiposDeCampos[4] 	= "funcao:testaMes('"+mes+"')";
		tiposDeCampos[5] 	= "select";
		
		if(validaForm(nomeform, campos, tiposDeCampos, submeterForm )){
			var total = parseInt( mascaraglobal( "[#]", primeiro ) ) + parseInt( mascaraglobal( "[#]", contra ) ) + parseInt( mascaraglobal( "[#]", rendimento ) );
			var ultimoDesformatado = parseInt( mascaraglobal( "[#]", ultimo ) );
			
			//if(ultimoDesformatado > total){
			//	alert("O saldo do último dia do mês, não pode ser maior que a soma dos outros campos.");
			//	return false;
			//}
			document.formulario.submit();
		}
	}

	/**
	 * function testaMes(mes);
	 * descscricao 	: Testa se o mês e valido;
	 */
	function testaMes(mes){
		var erro = "O mês não é um valor válido.";
		if(!mes){
			return "O campo mês é obrigatório";
		}
		if (!validaInteiro(mes)){
	       return erro;
	    }
	    if (mes.length < 2){
	    	 mes = 0+mes;
	    }
	    if (mes > 12 || mes < 1){
	       return erro;
	    }
		return true;
	}

	/**
	 * function mascaraComNegativo();
	 * descscricao 	: Mascara valores negativos ou não
	 */
	function mascaraValor( obj ){
		valor = obj.value;
		var complemento = '';
		
		if( valor.substr(0, 1) == '-' ){
			complemento = '-';
		}
		obj.value = complemento+mascaraglobal( '[###.]###,##', valor );
	}
	
	function carregaMes(ano, prsid){
		return new Ajax.Request(window.location.href,{	
			method: 'post',
			parameters: '&requisicao=carregames&anor='+ano+'&prsid='+prsid,
			asynchronous: false,
			onComplete: function(resposta)
			{	
				$('areaMes').innerHTML = resposta.responseText;
			}
		});
	}

	function janelaImpressao(prsid, ano) {
		return window.open('cte.php?modulo=principal/monitoraFinanceiro/imprimir_monitora_convenio&acao=A&prsid='+prsid+'&ano='+ano,'monitoraConvenio','height=500,width=800,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
		
	numProcesso = $('numProcesso').innerHTML;
	numProcessoF = mascaraglobal( '#####.######/####-##', numProcesso );
	$('numProcesso').innerHTML = numProcessoF;
</script>
</body>
</html>
