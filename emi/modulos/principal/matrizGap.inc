<?php
 
switch ($_REQUEST["requisicao"]) {
	
	case "salvaDados" :
		$emi->cadastraMatrizGap( $_REQUEST );
	
	break;
	
	case "alteraritemGap":
		echo $emi->buscaDadosItemGap( $_REQUEST["mdoid"] );
		die;
	break;
	
	case "excluiritemGap" :
		$emi->excluirItemGap( $_REQUEST["mdoid"] );
	break;
	
	case "salvaBeneficiarios" :
		$emi->salvaBeneficiarios( $_REQUEST );
	break;
	
}

if(!$_REQUEST["papid"] && !$_SESSION["emi"]["papid"]){
	print "<script>
				  alert('O PRC selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
}

if ( $_REQUEST["papid"] || $_SESSION["emi"]["papid"] ){

	$_SESSION["emi"]["papid"] = !empty($_REQUEST["papid"]) ? $_REQUEST["papid"] : $_SESSION["emi"]["papid"];
	$papid = $_SESSION["emi"]["papid"];
	
	if ( !emiVerificaGap( $papid ) ){
		
		unset( $_SESSION["emi"]["papid"] );
		
		print "<script>
				  alert('O PRC selecionado não existe ou está em branco!');
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

if( $_SESSION["emi"]["emeidEscola"] ){
	$emi->montaCabecalho( $_SESSION["emi"]["emeidEscola"], "escola" );
}
if( $_SESSION["emi"]["papid"] ){
	echo '<table class="tabela" bgcolor="#DCDCDC" align="center" eight="100%" cellspacing="1" cellpadding="3" ><tr><td align="center" ><b>Dados do PRC</b></td></tr></table>';
	$emi->montaCabecalho( $_SESSION["emi"]["papid"], "macrocampo" );
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" name="emiSelecionaEscolas" id="emiSelecionaEscolas" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="salvaBeneficiarios"/>
	<input type="hidden" id="papid" name="papid" value="<?=$papid?>"/>
	<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td bgcolor="#e9e9e9" align="center" colspan="7" "><b>Beneficiários <br /> Número de Alunos Beneficiários</b></td>
		</tr>
		<tr>
			<td colspan="7" ">
				<table bgcolor="#f5f5f5" height="100%" width="100%" cellspacing="1" cellpadding="3" align="center" >
					<tr bgcolor="#e9e9e9">
						<td colspan="3" width="30%" align="center" ><b>1º Ano</b></td>
						<td colspan="3" width="30%" align="center" ><b>2º Ano</b></td>
						<td colspan="3" width="30%" align="center" ><b>3º Ano</b></td>
						<td width="10%" rowspan="2" align="center" ><b>Total</b></td>
					</tr>
					<tr>
						<td align="center" >Mat.</td>
						<td align="center" >Vesp.</td>
						<td align="center" >Not.</td>
						<td align="center" >Mat.</td>
						<td align="center" >Vesp.</td>
						<td align="center" >Not.</td>
						<td align="center" >Mat.</td>
						<td align="center" >Vesp.</td>
						<td align="center" >Not.</td>
					</tr>
					<?php $arrBen = $emi->pegaBeneficiarios($papid); 
					if($arrBen){
						foreach($arrBen as $column => $ben){
							if($column != "benid"){
							$column = str_replace(array("benqtd","ano"),array("",""),$column);
							$ano = substr($column,0,1);
							$campo = str_replace($ano,"",$column)."_".$ano;
							${$campo} = number_format($ben,"",2,".");
							$total += $ben;
							}
						}
					}
					
					?>
					<tr>
						<td align="center" ><?php echo campo_texto("mat_1","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
						<td align="center" ><?php echo campo_texto("vesp_1","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
						<td align="center" ><?php echo campo_texto("not_1","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
						<td align="center" ><?php echo campo_texto("mat_2","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
						<td align="center" ><?php echo campo_texto("vesp_2","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
						<td align="center" ><?php echo campo_texto("not_2","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
						<td align="center" ><?php echo campo_texto("mat_3","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
						<td align="center" ><?php echo campo_texto("vesp_3","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
						<td align="center" ><?php echo campo_texto("not_3","N","S","","15","20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
						<td bgcolor="#e9e9e9" align="center" id="td_total_beneficiarios" ><?php echo number_format($total,"",2,".") ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td bgcolor="#e9e9e9" align="center" colspan="7" "><input type="button" name="btn_salvar" value="Salvar" onclick="salvarBeneficiarios()"  /></td>
		</tr>
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
				$estadoAtual = emiPegarEstadoAtual( $_SESSION["emi"]["emiid"], "G" );
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
		
		<?php emiListaMatrizGap( $papid, $_REQUEST["acao"], $disCadastro ); ?>
		
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