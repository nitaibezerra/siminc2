<?php
include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
//if($_POST["submetido"]) {
	if($_POST["pesquisar"]) {

		if($_POST['gpmid'][0] <> ""){
			$leftjoingpmid  = "LEFT JOIN elabrev.grupomatriz gp ON   ma.gpmid = gp.gpmid";
			$gpmid          = "AND gp.gpmid IN ('" . $_POST['gpmid'] . "')";
		}else{
			$gpmid          = "";
		}
		if($_POST['fonte'][0] <> ""){
			$leftjoinfonte = " LEFT JOIN elabrev.fonterecursomatriz fnrM ON ma.mtrid = fnrM.mtrid LEFT JOIN public.fonterecurso f ON fnrM.foncod = f.foncod";
			$fonte         = " AND f.foncod IN ('" . implode( "','", $_POST['fonte'] ) . "')";
		}else{
			$fonte         = "";
		}
		if($_POST['gnd'][0] <> "" ){
			$leftjoingnd  = " LEFT JOIN	elabrev.matrizgnd mg ON mg.mtrid = ma.mtrid	LEFT JOIN gnd g on mg.gndcod = g.gndcod";
			$gnd          = " AND g.gndcod IN ('" . implode( "','", $_POST['gnd'] ) . "')";
		}else{
			$gnd          = "";
		}
		if($_POST['unidade'][0] <> "" ){
			$leftjoinunidade =" LEFT JOIN elabrev.unidadematriz uM ON ma.mtrid = uM.mtrid LEFT JOIN	unidade u ON  u.unicod = uM.unicod";
			$unidade         = "AND u.unicod IN ('" . implode( "','", $_POST['unidade'] ) . "')";
		}else{
			$unidade         =  "";
		}
		if($_POST['acacod'][0] <> ""){
			$leftjoinacacod = " LEFT JOIN elabrev.acaomatriz am ON am.mtrid = ma.mtrid LEFT JOIN monitora.acao ac ON ac.acacod = am.acacod";
			$acacod = "AND am.acacod IN ('" . implode( "','", $_POST['acacod'] ) . "')";
		}else{
			$acacod = "";
		}
		if($_POST['subacao'][0] <> ""){
			$leftjoinsubacao =" LEFT JOIN elabrev.matrizsubacao maS ON maS.mtrid = ma.mtrid LEFT JOIN monitora.pi_subacao  pi ON  pi.sbaid = maS.sbaid";
			$subacao = "AND pi.sbacod IN ('" . implode( "','", $_POST['subacao'] ) . "')";
		}else{
			$subacao = "";
		}
		$mtrsigla = $_POST['mtrsigla'][0] <> "" ? "AND ma.mtrsigla IN ('" . $_POST['mtrsigla']. "')" : "";
		$mtrdsc = $_POST['mtrdsc'] <> "" ? "AND ma.mtrdsc LIKE('%" . $_POST['mtrdsc'] . "%')" : "";


		$sql_matrizes = "select distinct '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||ma.mtrid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||ma.mtrid||')\">' as acao,
		ma.mtrdsc, ma.mtrsigla, ma.mtrano
		FROM
			elabrev.matriz ma
			{$leftjoingpmid}
			{$leftjoinfonte}
			{$leftjoingnd}
			{$leftjoinunidade}
			{$leftjoinacacod}
			{$leftjoinsubacao}
		WHERE

			ma.mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "'
			".$mtrdsc."
			".$fonte."
			".$gnd."
			".$acacod."
			".$gpmid."
			".$mtrsigla."
			".$unidade."
			".$subacao."
		";
	//ver($sql_matrizes, D);
//		die;
		$db->executar($sql_matrizes);
		$db->commit();
	}
//}
		#pegando o orgcod

        $sql= "select orgcod from instituicao where ittstatus='A'";

        $RS = $db->record_set($sql);

        $res = $db->carrega_registro($RS,0);

        $_SESSION['ittorgao']= trim($res['orgcod']);

        // VARIÁVEIS

		// campos da entidade matriz
			$mtrid = (integer) $_REQUEST['mtrid'];
			$mtrdsc = (string) $_REQUEST['mtrdsc'];
			$mtrsigla = (string) $_REQUEST['mtrsigla'];
		//$mtrano = (string) $_REQUEST['mtrano'];
			$mtrano = $_SESSION['exercicio'] + 1;
			$gpmid = (integer) $_REQUEST['gpmid'];
			$mtrmatriz = (boolean) $_REQUEST['mtrmatriz'];

		// variáveis gerais
			$modulo = $_REQUEST['modulo'];
			$act = trim( $_REQUEST['act'] ); // indica se o formulario foi submetido
		//$titulo_modulo = $act == 'alterar' ? 'Alterar Coluna' : 'Incluir Coluna';
			$titulo_modulo = $_REQUEST['acao'] == 'A' ? 'Alterar Coluna' : 'Consultar Colunas';
			$sql_combo_grupo_matriz = "select gpmid as codigo, gpmdsc as descricao from elabrev.grupomatriz";
			$cabecalho_matrizes = array( 'Ações', 'Descrição', 'Sigla', 'Execução' );

			$sql_fonte = "select foncod as codigo, foncod || ' - ' || fondsc as descricao from public.fonterecurso where fonstatus = 'A' order by foncod";
			$sql_gnd = "select gndcod as codigo, gndcod || ' - ' || gnddsc as descricao from gnd where gndstatus = 'A' order by codigo";
			$sql_acao = "select distinct acacod as codigo, acacod || ' - ' || acadsc as descricao from elabrev.ppaacao_orcamento where prgano = '" . $_SESSION['exercicio'] . "' and acasnrap = 'f' order by acacod";
			$sql_unidade = "select unicod as codigo, unicod || ' - ' || unidsc as descricao from unidade where unitpocod = 'U' and unistatus = 'A' and orgcod = '" . ( (integer) $_SESSION['ittorgao'] ) . "' order by unicod";
			$sql_subacao = "select sbacod as codigo, sbacod || ' - ' || sbatitulo as descricao from monitora.pi_subacao where sbastatus = 'A' and sbasituacao = 'A' and pieid is not null order by sbacod;";
			$mensagem_alert = '';
		// FIM VARIÁVEIS



	include APPRAIZ."includes/cabecalho.inc";

?>
<br/>
<? $db->cria_aba( $abacod_tela, $url, '' ); ?>
<? monta_titulo( $titulo_modulo, 'Administração de Colunas' ); ?>

<form method="post" name="formulario">
	<input type='hidden' name="submetido" value="1">
	<input type='hidden' id="pesquisar" name="pesquisar" value="">
	<input type="hidden" name="modulo" value="<?= $modulo ?>">
	<input type="hidden" name="mtrid" value="<?= $mtrid ?>">
	<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>">
	<input type="hidden" name="act" value="<?= $act ?>">
	<!-- FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
	<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<!-- EXERCICIO -->
			<td align='right' class="SubTituloDireita">Exercício:</td>
			<td><?= campo_texto( 'mtrano', 'N', 'N', '', 4, 4, '####', '' ); ?></td>
		</tr>
		<tr>
			<!-- DESCRICAO -->
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<td><?= campo_texto( 'mtrdsc', 'N', 'S', '', 50, 200, '', '' ); ?></td>
		</tr>
		<tr>
			<!-- SIGLA -->
			<td align='right' class="SubTituloDireita">Sigla:</td>
			<td><?= campo_texto( 'mtrsigla', 'N', 'S', '', 7, 7, '', '' ); ?></td>
		</tr>
		<tr>
			<!-- GRUPO MATRIZ -->
			<td align='right' class="SubTituloDireita">Grupo Coluna:</td>
			<td><? $db->monta_combo( 'gpmid', $sql_combo_grupo_matriz, 'S', '-- Selecione o Grupo --', '', '' ); ?></td>
		</tr>
		<tr>
			<!-- GRUPO MATRIZ -->
			<td align='right' class="SubTituloDireita">Fontes de Recurso:</td>
			<td>
				<? $sql_auxiliar = "select f.foncod as codigo, f.foncod || ' - ' || f.fondsc as descricao from elabrev.fonterecursomatriz fm inner join public.fonterecurso f on fm.foncod = f.foncod where fm.mtrid = " . $mtrid . $where_fontes_fixas; ?>
				<? $fonte = $db->carregar( $sql_auxiliar ); ?>
				<? combo_popup( 'fonte', $sql_fonte, 'Selecione a(s) Fonte(s) de Recurso', '600x700', 0, $fontes_fixas, 'Não é possível remover fonte, pois ela já possui valor de limite distribuido por essa coluna.' ); ?>
			</td>
		</tr>
		<tr>
			<!-- GRUPO MATRIZ -->
			<td align='right' class="SubTituloDireita">GND:</td>
			<td>
				<? $sql_auxiliar = "select g.gndcod as codigo, g.gndcod || ' - ' || g.gnddsc as descricao from elabrev.matrizgnd mg inner join gnd g on mg.gndcod = g.gndcod where mg.mtrid = " . $mtrid; ?>
				<? $gnd = $db->carregar( $sql_auxiliar ); ?>
				<? $sql_auxiliar = "select g.gndcod from elabrev.matrizgnd mg inner join gnd g on mg.gndcod = g.gndcod where mg.mtrid = " . $mtrid; ?>
				<? $gnds = $db->carregar( $sql_auxiliar ); ?>
				<? $gnds_fixos = array(); ?>
				<? if ( $gnds ) : ?>
					<? foreach ( $gnds as $gnd_fixo ) : ?>
						<? array_push( $gnds_fixos, $gnd_fixo['gndcod'] ); ?>
					<? endforeach; ?>
				<? endif; ?>
				<? combo_popup( 'gnd', $sql_gnd, 'Selecione o(s) GND', '400x400', 0, $gnds_fixos, 'Não é possível remover gnd, pois a coluna já possui valor de limite distribuido.' ); ?>
			</td>
		</tr>
		<tr>
			<!-- AÇÃO -->
			<td align='right' class="SubTituloDireita">Ações Relacionadas:</td>
			<td>
				<? $sql_auxiliar = "select distinct a.acacod as codigo, a.acacod || ' - ' || a2.acadsc as descricao from elabrev.matrizgnd mg inner join acaomatriz a on mg.mtrid = a.mtrid inner join acao a2 on a.acacod=a2.acacod and acasnrap='f' where mg.mtrid = " . $mtrid; ?>
				<? $acacod = $db->carregar( $sql_auxiliar ); ?>
				<? $sql_auxiliar = "select distinct a.acacod from elabrev.matrizgnd mg inner join acaomatriz a on mg.mtrid = a.mtrid inner join acao a2 on a.acacod=a2.acacod and acasnrap='f' where mg.mtrid = " . $mtrid; ?>
				<? $acoes = $db->carregar( $sql_auxiliar ); ?>
				<? $acoes_fixas = array(); ?>
				<? if ( $acoes ) : ?>
					<? foreach ( $acoes as $acao_fixa ) : ?>
						<? array_push( $acoes_fixas, $acao_fixa['acacod'] ); ?>
					<? endforeach; ?>
				<? endif; ?>
				<? combo_popup( 'acacod', $sql_acao, 'Selecione a(s) Ação(ões)', '400x400', 0, $acoes_fixas, 'Não é possível remover ação relacionada, pois a coluna já possui valor de limite distribuido.' ); ?>

			</td>
		</tr>
		<tr>
			<!-- GRUPO MATRIZ -->
			<td align='right' class="SubTituloDireita">Unidades Orçamentárias:</td>
			<td>
				<? $sql_auxiliar = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from elabrev.unidadematriz um inner join unidade u on um.unicod = u.unicod and um.unitpocod = u.unitpocod where um.mtrid = " . $mtrid . $where_unidades_fixas; ?>
				<? $unidade = $db->carregar( $sql_auxiliar ); ?>
				<? combo_popup( 'unidade', $sql_unidade, 'Selecione a(s) Unidade(s) Orçamentárias', '400x500', 0, $unidades_fixas, 'Não é possível remover unidade, pois ela já possui valor de limite atribuido à ela por essa coluna.' ); ?>
			</td>
		</tr>
		<tr>
			<!-- SUBACAO -->
			<td align='right' class="SubTituloDireita">Subação:</td>
			<td>
				<?php $sql_auxiliar = "select pi.sbacod as codigo, pi.sbacod || ' - ' || pi.sbatitulo as descricao from monitora.pi_subacao as pi inner join elabrev.matrizsubacao as ma on pi.sbaid = ma.sbaid where ma.mtrid = " . $mtrid . $where_unidades_fixas; ?>
				<?php $subacao = $db->carregar( $sql_auxiliar ); ?>
				<?php combo_popup( 'subacao', $sql_subacao, 'Selecione a(s) Subação(ões)', '400x500', 0, $unidades_fixas, 'Não é possível remover unidade, pois ela já possui valor de limite atribuido à ela por essa coluna.' ); ?>
			</td>
		</tr>

		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<input type="button" name="bsubmeter" value="Pesquisar" class="botao" onclick="pesquisaMatriz( <?=$mtrid?>, 1 );"/>
			</td>
		</tr>
	</table>
	<!-- FIM FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
</form>
<!-- LISTAGEM -->
<?php
/*if($_POST["pesquisar"]) {
	$db->monta_lista( $sql_matrizes, $cabecalho_matrizes, 200, 20, '', '', '' );
}else{
	$sql_matrizesPadrao = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||mtrid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||mtrid||')\">' as acao, mtrdsc, mtrsigla, mtrano from elabrev.matriz where mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "'";
	$db->monta_lista( $sql_matrizesPadrao, $cabecalho_matrizes, 200, 20, '', '', '' );
}*/

if($_POST["pesquisar"]) {
	//ver($sql_matrizes);
	//$sql_matrizes = "";
	$obMontaListaAjax = new MontaListaAjax($db, true);
	$obMontaListaAjax->montaLista($sql_matrizes, $cabecalho_matrizes,200, 20, '', '', '', '', '', '', '', '' );
} else {
	$sql_matrizesPadrao = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||mtrid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||mtrid||')\">' as acao, mtrdsc, mtrsigla, mtrano from elabrev.matriz where mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "'";
	$obMontaListaAjax = new MontaListaAjax($db, true);
	$obMontaListaAjax->montaLista($sql_matrizesPadrao, $cabecalho_matrizes,200, 20, '', '', '', '', '', '', '', '' );
}

?>

<!-- FIM LISTAGEM -->
<script language="javascript"><!--


		/**
		 * Indica se a coluna atual possui algum limitefonte ou não.
		 *
		 * @var integer
		 */
		var possui_limitefonte = <?= $possui_limitefonte ? 'true' : 'false' ?>;

		/**
		 * Quantidade de fontes que possuem limite definido para a coluna em questão.
		 *
		 * @far integer
		 */
		var quantidade_fontes_fixas = <?= count( $fontes_fixas ) ?>;

		/**
		 * Indica se a coluna atual possui despesa ou não.
		 *
		 * @var integer
		 */
		var possui_despesa = <?= $possui_despesa ? 'true' : 'false' ?>

		/**
		 * Prepara formulário para alteração.
		 *
		 * @param integer mtrid
		 * @return void
		 */
		function altera_coluna( mtrid )
		{
			document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/matriz&acao=A&act=exibir&mtrid=' + mtrid;
		}

/**
 * Prepara formulário para remoção.
 *
 * @param integer mtrid
 * @return void
 */
function excluir_coluna(mtrid)
{
    if (window.confirm("Confirma a exclusão da coluna selecionada (" + mtrid + ") em Colunas?")) {
        if (window.confirm('Esta ação NÃO PODERÁ SER DESFEITA. Confirma?')) {
            document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/matriz&acao=I&act=excluir&mtrid=' + mtrid;
        }
    }
    return;
}

		/**
		 * Verifica se formulário este correto e o submete.
		 *
		 * @return void
		 */
		function validar_cadastro()
		{
			prepara_formulario();
			var formulario = document.formulario;
			if ( !validaBranco( formulario.mtrano, 'Exercício' ) ) return;
			if ( !validaBranco( formulario.mtrdsc, 'Descrição' ) ) return;
			if ( !validaBranco( formulario.gpmid, 'Grupo Coluna' ) ) return;
			if ( !possui_limitefonte )
			{
				if ( !validaBranco( formulario.fonte, 'Fontes de Recurso' ) ) return;
				if ( !validaBranco( formulario.unidade, 'Unidades Orçamentárias' ) ) return;
			}
			if ( !possui_despesa )
			{
				if ( !validaBranco( formulario.gnd, 'GND' ) ) return;
//				if ( !validaBranco( formulario.acacod, 'Ações Relacionadas' ) ) return;
			}
			// verifica quantidade de fontes de acordo com o campo mtrmatriz
			// caso seja matriz só pode haver uma fonte selecionada
			if ( formulario.mtrmatriz_check[0].checked == '1' && formulario.fonte.options.length != 1 )
			{
				alert( 'Para matrizes de distribuição só pode haver uma fonte de recurso.' );
				return;
			}
			formulario.mtrmatriz.value = formulario.mtrmatriz_check[0].checked ? '1' : '0';
			formulario.submit();
		}

//		<? if ( $mensagem_alert != '' ) : ?>
//			alert( '<?= str_replace( "'", "\\'", $mensagem_alert ) ?>' );
//		<? endif; ?>


		function pesquisaMatriz( mtrid, condicao ){

			var formulario = document.formulario;


			selectAllOptions( formulario.fonte );
			selectAllOptions( formulario.gnd );
			selectAllOptions( formulario.acacod );
			selectAllOptions( formulario.unidade );
			selectAllOptions( formulario.subacao );

			document.getElementById('pesquisar').value = condicao;

			formulario.submit();


		}

</script>