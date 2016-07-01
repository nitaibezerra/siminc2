<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . 'includes/workflow.php';
//include  APPRAIZ."includes/cabecalho.inc";
//print "<br/>";

$obRefor = new reformulacaoPTA( $db );

if($_REQUEST['carregaEspecificacao']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obRefor->listaEspecificacaoCadastradas($_POST['ptiid'], $_POST['ptrid'], true, true);
	exit;
}

if( empty($_REQUEST['ptrid']) && empty( $_SESSION["emenda"]["ptridReformulacao"] ) ){
	print "<script>"
		. "    alert('Falta o PTRID!');"
		. "    window.close();"
		. "</script>";
	
	die;
}

$ptrid = (!empty($_REQUEST['ptrid']) ? $_REQUEST['ptrid'] : $_SESSION["emenda"]["ptridReformulacao"]);
$_SESSION["emenda"]["ptridReformulacao"] = $ptrid;

$ptridPai = $db->pegaUm("SELECT ptridpai FROM emenda.planotrabalho where ptrid = $ptrid");

$db->cria_aba( $abacod_tela, $url, '');

echo cabecalhoPlanoTrabalho($ptrid, true);

echo '<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr bgcolor="#D0D0D0">
				<td align="left">
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarPTA( '. $ptridPai .' );">
					<a style="cursor:pointer;" onclick="visualizarPTA( '. $ptridPai .' );"> Visualizar PTA Original </a>
				</td>
				<td align="right">
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $_SESSION["emenda"]["ptridReformulacao"] .' );">
					<a style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $_SESSION["emenda"]["ptridReformulacao"] .' );"> Visualizar Histórico PTA </a>
				</td>
			</tr>
		  </table>';

$arIniciativa = $obRefor->carregaDadosIniciativa( $ptrid );
?>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script type="text/javascript">
function carregaEspecificacao(idImg, ptiid, ptrid, div_nome, tr_nome){	
	var img = document.getElementById( idImg );

	if($(div_nome).style.display == 'none'){
		$(div_nome).style.display = '';
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
		montaListaEspecificacao( ptiid, ptrid, div_nome );
	} else {
		$(div_nome).style.display = 'none';
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function montaListaEspecificacao(ptiid, ptrid, div_nome){
	var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacaoPTA&acao=A', {
			        method:     'post',
			        parameters: '&carregaEspecificacao=true&ptiid='+ptiid+'&ptrid='+ptrid,
			        asynchronous: false,
			        onComplete: function (res){
						$(div_nome).innerHTML = res.responseText;
			        }
			  });
}

function editarCronograma(ptrid){
	window.open('emenda.php?modulo=principal/cronogramaDesembolsoReformulacao&acao=A&ptrid='+ptrid,'pagecronograma','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=550, width=1110');	
}

function manterDadosEspecificacao( ptiid, ptrid, iniid ){
	window.open('emenda.php?modulo=principal/insereEspecificacaoReformulacaoPTA&acao=A&ptiid='+ptiid+'&ptrid='+ptrid+'&iniid='+iniid,'especificacao','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=700, width=1210');}
function editarEscolaBeneficiada( ptrid ){
	window.open('emenda.php?modulo=principal/cadastrarEscolaBeneficiadaReformulacao&acao=A&ptrid='+ptrid,'pagebeneficiada','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1210');
}
</script>

<form id="formReformulacao" name="formReformulacao" method="post" action="">

<table width='100%'>
<tr>
<td>
	<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tbody>
	<tr>
		<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Iniciativa</td>
	</tr>
 	</tbody>
<tr><td>
	<table class="listagem" width="100%">
	<thead>
	<tr style="background-color: #cccccc;">
		<td class="SubTitulocentro" width='5%'></td>
		<td class="SubTitulocentro" align='center' width='35%'>Iniciativa a ser executada</td>
		<td class="SubTitulocentro" align='center' width='30%'>Beneficiários</td>
		<td class="SubTitulocentro" align='center' width='10%'>Total</td>
		<td class="SubTitulocentro" align='center' width='10%'>Total do Proponente</td>
		<td class="SubTitulocentro" align='center' width='10%'>Total do Concedente</td>
	</tr>
	</thead>
	<tbody>
<?php
	$html = '';
	foreach( $arIniciativa as $acoes ){
	 $html.= '	<tr id="tr_'.$key.'">
				<td><img src="/imagens/mais.gif" border="0" id="img_dimensao_'.$acoes['codigo'].'" onClick="carregaEspecificacao(this.id, '.$acoes['codigo'].', '.$ptrid.', \'listaEspecificacao_'.$acoes['codigo'].'\', \'trV_'.$acoes['codigo'].'\');"> '.$acoes['acao'].'</td>
				<td>'.$acoes['descricao'].'</td>';
	 	$html.= "<td><div id='lista'>".$obRefor->ListarbeneficiariosAcaoPlanoTrabalho($acoes['codigo'])."</div>";
	 $html.= "</td>
				<td align='right'>".number_format($acoes['ptivalortotal'],2,',','.')."</td>";
	 	$html.="	<td align='right'>".number_format($acoes['ptivalorproponente'],2,',','.')."</td>";
	 $html.='	<td align="right">'.number_format($acoes['ptivalorconcedente'],2,',','.').'</td>
	 		<tr>
	 		<tr id="trV_'.$acoes['codigo'].'" style="display: none">
							  	   <td colspan="5"><div id="listaEspecificacao_'.$acoes['codigo'].'" style="display: none"></div></td>
							  	</tr>';
	 	$valorTotalIniciativas += $acoes['ptivalortotal'];
	 	$valorTotalConcedente += $acoes['ptivalorconcedente'];
	 	$valorTotalProponente += $acoes['ptivalorproponente'];
	}
	echo $html;
?>
			<tr>
				<td colspan="2" align="center"><b>Totalizador</b></td>
				<td><?//=$obRefor->totalizaBeneficiariosPlanoTrabalho( $ptrid );?></td>
				<td align='right'>
					<font color='0000FF'><b>R$ <?=number_format($valorTotalIniciativas,2,',','.');?></b></font>
	 			</td>
				<td align='right'>
					<b>R$ <?=number_format($valorTotalProponente,2,',','.');?></b>
	 			</td>
				<td align='right'>
					<b>R$ <?=number_format($valorTotalConcedente,2,',','.');?></b>
	 			</td>
	 		<tr>
	</tbody>
	</table>

</td></tr>
</table>
<?
$obCronograma = new CronogramaExecucao();
$obCronograma->ptrid = $ptrid;
echo $obRefor->montaListaCronogramaDesembolso( $ptrid, $obCronograma, true );
 ?>
	<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tbody>
			<tr>
				<td style="text-align: center;" colspan="4"><input type="button" value="Editar Cronograma de Desembolso" id="btncronograma" name="btncronograma" onclick="editarCronograma(<?=$ptrid ?>)"></td>
			</tr>
		<?php
			$obValida = new ValidacaoReformulacaoPTA( $ptrid );
			if( $obValida->validaInsereEscolasBeneficiario() <> 0 ){
		?>
			<tr>
				<td>&nbsp;</td>
			</tr>
			<tr>
		 		<td class="subtituloesquerda" colspan="4" style="background-color: #d0d0d0;">Escolas Beneficiadas</td>
		 	</tr>
			<tr>
				<td colspan="4">
						 	<?
						 	$sql = "SELECT DISTINCT
								    ent.entcodent,
								    ent.entnome, 
								    peb.esbquantidadealunos
								FROM
									emenda.ptescolasbeneficiadas peb
								    inner join entidade.entidade ent
								    	on ent.entid = peb.entid
								    inner join entidade.funcaoentidade fent
								        ON fent.entid = ent.entid
								WHERE
									ent.entstatus = 'A'
								    and fent.funid in(3,4,11,12,16,18,56) 
								    and peb.ptrid = $ptrid";
						 	
						 	$cabecalho = array("Código Censo Escolar - INEP", "Nome da Escola", "Alunos Beneficiados");
							$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 20, 4, 'N','Center','','form', $tamanho, $alinhamento);
						 	
							/*foreach ($arEscola as $k => $v) {
								echo '<tr>
								 		<td>'.$v['entcodent'].'</td>
								 		<td>'.$v['entnome'].'</td>
								 		<td>'.$v['esbquantidadealunos'].'</td>
								 	</tr>';
							}*/							
						 	?>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="4"><input type="button" value="Editar Escola Beneficiadas" name="btedita" id="btedita" onclick="editarEscolaBeneficiada(<?=$ptrid ?>);"></td>
				</tr>
				<? } ?>
			</table>
		</td>
	</tr>
	</tbody>   
</form>
</body>