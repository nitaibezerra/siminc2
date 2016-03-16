<?php
	//guardando o id da unidade
	$uexid 					= $_REQUEST['uexid'];
	$modeloUnidadeExecutora = new UnidadeExecutora( $uexid );
	$dadosUnidade			= $modeloUnidadeExecutora->getDados();

	//recuperando todos os PTRES
	$modeloPtresDespesa = new PtresDespesa();
	$dadosPtres 		= $modeloPtresDespesa->getByWhere(array("ptr.ptrid = {$_REQUEST['ptrid']}"),$uexid);

	$modeloNatureza		= new PtresNatureza();
	$dadosNatureza		= $modeloNatureza->getByWhere(array("ptn.ptnid = {$_REQUEST['ptnid']}"));
	$modeloReferencia	= new ReferenciaDespesa($_REQUEST['rfdid']);
	$dadosReferencia    = $modeloReferencia->getDados();

	$modeloExecucao		= new DespesaExecucao();
	$despesaExecucao	= $modeloExecucao->getDespesaByWhere(array("ptnid = {$_REQUEST['ptnid']}"," uexid = {$_REQUEST['uexid']}"," ptrid = {$_REQUEST['ptrid']}"," rfdid = {$_REQUEST['rfdid']}"));
	if($despesaExecucao){
		extract($despesaExecucao);
		$dpevalor = formata_valor($dpevalor);
	}
?>

<html>
    <head>
        <title>Inserir Intervenções</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <body>
    <form method="POST"  name="formulario" id="formulario">
    	<input type="hidden" id="act" name="act" value="" />
    	<input type="hidden" id="ptrid" name="ptrid" value="<?php echo $_REQUEST['ptrid']; ?>" />
    	<input type="hidden" id="ptnid" name="ptnid" value="<?php echo $_REQUEST['ptnid']; ?>" />
    	<input type="hidden" id="rfdid" name="rfdid" value="<?php echo $_REQUEST['rfdid']; ?>" />
        <table border="0" cellspacing="1" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
            <tr>
                <td width="100%" align="center">
                    <label class="TituloTela" style="color: #000000;">
                        Dados Execução
                    </label>
                </td>
        </table>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:100%;">
	        <tr>
	            <td align='right' class="SubTituloDireita">
	            	Unidade
	            </td>
	            <td bgcolor="#EDEAEA">
					<?php echo $dadosUnidade['descricao']; ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	PTRES
	            </td>
	            <td bgcolor="#EDEAEA">
					<?php echo $dadosPtres[0]['ptres']." - ".$dadosPtres[0]['funcional']; ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Natureza
	            </td>
	            <td bgcolor="#EDEAEA">
					<?php echo $dadosNatureza[0]['natureza']; ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Referência
	            </td>
	            <td bgcolor="#EDEAEA">
					<?php echo $dadosReferencia['rfdnome']; ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Valor Executado:
	            </td>
	            <td>
					<?php echo campo_texto( "dpevalor", 'S','S','', 15, 15, '###.###.###.###,##', '', 'left', '', '', "id='dpevalor'"); ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Número do Documento:
	            </td>
	            <td>
					<?php echo campo_texto( "dpenudocumento", 'S','S','', 28, 50, '', '', '', '', '', "id='dpenudocumento'"); ?>
	            </td>
	       	</tr>
	        <tr>
	            <td align='right' class="SubTituloDireita" width="35%">
	            	Observações:
	            </td>
	            <td>
	            	<?php echo campo_textarea('dpeobservacoes', 'S', 'S', '', 40, 4, 500,'','','' ); ?>
	            </td>
	       	</tr>
			  <tr bgcolor="#CCCCCC">
				   <td></td>
				   <td>
					   <input type="button" name="btnSalvar" value='Salvar' onclick="salvar()" class="botao">
					   <input type="button" name="btnCancelar" value='Cancelar' onclick="javascript: window.close();" class="botao">
				   </td>
			  </tr>
			</table>
		</form>
    </body>
</html>

<script type="text/javascript">
	function salvar(){
        if (!validaBranco(document.formulario.dpevalor, 'Valor Executado')) return;
        if (!validaBranco(document.formulario.dpenudocumento, 'Número do Documento')) return;
        if (!validaBranco(document.formulario.dpeobservacoes, 'Observações')) return;

        $('#act').val('salvar');
        $('#formulario').submit();
	}
</script>