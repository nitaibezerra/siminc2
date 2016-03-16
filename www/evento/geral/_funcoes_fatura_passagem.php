<?php 

function listaFaturas( $request ){
	
	global $db;
	
	$where = Array();
	if( $request['fatnufatura'] ){
		array_push($where," fatnufatura = ".$request['fatnufatura']." ");
	}
	
	if( $request['inicio'] ){
		$request['inicio'] = explode('/',$request['inicio']);
		$request['inicio'] = $request['inicio'][2].'/'.$request['inicio'][1].'/'.$request['inicio'][0];
		array_push($where," fatdtemissao >= '%".$request['inicio']."%' ");
	}
	
	if( $request['fim'] ){
		$request['fim'] = explode('/',$request['fim']);
		$request['fim'] = $request['fim'][2].'/'.$request['fim'][1].'/'.$request['fim'][0];
		array_push($where," fatdtemissao <= '%".$request['fim']."%' ");
	}
	
	$sql = "SELECT
				'' as editar,
				'' as excluir,
				fatnufatura,
				to_char(fatdtemissao,'DD/MM/YYYY') as emissão
			FROM
				evento.fatura
			WHERE
				1=1 --fatstatus = 'A'
			".(count($where)>0?" AND ".implode(' AND ',$where) : "");
//	ver($sql,d);
	$faturas = $db->carregar($sql);
	$cabecalho = array("&nbsp;", "&nbsp;", "N° Fatura", "Data de Inclusão");
	$db->monta_lista_array($faturas, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

function listaPassagensFaturadas( $fatid ){
	
	global $db;
	
	$sql = "SELECT
				CASE 
					WHEN i.psdid is null
						THEN '<img align=\"top\" width=\"15px\" border=\"0\" src=\"../imagens/erro_checklist.png\" tittle=\"Passagem não existe.\">'
					WHEN pe.psdtarifapraticada > p.psdtarifapraticada OR pe.psdvlrmulta > p.psdvlrmulta OR pe.psdvlrdifremarcacao_ > p.psdvlrdifremarcacao_
						THEN '<img align=\"top\" width=\"15px\" border=\"0\" src=\"../imagens/exclamacao_checklist.png\" tittle=\"Valores incorretos.\">'
					ELSE '<img align=\"top\" width=\"15px\" border=\"0\" src=\"../imagens/check_checklist.png\" tittle=\"OK.\">'
				END as status,
				pe.psdnumpcdp, 
				pe.psdnoproposto, 
				pe.aeroidori||'/'||pe.aeroiddes||'<br> ('|| pe.psdnuvoo||')' as voo, 
       			to_char(pe.psddtemissaobilhete,'DD/MM/YYYY') as data, 
       			pe.psdnubilhete,  
       			pe.psdnuempenho||'/'||pe.psdanoempenho||' - '||pe.psddsempenho as empenho, 
		       	pe.psdnoorgao, 
		       	pe.psdcontrole, 
       			pe.psdtarifapraticada+pe.psdvlrmulta+pe.psdvlrdifremarcacao_ as valorEmp,
       			p.psdtarifapraticada+p.psdvlrmulta+p.psdvlrdifremarcacao_ as valor
		  	FROM 
		  		evento.itendafatura i
		  	INNER JOIN evento.passagemempresa pe ON pe.psdempid = i.psdempid
		  	LEFT  JOIN evento.passagemscpd 	   p ON i.psdid 	= p.psdid
		  	WHERE
		  		psdstatus = 'A' AND fatid = $fatid
		  	ORDER BY
		  		psdid DESC";
	$passagensPCDP = $db->carregar($sql);
	$cabecalho = array("N° PCDP", "Proposto por:", "Vôo", "Valor", "Data Emissão Bilhete", "N° Bilhete", "Empenho", "Orgão");
	$db->monta_lista_array($passagensPCDP, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

function formataDataWS( $data ){
	$data = substr($data, 8,2).'/'.substr($data, 5,2).'/'.substr($data, 0,4);
	return $data;
}

function importarFatura( $request ){
	
	global $db;

	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	
	// Create the client instance
	$wsdl = 'http://www.itsviagens.com.br/ServicosWeb/Service.asmx?WSDL';
	$options = Array(
					'exceptions'	=> true,
			        'trace'			=> true,
					'proxy_host'     => "proxy.mec.gov.br",
                    'proxy_port'     => 8080,
					'encoding'		=> 'ISO-8859-1' 
	);
	$client = new SoapClient($wsdl, $options);
	
	// Call the SOAP method
	$autenticacao = $client->__soapCall('WebFaturaBilhetes', Array('WebFaturaBilhetes' => Array('Cliente' => 27115,'Fatura' => $request['codigo'])));
	$autenticacao = simplexml_load_string($autenticacao->WebFaturaBilhetesResult->any);
	
	$fatura = get_object_vars($autenticacao->NewDataSet->Faturas);
	$bilhetes = get_object_vars($autenticacao->NewDataSet);
	$bilhetes = $bilhetes['Bilhetes'];
	
	if( $request['gerarXLS'] ){
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=fatura_".$request['codigo'].".xls");
		header ( "Content-Disposition: attachment; filename=fatura_".$request['codigo'].".xls");
		header ( "Content-Description: MID Gera excel" );
	}else{
		include  APPRAIZ."includes/cabecalho.inc";
		echo'<br>';
		$html = '<div style="float:left">
					<input type="button" id="importarFat" value="Importar Faturamento." />	
				 </div>
				 <div style="float:right">
					<input type="button" id="gerarXLS" value="Gerar XLS." />	
				 </div>';
		monta_titulo( 'Faturas de Passagens - WS', $html );
	}
	?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript">
	$(document).ready(function() { 
		$('#aguarde').hide();
		$('#importarFat').click(function(){
			windowOpen('evento.php?modulo=principal/popupImportaFatura&acao=A',
	                	'', 'height=150,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
		});
		$('#gerarXLS').click(function(){
			window.location = 'evento.php?modulo=principal/cadFatura&acao=A&req=importarFatura&gerarXLS=true&codigo='+$('#fatura').val();
		});
	});
	</script>
	<form method="post" name="formListaFat" id="formListaFat" action="">
		<input type="hidden" value="<?=$request['codigo'] ?>" id="fatura" />
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="subtituloDireita" colspan="2"><b>Fatura:</b></td>
				<td colspan="<?=(count($bilhetes[0])-2) ?>"><?=$fatura['HANDLE']; ?></td>
			</tr>
			<tr>
				<td class="subtituloDireita" colspan="2"><b>Data da fatura:</b></td>
				<td colspan="<?=(count($bilhetes[0])-2) ?>"><?=date('d/m/Y',$fatura['EMISSAOFATURA'])?></td>
			</tr>
			<tr bgcolor="#DCDCDC">
				<?php 
				foreach( $bilhetes[0] as $k => $valor){
				?>
				<td><b><?=$k ?></b></td>
				<?php 
				}
				?>
			</tr>
			<?php 
			foreach( $bilhetes as $k => $bilhete ){
				$bilhet = get_object_vars($bilhete);
				$cor = $k%2 == 0 ? 'bgcolor="white"' : '';
			?>
			<tr <?=$cor ?>>
				<?php 
				foreach( $bilhete as $t => $valor){
					if( $t == 'DATAEMISSAO' || $t == 'EMISSAOFATURA' || $t == 'DATAVENCIMENTO' ){ /*$valor = get_object_vars($valor); */$valor = formataDataWS($valor[0]); }
				?>
				<td><?=$valor ?></td>
				<?php 
				}
				?>
			</tr>
			<?php 
			}
			?>
		</table>
	</form>
	<?php 
	// Check for a fault
//	if ($client->fault) {
//	    echo '<h2>Fault</h2><pre>';
//	    print_r($result);
//	    echo '</pre>';
//	} else {
//	    // Check for errors
//	    $err = $client->getError();
//	    if ($err) {
//	        // Display the error
//	        echo '<h2>Error</h2><pre>' . $err . '</pre>';
//	    } else {
//	        // Display the result
//	        echo '<h2>Result</h2><pre>';
//	        print_r($autenticacao);
//	    echo '</pre>';
//	    }
//	}
}

function listaPassagensSCDP( $request ){
	
	global $db;
	
	$sql = "SELECT 
				psdnumpcdp, 
				psdnoproposto, 
				aeroidori, 
				aeroiddes, 
				psdnuvoo, 
       			psdtarifapraticada, 
       			to_char(psddtemissaobilhete,'DD/MM/YYYY') as data, 
       			psdnubilhete, 
       			psdvlrmulta, 
       			psdvlrdifremarcacao_, 
       			psdnuempenho||'/'||psdanoempenho||' - '||psddsempenho as empenho, 
		       	psdnoorgao, 
		       	psdcontrole 
		  	FROM 
		  		evento.passagem_scpd
		  	WHERE
		  		psdstatus = 'A'
		  	ORDER BY
		  		psdid DESC";
//	ver($sql,d);
	$passagensPCDP = $db->carregar($sql);
	$cabecalho = array("N° PCDP", "Proposto por:", "Origem", "Destino", "N° Vôo", "Tarifa", "Data Emissão Bilhete", "N° Bilhete", "Multa", "Vlr Remarcação", "Empenho", "Orgão");
	$db->monta_lista_array($passagensPCDP, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

function existePassagem( $numpcdp ){
	
	global $db;
	
	$sql = "SELECT
				true
			FROM
				evento.passagemscpd
			WHERE
				psdnumpcdp = '".$numpcdp."'";
	return $db->pegaUm($sql);
}

?>