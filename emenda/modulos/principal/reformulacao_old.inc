<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . 'includes/workflow.php';

if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	getDataFinalConvenio($_POST['vigdias'], $_POST['vigdatainicio']);
	exit;
}

unset( $_SESSION["emenda"]["ptridReformulacao"] );
unset( $_SESSION["emenda"]["ptiidReformulacao"] );
unset( $_SESSION["emenda"]["iniidReformulacao"] );


function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0];
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	
	$obData = new Data();
	$dias = $obData->quantidadeDeDiasEntreDuasDatas( date('Y-m-d'), formata_data_sql($dataFormatada) );
	
	echo $dataFormatada.'_'.$dias;
}

verificaPermissao();

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

if (!$_SESSION["emenda"]["ptridAnalise"] && $_REQUEST[ptrid]) $_SESSION["emenda"]["ptridAnalise"] = $_REQUEST[ptrid];

validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

$ptridAnalise = $_SESSION["emenda"]["ptridAnalise"];

$estado = pegarEstadoAtual( $ptridAnalise );

$usucpf = $_SESSION[usucpf];

$obConvenio = new MinutaConvenio($ptridAnalise);

if( $_GET['action'] == 'E' && !empty($_GET['refid']) ){
	if( $_GET['refsituacao'] ){
		if( $_GET['refsituacao'] == 'A' && $estado == EM_SOLICITACAO_REFORMULACAO ){
			$sql = "SELECT * FROM emenda.ptvigencia WHERE refid = ".$_GET['refid'];
			if( $db->executar( $sql ) ){
				$sql = "DELETE FROM emenda.ptvigencia WHERE refid = ".$_GET['refid'];
				$db->executar( $sql );
			}
			$sql = "SELECT * FROM emenda.reformulatipos WHERE refid = ".$_GET['refid'];
			if( $db->executar( $sql ) ){
				$sql = "DELETE FROM emenda.reformulatipos WHERE refid = ".$_GET['refid'];
				$db->executar( $sql );
			}
			$sql = "UPDATE emenda.planotrabalho SET 
					refid = NULL, ptrdataalteracao = NULL
					, usucpfalteracao = NULL 
					WHERE refid = ".$_GET['refid'];
			$db->executar($sql);
			$sql = "DELETE FROM emenda.ptminreformulacao WHERE refid = ".$_GET['refid']." and refsituacao = 'A'";
		} else {	
			$sql = "UPDATE emenda.ptminreformulacao SET refstatus = 'I', refdataalteracao = now(), usucpfalteracao = '". $usucpf ."'
					WHERE refid = ".$_GET['refid']." and refsituacao = 'A'";
		}
		$db->executar( $sql );
		
		$sql = "UPDATE emenda.ptvigencia SET vigstatus = 'I'
				WHERE refid = ".$_GET['refid']." and ptrid = '$ptridAnalise'";
		
		$db->executar( $sql );
		
		$db->commit();
		$db->sucesso( 'principal/reformulacao' );
	} else {
		echo "<script>
				alert('Não é permitido realizar a exclução de uma reformulação com situação de GERAÇÃO DO TERMO ADITIVO');
				window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=A';
			  </script>";
		exit();
	}	
}


if ($_REQUEST['salvar1']) {
	$d = explode("/", $_POST[vigdatainicio]);
	$_POST[vigdatainicio] = "$d[2]-$d[1]-$d[0]";	
	
	$sql = "SELECT pmcid FROM emenda.ptminutaconvenio WHERE (ptrid = '$ptridAnalise') AND (pmcstatus = 'A')";
	$dados = $db->carregar($sql);
	$pmcid = $dados[0]['pmcid'];

	// Salva informações do texto
	if ($_REQUEST['id']){		
		$sql = "UPDATE emenda.ptminreformulacao SET 
				reftexto = '". $_POST[reftexto] ."',
				usucpfalteracao = '". $usucpf ."',
				refdataalteracao = now(),
				refsituacao = '". $_POST[situacao] ."',
				refdsc = '". $_POST[refdsc] ."'";
				if ($_POST[mdoid]){
					$sql .= ", mdoid = ". $_POST[mdoid];
				}
		$sql .= " where refid = ". $_REQUEST['id'];
		
		$db->executar($sql);
		$db->commit();
		
		if( $_POST['refid_vigencia'] ){
			if( empty( $_POST['trefid'][3] ) ){
				$sql = "DELETE FROM emenda.ptvigencia WHERE refid = ".$_REQUEST['id']." AND vigstatus = 'A'";
			} else {
				$sql = "UPDATE emenda.ptvigencia SET 
							vigdatainicio = '". $_POST[vigdatainicio] ."',
							vigdias = '". $_POST[vigdias] . "',
							vigdatafim = '". $_POST[vigdatainicio]. "'::date + interval '". $_POST[vigdias] . " days'
					WHERE ptrid = ". $ptridAnalise . "
					AND  refid = ". $_REQUEST['id'] . "
					AND vigstatus = 'A'";
			}
			
			$db->executar($sql);
			$db->commit();	
		} else {
			$refid = $_REQUEST['id'];
			insereVigenciaReformulacao( $_POST, $ptridAnalise, $pmcid, $refid );
		}
		
		
		// Deleta as opcoes para incluir novamente
		if (count($_POST[trefid]))
		{
			$sql = "DELETE FROM emenda.reformulatipos 
					WHERE refid = ". $_REQUEST['id'];
			$db->executar($sql);
			$db->commit();
			
		}
		$refid = $_REQUEST['id']; 
		
	}else {
		
		$sql = "INSERT INTO emenda.ptminreformulacao
						(
							ptrid,
							reftexto,
							refstatus,
							usucpfinclusao,
							refdatainclusao,
							pmcid,
							refsituacao,
							refdsc ";
						if ($_POST[mdoid]){
							$sql .= ", mdoid";
						}
						
		$sql .= " 		)
						VALUES
						(
							'$ptridAnalise',
							'$_POST[reftexto]',
							'A',
							'$usucpf',
							NOW(),
							'$pmcid',
							'$_POST[situacao]',
							'$_POST[refdsc]'";
							
						if ($_POST[mdoid]){
							$sql .= ", '$_POST[mdoid]'";
						}
		$sql .= " 				
						)";
						
		
		$db->executar($sql);
		$db->commit();
	
		// Pegar id do ultimo registro inserido
		$sql = "SELECT CURRVAL('emenda.ptminreformulacao_refid_seq') AS refid";
		$dados = $db->carregar($sql);
		$refid = $dados[0]['refid'];
	
		// Salva informações da vigência
		
		if ($_POST[vigdatainicio] && $_POST[vigdias] && !empty( $_POST['trefid'][3] ) )
		{	
			insereVigenciaReformulacao( $_POST, $ptridAnalise, $pmcid, $refid );
			// Salva informações da vigência
		}
		
		$sql = "UPDATE emenda.planotrabalho SET 
				refid = ".$refid. "
				, ptrdataalteracao = NOW()
				, usucpfalteracao = '".$usucpf. "' 
				WHERE ptrid = ". $ptridAnalise;
		$db->executar($sql);
		$db->commit();
		
		$xml  = "";
		$xml .= "<refid>$refid</refid>";
	
		$sql = "INSERT INTO 
					emenda.historicogeral
					(
						tabid,
						histidtabela,
						histtxtalteracao,
						histdatainclusao,
						usucpfinclusao,
						ptrid
					)
				VALUES
					(
						'1',
						'{$ptridAnalise}',
						'{$xml}',
						NOW(),
						'{$_SESSION[usucpf]}',
						'{$ptridAnalise}'
					)";
	
		$db->executar($sql);		
		$db->commit();	
	}
	
	// Salva opcoes
	if ($_POST[trefid])
	{
	//	foreach ($_POST[trefid] AS $k => $v)
		//{
			$sql = "INSERT INTO emenda.reformulatipos (refid, trefid) VALUES ('$refid', '$_POST[trefid]')";
			
			$db->executar($sql);
			$db->commit();			
	//	}
	}
	
	echo "<script>alert('Operação realizada com sucesso!'); window.open('emenda.php?modulo=principal/reformulacao&acao=A','_top');</script>"; exit;
	
	
} else if($_REQUEST['mdoid'] != "") {
	
	$sql = "SELECT mdoconteudo FROM emenda.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
	
	$dados = $db->carregar($sql);
	$imitexto = $dados[0]['mdoconteudo'];
	
	if($imitexto){
		$obConvenio->alteraMacrosConvenio($imitexto, 0);
	}
	else{
		$imitexto = "Não existem informações sobre esta minuta.";
	}
	
	echo $imitexto;
	
	exit;
}

function insereVigenciaReformulacao($post, $ptridAnalise, $pmcid, $refid){
	global $db;
	
	$sql = "UPDATE emenda.ptvigencia SET vigstatus = 'I' WHERE (ptrid = '$ptridAnalise') AND (vigstatus = 'A') AND (refid is not null)";
	$db->executar($sql);
	$db->commit();
	
	$sql = "INSERT INTO emenda.ptvigencia
				(	ptrid,
					vigdatainicio,
					vigdias,
					vigdatafim,
					pmcid,
					vigstatus,
					vigdatainclusao,
					usucpfinclusao,
					refid
				)			
				VALUES
				(
					'$ptridAnalise',
					'$post[vigdatainicio]',
					'$post[vigdias]',
					'$post[vigdatainicio]'::date + interval '$post[vigdias] days',
					'$pmcid',
					'A',
					NOW(),
					'".$_SESSION['usucpf']."',
					'$refid'
				)";
	$db->executar($sql);
	return $db->commit();
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$arMnuid = array();
if( !$boReformulacao && pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] ) == EM_REFORMULACAO ){
	$arMnuid = array('6254');
} else {
	if( $boReformulacao ){
		$arMnuid = array('5501', '5499');
	}
}

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );
echo montaTituloReformulacao( $_SESSION["emenda"]["ptridAnalise"], '', $boReformulacao );
monta_titulo( 'Reformulação', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"]);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], true, $boReformulacao );


$titulo = ($estado == EM_SOLICITACAO_REFORMULACAO ? "<b>Selecione um tipo de Reformulação</b>" : "<b>Iniciar Reformulação para o Plano de Trabalho</b>");

monta_titulo( '', $titulo);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form id="formReformulacao" name="formReformulacao" method="post" action="emenda.php?modulo=principal/reformulacao&acao=A">
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="salvar1" id="salvar1" value="1" />
<input type="hidden" name="id" id="id" value="<?echo $_GET[refid];?>" />

<table width='100%' class="tabela" align="center">
<td>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
	<?php
		// Caso esteja visualizando uma alteração anterior
		
		if ($_GET[refid])
		{
			$sql = "SELECT *
						FROM emenda.ptminreformulacao
						WHERE refid = '$_GET[refid]'";
			$dadosReformulacao = $db->carregar($sql);
			
		}
	?>
	<tr style='display:none;'>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Situação:</b>				
        </td>
        <td>
        	<input type="radio" name="situacao" onclick="if(this.checked){alteracao(this, i);}" value="A" checked/> Análise
        	<?php
				// Caso a analise da reformulacao já tenha acontecido, abrir para elabora o termo aditivo.
				
				if ($_GET[refid])
				{
					$sql = "SELECT anaid
								FROM emenda.analise ana
								INNER JOIN emenda.planotrabalho ptr on ana.ptrid = ptr.ptrid
								WHERE ptr.refid = '$_GET[refid]'
								AND ana.anatipo = 'T'
								AND ana.anadataconclusao is not null";
					$dadosAnalise = $db->pegaUm($sql);

					if ($dadosAnalise["anaid"]) {

					($dadosReformulacao[0]["refsituacao"]=="T") ? $checked = "checked=\"checked\"" : $checked = "";
						
					//echo "<input type='radio' name='situacao' onclick='if(this.checked){alteracao(this, i);}' value='T' $checked /> Gerar Termo Aditivo";
		
					}
				}
			?>
    	</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Tipo de Reformulação:</b>				
        </td>
        <td>

		<?php
				// Caso esteja visualizando uma alteração anterior
				
				if ($_GET[refid])
				{
					$sql = "SELECT *
								FROM emenda.reformulatipos
								WHERE refid = '$_GET[refid]'";
					if ($dadosTipos = $db->carregar($sql))
					{
						foreach ($dadosTipos AS $k => $dados)
						{
							$trefid[$dados[trefid]] = ' checked';
						}
					}
					
					$sql = "SELECT *
								FROM emenda.ptvigencia
								WHERE refid = '$_GET[refid]'";
					$dadosVigencia = $db->carregar($sql);
				}

				// Caso nao tenha vigencia pegar o padrao
				
				if (!$dadosVigencia[0])
				{
					$sql = "SELECT * FROM emenda.ptvigencia WHERE (ptrid = '$ptridAnalise') AND (vigstatus = 'A')";
					$dadosVigencia = $db->carregar($sql);
				}

				$perfil  = pegaPerfilArray($_SESSION['usucpf']);

				// INSTITUICAO_BENEFICIADA = 274
		        if (in_array(274, $perfil)) $externo = "and trefexterno = 'S'";
				
				// Carrega opcoes da reformulação
				$sql = "SELECT *
						FROM emenda.tiporeformulacao
						WHERE trefstatus = 'A'$externo";

				if ($dados = $db->carregar($sql))
				{
					$html .= "<table id='opcoes'><tr>";
					
					for ($i=0; $i<count($dados); $i++)
					{	
						//if (!($i % 5)) $html .= "</tr><tr>";
												
						//nome e valor = 
						//$html .= "<tr><td><input type='radio' name='trefid[{$dados[$i][trefid]}]' value='{$dados[$i][trefformulario]}' onclick='alteracao(this,{$dados[$i][trefformulario]})' {$trefid[$dados[$i][trefid]]}/>{$dados[$i][trefnome]}</td></tr>";
						if ($_GET[refid]) {
							$html .= "<tr><td><input type='radio' name='trefid' disabled value='{$dados[$i][trefid]}' onclick='alteracao(this,{$dados[$i][trefformulario]})' {$trefid[$dados[$i][trefid]]}/>{$dados[$i][trefnome]}</td></tr>";
						} else {
							$html .= "<tr><td><input type='radio' name='trefid' value='{$dados[$i][trefid]}' onclick='alteracao(this,{$dados[$i][trefformulario]})' {$trefid[$dados[$i][trefid]]}/>{$dados[$i][trefnome]}</td></tr>";
						}
					}

					$html .= "</table>\n";
				}

				echo $html;
			?> 
			
        </td>
    </tr><!--

    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" id="2" style="display:none">
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Minuta cadastrada:</b>
        </td>
        <td>
        	<a href=''>Minuta de Convênio</a>
        </td>
    </tr>
    </table>
        
    --><table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" id="3" style="display:none">
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Data da Celebração:</b>
        </td>
        <td>
		<?
			$vigdatainicio = $dadosVigencia[0][vigdatainicio];
			if (!$vigdatainicio) $vigdatainicio = date("Y-m-d");
			
			$vigdatainicio = ($vigdatainicio ? formata_data($vigdatainicio) : "");
			echo campo_data2('vigdatainicio', 'S', 'S','Prazo Inicio', '', '', 'calculaDiasVigencia();');
		?>
		<input type="hidden" name="refid_vigencia" id="refid_vigencia" value="<?=$dadosVigencia[0]['refid']; ?>">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            <b>Dias de Vigência:</b>
        </td>
        <td>
		<?//prddiasvigencia
			$vigdias = $dadosVigencia[0][vigdias];
			echo campo_texto( 'vigdias', 'S', 'S', '', 11, 20, '[#]', '', '', '', 0, 'id="vigdias"','', $vigdias,'calculaDiasVigencia();' );
			$vigdias = $vigdias ? $vigdias : 0;
		?>
		<input type="hidden" name="vigdias_atual" id="vigdias_atual" value="<?=$vigdias; ?>">
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            <b>Data Final da Vigência:</b>
        </td>
        <td>
        <?
			//prddiasvigencia
			$vigdatafim = ($vigdatafim ? formata_data($vigdatafim) : "");
			echo campo_texto( 'vigdatafim', 'S', 'N', '', 11, 20, '', '', '', '', 0, 'id="vigdatafim"','','','' );
			$dias = $dias ? $dias : 0;
		?>
		<input type="hidden" name="diasVencimento" id="diasVencimento" value="<?=$dias; ?>">
        </td>
    </tr>        
    </table>
    
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" id="1" style="display:none">
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Modelo do termo aditivo utilizado:</b>
        </td>
        <td>
        <select name="mdoid" id="mdoid" onchange="carregaTexto();"><option value='0'>Escolher</option>
	    <?php 
	    $arDados = array();
	    
	    	$sql = "select modelosdocumentos.mdoid, modelosdocumentos.mdonome
						from emenda.modelosdocumentos
			     		where modelosdocumentos.mdostatus = 'A'
			     		and tpdcod = 21 ";
		    
		    $arDados = $db->carregar($sql);
	    
	    $count = 1;
	    
	    $perfil = pegaPerfil($_SESSION['usucpf']);
	    
	    if($arDados) {
	    	foreach($arDados as $dados){
	    		($dadosReformulacao[0][mdoid] == $dados[mdoid]) ? $selected = ' selected' : $selected = '';
	    		
	    		echo "<option value='{$dados[mdoid]}'$selected>{$dados[mdonome]}</option>";
	    	}
	    }
	    
	    $imitexto = "{$dadosReformulacao[0][reftexto]}";
        ?>
        </select>
        </td>    
    </tr>
    <tr>
    <td colspan='2' height='300'>
		<div>
			<textarea id="texto" name="reftexto" rows="90" cols="80" style="width:100%;height:400px;" class="emendatinymce"><?=$imitexto?></textarea>
		</div>		
    </td>
    </tr>
    </table>
    
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Justificativa:</b>
        </td>
        <td>
        <?php
			if( $_GET[refid] ){
				$sql = "SELECT
						*
					FROM
						emenda.ptminreformulacao
					WHERE
						refid = '$_GET[refid]'";
			$dadosRef = $db->carregar($sql);
			}
        ?>
        	<?=campo_textarea('refdsc', 'S', 'S', 'Descrição da Reformulação', 80, 5, 500, '', '', '', '', 'Descrição da Reformulação', $dadosRef[0]['refdsc']);?>
        </td>
    </tr>
    </table>

	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="4">
			<?
			$arRegistro = montaTiposReformulacao( $ptridAnalise );
		    
			//if( ($estado == EM_SOLICITACAO_REFORMULACAO || $estado == EM_GERACAO_TERMO_ADITIVO ) && !$boHabilita || $_REQUEST['refid'] ){
			//	echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();"/>';	
			//}else{
			// 	echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();" disabled="disabled"/>';
			// }
			
			
			$sql = "SELECT refsituacaoreformulacao FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'C' AND refstatus = 'A' AND ptrid = ".$ptridAnalise;
			$testaSit = $db->pegaUm( $sql );
			if( empty( $testaSit ) ){					
				echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();"/>';	
			} else {
			 	echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();" disabled="disabled"/>';
			}
			
			
			?>
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="cancelarReformulacao();" />
		</td>
	</tr>
    <tr>
    	<td>
    	<table align='left'>   	
    	</table>
    	</td>
    </tr>	
	</table>

	
</td>
<td width='100' align="center">
<?
	$sql = "SELECT 
			  refid
			FROM 
			  emenda.ptminreformulacao
			WHERE
				ptrid = $ptridAnalise
				and refstatus = 'A'
				and refsituacao = 'A'";
	
	$refidWF = $db->pegaUm( $sql );
	
	$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
	wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"], 'refid' => $refidWF) );
?>
</td>
</table>    
</form>
<?
	monta_titulo('', 'Lista de Reformulação');
	$cabecalho = array("Opção", "Tipo Reformulação", "Data Inclusão", "Status", "Situação");
	$db->monta_lista_array($arRegistro, $cabecalho, 20, 4, 'N','Center');
?>
</body>
<script type="text/javascript" src="js/reformulacao.js"></script>