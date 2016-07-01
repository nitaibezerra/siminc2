<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:elabrev.php
   Finalidade: permitir a abertura de todas as páginas do sistema com segurança
   */

	include_once("config.inc");
	include "../../includes/classes_simec.inc";
	include "../../includes/funcoes.inc";
	
	header( 'Content-Type: text/html; charset=iso-8859-1' );

	// querys para os campos texto ajax
	$sql_natureza = "select ndpcod as valor, ndpcod || ' - ' || ndpdsc as descricao from public.naturezadespesa where ndpcod like '%s%' and ndpstatus = 'A' order by ndpcod";
	$sql_uso = "select iducod as valor, iducod || ' - ' || idudsc as descricao from public.identifuso where iducod like '%s%' and idustatus = 'A' order by iducod";
	$sql_fonte = "select foncod as valor, foncod || ' - ' || fondsc as descricao from public.fonterecurso where foncod like '%s%' and fonstatus = 'A' order by foncod";
	$sql_oc = "select idocod as valor, idocod || ' - ' || idodsc as descricao from public.idoc where idocod like '%s%' and idostatus = 'A' order by idodsc";
	
	/*
	$natureza = $_REQUEST['natureza'];
	$uso = $_REQUEST['uso'];
	$fonte = $_REQUEST['fonte'];
	$oc = $_REQUEST['oc'];
	*/
	
?>
<html>
	<head>
		<title>Detalhamento da Proposta</title>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/livesearch.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table  class="tabela" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<td width="110" align="right" class="SubTituloDireita">Natureza da Despesa:</td>
				<td align="left">
					<?= campo_texto_ajax( $sql_natureza, 'natureza', 'natureza', 'Natureza da Despesa', '', 15, 9, '#.#.##.##', 'left', true, true, false, true ) ?>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				 <!--
					<iframe name="destalhamento_despesa" action="" method="post">
						<form action=""></form>
					</iframe>
				-->
				...
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Ident. de Uso:</td>
				<td align="left">
					<?= campo_texto_ajax( $sql_uso, 'uso', 'uso', 'Ident. de Uso', '', 5, 1, '#', 'left', true, true, false, true ) ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Fonte de Recursos:</td>
				<td align="left">
					<?= campo_texto_ajax( $sql_fonte, 'fonte', 'fonte', 'Fonte de Recursos', '', 5, 3, '###', 'left', true, true, false, true ) ?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Ident. Op. de Crédito:</td>
				<td align="left">
					<?= campo_texto_ajax( $sql_oc, 'oc', 'oc', 'Ident. Op. de Crédito', '', 10, 5, '#####', 'left', true, true, false, true ) ?>
				</td>
			</tr>
		</table>
		<br/>
		<table  class="tabela" border="0" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<td align="right" class="SubTituloDireita">Valor da Proposta:</td>
				<td align="left" width="160">
					<?= campo_texto( 'valor', 'S', 'S', '', 25, 17, '##.###.###.###,##', '', 'left', '', 0, ' id="valor" ' ); ?>
				</td>
			</tr>
		</table>
		<br/>
		<table border="0" width="95%" cellspacing="0" cellpadding="0" align="center">
			<tr>
				<td align="right">
					<input type="button" name="adicionar" value="Adicionar" onclick="javascript:adicionar();"/>
					&nbsp;
					<input type="button" name="cancelar" value="Cancelar" onclick="javascript:cancelar();"/>
				</td>
			</tr>
		</table>
		<script type="text/javascript">
			
			/**
			 * Insere item financeiro na tabela da janela origem. A janela
			 * popup é fechada.
			 * 
			 * @return void
			 */
			function adicionar()
			{
				var natureza = document.getElementById( 'natureza' );
				var uso = document.getElementById( 'uso' );
				var fonte = document.getElementById( 'fonte' );
				var oc = document.getElementById( 'oc' );
				var valor = document.getElementById( 'valor' );
				if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
				if ( !validaBranco( uso, 'Ident. de Uso' ) ) return;
				if ( !validaBranco( fonte, 'Fontes de Recursos' ) ) return;
				if ( !validaBranco( oc, 'Ident. Op. de Crédito' ) ) return;
				if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
				window.opener.item_financeiro_adicionar( natureza.value, uso.value, fonte.value, oc.value, valor.value );
				window.close();
			}
			
			/**
			 * Cancela a inserção do item financeiro. A janela popup é fechada.
			 * 
			 * @return void
			 */
			function cancelar()
			{
				window.close();
			}
			
		</script>
	</body>
</html>