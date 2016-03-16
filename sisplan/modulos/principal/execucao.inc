<?php
	include APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	monta_titulo($titulo_modulo,"");

	$modelo = new UnidadeExecutora();

	// Filtros de Unidade
	if(!empty($_POST['uexid'][0]) && is_array($_POST['uexid'])){
		$where[] = " uex.uexid IN (".implode(',', $_POST['uexid']).")";
	}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="POST"  name="formulario" id="formulario">
	    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita" width="35%">
                 Unidades:
            </td>
            <td>
                <?php
                	if(!empty($_POST['uexid'][0]) && is_array($_POST['uexid'])){
						$uexid = $modelo->getDadosComboPopup($where);
					}
					$sqlCampos = $modelo->getSqlComboPopup($_SESSION['usucpf']);
                    combo_popup( 'uexid', $sqlCampos, 'Selecione as Unidades', '400x400', 0, array(), '', 'S', true, false, 10, 400, null, null, '', '', null, true, true, '', '' , '', array('descricao'));
                    echo obrigatorio();
                ?>
            </td>
       	</tr>
		  <tr bgcolor="#CCCCCC">
			   <td></td>
			   <td>
				   <input type="button" name="btnPesquisar" value='Pesquisar' onclick="pesquisarSubmit()" class="botao">
				   <input type="button" name="btnLimpar" value='Limpar' onclick="limpar()" class="botao">
			   </td>
		  </tr>
    	</table>
</form>

<?php

	$arConfig = array("style" => "width:95%;",
					  "totalLinha" => false,
					  "totalRegistro" => true);

	$arCabecalho = array('Ação',
						 'Sigla',
						 'Nome',
						 'Total Executado',
						 'Situação',
						 'Usuário ultima alteração',
						 'Data de alteração');

	$acao = '<center><img src=\'/imagens/editar_nome.gif\' style="cursor:pointer" title="Visualizar Execução" onclick="editar( {uexid} )"></center>';


	$rs = $modelo->getDadosListaExecucao($where ? $where : array());

	$oLista = new Lista($arConfig);
	$oLista->setCabecalho( $arCabecalho );
	$oLista->setCorpo( $rs, $arConfigCol );
	$oLista->setAcao( $acao );
	$oLista->show();
?>

<script type="text/javascript">
	function editar(uexid){
		location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/cadastrarExecucao&acao=A&uexid='+uexid;
	}

	function pesquisarSubmit(){
		selectAllOptions( formulario.uexid );
		$('#formulario').submit();
	}

	function limpar(){
		location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/execucao&acao=A';
	}
</script>