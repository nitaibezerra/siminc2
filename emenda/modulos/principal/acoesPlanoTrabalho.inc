<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

$obPTA->verificaPermissao();

$obPTA->verificaExistePTA( $_SESSION["emenda"]["ptrid"] );
$obPTA->validaSessionPTA( $_SESSION["emenda"]["ptrid"] );
$obPTA->validaSessionPTA( $_SESSION['emenda']['federal'] );

$enbid = $_SESSION["emenda"]["enbid"];

$emendaFederal = verificaEmendaFederal($_SESSION["emenda"]["ptrid"]);
if( $emendaFederal ){
	echo "<script>window.location.href='emenda.php?modulo=principal/orcamentoImpositivoPar&acao=A';</script>";
	exit();
}
unset($_SESSION["emenda"]["ptiid"]);
unset($_SESSION["emenda"]["iniid"]);

//Excluir ação
if ( ($_REQUEST['evento']=='E') && !empty( $_REQUEST['ptiid'] ) ){
	$retorno = $obPTA->deletaIniciativaPTA( $_REQUEST['ptiid'] );
	
	if( $retorno ){
		echo "<script>alert('Iniciativa excluída com sucesso!');
				 window.location.href='emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A';</script>";
	} else {
		echo "<script>alert('Erro na exclusão da iniciativa!');</script>";
	}
	break;
	
}
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
$estadoAtual = $obPTA->pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

echo $obPTA->exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Iniciativa a serem executadas', '');

echo $obPTA->cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);
echo "<br />";

echo montarAbasArray(carregaAbasPlanoTrabalho('acoesPlanoTrabalho'), "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A");

$ptrid 	  = $_SESSION["emenda"]["ptrid"];
$ptridpai = $_SESSION["emenda"]["ptridpai"];
//echo montarAbasArray(carregaAbasPlanoTrabalho('acoesPlanoTrabalho'), "/emenda/emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A");

$arPerfil = $obPTA->pegaPerfil();

if( !empty($_SESSION['emenda']['federal']) ){
	$federal = $_SESSION['emenda']['federal'];
} else {
	$federal = $obPTA->buscaEmendaDescentraliza( $ptrid );
}

$dados = $obPTA->carregaDadosIniciativaPTA( $ptrid );
	
	if(!$dados){
		//echo "<script>alert('Não existe nenhuma inicitiva cadastra para este plano de trabalho. Para cadastra clique no link Insere Nova Ação!');</script>";
		$msg = "<span>Informe as iniciativas que serão executadas em seu projeto clicando no link 'Inserir Iniciativa'.<br><br>
As iniciativas são as ações que se pretende realizar com o plano de trabalho.
Elas são compostas de especificações, que são um detalhamento da iniciativa, onde os valores financeiros serão definidos.</span>";
		
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela">';
		print '<tr><td align="left" style="color:#000000;">'.$msg.'</td></tr>';
		print '</table>';
	}else{
		monta_titulo( '', '<b>Lista de Iniciativa(s)</b>');
?>	
	
<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr><td>
	<table class="listagem" width="100%">
	<thead>
	<tr style="background-color: #cccccc;">
		<td class="SubTitulocentro" width='5%'></td>
		<td class="SubTitulocentro" align='center' width='35%'>Iniciativa a ser executada</td>
		<?if( $federal == 'N' ){ ?>
		<td class="SubTitulocentro" align='center' width='30%'>Beneficiários</td>
		<?} ?>
		<td class="SubTitulocentro" align='center' width='10%'>Total</td>
		<?if( $federal == 'N' ){ ?>
			<td class="SubTitulocentro" align='center' width='10%'>Total do Proponente</td>
		<?} ?>
		<td class="SubTitulocentro" align='center' width='10%'>Total do Concedente</td>
	</tr>
	</thead>
	<tbody>
<?php
	$html = '';
	foreach( $dados as $acoes ){
	 $html.= "	<tr>
				<td>".$acoes['acao']."</td>
				<td>".$acoes['descricao']."</td>";
	 if( $federal == 'N' ){
	 	$html.= "<td><div id='lista'>".$obPTA->ListarbeneficiariosAcaoPlanoTrabalho($acoes['codigo'])."</div>";
	 }
	 $html.= "</td>
				<td align='right'>".number_format($acoes['ptivalortotal'],2,',','.')."</td>";
	 if( $federal == 'N' ){
	 	$html.="	<td align='right'>".number_format($acoes['ptivalorproponente'],2,',','.')."</td>";
	 }
	 $html.="	<td align='right'>".number_format($acoes['ptivalorconcedente'],2,',','.')."</td>
	 		<tr>";
	 	$valorTotalIniciativas += $acoes['ptivalortotal'];
	 	$valorTotalConcedente += $acoes['ptivalorconcedente'];
	 	$valorTotalProponente += $acoes['ptivalorproponente'];
	}
	echo $html;
?>
			<tr>
				<td colspan="2" align="center"><b>Totalizador</b></td>
				<?if( $federal == 'N' ){ ?>
					<td><?//=$obPTA->totalizaBeneficiariosPlanoTrabalho( $_SESSION["emenda"]["ptrid"] );?></td>
				<?} ?>
				<td align='right'>
					<font color='0000FF'><b>R$ <?=number_format($valorTotalIniciativas,2,',','.');?></b></font>
	 			</td>
	 			<?if( $federal == 'N' ){ ?>
					<td align='right'>
						<b>R$ <?=number_format($valorTotalProponente,2,',','.');?></b>
		 			</td>
	 			<?} ?>
				<td align='right'>
					<b>R$ <?=number_format($valorTotalConcedente,2,',','.');?></b>
	 			</td>
	 		<tr>
	</tbody>
	</table>

</td></tr>
</table>	
<?} ?>

<!--  ************************************************************************************************ -->	
<table style="border:0px" class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
		<?
			if( verificaPermissaoPerfil('pta', 'boolean', $estadoAtual, true)  ) {
				if( $obPTA->verificaExclusaoIniciativaPTA($ptrid) ){
					echo '<a href="emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A">
							<img src="/imagens/gif_inclui.gif" border="0">
							Inserir Iniciativa
							</a>'; 
				} else {
					echo '<img src="/imagens/gif_inclui_d.gif" border="0">Inserir Iniciativa';
				}?>
		<? } else { ?>
			<img src="/imagens/gif_inclui_d.gif" border="0">Inserir Iniciativa
		<? } ?>
		</td>
	</tr>
</table>	


<script type="text/javascript">

function removeracao(ptiid){
	var conf = confirm("Você realmente deseja excluir esta Iniciativa do plano de trabalho?");	
	if(conf) {
		location.href="emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A&evento=E&ptiid="+ptiid;	
	}
}
function alteracao(ptiid){
	location.href="emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid="+ptiid;
}
</script>