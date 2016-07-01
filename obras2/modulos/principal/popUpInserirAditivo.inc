<?
ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"',true);

$contrato = new Contrato( $_SESSION['obras2']['crtid'] );
extract( $contrato->getDados() );

unset($ttaid);

//if( empty($crtid) ) {
//	die("<script>
//			 alert('Contrato não encontrado.');
//			 window.close();
//		 </script>");
//}
if(possui_perfil(array(PFLCOD_CALL_CENTER))){
    $desabilita = "S";
}

if ( $_POST['operacao'] == 'salvar' ){
	
	// Replica Contrato inserindo crtidpai 
	$contrato 			= new Contrato( $crtid );
	$contrato->crtidpai = $contrato->crtid;
	$contrato->crtid	= null;
	$contrato->usucpf	= $_SESSION['usucpf'];
	$crtidNovo 			= $contrato->salvar();
	
	$obraContrato 	 = new ObrasContrato();
	$obraContrato2 	 = new ObrasContrato();
	$obras 		  	 = new Obras();
	$itemComposicao  = new ItensComposicaoObras();
	$detalheItem	 = new DetalheItem();
	$previsaoDetalhe = new PrevisaoDetalheItem();


    // Salva a nova planilha de custos
    $arqidcusto = null;
    if ( $_FILES['arqidcusto'] && $_FILES['arquivo']['error'] == 0 ){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file  		  = new FilesSimec(null, null, 'obras2');
        $returnUpload = $file->setUpload(null, "arqidcusto", false, false);

        $arqidcusto = $file->getIdArquivo();
    }elseif ( $_FILES['arqidcusto']['error'] != 0 && $_FILES['arqidcusto']['error'] != 4 ){
        $returnUpload = false;
    }

	// Busca obras vinculadas ao contrato
	$arOcrid = $obraContrato->listaIdByContrato( $crtid );
	foreach ($arOcrid as $ocrid){
		// Carrega Obra Contrato
		$obraContrato2->carregarPorId( $ocrid );
		
		// Replica Obra
		$obras->carregarPorId( $obraContrato2->obrid );
		$obrid			 = $obras->obrid;
		$obras->obridpai = $obras->obrid;
		$obras->obrid 	 = null;
		$obridNovo 		 = $obras->salvar();
		$obras->clearDados();
		
		// Replica Obra Contrato
		$obraContrato2->carregarPorId( $ocrid );
		$obraContrato2->crtid 		= $crtidNovo;
		$obraContrato2->obrid 		= $obridNovo;
		$obraContrato2->ocrid 		= null;
		$obraContrato2->salvar();
		$obraContrato2->clearDados();
		
		$arCamposNulo = array();
		// Carrega e atualiza Obra Contrato ATUAL
		$obraContrato->carregarPorId( $ocrid );
		$_POST['obrid'] 		= ($_POST['obrid'] ? $_POST['obrid'] : array());
		$obraContrato->ocraditivado 	= ( in_array($obrid, $_POST['obrid']) ? 't' : 'f');
        $obraContrato->arqidcusto 		= ($arqidcusto) ? $arqidcusto : $obraContrato->arqidcusto;

		if ( $_POST['ocrnovoprazoexecucao'][$obrid] ){
			$obraContrato->ocrprazoexecucao = $_POST['ocrnovoprazoexecucao'][$obrid];
		}else{
			$obraContrato->ocrprazoexecucao = NULL;
			$arCamposNulo[]  				= 'ocrprazoexecucao';
		}
		
		if ( $_POST['ocrnovodtterminoexecucao'][$obrid] ){
			$obraContrato->ocrdtterminoexecucao = formata_data_sql($_POST['ocrnovodtterminoexecucao'][$obrid]);
		}else{
			$obraContrato->ocrdtterminoexecucao = NULL;
			$arCamposNulo[]  					= 'ocrdtterminoexecucao';
		}
		
		if ( $_POST['ocrnovovalorexecucao'][$obrid] ){
			$obraContrato->ocrvalorexecucao = str_replace(array(".",","),array("","."),$_POST['ocrnovovalorexecucao'][$obrid]);
		}else{
			$obraContrato->ocrvalorexecucao = NULL;
			$arCamposNulo[]  			    = 'ocrvalorexecucao';
		}
		
		if ( $_POST['ocrnovocustounitario'][$obrid] ){
			$obraContrato->ocrcustounitario = str_replace(array(".",","),array("","."),$_POST['ocrnovocustounitario'][$obrid]);
		}else{
			$obraContrato->ocrcustounitario = NULL;
			$arCamposNulo[]  			    = 'ocrcustounitario';
		}
		
		$obraContrato->salvar(true, true, $arCamposNulo);
		$obraContrato->clearDados();
		
		// Busca ItensComposicaoObras
		$arIcoid = $itemComposicao->getIdByObra( $obrid );
		foreach ( $arIcoid as $icoid ){
			// Replica ItensComposicaoObras
			$itemComposicao->carregarPorId( $icoid );
			$itemComposicao->obrid = $obridNovo;
			$itemComposicao->icoid = null;
			$icoidNovo 			   = $itemComposicao->salvar();
			$itemComposicao->clearDados();
			
			// Busca detalheItem
			$arDitid = $detalheItem->getAllID( $icoid );
			foreach ( $arDitid as $ditid ){
				// Replica detalheItem
				$detalheItem->carregarPorId( $ditid );
				$detalheItem->icoid = $icoidNovo;
				$detalheItem->ditid = null;
				$ditidNovo 			= $detalheItem->salvar();
				$detalheItem->clearDados();
				
				// Busca PrevisaoDetalheItem
				$arPdiid = $previsaoDetalhe->getIdByDetalheItem( $ditid );
				foreach ( $arPdiid as $pdiid ){
					$previsaoDetalhe->carregarPorId( $pdiid );
					$previsaoDetalhe->ditid = $ditidNovo;
					$previsaoDetalhe->pdiid = null;
					$previsaoDetalhe->salvar();
				}
			}
		}
		
	}
	
	// Insere Arquivo
	if ( $_FILES['arquivo'] && $_FILES['arquivo']['error'] == 0 ){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$file  		  = new FilesSimec(null, null, 'obras2');
		$returnUpload = $file->setUpload(null, "arquivo", false, false);
		
		$arqid = $file->getIdArquivo();
	}elseif ( $_FILES['arquivo']['error'] != 0 && $_FILES['arquivo']['error'] != 4 ){
		$returnUpload = false;	
	}	
	
	if ( $returnUpload == false ){
    	if ( $_FILES['arquivo']['error'] == 3 ){
			echo('<script type="text/javascript">
					alert(\'Falha no upload do arquivo: ' . $_FILES['arquivo']['name'] . '\nAnexe-o novamente!\');
				 </script>');
    	}else{
			$db->rollback();
			
			enviaEmailChiavicatti( $arquivo );
			
			die('<script type="text/javascript">
					alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
					window.location.href = window.location.href; 
				 </script>');
    	}	
	}
	
	// Atualiza CONTRATO
	$contrato->carregarPorId( $crtid );
	
	$contrato->arqid 		        = ($arqid ? $arqid : null);
	$contrato->crtprazovigencia 		= ($contrato->crtprazovigencia ? $contrato->crtprazovigencia : 0);
	$contrato->crtsupressao 		= ( $_POST['trasupressao'] == 'S'  ? 't' : 'f' );
	$contrato->crtjustificativa 		= $_POST['trajustificativa'];
	$contrato->crtdenominacao 		= $_POST['tradsc'];
	$contrato->ttaid 			= $_POST['ttaid'];
	$contrato->crtdtassinaturaaditivo 	= formata_data_sql( $_POST['tradtassinatura'] );
//	$contrato->crtdtassinatura 	= (!empty( $_POST['tradtassinatura'] ) ? formata_data_sql( $_POST['tradtassinatura'] ) : $contrato->crtdtassinatura);
	$contrato->crtprazovigencia 		= (!empty( $_POST['traprazovigencia'] ) ? ($contrato->crtprazovigencia + $_POST['traprazovigencia']) : $contrato->crtprazovigencia);
	$contrato->crtdttermino 		= (!empty( $_POST['traterminovigencia'] ) ? formata_data_sql( $_POST['traterminovigencia'] ) : $contrato->crtdttermino);
	$contrato->crtvalorexecucao 		= (!empty( $_POST['travlrfinalobra'] ) ? MoedaToBd( $_POST['travlrfinalobra'] ) : $contrato->crtvalorexecucao);
//	$contrato->crtpercentualdbi = (!empty( $_POST['crtpercentualdbi'] ) ? $_POST['crtpercentualdbi'] : $contrato->crtpercentualdbi);
	$contrato->salvar();

    $tipoAditivo = new TipoTermoAditivo($contrato->ttaid);
    $obraContrato->carregarPorId( $ocrid );
    $obrid = $obraContrato->obrid;

    $registroAtividade = new RegistroAtividade();
    $registroAtividade->obrid = $obrid;
    $registroAtividade->usucpf = $_SESSION['usucpf'];
    $registroAtividade->rgadscsimplificada = 'ADITIVO INSERIDO';
    $registroAtividade->rgadsccompleta = 'ADITIVO de ' . $tipoAditivo->ttadsc . ' inserido sob a justificativa: ' . $contrato->crtjustificativa;
    $registroAtividade->rgadtinclusao = 'NOW()';
    $registroAtividade->rgastatus = 'A';
    $registroAtividade->salvar();


    $db->commit();
	
	die("<script>
			alert('Operação realizada com sucesso!');
			window.opener.location.href = '?modulo=principal/cadContrato&acao=E'; 
			window.close();
		 </script>");
}

if ($_POST['tipoTela'] == 1){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>

        <tr>
            <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
            <td>
                <?= campo_data2( 'traterminovigencia', 'S', 'S', '', 'S', '', 'dtVigencia();', null,  'dtVigencia();'); ?>
            </td>
        </tr>
	  <tr>
	    <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
	    <td>
		<?= campo_texto( 'traprazovigencia', 'S', 'N', '', 5, 10, '[#]', '', 'left', '', 0); ?>
		</td>
	  </tr>

<!--
	  <tr>
	    <th class="SubTituloDireita">Prazo do Aditivo de Execução da Obra (dias):</th>
	    <td>
		<?//= campo_texto( 'traprazoaditivadoexec', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtExecucao();"', 'dtExecucao();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Término da Execução do Aditivo da Obra:</th>
	    <td>
		<?//= campo_texto( 'traterminoexec', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
-->	  
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Anexo:</th>
		<td id="anexo">
			<input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
	</table>
<?
die;
}
elseif ($_POST['tipoTela'] == 2){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>

        <tr>
            <th class="SubTituloDireita">Valor Final do Contrato Incluindo Aditivo(R$):</th>
            <td>
                <?=campo_texto( 'travlrfinalobra', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0,  'onchange="calculoValorFinal();"', 'calculoValorFinal();' ); ?>
            </td>
        </tr>

	  <tr>
	    <th class="SubTituloDireita">Aditivo de Supressão:</th>
	    <td>
		<ul style="margin: 0; padding: 0;">
			<li style="margin:0;width:80px; list-style-type: none; float:left; ">
				<input class="disabled" type="radio" name="trasupressao" value="S" id="radio_opcao_trasupressao_S">
                <label for="radio_opcao_trasupressao_S">Sim</label>
			</li>
			<li style="margin:0;width:80px; list-style-type: none; float:left; ">
				<input class="disabled" type="radio" name="trasupressao" value="N" id="radio_opcao_trasupressao_N">
				<label for="radio_opcao_trasupressao_N">Não</label>
			</li>
		</ul>
		</td>
	  </tr>
        <script type="text/javascript">
            $(function(){
                $('[name="trasupressao"]').click(function(){return false;});
            });
        </script>
	  <tr>
	    <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlraditivo', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0 ); ?>
		</td>
	  </tr>

<!--  
	  <tr>
	    <th class="SubTituloDireita">Valor Final da Obra Incluindo Aditivo(R$):</th>
	    <td>
		<?//=campo_texto( 'travlrfinalobra', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
	  </td>
	  <tr>
	    <th class="SubTituloDireita">Acréscimo ou Supressão de Área/Quantidade:</th>
	    <td>
		<?//=campo_texto( 'travlrqtdareaacresc', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="calculoQtdFinal();"', 'calculoQtdFinal();' ); ?>
		<?php 
//			$sql = "SELECT
//							umdid as codigo,
//							umdeesc as descricao
//					FROM
//							obras.unidademedida
//					WHERE
//							umdstatus = 'A'";
//			$umdidareaacresc = $dados['umdidobraconstruida'];
//			$db->monta_combo( 'umdidareaacresc', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Área/Quantidade Final Incluindo Aditivo(R$):</th>
	    <td>
		<?//=campo_texto( 'travlrqtdareafinal', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
		<?php 
//			$sql = "SELECT
//							umdid as codigo,
//							umdeesc as descricao
//					FROM
//							obras.unidademedida
//					WHERE
//							umdstatus = 'A'";
//			$umdidareafinal = $dados['umdidobraconstruida'];
//			$db->monta_combo( 'umdidareafinal', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade2' ); ?>
		</td>
	  </tr>
-->	  
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Arquivo:</th>
		<td id="anexo">
                    <input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Planilha de Custos:</th>
            <td id="arqidcusto" >
                <input type="file" name="arqidcusto" id="arqidcusto"/>
            </td>
        </tr>
	</table>
<?
die;
}elseif ($_POST['tipoTela'] == 3){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 50, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
            <td>
                <?= campo_data2( 'traterminovigencia', 'S', 'S', '', 'S', '', 'dtVigencia();', null,  'dtVigencia();'); ?>
            </td>
        </tr>
        <tr>
            <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
            <td>
                <?= campo_texto( 'traprazovigencia', 'S', 'N', '', 5, 10, '[#]', '', 'left', '', 0); ?>
            </td>
        </tr>
	  </tr>
<!--
	  <tr>
	    <th class="SubTituloDireita">Prazo do Aditivo de Execução da Obra (dias):</th>
	    <td>
		<?= campo_texto( 'traprazoaditivadoexec', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtExecucao();"', 'dtExecucao();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Término da Execução do Aditivo da Obra:</th>
	    <td>
		<?= campo_texto( 'traterminoexec', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
-->
        <tr>
            <th class="SubTituloDireita">Valor Final do Contrato Incluindo Aditivo(R$):</th>
            <td>
                <?=campo_texto( 'travlrfinalobra', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0,  'onchange="calculoValorFinal();"', 'calculoValorFinal();' ); ?>
            </td>
        </tr>

        <tr>
            <th class="SubTituloDireita">Aditivo de Supressão:</th>
            <td>
                <ul style="margin: 0; padding: 0;">
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled"  type="radio" name="trasupressao" value="S" id="radio_opcao_trasupressao_S">
                        <label for="radio_opcao_trasupressao_S">Sim</label>
                    </li>
                    <li style="margin:0;width:80px; list-style-type: none; float:left; ">
                        <input class="disabled"  type="radio" name="trasupressao" value="N" id="radio_opcao_trasupressao_N">
                        <label for="radio_opcao_trasupressao_N">Não</label>
                    </li>
                </ul>
            </td>
        </tr>
        <script type="text/javascript">
            $(function(){
                $('[name="trasupressao"]').click(function(){return false;});
            });
        </script>
        <tr>
            <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
            <td>
                <?= campo_texto( 'travlraditivo', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0 ); ?>
            </td>
        </tr>

<!--	  	
	  <tr>
	    <th class="SubTituloDireita">Alteração da Área/Quantidade:</th>
	    <td>
		<?= campo_texto( 'travlrqtdareaalterada', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="calculoQtdFinal();"', 'calculoQtdFinal();' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareaalterada = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareaalterada', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Área/Quantidade Final Incluindo Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlrqtdareafinal', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareafinal = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareafinal', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade2' ); ?>
		</td>
	  </tr>
-->	  
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	  <tr>
		<th class="SubTituloDireita">Arquivo:</th>
		<td id="anexo">
			<input type="file" name="arquivo" id="arquivo"/>
		</td>
	  </tr>
        <tr>
            <th class="SubTituloDireita">Planilha de Custos:</th>
            <td id="arqidcusto">
                <input type="file" name="arqidcusto" id="arqidcusto"/>
            </td>
        </tr>
	</table>
<?
die;
}
?>
<html>
<head>
	<title>Inserir Aditivo</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body topmargin="0" leftmargin="0" onload="abreAditivo('<?=$ttaid ?>');">
<form method="post" action="" name="form_aditivo" id="form_aditivo" enctype="multipart/form-data">
<table class="Tabela" align="center">
  <tr>
    <th class="SubTituloDireita">Tipo de Aditivo:</th>
    <td>
    <? 
	$sql = "SELECT 
				ttaid AS codigo, 
				ttadsc AS descricao
			FROM 
				obras2.tipotermoaditivo;";
	    					  
	$db->monta_combo("ttaid", $sql, 'S', "Selecione...", "abreAditivo", '', '', '', 'S', 'ttaid');
	?>
    <input type="hidden" name="operacao" 				value="salvar">
	<input type="hidden" name="crtdtassinatura" value="<?=formata_data( $crtdtassinatura ) ?>">	
	<input type="hidden" name="crtdttermino" value="<?=formata_data( $crtdttermino )?>">
	<input type="hidden" name="crtprazovigencia" value="<?=$crtprazovigencia?>">
	<input type="hidden" name="crtvalorexecucao" value="<?=$crtvalorexecucao ?>">
		
<!--  
    <input type="hidden" name="traid" 					value="<?=$traid ?>">
    <input type="hidden" name="obrdttermino" 			value="<?=($obAditivo->traterminoexec ? $obAditivo->traterminoexec : ($dadosTermo['traterminoexecmax'] ? $dadosTermo['traterminoexecmax'] : formata_data($dados['obrdttermino'])))?>">
    <input type="hidden" name="crtdttermino" 		value="<?=($obAditivo->traterminovigencia ? $obAditivo->traterminovigencia : ($dadosTermo['traterminovigenciamax'] ? formata_data($dadosTermo['traterminovigenciamax']) : formata_data($dados['crtdttermino'])))?>">
    <input type="hidden" name="crtdtassinatura" value="<?=formata_data($dados['crtdtassinatura']) ?>">
    <input type="hidden" name="crtprazovigencia" 		value="<?=($dadosTermo['traprazovigenciamax'] ? ($dadosTermo['traprazovigenciamax'] + $dados['crtprazovigencia']) : $dados['crtprazovigencia'])?>">
    
    <input type="hidden" name="crtvalorexecucao" 		value="<?=$valorContrato ?>">
    <input type="hidden" name="obrqtdconstruida" 		value="<?=($obAditivo->travlrqtdareafinal ? number_format($obAditivo->travlrqtdareafinal ,2,',','.') : ($dadosTermo['travlrqtdareafinalmax'] ? ($dadosTermo['travlrqtdareafinalmax']) : number_format($dados['obrqtdconstruida'] ,2,',','.')))?>">
    <input type="hidden" name="tobraid" 				value="<?=$dados['tobraid']?>">
-->	
    </td>
  </tr>
  <tr style="display:none;" id="trTelaAditivo">
  	<td id="telaAditivo" colspan="2"></td>
  </tr>
  <tr id="trTelaAditivoObrasVinc">
  	<td id="telaAditivoObrasVinc" colspan="2" style="">
  		<fieldset style="display:none">
  			<legend>Obras Vinculadas ao Aditivo</legend>
	  		<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" style="margin-bottom: 10px;" class="listagem">
	  		<tr bgcolor="#F5F5F5">
	  			<td class="SubTituloCentro">&nbsp;</td>
	  			<td class="SubTituloCentro">Obra</td>
	  			<td class="SubTituloCentro">Início Anterior</td>
	  			<td class="SubTituloCentro">Prazo Anterior</td>
	  			<td class="SubTituloCentro">Término Anterior</td>
	  			<td class="SubTituloCentro">Valor Anterior</td>
	  			<td class="SubTituloCentro" id="td_tit_prz1">Acréscimo de Prazo</td>
	  			<td class="SubTituloCentro" id="td_tit_prz2">Novo Prazo</td>
	  			<td class="SubTituloCentro" id="td_tit_prz3">Novo Término</td>
	  			<td class="SubTituloCentro" id="td_tit_vlr1">Acréscimo de valor</td>
	  			<td class="SubTituloCentro" id="td_tit_vlr2">Novo Valor</td>
	  		</tr>
  	<?php 
  	$obraContrato  = new ObrasContrato();
  	$arDadosContrato = $obraContrato->listaByContrato( $crtid, null, array('IS(obraAditivo)' => true) );
  	
  	$i=0;
  	foreach ( $arDadosContrato as $dadosContrato ){
  		$background = ($i%2 == 0 ? '#FFFFFF' : '#F5F5F5');
  		$i++;
  	?>
	  		<tr bgcolor="<?=$background?>">
	  			<td align="center" width="10">
	  				<input name="obrid[]" type="checkbox" onclick="habilitarCamposObra(this);" id="obrid_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['obrid'] ?>">
	  			</td>
	  			<td>
	  			
	  				<input type="hidden" name="ocrqtdconstrucao" id="ocrqtdconstrucao_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['ocrqtdconstrucao'] ?>">
	  				<input type="hidden" name="ocrcustounitario" id="ocrcustounitario_<?=$dadosContrato['obrid'] ?>" value="<?=$dadosContrato['ocrcustounitario'] ?>">
	  				<input type="hidden" name="ocrnovocustounitario[<?=$dadosContrato['obrid'] ?>]" id="ocrnovocustounitario_<?=$dadosContrato['obrid'] ?>" value="">
	  				
	  				<label for="obrid_<?=$dadosContrato['obrid'] ?>" style="cursor:pointer;">
	  				<?=$dadosContrato['obrnome']?>
	  				</label>
	  			</td>
	  			<td>
	  				<?=campo_data2('ocrdtinicioexecucao', 'N', 'N','Início de Execução do Objeto', '', '', '', formata_data($dadosContrato['ocrdtinicioexecucao']), '', '', 'ocrdtinicioexecucao_' . $dadosContrato['obrid']); ?>
	  			</td>
	  			<td>
	  				<?=campo_texto('ocrprazoexecucao', 'N', 'N', '', 5, 10, '[#]', '', 'right', '', 0, 'id="ocrprazoexecucao_'.$dadosContrato['obrid'].'" onchange=""','', $dadosContrato['ocrprazoexecucao'], '' ); ?>
	  			</td>
	  			<td>
	  				<?=campo_data2('ocrdtterminoexecucao', 'N', 'N','Término de Execução do Objeto', '', '', '', formata_data($dadosContrato['ocrdtterminoexecucao']), '', '', 'ocrdtterminoexecucao_'.$dadosContrato['obrid']); ?>
	  			</td>
	  			<td>
					<?=campo_texto('ocrvalorexecucao', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrvalorexecucao_'.$dadosContrato['obrid'].'"','', number_format( $dadosContrato['ocrvalorexecucao'], 2, ',', '.'), '' ); ?>
				</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_texto('acrescimoprazoexecucao', 'S', 'N', '', 5, 10, '[#]', '', 'right', '', 0, 'id="acrescimoprazoexecucao_'.$dadosContrato['obrid'].'" onchange="dataTerminoObraAditivo(this);"','dataTerminoObraAditivo(this);', '', '' ); ?>
	  			</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_texto('ocrnovoprazoexecucao['.$dadosContrato['obrid'].']', 'N', 'N', '', 5, 10, '[#]', '', 'right', '', 0, 'id="ocrnovoprazoexecucao_'.$dadosContrato['obrid'].'"', '', '', '' ); ?>
	  			</td>
	  			<td class="td_prazo_exec">
	  				<?=campo_data2('ocrnovodtterminoexecucao['.$dadosContrato['obrid'].']', 'N', 'N','Término de Execução do Objeto', '', '', '', '', '', '', 'ocrnovodtterminoexecucao_'.$dadosContrato['obrid']); ?>
	  			</td>
	  			<td class="td_valor_exec">
					<?=campo_texto('acrescimovalorexecucao', 'S', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="acrescimovalorexecucao_'.$dadosContrato['obrid'].'" onBlur="preencheValorAditivo(this);"','preencheValorAditivo(this);', '' ); ?>
				</td>
	  			<td class="td_valor_exec">
					<?=campo_texto('ocrnovovalorexecucao['.$dadosContrato['obrid'].']', 'N', 'N', '', 12, 18, '[###.]###,##', '', 'right', '', 0, 'id="ocrnovovalorexecucao_'.$dadosContrato['obrid'].'"','', '' ); ?>
				</td>
	  		</tr>
  	<?php
  	}
  	?>
  			</table>
  		</fieldset>	
  	</td>
  </tr>
      
  <tr style="display:none;" id="trButton" bgcolor="#C0C0C0">
  	<td colspan="2" align="center">
  		<input id="salvar" type="button" value="Salvar" onclick="validaForm(); //valTerminoVigenciaExecucao();">
  	</td>
  </tr>
</table>
</form>

<script type="text/javascript">

function abreAditivo(value){

	if (value){
		divCarregando();
		var traid = '<?=$_REQUEST['traid'] ?>';
		var dado = {async: false, "tipoTela" : value, "traid" : traid};
		$('#telaAditivo').load("?modulo=principal/popUpInserirAditivo&acao=A", dado, function (){ 
					if(traid){atualizaParaEdicao();}
					<? 
//					if ($traid){
//						$sql = "SELECT 
//					  				COUNT(*) 
//					  			FROM 
//						  			obras.supervisao s 
//								JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
//						  		JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
//						  		  									  AND ito.traid = {$traid}";
//						$possuiSupervisao = $db->pegaUm( $sql );
//						if ($possuiSupervisao || ($traid != $dadosTermo['traidmax']) ){
					?>
//						$(document).ready(function (){
//							$('#form_aditivo :input').attr('disabled', true);
//						});
					<?		
//						}
//					}
					?>
					divCarregado(); 
				});
				
		if( value==1 ) {
		
			$("[name*=acrescimovalorexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_valor_exec').hide();
				$('#td_tit_vlr1').hide();
				$('#td_tit_vlr2').hide();

				$('.td_prazo_exec').show();
				$('#td_tit_prz1').show();
				$('#td_tit_prz2').show();
				$('#td_tit_prz3').show();

				
			});
		}else if( value==2 ) {
			$("[name*=acrescimoprazoexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_prazo_exec').hide();
				$('#td_tit_prz1').hide();
				$('#td_tit_prz2').hide();
				$('#td_tit_prz3').hide();
				
				$('.td_valor_exec').show();
				$('#td_tit_vlr1').show();
				$('#td_tit_vlr2').show();				
				
			});
		
		}else if ( value==3 ){
			$("[name*=acrescimoprazoexecucao]").each(function (){
				$(this).val("0");
				$(this).keyup();
				$('.td_prazo_exec').show();
				$('#td_tit_prz1').show();
				$('#td_tit_prz2').show();
				$('#td_tit_prz3').show();
				
				$('.td_valor_exec').show();
				$('#td_tit_vlr1').show();
				$('#td_tit_vlr2').show();				
				
			});
		}
		//
		$('#trTelaAditivo').show();
		$('#trTelaAditivoObrasVinc').show();
		$('#trButton').show();
	}else{
		$('#telaAditivo').html("");
		$('#trTelaAditivo').hide();
		$('#trTelaAditivoObrasVinc').hide();
		$('#trButton').hide();
	}
        <?php
    if ($desabilita == 'S') {?>
        $("#ttaid" ).click(function() {
            $(document).ready(function (){
                setTimeout(function(){ 
                                        jQuery('#form_aditivo').find('input[type=text]').attr('disabled',true); 
                                        jQuery('#form_aditivo').find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                                        jQuery('#anexo').find('input[name=arquivo]').attr('disabled',true);
                                        jQuery('#arqidcusto').find('input[name=arqidcusto]').attr('disabled',true);
                                        jQuery('#trajustificativa').attr('disabled','disabled');
                                        jQuery('#salvar').attr('disabled', true);
                                      }, 1500);
            });
        });
<?php }?>
}



//var msgIni = '';
//if ($.trim($('[name="crtdttermino"]').val()) == ''){
//	msgIni = 'A "Data de término do contrato (aba contratação)", não foi informado!\n';
//}
//
//if ($.trim($("[name='obrdttermino']").val()) == ''){
//	msgIni += 'O "Término de execução da obra (aba contratação)", não foi informado!\n';
//}
//
//if ($.trim($('[name="crtdtassinatura"]').val()) == ''){
//	msgIni += 'A "Data de Assinatura do Contrato (aba contratação)" não foi informada!\n';
//}
//
//if ($.trim($('[name="crtprazovigencia"]').val()) == ''){
//	msgIni += 'O "Prazo de Vigência do Contrato (aba contratação)" não foi informado!\n';
//}
//
//if ( msgIni != '' ){ alert( msgIni + 'Informe-o antes de incluir o aditivo.' ); }


//function atualizaParaEdicao(){
//	var obDt  = new Data();
//	var obCal = new Calculo();
//	
//	/* Cálculos de números */
//	// Campos de comparação (digitados)
//	var travlraditivo 		  = $("[name='travlraditivo']");
//	var travlrqtdareaalterada = $("[name='travlrqtdareaalterada']");
//	var travlrqtdareaacresc   = $("[name='travlrqtdareaacresc']");
//	// Campos de comparação (hidden)
//	var crtvalorexecucao = $('[name="crtvalorexecucao"]');
//	var obrqtdconstruida = $('[name="obrqtdconstruida"]');
//	
//	if ( crtvalorexecucao[0] && travlraditivo[0] ){
//		var aditivoSup	 = $("[name='trasupressao']:checked").val();
//		// Ao contrário, para tornar o valor real 
//		var operador = aditivoSup == 'S' ? '+' : '-'; 
//		
//		crtvalorexecucao.val( mascaraglobal('[###.]###,##', (obCal.operacao(crtvalorexecucao.val(), travlraditivo.val(), operador)) ) );
//	}
//	if ( obrqtdconstruida[0] && travlrqtdareaalterada[0] ){
//		obrqtdconstruida.val( mascaraglobal('[###.]###,##', (obCal.operacao(obrqtdconstruida.val(), travlrqtdareaalterada.val(), '-')) ) );
//	}else if ( crtvalorexecucao[0] && travlrqtdareaacresc[0] ){
//		obrqtdconstruida.val( mascaraglobal('[###.]###,##', (obCal.operacao(obrqtdconstruida.val(), travlrqtdareaacresc.val(), '-')) ) );
//	}
//	/* FIM - Cálculos de números */
//	/* Cálculos de datas */
//	// Campos de comparação (digitados)
//	var traprazoaditivadoexec = $('[name="traprazoaditivadoexec"]');
//	var traprazovigencia	  = $('[name="traprazovigencia"]');
//	// Campos de comparação (hidden)
//	var obrdttermino	  = $('[name="obrdttermino"]');
//	var crtdttermino = $('[name="crtdttermino"]');
//	
//	if ( obrdttermino[0] && traprazoaditivadoexec[0] ){
//		obrdttermino.val( obDt.dtAddDia(obrdttermino.val(), (- traprazoaditivadoexec.val())) );
//	}
//	
//	if ( crtdttermino[0] && traprazovigencia[0] ){
//		crtdttermino.val( obDt.dtAddDia(crtdttermino.val(), (- traprazovigencia.val())) );
//	}
//	
//	/* FIM - Cálculos de datas */
//}

/*
 *	Funções utilizadas no aditivo de PRAZO
 */
function dtVigencia(){

    var dt   = $("[name='crtdttermino']").val();
    var dtFim  = $("[name='traterminovigencia']").val();
    var date1 = new Date( dtFim.substr(6,4) + '-' + dtFim.substr(3,2) + '-' + dtFim.substr(0,2) );
    var date2 = new Date( dt.substr(6,4) + '-' + dt.substr(3,2) + '-' + dt.substr(0,2) );
    var diffDays = parseInt((date1 - date2) / (1000 * 60 * 60 * 24));

    if ( dt && diffDays ){
        if(diffDays <= 0){
            alert('O campo "Data de Término da Vigência do Aditivo do Contrato" deve ser maior que a data de termino do contrato atual.');
            $("[name='traprazovigencia']").val('');
            $("[name='traterminovigencia']").val('');
        } else {
            $("[name='traprazovigencia']").val(diffDays);
        }
    }else{
        if ( !dt ){
            alert('O "Data de término do contrato (aba contratação)", não foi informado!\nInforme-o antes de incluir o aditivo.');
        }
        $("[name='traprazovigencia']").val('');
        $("[name='traterminovigencia']").val('');
    }

    $("[name='acrescimoprazoexecucao']:eq(0)").val($("[name='traprazovigencia']").val()).change();
}

//function dtExecucao(){
//	var dt   = $("[name='obrdttermino']").val();
//	var dia  = $("[name='traprazoaditivadoexec']").val();
//	
//	if ( dt && dia ){ 
//		var obDt 	  = new Data();
//		dtTerminoExec = obDt.dtAddDia(dt, dia); 
//	}else{
//		if ( !dt ){
//			alert('O "Término de execução da obra (aba contratação)", não foi informado!\nInforme-o antes de incluir o aditivo.');
//		}
//		$("[name='traprazoaditivadoexec']").val('');
//		dtTerminoExec = ""; 
//	}
//	$("[name='traterminoexec']").val( dtTerminoExec );
//}

function valDtAssinaturaContrato(){
	var msg;
	var dtAssinaturaAditivo = $("#tradtassinatura").val();
	var dtAssinaturaContrat = $('[name="crtdtassinatura"]').val();
	var diaVigenciaContrat	= $('[name="crtprazovigencia"]').val();
	
	if ( dtAssinaturaAditivo && dtAssinaturaContrat && diaVigenciaContrat ){
		var obDt = new Data();
		var dtTerminoVigenciaContrat = obDt.dtAddDia(dtAssinaturaContrat, diaVigenciaContrat);
		
		if ( !obDt.comparaData(dtAssinaturaAditivo, dtTerminoVigenciaContrat, '<=') ){
			alert('A "Data de assinatura do aditivo" deve ser MENOR ou IGUAL ao "Término da vigência do contrato ( '+dtTerminoVigenciaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}
		
		if ( !obDt.comparaData(dtAssinaturaAditivo, dtAssinaturaContrat, '>') ){
			alert('A "Data de assinatura do aditivo" deve ser MAIOR do que a "Data de Assinatura do Contrato ( '+dtAssinaturaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}
		
	}else{
		if ( dtAssinaturaContrat == "" ){
			msg = 'A "Data de Assinatura do Contrato (aba contratação)" não foi informada!\nInforme-a antes de incluir o aditivo.\n';
		}
		if ( diaVigenciaContrat == "" ){
			msg += 'O "Prazo de Vigência do Contrato (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.\n';
		}
		if (msg != ""){
			alert( msg );
			$("#tradtassinatura").val("");
			return false;
		}
	}
	return true;
}

function valPrazoVigenciaExecucao(){
//	var prazoVigencia = $("[name='traprazovigencia']").val() ? new Number( $("[name='traprazovigencia']").val() ) : '';
//	var prazoExecucao = $("[name='traprazoaditivadoexec']").val() ? new Number( $("[name='traprazoaditivadoexec']").val() ) : '';
	var prazoVigencia = $("[name='traterminovigencia']").val() ;
	var prazoExecucao = $("[name='traterminoexec']").val() ;
	
	if ( prazoVigencia && prazoExecucao ){
		var obDt = new Data();
		if ( obDt.comparaData(prazoVigencia, prazoExecucao, '<') ){
			alert('A "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
//			alert('O "Prazo de Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Prazo do Aditivo de Execução da Obra"!');
			return false;
		}
	}
	return true;
}

function valPrazoObra() {
	var dtTerminoVigencia = $("[name='traterminovigencia']").val();
	
	var invalidar = false;
	
	$("[name^='ocrnovodtterminoexecucao[']").each(function (){
		var dtNovoTerminoExecucao = $(this).val();
		 
		var obDt = new Data();
		if ( !obDt.comparaData(dtTerminoVigencia, dtNovoTerminoExecucao, '>=') ){
			alert('O "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Data do Novo Término da Execução da Obra"!');
			invalidar = true;
		}
	
	});
	
	if(invalidar) {
	
		return false;
		 
	} else {
	
		return true;
		
	}
}

function valTerminoVigenciaExecucao(){
	var dtTerminoVigencia = $("[name='traterminovigencia']").val();
	var dtTerminoExecucao = $("[name='traterminoexec']").val();
	
	if ( dtTerminoVigencia && dtTerminoExecucao ){
		var obDt = new Data();
		if ( !obDt.comparaData(dtTerminoVigencia, dtTerminoExecucao, '>=') ){
			alert('O "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
			return false;
		}
	}
	return true;
}
/*
 *	FIM - Funções utilizadas no aditivo de PRAZO
 */

/*
 * Funções usadas no aditivo de VALOR 
*/
function calculoValorFinal (){
    var vlrCustoObra = $("[name='crtvalorexecucao']").val();
    var vlrFinal	 = $("[name='travlrfinalobra']").val();

    if ( vlrCustoObra == "" ){
        alert('O "valor Contratado da Obra (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.');
        $("[name='travlraditivo']").val("");
        return false;
    }

    var obCal = new Calculo();
    var comp = obCal.comparar(vlrFinal, vlrCustoObra, '<');
    var operador = comp == true ? '-' : '+';
    var vlrAditivo = obCal.operacao(vlrCustoObra, vlrFinal, operador);

    $("[name='travlraditivo']").val( mascaraglobal('[###.]###,##', vlrAditivo ));

    if(operador == '-'){
        jQuery("[name='trasupressao']").eq(0).attr('checked', true);
    } else {
        jQuery("[name='trasupressao']").eq(1).attr('checked', true);
    }
}

//function calculoQtdFinal(){
//	var qtdConstruida = $("[name='obrqtdconstruida']")[0] 	   ? $("[name='obrqtdconstruida']").val() 	   : "";
//	var qtdAcresArea  = $("[name='travlrqtdareaalterada']")[0] ? $("[name='travlrqtdareaalterada']").val() : "";
//	var qtdAlterArea  = $("[name='travlrqtdareaacresc']")[0]   ? $("[name='travlrqtdareaacresc']").val()   : "";
//	
//	if ( qtdConstruida == "" ){
//		alert('A "Área/Quantidade a ser Construída (aba contratação)" não foi informada!\nInforme-a antes de incluir o aditivo.');
//		$("[name='travlrqtdareaalterada']").val("");
//		$("[name='travlrqtdareaacresc']").val("");
//		return false;
//	}
//	
//	var obCal = new Calculo();
//	qtd = qtdAcresArea ? qtdAcresArea : qtdAlterArea;
//	
//	if ( qtd != 0 && qtd != "" ){
//		var qtdFinal = obCal.operacao(qtd, qtdConstruida);
//		qtdFinal 	 = mascaraglobal('[###.]###,##', qtdFinal );
//		$("[name='travlrqtdareafinal']").val( qtdFinal );
//	}else{
//		$("[name='travlrqtdareafinal']").val("");
//	}	
//	return true;
//}

//function comparacaoValAditivo(){
////	3 => "Ampliação"
////	4 => "Reforma"
////	1 => "Construção"
////	5 => "Reforma/Ampliação"
//
//	var tipoObra 	 = $("[name='tobraid']").val();
//	var vlrAditivo   = $("[name='travlraditivo']").val();
//	var vlrCustoObra = $("[name='crtvalorexecucao']").val();
//
//	var obCal = new Calculo();
//	
//	vlrAditivo   = vlrAditivo ? obCal.converteMonetario( vlrAditivo ) : 0;
//	vlrCustoObra = vlrCustoObra ? obCal.converteMonetario( vlrCustoObra ) : 0;
//
//	var percent = (vlrAditivo / vlrCustoObra);  
//
//	if ( ((tipoObra == 3 || tipoObra == 1) && percent >= 0.25) || ((tipoObra == 5 || tipoObra == 4) && percent >= 0.5) ){
//		alert('O Valor do aditivo ultrapassa os limites do Art 65 da Lei 8.666/93 - \nO usuário deverá inserir na justificativa o enquadramento no 2º do mesmo Art 65.');
//	}
//}
 
/*
 * FIM -> Funções usadas no aditivo de VALOR 
*/

function valDtTermino(){
	var retorno = true;
	if ($.trim($('[name="crtdttermino"]').val()) == ''){
		alert('A "Data de término do contrato (aba contratação)", não foi informado!\n');
		retorno = false;
	}
	
	return retorno;
}

function valObrasVinculadas(){
	var retorno = true;

	if ( $("[id*=obrid_]:checked").length == 0 ){
		alert('É necessário informar a(s) obra(s) que será(ão) vinculada(s) ao aditivo.');
		retorno = false;
	}

	return retorno;
}

function validaForm(){
	try{
		$(":text").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
				return false;
			}
		});
		
		$("textarea").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
			}
		});
		
		if($("#arquivo").val()=='') {
			alert('Arquivo é obrigatório');
			return false;
		}
		
		if ( !valDtAssinaturaContrato() ) throw "Erro: valDtAssinaturaContrato();";
		if ( !valPrazoVigenciaExecucao() ) throw "Erro: valPrazoVigenciaExecucao();";
		if ( !valTerminoVigenciaExecucao() ) throw "Erro: valTerminoVigenciaExecucao();";
		if ( !valDtTermino() ) throw "Erro: valDtTermino();";
		if ( !valObrasVinculadas() ) throw "Erro: valObrasVinculadas();";
		if(jQuery("#traterminovigencia").length) {
			if ( !valPrazoObra() ) throw "Erro: valPrazoObra();";
		}
		$('#form_aditivo').submit();		
	}catch(err){
		return false; 
	}

}

function habilitarCamposObra(obj) {
	
	if(obj.checked) {

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("className","obrigatorio normal");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("className","obrigatorio normal");

		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("readonly","");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("readonly","");
		
	} else {
	
		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("className","disabled");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("className","disabled");
		
		jQuery('#acrescimoprazoexecucao_'+obj.value).attr("readonly","readonly");
		jQuery('#acrescimovalorexecucao_'+obj.value).attr("readonly","readonly");

	}

}

function dataTerminoObraAditivo(obj){
	var table = jQuery( obj ).parents('tr')[0];
	
	var prExecucao = parseInt(jQuery( table ).find('[id*=ocrprazoexecucao]').val());
	var acrescPraz = parseInt(jQuery( table ).find('[id*=acrescimoprazoexecucao]').val());
	
	jQuery( table ).find('[id*=ocrnovoprazoexecucao]').val(prExecucao+acrescPraz);
	
	var dtInicio  = jQuery( table ).find('[id*=ocrdtinicioexecucao]').val();
	var diaPrazo  = jQuery( table ).find('[id*=ocrnovoprazoexecucao]').val();
	var dtTermino = ''; 

	if (dtInicio && diaPrazo){
		var obDt = new Data();
		dtTermino = obDt.dtAddDia(dtInicio, diaPrazo);
	}
	
	jQuery( table ).find('[id*=ocrnovodtterminoexecucao]').val( dtTermino );
}


function preencheValorAditivo( obj ){
	var calc = new Calculo(); 
	var table = jQuery( obj ).parents('tr')[0];
	
	var vlrExecucaoAnt      = parseFloat(replaceAll(replaceAll(jQuery( table ).find('[id*=ocrvalorexecucao]').val(),'.',''),',','.'));
	var acrescVlrExecucao   = parseFloat(replaceAll(replaceAll(jQuery( table ).find('[id*=acrescimovalorexecucao]').val(),'.',''),',','.'));
	
	if(jQuery('#radio_opcao_trasupressao_N').attr('checked')) {
		var total = vlrExecucaoAnt+acrescVlrExecucao;
		jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
	} else {
	
		if(jQuery('#radio_opcao_trasupressao_S').attr('checked')) {
			var total = vlrExecucaoAnt-acrescVlrExecucao;
			jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',total.toFixed(2)));
		} else {
		
			if ( jQuery("[name^='trasupressao']").length ){
			
//				alert('Marque a opção Aditivo de Supressão');
//				jQuery( table ).find('[id*=acrescimovalorexecucao]').val('');
//				return false;
			  
			  
			} else {
			
				jQuery( table ).find('[id*=ocrnovovalorexecucao]').val(mascaraglobal('###.###.###,##',vlrExecucaoAnt.toFixed(2)));
			
				
			}
		}
	
	}
	
	var vlrExecucao      = jQuery( table ).find('[id*=ocrnovovalorexecucao]').val();
	var qtdArea          = jQuery( table ).find('[id*=ocrqtdconstrucao]').val();
	var custoUnit        = jQuery( table ).find('[id*=ocrnovocustounitario]').val();
	var vlrCustoUnitario = '';
	
	if ( vlrExecucao != '' && qtdArea != '' ){
		vlrCustoUnitario = mascaraglobal('###.###.###,##', calc.operacao(vlrExecucao, qtdArea, '/') );
	}
	
	jQuery( table ).find('[id*=ocrnovocustounitario]').val(vlrCustoUnitario);

}
</script>
</body>
</html>