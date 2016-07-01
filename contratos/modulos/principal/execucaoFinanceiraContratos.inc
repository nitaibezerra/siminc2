<?php 
include_once APPRAIZ."includes/classes/Modelo.class.inc";
include_once APPRAIZ."contratos/classes/FaturaContrato.class.inc";

if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe'])
{
	$n = new $_REQUEST['classe']();
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}

if($_REQUEST['requisicao'] && $_REQUEST['classe'])
{
	$n = new $_REQUEST['classe']();
	$n->$_REQUEST['requisicao']();
}
//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();

// Por padrão os campos vem habilitados
// $desabilitado   = true;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE, PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_GESTOR_FINANCEIRO_UNIDADE));
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {

// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
cabecalhoContrato( $_SESSION['ctrid'] );
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoContrato($_SESSION['ctrid']);

//pega valor empenho para sobrepor o valor do contrato
/*
$rsContratosVinculados = $db->carregarColuna("select trim(numempenho) as numempenho from contratos.empenhovinculocontrato where ctrid={$_SESSION['ctrid']}");
if($rsContratosVinculados) {
	$vlr_empenho = pegaValorPorEmpenhos($rsContratosVinculados);
}
*/

?>
<style>
	.link{cursor:pointer}
	.numeric{text-align:right;color:#2A66D4}
	.red{color:red}
	.bold{font-weight:bold}
	.center{text-align:center}
	.right{text-align:right}
	.left{text-align:left}
	.espaco_td{padding-left:10px}
	.espaco_td_total{padding-right:10px}
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

	function wf_exibirHistorico( docid )
	{
		var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}
	
	function abreOrdemBancaria(obj,epcid)
	{
		if(!$("#td_epcid_"+epcid).html()){
			var tr = $(obj).parent().parent();
			var html = "<tr id=\"tr_epcid_"+epcid+"\" ><td id=\"td_epcid_"+epcid+"\"  colspan=6  ></td></tr>";
			$(tr).closest('tr').after(html);
			$("#td_epcid_"+epcid).html("<center>Carregando...</center>");
			$(obj).attr("src","../imagens/menos.gif");
			$.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=listaOrdemBancaria&classe=FaturaContrato&epcid="+epcid,
			   success: function(msg){
				   $("#td_epcid_"+epcid).html(msg);
			   }
			 });
		}else{
			if($(obj).attr("src") == "../imagens/mais.gif"){
				$("#tr_epcid_"+epcid).show();
				$(obj).attr("src","../imagens/menos.gif");
			}else{
				$("#tr_epcid_"+epcid).hide();
				$(obj).attr("src","../imagens/mais.gif");
			}
		}
	}
	function abreNotaFiscal(obj,orbid)
	{
		if(!$("#td_orbid_"+orbid).html()){
			var tr = $(obj).parent().parent();
			var html = "<tr id=\"tr_orbid_"+orbid+"\" ><td id=\"td_orbid_"+orbid+"\"  colspan=6  ></td></tr>";
			$(tr).closest('tr').after(html);
			$("#td_orbid_"+orbid).html("<center>Carregando...</center>");
			$(obj).attr("src","../imagens/menos.gif");
			$.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=listaNotaFiscal&classe=FaturaContrato&obfid="+orbid,
			   success: function(msg){
				   $("#td_orbid_"+orbid).html(msg);
			   }
			 });
		}else{
			if($(obj).attr("src") == "../imagens/mais.gif"){
				$("#tr_orbid_"+orbid).show();
				$(obj).attr("src","../imagens/menos.gif");
			}else{
				$("#tr_orbid_"+orbid).hide();
				$(obj).attr("src","../imagens/mais.gif");
			}
		}
	}
	var caminho_atual = "contratos.php";
/*	
	function popUpEmpenho(numempenho,cnpj)
	{
		return windowOpen( caminho_atual + '?modulo=principal/popUpFatura&acao=A&requisicao=popUpEmpenho&numempenho='+numempenho+'&cnpj='+cnpj,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
	
	function popUpOB(ob,numempenho)
	{
		return windowOpen( caminho_atual + '?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho='+numempenho+'&ob='+ob,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
*/	
	function popUpFatura(ftcid,cnpj)
	{
		return windowOpen( caminho_atual + '?modulo=principal/popUpFatura&acao=A&requisicao=popUpFatura&ftcid='+ftcid+'&cnpj='+cnpj,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}

	function addNovaNota()
	{
		window.location='?modulo=principal/addNotaContratos&acao=A';
	}

	function editarFaturamento(ftcid)
	{
		window.location='?modulo=principal/addNotaContratos&acao=A&ftcid='+ftcid;
	}
	function excluirFaturamento(ftcid)
	{
		if(confirm("Deseja realmente excluir este Pagamento?")){
			window.location='?modulo=principal/execucaoFinanceiraContratos&acao=A&classe=FaturaContrato&requisicaoAjax=excluirFaturamento&ftcid='+ftcid;
		}
	}
	
</script>

<form name=formulario id=formulario method=post >
	<input type="hidden" name="requisicao" id="requisicao" value=""> 
	<input type="hidden" name="classe" id="classe" value=""> 
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Buscar:</td>
	        <td>
        		<?php
					$arrAtributos 					= false;
					$arrAtributos['name'] 			= "pesquisar";
					$arrAtributos['obrigatorio'] 	= false;
					$arrAtributos['habilitado'] 	= true;
					$arrAtributos['size'] 			= 60;
					$arrAtributos['maxsize'] 		= 60;
					$arrAtributos['align'] 			= "left";
					$arrAtributos['value'] 			= $_POST['pesquisar'];
					echo campo_texto($arrAtributos)
				?>
	        </td>      
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' ></td>
	        <td>
	        	<input type="submit" name="btn_buscar" value="Buscar" >
<?php 
        	if( $_POST['pesquisar'] ):
?>
	        	<input type="button" name="btn_novo" value="Ver Todas" onclick="window.location=window.location" >
<?php 
        	endif;

//             $perfil = pegaPerfilGeral();

//             if (in_array(PERFIL_FISCAL_CONTRATO, $perfil))
//                 $desabilitado = false;

        	if ( !$desabilitado ):
?>
	        	<input type="button" name="btn_novo" value="Novo pagamento" onclick="addNovaNota()" >
<?php 
			endif;
?>
	        </td>
	    </tr> 
	</table>
</form>
<?php 
$fatura   = new FaturaContrato();
$arrDados = $fatura->listaExecucaoFinanceira2($_SESSION['ctrid'], $_POST);


 
// $sql = "select
//     				case when esd.esdid = ".ESTADO_WK_FATURAMENTO_EM_CADASTRAMENTO." AND 'S' = '". $somenteLeitura ."'
//     				then '<img  src=\"../imagens/alterar.gif\" class=\"link\" align=absmiddle onclick=\"editarFaturamento(' || ftc.ftcid || ')\"  /> <img  src=\"../imagens/excluir.gif\" class=\"link\" align=absmiddle onclick=\"excluirFaturamento(' || ftc.ftcid || ')\"  /> <img style=\"cursor: pointer;\" align=absmiddle src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''|| doc.docid ||'\' );\">'
//     				else  '<img  src=\"../imagens/consultar.gif\" class=\"link\" align=absmiddle onclick=\"editarFaturamento(' || ftc.ftcid || ')\"  /> <img style=\"cursor: pointer;\" align=absmiddle src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''|| doc.docid ||'\' );\">'
//     				end as acao,



?>
<div style="width:95%;text-align:center;margin-left: auto; margin-right: auto;margin-top:5px" >
	<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
		<thead>
			<td width="10%" class="bold center" >Ação</td>
			<td width="10%" class="bold center" >Número da NF</td>
			<td width="10%" class="bold center" >Data da NF</td>
			<td width="10%" class="bold center" >Situação da NF</td>
			<!-- 
			<td width="15%" class="bold center" >Responsável</td>
			-->
			
			<td width="15%" class="bold center" valign="top">
				(A)
				<br>
				Valor da NF - Glosa
			</td>
			<td width="15%" class="bold center" valign="top">
				(B)
				<br>
				Soma das OBs da NF
			</td>
			<td width="15%" class="bold center" valign="top">
				(C)
				<br>
				Deduções Legais
				<br>
				(C) = (A) - (B)
			</td>
			<td width="15%" class="bold center" valign="top">
				(D)
				<br>
				Saldo do Contrato
				<br>
				<?php
				echo ($arrDados ? "<font size='1'>(Valor total: R$" . number_format($arrDados[0]['valorcontrato'], 2, ',', '.').")</font>" : "");
				?>
				<br>
				(D) = (valor do contrato) - (A)				
			</td>
		</thead>
	</table>
</div>
<div style="width:95%;text-align:center;margin-left: auto; margin-right: auto; <?php echo count($arrDados) > 10 ? "overflow-y: scroll; height: 270px" : ""?>" >
	<table class="" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
<?php

		if($arrDados):
			$totalNf  = 0;
			$totalOb  = 0;
			$totalDed = 0;
			
			foreach($arrDados as $n => $dado): 
				if ( empty($dado['ftcid']) ){
					$dado['ftcvalor'] 		= "Não informado";
					$dado['ftcdataemissao'] = "Não informado";
					$dado['ftcnumero'] 		= "Não informado";
					$dado['esddsc'] 		= "Não informado";
// 					$dado['usunome'] 		= "Não informado";
				}

				$obfdeducao  = (($dado['ftcvalor'] > 0 && $dado['obfvalortotal'] > 0) ? ($dado['ftcvalor'] - $dado['obfvalortotal']) : '-');
				$totalNf 	+= $dado['ftcvalor'] - $dado['ftcglosa'];
				$totalOb 	+= $dado['obfvalortotal'];
				$totalDed 	+= (is_numeric($obfdeducao) ? $obfdeducao : 0);
				
				$cor = ($n%2 ? "#f7f7f7" : "");
?>
			<tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';">
				<td width="10%" class="espaco_td right" >
<?php 
			if ( $dado['ftcid'] ):
				if ( $dado['esdid'] == ESTADO_WK_FATURAMENTO_EM_CADASTRAMENTO && $desabilitado == false ){
?>					
					<img  src="../imagens/alterar.gif" class="link" align="absmiddle" onclick="editarFaturamento(<?php echo $dado['ftcid']?>)"  /> 
					<img  src="../imagens/excluir.gif" class="link" align="absmiddle" onclick="excluirFaturamento(<?php echo $dado['ftcid']?>)"  /> 
<?php 
				}else{
?>
					<img  src="../imagens/consultar.gif" class="link" align="absmiddle" onclick="editarFaturamento(<?php echo $dado['ftcid']?>)"  /> 
<?php
				}
?>				
					<img src="../imagens/fluxodoc.gif" onclick="wf_exibirHistorico('<?php echo $dado['docid']?>');" title="Histórico" align="absmiddle" style="cursor: pointer;">
<?php					
			endif;
?>				
				
				</td>
				<td width="10%" align="right" class="<?php echo ($dado['ftcid'] ? 'espaco_td' : 'espaco_td red'); ?>" >
					<a href="javascript:popUpFatura('<?php echo $dado['ftcid'] ? $dado['ftcid'] : 'NI' ?>','<?php echo $dado['cnpj'] ?>')" style="<?php echo ($dado['ftcid'] ? '' : 'color:red;'); ?>">
						<?php echo ($dado['ftcnumero'])?>
					</a>
				</td>
				<td width="10%" align="center" class="<?php echo ($dado['ftcid'] ? 'espaco_td' : 'espaco_td red'); ?>" >
					<?php echo ($dado['ftcdataemissao'] && $dado['ftcid'] ? formata_data( $dado['ftcdataemissao'] ) : $dado['ftcdataemissao'])?>
				</td>
				<td width="10%" align="center" class="<?php echo ($dado['ftcid'] ? 'espaco_td right' : 'right red'); ?>">
					<?php echo ($dado['esddsc'])?>
				</td>
				<!--  
				<td width="15%" align="center" class="<?php //echo ($dado['ftcid'] ? 'espaco_td right' : 'right red'); ?>">
					<?php //echo ($dado['usunome'])?>
				</td>
				-->
				</td>
				<td width="15%" class="<?php echo ($dado['ftcid'] ? 'numeric' : 'right red'); ?>">
					<?php echo ($dado['ftcvalor'] > 0 ? number_format(($dado['ftcvalor']-$dado['ftcglosa']), 2, ',','.') : 'Não informado') ?>
				</td>
				<td width="15%" class="espaco_td numeric" >
					<span>
					<?php echo ($dado['obfvalortotal'] ? number_format($dado['obfvalortotal'], 2, ',', '.') : "-");?>
					</span>
				</td>
				<td width="15%" class="espaco_td numeric" >
					<span class="numeric">
					<?php echo (is_numeric($obfdeducao) ? number_format($obfdeducao, 2, ',', '.') : "-");?>
					</span>
				</td>
				<td width="25%" class="espaco_td numeric" >
					<?php 
// 					$soma_fatura += $dado['obfvalortotal'];
					$soma_fatura += (is_numeric( $dado['ftcvalor'] ) ? $dado['ftcvalor'] : 0);
					$sobra 		  = $dado['valorcontrato'] - $soma_fatura;
					
					if($sobra < 0): 
					?>
						<span class="red" ><?php echo $sobra ? number_format($sobra, 2, ',', '.') : "-";?></span>
					<?php else: ?>
						<?php echo $sobra ? number_format($sobra, 2, ',', '.') : "0,00";?>
					<?php endif; ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else: ?>
		<tr>
			<td colspan="7" align="center">Não foram encontrados registros.</td>
		</tr>
	<?php endif; ?>
	</table>
</div>
<?php if($arrDados): ?>
	<div style="width:95%;text-align:center;margin-left: auto; margin-right: auto;" >
		<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
			<thead>
				<td width="40%" class="bold" >Total de registros: <?php echo count($arrDados)?></td>
				<td width="15%" class="numeric bold"><?php echo number_format($totalNf, 2, ',', '.')?></td>
				<td width="15%" class="numeric bold" >
				<?php echo (is_numeric( $totalOb ) ? number_format($totalOb, 2, ',', '.') : '-'); ?>
				</td>
				<td width="15%" class="numeric bold" >
				<?php echo (is_numeric( $totalDed ) ? number_format($totalDed, 2, ',', '.') : '-'); ?>
				</td>
				<td width="15%" class="bold right">
					Saldo restante do Contrato:<br>
					<font class="numeric">
					<?php echo $sobra ? "R$ " . number_format($sobra, 2, ',', '.') : "0,00"; ?>
					</font>
					<?php // if($sobra < 0): ?>
						<!--  
						<span class="red" >(Saldo restante do Contrato) <?php // echo $sobra ? number_format($sobra, 2, ',', '.') : "";?></span>
						-->
					<?php // else: ?>
						<!--  
						(Saldo restante do Contrato) <?php // echo $sobra ? number_format($sobra, 2, ',', '.') : "";?>
						-->
					<?php // endif; ?>
					<?php // echo count($arrDados) > 10 ? "&nbsp;&nbsp;&nbsp;&nbsp;" : ""?>
				</td>
			</thead>
		</table>
	</div>
<?php endif; ?>

<?php if($_SESSION['administrativo']['contratos']['notafiscal']['alert']): ?>
<script>
	alert('<?php echo $_SESSION['administrativo']['contratos']['notafiscal']['alert'] ?>');
	<?php unset($_SESSION['administrativo']['contratos']['notafiscal']['alert']) ?>
</script>
<?php endif;?>