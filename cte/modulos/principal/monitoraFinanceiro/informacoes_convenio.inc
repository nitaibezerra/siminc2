<?php
if( $_REQUEST['req'] == 'importarParecer' ){
	$sql = "SELECT DISTINCT
				hmpparecer
			FROM 
				cte.historicomonitoramentoconveniopareceres 
			WHERE
				hmpid = {$_REQUEST['hmpid']}";
	$parecer = $db->pegaUm($sql);
	echo $parecer;
	die();
}

include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
$obMontaListaAjax = new MontaListaAjax($db, true);   

// Histórico de informações do convênio.
function listaHistorico( $prsid ){
	
	global $db,$obMontaListaAjax;
	$sql = "SELECT 
				'<img title=\"Importar Parecer\" src=\"../imagens/indicador-verde2.png\" onclick=\"importarParecer( ' || hmpid || ' )\">' as acao,
				CASE WHEN hmpstatus = 'A'
					THEN '<strong> ' || to_char(hmpdatamonitoramento, 'DD/MM/YYYY') || '</strong>' 
					ELSE to_char(hmpdatamonitoramento, 'DD/MM/YYYY')
				END AS data,
				CASE WHEN hmpstatus = 'A'
					THEN '<strong>' || substring(hmpparecer from 1 for 37) || '</strong>'  
					ELSE substring(hmpparecer from 1 for 37) || ' ' 
				END AS parecer, 
				CASE WHEN hmpstatus = 'A'
					THEN '<strong>' || u.usunome || '</strong>'   
					ELSE u.usunome
				END AS parecista,
				CASE WHEN hmpstatus = 'A'
					THEN CASE WHEN hmprelevancia = 'A'
							THEN '<strong>Alta</strong>'
							ELSE CASE WHEN hmprelevancia = 'M' 
									THEN '<strong> Média </strong>'
									ELSE '<strong> Baixa </strong>' 
								 END
						 END   
					ELSE CASE WHEN hmprelevancia = 'A'
							THEN 'Alta'
							ELSE CASE WHEN hmprelevancia = 'M' 
									THEN 'Média'
									ELSE 'Baixa' 
								 END
						 END 
				END AS relevancia,
				CASE WHEN hmpstatus = 'A'
					THEN CASE WHEN hmpatendimento = 'F'
							THEN '<strong>FNDE</strong>'
							ELSE CASE WHEN hmpatendimento = 'V' 
									THEN '<strong> Vistoria </strong>'
									ELSE '<strong> Não informado </strong>' 
								 END
						 END   
					ELSE CASE WHEN hmpatendimento = 'F'
							THEN 'FNDE'
							ELSE CASE WHEN hmpatendimento = 'V' 
									THEN 'Vistoria'
									ELSE 'Não informado' 
								 END
						 END  
				END AS atendimento,
				CASE WHEN hmpstatus = 'A'
					THEN '<strong> Ativo </strong>'
					ELSE 'Inativo'
				END AS status
			FROM 
				cte.historicomonitoramentoconveniopareceres hmcp
			LEFT JOIN
				seguranca.usuario u ON u.usucpf = hmcp.usucpf
			WHERE
				prsid = {$prsid}
			ORDER BY
				hmpdatamonitoramento DESC, hmpid DESC";
	$registrosPorPagina = 10; 
	$cabecalho = Array('Ação','Data', 'Parecer', 'Parecista', 'Grau de Relevância','Local de Atendimento','Status');
	$obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 10, 'N', '', '', '');

}

/****************** INCLUDES *************************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'includes/classes/dateTime.inc');

/***************** DECLARAÇÃO DE VARIAVEIS & INSTANCIA OBJETOS ***************/
$prsid 				= $_REQUEST['prsid'];
$prsnumconvsape     = $db->pegaUm('SELECT prsnumconvsape FROM cte.projetosape WHERE prsid = '.$prsid);
$hmpparecer			= $_REQUEST['hmpparecer'];
$hmprelevancia		= $_REQUEST['hmprelevancia'];
$hmpatendimento		= $_REQUEST['hmpatendimento'];
$usucpf				= $_SESSION['usucpf'];
$obdata				= new Data();

/***************** SALVA DADOS DO FORM ***************************************/

if( $_REQUEST['salvar'] ){
	if( $hmpparecer != '' ){
		$sql = "UPDATE cte.historicomonitoramentoconveniopareceres
	   			SET 
	   				hmpstatus = 'I'
	 			WHERE 
	 				prsid = {$prsid};";
		
		$sql .= "INSERT INTO cte.historicomonitoramentoconveniopareceres(
			            prsid, 
			            hmpdatamonitoramento, 
			            hmpparecer, 
			            usucpf, 
			            hmprelevancia, 
			            hmpstatus,
			            hmpatendimento)
			    VALUES ({$prsid}, now(), '".$hmpparecer."', '{$usucpf}', '{$hmprelevancia}', 'A', '{$hmpatendimento}');";
		
		$db->executar($sql);
		$db->commit();
		echo "<script>alert(Dados salvos com sucesso!);</script>";
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
<div id="loader-container">
	<div id="loader">
		<img src="../imagens/wait.gif" border="0" align="middle">
		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>
<form name="formConv" id="formConv" action="" method="post">
	<input type=hidden name="salvar" id="salvar" value="false" />
	<input type=hidden name="prsid"  value="<?php echo $prsid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" >Informações do Convênio.</th>
		</tr>
		<tr>
			<td  class="SubTituloDireita" width="30%">Numero do convênio:</td>
			<td> <?=$prsnumconvsape; ?></td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Nome do Parecista:</td>
			<td><?=$_SESSION['usunome'] ?> </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" style="text-align:center;"><strong>Informações:</strong></td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<?=campo_textarea( 'hmpparecer', 'S', 'S', '', 100, 10, 2000); ?>
			</td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Data:</td>
			<td><?=$obdata->formataData(date("Y/m/d"), "DD de mesTextual de YYYY"); ?> </td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Grau de relevância:</td>
			<td>
				<input type="radio" name="hmprelevancia" value="A" />Alto<br />
				<input type="radio" name="hmprelevancia" value="M" />Médio<br />
				<input type="radio" name="hmprelevancia" value="B" checked="checked"/>Baixo<br />
			</td>
		</tr>
		<tr>
			<td  class="SubTituloDireita">Local de atendimento:</td>
			<td>
				<input type="radio" name="hmpatendimento" value="F" />FNDE<br />
				<input type="radio" name="hmpatendimento" value="V" checked="checked"/>Visita<br />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" style="text-align:center;">
				<input type="button" value="Gravar Novo Parecer" onclick="salvarDados();"/>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" >Histórico das Informações do Convênio.</th>
		</tr>
	</table>
</form>
<?=listaHistorico( $prsid ); ?>
<script type="text/javascript" src="../../includes/funcoes.js"></script>
<script type="text/javascript">
	
	/**
	 * function salvarDados();
	 * descscricao 	: Valida dados e submete o formulario;
	 */
	function salvarDados(){
		var formularioConv = $('formConv');
		var hmpparecer = $('hmpparecer');
		var salvar 	   = $('salvar');
		if( hmpparecer.value == '' ){
			alert('Campo obrigatório');
			hmpparecer.focus();
			return false;
		}

		formularioConv.submit();
	}

	/**
	 *function importarParecer(hmpid)
	 *importa parecer a ser resproveitado em novo parecer
	 */
	function importarParecer( hmpid ){
		var parecer = $('hmpparecer');
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=importarParecer&hmpid='+hmpid,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				$('loader-container').hide();
				parecer.value = res.responseText;
			}
		});	
	}
	$('loader-container').hide();
</script>
</body>
</html>
