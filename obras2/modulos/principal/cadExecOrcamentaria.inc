<?php
$empid 		     = $_SESSION['obras2']['empid'];
$obrid 		     = $_SESSION['obras2']['obrid'];
$ioctipodetalhamento = $_GET['ioctipodetalhamento'];

$itemExecOrcamentaria = new ItensExecucaoOrcamentaria( $_REQUEST['iocid'] );
if ( $itemExecOrcamentaria->iocid ){
	extract( $itemExecOrcamentaria->getDados() );
}

//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if( possui_perfil(array(PFLCOD_CALL_CENTER))){
    $desabilita = "disabled = 'disabled'";
    $hab        = "N";
}else{
    $desabilita = "";
    $hab        = "S";
}
switch ( $_POST['requisicao'] ){
	case 'salvar':
		$arDado = $_POST;

		$arqid	 = null;
	    $arquivo = $_FILES["arquivo"];
	    if ( $_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
                    include_once APPRAIZ."includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec(null, null,"obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo', false);
                    $arqid = $file->getIdArquivo();
	    }
	    
	    $arCamposNulo = array();
	    
	    if ( $arDado['arqid'] || $arqid ){
		    $arDado['arqid'] = ($arqid ? $arqid : $arDado['arqid']);
	    }else{
		    $arDado['arqid'] = NULL;
		    $arCamposNulo[]  = 'arqid';
	    }
		
		if ( $_GET['acao'] == 'A' ){
			$arDado['empid'] = $_SESSION['obras2']['empid'];
		}else{
			$arDado['obrid'] = $_SESSION['obras2']['obrid'];
		}
		$arDado['usucpf'] 			   = $_SESSION['usucpf'];
		$arDado['ioctipodetalhamento'] = $ioctipodetalhamento; 
		$arDado['iocdtposicao'] 	   = formata_data_sql( $arDado['iocdtposicao'] ); 
		$arDado['iocvalor'] 		   = MoedaToBd( $arDado['iocvalor'] ); 
		
//		if ( empty($arDado['rgadscsimplificada']) ){
//			$arDado['rgadscsimplificada'] = null;
//			$arCamposNulo[] = 'rgadscsimplificada'; 	
//		}
		
		$itemExecOrcamentaria->popularDadosObjeto( $arDado )
				  	     	 ->salvar(true, true, $arCamposNulo);
		$db->commit();				  		   

		die("<script>
				alert('Operação realizada com sucesso!'); 
				window.opener.location.replace( window.opener.location ); 
				window.close();
			 </script>");
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
}

$execOrcamentaria = new ExecucaoOrcamentaria();
if ( $ioctipodetalhamento == TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO ){
	$msgTitle    = 'Cadastro de Detalhamento Orçamentário Empenhado';
	$msgCampoVlr = 'Valor Empenhado (R$)';
	$msgCampoPer = '% Empenhado';
	$msgScript 	 = 'empenhado';
}else{
	$msgTitle = 'Cadastro de Detalhamento Orçamentário Liquidado';
	$msgCampoVlr = 'Valor Liquidado (R$)';
	$msgCampoPer = '% Liquidado';
	$msgScript 	 = 'liquidado';
}

if ( $_GET['acao'] == 'A' ){
	// empreendimento || obra || orgao
	verificaSessao( 'empreendimento' );
	
	extract( $execOrcamentaria->carregaPorEmpid( $empid ) );
}else{
	// empreendimento || obra || orgao
	verificaSessao( 'obra' );
	
	extract( $execOrcamentaria->carregaPorObrid( $obrid ) );
}

$eocvlrtotal = $eorvlrcapital + $eorvlrcusteio;
if ( $eocvlrtotal ){
	$iocpercent = ($iocvalor / $eocvlrtotal * 100);
}
$arquivo = new Arquivo( $arqid );
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( $msgTitle, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<!--Formulário do Popup-->
<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="salvar">
<input type="hidden" name="eorid" id="eorid" value="<?php echo $eorid ?>">
<input type="hidden" name="iocid" id="iocid" value="<?php echo $iocid ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="subtitulocentro" colspan="2">Orçamento para a Obra</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="190px">Capital (R$):</td>
		<td>
			<?php
				$eorvlrcapital = number_format($eorvlrcapital, 2, ",", ".");
				echo $eorvlrcapital;
			?>
			<input type="hidden" name="eorvlrcapital" id="eorvlrcapital" value="<?php echo $eorvlrcapital; ?>">
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Custeio (R$):</td>
		<td>
			<?php
				$eorvlrcusteio = number_format($eorvlrcusteio, 2, ",", ".");
				echo $eorvlrcusteio;
			?>
			<input type="hidden" name="eorvlrcusteio" id="eorvlrcusteio" value="<?php echo $eorvlrcusteio; ?>">
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Total (R$):</td>
		<td>
			<?php
				$eocvlrtotal  = number_format($eocvlrtotal, 2, ",", "."); 
				echo $eocvlrtotal;
			?>
			<input type="hidden" name="eocvlrtotal" id="eocvlrtotal" value="<?php echo $eocvlrtotal; ?>">
		</td>
	</tr>

	<tr>
		<td class="subtitulocentro" colspan="2">Detalhamento Orçamentário</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="190px">Data:</td>
		<td>
		<?php
		echo campo_data2( 'iocdtposicao', 'S', $hab, '', 'S' );	
		?> 
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="190px"><?php echo $msgCampoVlr ?>:</td>
		<td>
		<?php
		$iocvalor = number_format($iocvalor, 2, ",", ".");
		echo campo_texto( 'iocvalor', 'S', $hab, '', 13, 15, '###.###.###,##', null, 'right', '', 0, 'id="iocvalor"', 'calculaPercentualValor();');	
		?> 
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><?php echo $msgCampoPer; ?>:</td>
		<td id="iocpercent">
		<?php 
		$iocpercent = number_format($iocpercent, 2, ",", ".");
		echo $iocpercent 
		?>
		</td>
	</tr>	
	
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
		<td id="anexo">
			<input type="hidden" name="arqid" id="arqid" value="<?php echo $arquivo->arqid; ?>"/>
			<input type="file" name="arquivo" id="arquivo"/>
			<br>
			<?php
			if ( $arquivo->arqid ){
				echo "<a href='javascript:DownloadArquivo(" . $arquivo->arqid . ");'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
			}
			?>
		</td>
	</tr>
	
	
	<tr bgcolor="#c0c0c0">
		<td>&nbsp;</td>
		<td>
			<input type="button" value="Salvar" <?php echo $desabilita;?> onclick="validaExecOrc();" style="cursor:pointer;"/>
			<input type="button" value="Fechar" onclick="window.close();" style="cursor:pointer;"/>
		</td>
	</tr>
	
</table>
</form>
<script type="text/javascript">
<!--demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.-->
 var hab = "<?php echo $hab?>";
 if(hab == "N"){
     $('#anexo').find('input[name=arquivo]').attr('disabled','disabled');
 }
DownloadArquivo = function(){
    $('#requisicao').val('download');
    $('#formulario').submit();
}

function validaExecOrc(){
	var obCalc = new Calculo();

	var vlrData  = $('#iocdtposicao').val();
	var vlrValor = $('#iocvalor').val();
	var vlrValorPerc;
	var vlrTotal = $('#eocvlrtotal').val();
	
	if( !vlrData || !vlrValor ){
		alert("Todos os campos obrigatórios devem ser preenchidos!");
		return false;
	}
	
	if ( obCalc.comparar(vlrValor, '0,00', '<=') ){
		alert("O valor <?php echo $msgScript; ?> deve ser maior que R$0,00");
		return false;
	}

	if( obCalc.comparar(vlrValor, vlrTotal, '>') ){
		alert("O valor <?php echo $msgScript; ?> não pode ser maior que o valor total!");
		return false;
	}
	
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}

function calculaPercentualValor(){
	var obCalc = new Calculo();

	var vlrValor = $('#iocvalor').val();
	var vlrValorPerc;
	var vlrTotal = $('#eocvlrtotal').val();
	
	vlrValor = (vlrValor ? vlrValor : '0,00');
	
	if( obCalc.comparar(vlrValor, vlrTotal, '>') ){
		alert("O valor <?php echo $msgScript; ?> não pode ser maior que o valor total!");
		vlrValor = vlrTotal;
	}
	
	vlrValorPerc = obCalc.operacao(vlrValor, vlrTotal, '/', 10);
	vlrValorPerc = obCalc.operacao(vlrValorPerc, '100,00', '*');
	vlrValorPerc = mascaraglobal('###.###.###,##', vlrValorPerc);		
	$('#iocvalor').val( vlrValor );
	$('#iocpercent').html( vlrValorPerc );
}
//-->
</script>

