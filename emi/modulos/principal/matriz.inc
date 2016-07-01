<?php
 
switch ($_REQUEST["requisicao"]) {
	
	case "salvaDados" :
		$emi->cadastraMatriz( $_REQUEST );
	
	break;
	
	case "alteraritem":
		echo $emi->buscaDadosItem( $_REQUEST["mdoid"] );
		die;
	break;
	
	case "excluiritem" :
		$emi->excluirItem( $_REQUEST["mdoid"] );
	break;
	
}

if ( $_REQUEST["papid"] || $_SESSION["emi"]["papid"] ){

	$_SESSION["emi"]["papid"] = !empty($_REQUEST["papid"]) ? $_REQUEST["papid"] : $_SESSION["emi"]["papid"];
	$papid = $_SESSION["emi"]["papid"];
	
	if ( !emiVerificaPap( $papid ) ){
		
		unset( $_SESSION["emi"]["papid"] );
		
		print "<script>
				  alert('O PAP selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
	}
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo("Matriz de Distribuição Orçamentária / Ação-Atividade", "");

if( $_SESSION["emi"]["papid"] ){
	
	$tipoCabecalho = $_REQUEST["acao"] == "A" ? "matriz" : "matrizsecretaria";
	$emi->montaCabecalho( $_SESSION["emi"]["papid"], $tipoCabecalho );
	
}else{
	
	print "<script>
			  alert('O PAP selecionado não existe ou está em branco!');
			  history.back(-1);
		  </script>";
	die;
	
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="emiSelecionaEscolas" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="salvaDados"/>
	<input type="hidden" id="papid" name="papid" value="<?=$papid?>"/>
	<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="subtitulocentro" rowspan="2">Itens Financiáveis</td>
			<td class="subtitulocentro" rowspan="2">Especificação</td>
			<td class="subtitulocentro" rowspan="2">Unidade</td>
			<td class="subtitulocentro" rowspan="1" colspan="3">Estimativa Orçamentária</td>
			<td class="subtitulocentro" rowspan="2">Ação</td>
		</tr>
		<tr>
			<td class="subtitulocentro">Quantidade <br/> A </td>
			<td class="subtitulocentro">Valor Unitário (R$) <br/> B </td>
			<td class="subtitulocentro">Total (R$) <br/> A x B </td>
		</tr>
		<tr>
			<td align="center">
				<?php 
					
					$sql = "SELECT
								itfid as codigo, 
								itfdsc as descricao
							FROM
								emi.emitemfinanciavel
							ORDER BY
								descricao";
					
					$db->monta_combo("itfid", $sql, $emiSomenteLeitura, "Selecione...", '', '', '', 150, 'N', 'itfid');
					
				?>
			</td>
			<td>
				<?php echo campo_textarea( 'mdoespecificacao', 'N', $emiSomenteLeitura, '', 60, 4, 1000, '', 0, ''); ?>
			</td>
			<td>
				<?php 
					
					$sqlUnidadeMedida = "SELECT
											unddid as codigo,
											undddsc as descricao
										 FROM
										 	cte.unidademedidadetalhamento 
										 ORDER BY
										 	undddsc";
						
					$db->monta_combo("undid", $sqlUnidadeMedida, $emiSomenteLeitura, "Selecione...", '', '', '', 150, 'N', "undid" );
				?>
			</td>
			<td align='center'>
				<?php echo campo_texto( 'mdoqtd', 'N', $emiSomenteLeitura, '', 10, 8, '########', '', 'left', '', 0, "id='mdoqtd'","", "", "preencheTotal();" ); ?>
			</td>
			<td align='center'>
				<?php echo campo_texto( 'mdovalorunitario', 'N', $emiSomenteLeitura, '', 16, 14, '###.###.###,##', '', 'left', '', 0, "id='mdovalorunitario'", "", "", "preencheTotal();"); ?>
			</td>
			<td align='center'>
				<?php echo campo_texto( 'mdototal', 'N', 'N', '', 16, 14, '###.###.###,##', '', 'left', '', 0, "id='mdototal'" ); ?>
			</td> 
			<?php 
			
			// testa se eh cadastrador e o estado do documento
			if( in_array(EMI_PERFIL_CADASTRADOR, emiRecuperaPerfil()) )
			{
				$estadoAtual = emiPegarEstadoAtual( $_SESSION["emi"]["emiid"] );
				//unset($_SESSION["emi"]["emiid"]);
				if( $estadoAtual == EMI_NAO_INICIADO || $estadoAtual == EMI_EM_PRENCHIMENTO || $estadoAtual == EMI_CORRECAO_APROVADOS )
				{
					$disCadastro = false;

				}
				else
				{
					$disCadastro = true;
				}
			}
			else
			{
				$disCadastro = false;
			}

			?>
			<td align="center">
					<input type="hidden" value="" name="mdoid" id="mdoid"/>
				<?php if( $emiHabilitado && !$disCadastro ){ ?>
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereDadosMatriz();">
					<img src="/imagens/excluir.gif" style="cursor: pointer;" onclick="limpaDadosMatriz();">
				<?php }else{ ?>
					<img src="/imagens/gif_inclui_d.gif">
					<img src="/imagens/excluir_01.gif">
				<?php } ?>
			</td> 
		</tr>
		
		<?php emiListaMatrizPap( $papid, $_REQUEST["acao"], $disCadastro ); ?>
		
		<tr bgcolor="#C0C0C0">
			<td colspan="1">
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
			<td colspan="4" style="text-align:right;">
				<b>Total:</b>
			</td>
			<td colspan="1" style="text-align:right;">
				<b>R$ <?=number_format($totalGeralAxB, 2, ",", ".")?></b>
			</td>
			<td colspan="1"></td>
		</tr>
	</table>
</form>