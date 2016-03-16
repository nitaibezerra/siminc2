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
$sql = "SELECT * FROM cte.projetosape WHERE prsid = ".$prsid;
$dadosProjeto = $obDadosBancarios->pegaLinha($sql);
$dadosProjetoData = $obdata->formataData($dadosProjeto['prsiniciovigencia'], "DD de mesTextual de YYYY");

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
</form>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<div id="listaHistoricoBanc">
				<?php $obDadosBancarios->listaDadosBancarios($prsid, true); ?>
			</div>
		</td>
	</tr>
	<tr>
		<td style="text-align: center;">
			<input type="button" value="imprimir" onClick="javascript:window.print()" id="botao" />
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
	numProcesso = $('numProcesso').innerHTML;
	numProcessoF = mascaraglobal( '#####.######/####-##', numProcesso );
	$('numProcesso').innerHTML = numProcessoF;
</script>
</body>
</html>
