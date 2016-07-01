<?php

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$unicod = $_SESSION['unicod'];

?><br/><?



$db->cria_aba( $abacod_tela, $url, "&unicod=" . $unicod );
$titulo_modulo='Plano Aplicação - Investimento Global (2008 à 2011)';
monta_titulo($titulo_modulo,'');

// VARIÁVEIS
$itens_financeiro = array();
$itens_financeiro_despesa = array();

//apagar em produção
//$unicod = 26234;
$unitpocod = 'U';

$sql_valor_total_investimento = "
								SELECT lidvalor 
								FROM reuni.limitedespglobal as lim
								INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid							
								WHERE tipo.tipcod = 4 AND
								lim.unicod = '".$unicod."' AND
								lim.unitpocod = 'U'								
								
";

$item_financeiro_valor_total_investimento = $db->pegaUm($sql_valor_total_investimento);
$item_financeiro_valor_total_investimento = number_format( $item_financeiro_valor_total_investimento, 2, ",", "." );


$sql_valor_total_despesa = "
								SELECT lidvalor 
								FROM reuni.limitedespglobal as lim
								INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid							
								WHERE tipo.tipcod = 5 AND
								lim.unicod = '".$unicod."' AND
								lim.unitpocod = 'U'								
								
";

$item_financeiro_valor_total_despesa = $db->pegaUm($sql_valor_total_despesa);
$item_financeiro_valor_total_despesa = number_format( $item_financeiro_valor_total_despesa, 2, ",", "." );

//apagar em produção
$acaid = 0;

/*
			define a variável $act, que define qual o status atual do ação
			null - acessando a página pela primeira vez
			'finalizar_escolha_acao' - acessou página em branco e acabou de escolher a ação
			'gravar' - ação escolhida, dados carregados... agora é só gravar na base
		*/
		$act = !isset( $_REQUEST['act'] ) ? null : $_REQUEST['act'] ;

		//tenta gravar
		//apagar em produção
		$permissao_momento = true;
		//alimenta array
		if ( $act == 'gravar' && $permissao_momento )
		{
			$financeiro_total = 0; // inicia variável utilizada para ser verificada com o valor total do físico
			
			$auditoria = true;
			
			// ---- GRUPO 4
			// remover os valores
			$sqlRemove = "
				delete from reuni.lancamentodespglobal 
				where
					unicod = '" . $unicod . "' and
					tipid in ( select tipid from reuni.tipodesp where tipcod = 4 )
			";
			$db->executar( $sqlRemove, $auditoria );
			// alimenta variáveis do financeiro com os dados do formulário submetido
			if ( isset( $_REQUEST['natureza'] ) && count( $_REQUEST['natureza'] ) )
			{
				$itens = array();
				foreach ( $_REQUEST['natureza'] as $indice_item_financeiro => $valor_natureza )
				{
					$novo_item_financeiro = array(
						'natureza' => $valor_natureza,						
						'valor'    => $_REQUEST['valor'][$indice_item_financeiro]					
					);
					array_push( $itens, $novo_item_financeiro );					
				}
				foreach ( $itens as $item_financeiro )
				{
					$valor = str_replace( '.', '', $item_financeiro['valor'] );
					$valor = str_replace( ',', '.', $valor );
					$ndpcod = str_replace( '.', '', $item_financeiro['natureza'] );
					$grupo = substr( $ndpcod, 0, 4 );
					if ( $grupo != '3390' && $grupo != '4490' )
					{
						continue;
					}
					$sql = "
						insert into reuni.lancamentodespglobal 
						(
							tipid,
							ndpid,
							unicod,
							unitpocod,
							lantotal
						)
						values (
							( select tipid from reuni.tipodesp where tipodesp.tipcod = 4 ), 
							( select ndpid from public.naturezadespesa where naturezadespesa.ndpcod = '".$ndpcod."'),
							'".$unicod."', 
							'U',
							" . $valor . "
						)
					";
					$db->executar( $sql, $auditoria );
				}
			}
			
			
			// ---- GRUPO 5
			// remover os valores
			/*$sqlRemove = "
				delete from reuni.lancamentodespglobal 
				where
					unicod = '" . $unicod . "' and
					tipid in ( select tipid from reuni.tipodesp where tipcod = 5 )
			";
			$db->executar( $sqlRemove, $auditoria );
			// alimenta variáveis do financeiro com os dados do formulário submetido
			if ( isset( $_REQUEST['natureza_despesa'] ) && count( $_REQUEST['natureza_despesa'] ) )
			{
				$itens = array();
				foreach ( $_REQUEST['natureza_despesa'] as $indice_item_financeiro => $valor_natureza )
				{
					$novo_item_financeiro_despesa = array(
						'natureza' => $valor_natureza,						
						'valor'    => $_REQUEST['valor_despesa'][$indice_item_financeiro]					
					);
					array_push( $itens, $novo_item_financeiro_despesa );		
				}
				foreach ( $itens as $item_financeiro )
				{
					$valor = str_replace( '.', '', $item_financeiro['valor'] );
					$valor = str_replace( ',', '.', $valor );
					$ndpcod = str_replace( '.', '', $item_financeiro['natureza'] );
					$grupo = substr( $ndpcod, 0, 4 );
					if ( $grupo != '3390' && $grupo != '4490' )
					{
						continue;
					}			
					$sql = "
						insert into reuni.lancamentodespglobal 
						(
							tipid,
							ndpid,
							unicod,
							unitpocod,
							lantotal
						)
						values (
							( select tipid from reuni.tipodesp where tipodesp.tipcod = 5 ), 
							( select ndpid from public.naturezadespesa where naturezadespesa.ndpcod = '".$ndpcod."'),
							'".$unicod."', 
							'U',
							" . $valor . "
						)
					";
					$db->executar( $sql, $auditoria );
				}
			}*/
			
			$db->commit();
		}

$sql_unidade = " SELECT unidsc
				 FROM public.unidade 
				 WHERE public.unidade.unicod = '".$unicod."'				
				";

$unidsc = $db->pegaUm($sql_unidade);

// define identificador da ação que está sendo manipulada
/*if ( (boolean) $_REQUEST['acaid'] )
 {
 $acaid = (integer) $_REQUEST['acaid'];
 }
 else
 {
 $acaid = pegar_acaid( $_REQUEST['unicod'], $_REQUEST['prgcod'], $_REQUEST['acacod'], $_REQUEST['loccod'] );
 }*/

// verifica se usuário pode manipular os dados
//		$permissao_momento = verifica_permissao_momento( $acaid, $unicod );

//FUNÇÕES

/* Verifica se usuário possui permissão para manipular a despesa.
 *
 * @param integer $acaid
 * @param string $unicod
 * @return booelan
 */
function verifica_permissao_momento( $acaid, $unicod )
{
	return true;
}

// -------------- AQUI POVOAR!

//---- povoando array de invetimento

$sql_financeiro_natureza = "
		select
			substr( ndpcod, 1, 2 ) || '.' ||
			substr( ndpcod, 3, 2 ) || '.' ||
			substr( ndpcod, 5, 2 ) || '.' ||
			substr( ndpcod, 7, 2 )
				as codigo,
			ndpdsc as descricao, lan.lantotal as valor
		from naturezadespesa as nat
			inner join reuni.lancamentodespglobal as lan on lan.ndpid = nat.ndpid
			inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
		where
			substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
			ndpstatus = 'A' and
			lan.unicod = '".$unicod."' and
			lan.unitpocod = 'U' and 
			tipo.tipcod = '4'
			
		order by ndpcod
	";
$resultado_investimento = $db->carregar($sql_financeiro_natureza);
$resultado_investimento = is_array( $resultado_investimento ) ? $resultado_investimento : array(); 

	foreach ( $resultado_investimento as $item )
	{
		
		$novo_item_financeiro = array(
			'natureza'  => $item['codigo'],						
			'descricao' => $item['descricao'],	
			'valor'     => number_format( $item['valor'], 2, ",", "." )
		);
		array_push( $itens_financeiro, $novo_item_financeiro );					
	}
	

//---- povoando array de despesas

$sql_financeiro_natureza_despesa = "
		select
			substr( ndpcod, 1, 2 ) || '.' ||
			substr( ndpcod, 3, 2 ) || '.' ||
			substr( ndpcod, 5, 2 ) || '.' ||
			substr( ndpcod, 7, 2 )
				as codigo,
			ndpdsc as descricao, lan.lantotal as valor
		from naturezadespesa as nat
			inner join reuni.lancamentodespglobal as lan on lan.ndpid = nat.ndpid
			inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
		where
			substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
			ndpstatus = 'A' and
			lan.unicod = '".$unicod."' and
			lan.unitpocod = 'U' and 
			tipo.tipcod = '5'
			
		order by ndpcod
	";
$resultado_despesa = $db->carregar($sql_financeiro_natureza_despesa);
$resultado_despesa = is_array( $resultado_despesa ) ? $resultado_despesa : array(); 

	foreach ( $resultado_despesa as $item )
	{
		
		$novo_item_financeiro = array(
			'natureza'  => $item['codigo'],						
			'descricao' => $item['descricao'],	
			'valor'     => number_format( $item['valor'], 2, ",", "." )
		);
		array_push( $itens_financeiro_despesa, $novo_item_financeiro );					
	}

?>

<form name="formulario" method="post" style="margin: 0px;">
<input type="hidden" name="act" id="" value="" />

<table width="100%" cellpadding="0" cellspacing="0">
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center">
			<tr>
				<td width="20%" align="right" class="SubTituloDireita">Código:</td>
				<td width="80%"><?=$unicod ?></td>
			</tr>
			<tr>
				<td width="20%" align="right" class="SubTituloDireita">Nome da
				Instituição:</td>
				<td width="80%"><?=$unidsc ?></td>
			</tr>
		</table>
	
	
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center">
			<tr>
				<td align="center" class="SubTituloEsquerda">Informações Complementares</td>

			</tr>
			<tr>
				<td align="rigth">Programa 1073 - Brasil Universitário<br />
				Ação 8282 - Reuni</td>
			</tr>

		</table>
		</td>
	</tr>

	<!-- Despesas Fixas -->

	<!-- Pessoal-->

	<?
	
	/*$sql_despesa_pessoal = "SELECT 
							substr( lfxcodnatureza, 1, 2 ) || '.' ||
							substr( lfxcodnatureza, 3, 2 ) || '.' ||
							substr( lfxcodnatureza, 5, 2 )
							as codigo,
							lfxdscnatureza, lfxvalor
							FROM reuni.lancamentofixo 						
							WHERE unicod = ".$unicod." AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'
							ORDER BY lfxordem

	";
	
	$resultado  = $db->carregar($sql_despesa_pessoal);

	$sqp_soma_pessoal = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo				
				 		WHERE unicod = ".$unicod." AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'	
	";
	$total_pessoal = $db->pegaUm($sqp_soma_pessoal);
*/

	?>
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0"
			cellPadding="0" align="center">
			<tr>
				<td class="SubTituloEsquerda" style="cursor: pointer;" onclick="mostra_esconde_tr('conteudo_1')">1) - Despesas Orçamentárias de Pessoal</td>
			</tr>

		</table>
		</td>
	</tr>
	<tr id="conteudo_1">
		<td>
		<table align="center" border="0" class="tabela" cellpadding="3"
			cellspacing="1">
			<colgroup>
				<col style="width: 25%;" />
				<col style="width: 50%;" />
				<col style="width: 25%;" />
			</colgroup>
			
			<thead>
				<tr bgcolor="#dfdfdf">
					<td style="text-align: center;"><b>Cod. Despesa</b></td>
					<td style="text-align: center;"><b>Descrição</b></td>
					<td style="text-align: center;"><b>Valor</b></td>

				</tr>
			</thead>
			<!-- 
			<?php $cor = ''; ?>
			<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
			<?php foreach ( $resultado as $indice => $atributo ) :

			?>

			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>"
				onmouseout="this.style.backgroundColor='<?= $cor ?>';"
				onmouseover="this.style.backgroundColor='#ffffcc';">
				<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
				<td style="text-align: center;"><?= $atributo['lfxdscnatureza'] ?></td>
				<td style="text-align: center;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td>
			</tr>
			<?php endforeach; ?>
			
			 -->
			<tr bgcolor="#ffffff">
				<td />
				
				
				<td align="right"><b>Total</b></td>
				<!-- 
				<td align="center"><b><?= number_format($total_pessoal, 2, ",", ".") ?></b></td>
				 -->
			</tr>

		</table>
		</td>
	</tr>

	<!-- Bolsas/Capes -->

	<?/*

	$sql_despesa_bolsas = "SELECT 
							substr( lfxcodnatureza, 1, 2 ) || '.' ||
							substr( lfxcodnatureza, 3, 2 ) || '.' ||
							substr( lfxcodnatureza, 5, 2 )
							as codigo,
							lfxdscnatureza, lfxvalor
							FROM reuni.lancamentofixo 						
							WHERE unicod = ".$unicod." AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'
							ORDER BY lfxordem
	

	";
	$resultado  = $db->carregar($sql_despesa_bolsas);

	$sqp_soma_bolsas = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo				
				 		WHERE unicod = ".$unicod." AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'	
	";
	$total_bolsas = $db->pegaUm($sqp_soma_bolsas);
	*/
	?>
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0"
			cellPadding="0" align="center">
			<tr>
				<td class="SubTituloEsquerda" style="cursor: pointer;" onclick="mostra_esconde_tr('conteudo_2')">2) - Despesas Orçamentárias (Bolsas/Capes)</td>
			</tr>

		</table>
		</td>
	</tr>
	<tr id="conteudo_2">
		<td>
		<table align="center" border="0" class="tabela" cellpadding="3"
			cellspacing="1">
			<colgroup>
				<col style="width: 25%;" />
				<col style="width: 50%;" />
				<col style="width: 25%;" />
			</colgroup>
		
			<thead>
				<tr bgcolor="#dfdfdf">
					<td style="text-align: center;"><b>Cod. Despesa</b></td>
					<td style="text-align: center;"><b>Descrição</b></td>
					<td style="text-align: center;"><b>Valor</b></td>

				</tr>
			</thead>
			<!-- 
			<?php $cor = ''; ?>
			<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
			<?php foreach ( $resultado as $indice => $atributo ) :

			?>

			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>"
				onmouseout="this.style.backgroundColor='<?= $cor ?>';"
				onmouseover="this.style.backgroundColor='#ffffcc';">

				<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
				<td style="text-align: center;"><?= $atributo['lfxdscnatureza'] ?></td>
				<td style="text-align: center;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td>
			</tr>
			<?php endforeach; ?>			
			  -->
			<tr bgcolor="#ffffff">
				<td />
				
				
				<td align="right"><b>Total</b></td>
				<!-- 
				<td align="center"><b><?= number_format($total_bolsas, 2, ",", ".") ?></b></td>
				 -->
			</tr>


		</table>
		</td>
	</tr>


	<!-- Assitencia Estudantil -->

	<?/*

	$sql_despesa_bolsas_assitencia = "SELECT tipcod, tipdsc
								   FROM reuni.tipodesp						
								   WHERE tipcod = 3

	";
	$cabecalho  = $db->carregar($sql_despesa_bolsas_assitencia);

	$sql_despesa_assitencia = "SELECT 
								substr( lfxcodnatureza, 1, 2 ) || '.' ||
								substr( lfxcodnatureza, 3, 2 ) || '.' ||
								substr( lfxcodnatureza, 5, 2 )
								as codigo,
								lfxdscnatureza, lfxvalor
								FROM reuni.lancamentofixo 						
								WHERE unicod = ".$unicod." AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'
								ORDER BY lfxordem
	

	";
	$resultado  = $db->carregar($sql_despesa_assitencia);

	$sqp_soma_assitencia = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo				
				 		WHERE unicod = ".$unicod." AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'	
	";
	$total_assitencia = $db->pegaUm($sqp_soma_assitencia);
	*/
	?>
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0"
			cellPadding="0" align="center">
			<tr>
				<td class="SubTituloEsquerda" style="cursor: pointer;" onclick="mostra_esconde_tr('conteudo_3')">3) - Despesas Orçamentárias (Assistência Estudantil)</td>
			</tr>

		</table>
		</td>
	</tr>
	<tr id="conteudo_3">
		<td>
		<table align="center" border="0" class="tabela" cellpadding="3"
			cellspacing="1">
			<colgroup>
				<col style="width: 25%;" />
				<col style="width: 50%;" />
				<col style="width: 25%;" />
			</colgroup>
		
			<thead>
				<tr bgcolor="#dfdfdf">
					<td style="text-align: center;"><b>Cod. Despesa</b></td>
					<td style="text-align: center;"><b>Descrição</b></td>
					<td style="text-align: center;"><b>Valor</b></td>

				</tr>
			</thead>
			<!-- 
			<?php $cor = ''; ?>
			<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
			<?php foreach ( $resultado as $indice => $atributo ) :

			?>

			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>"
				onmouseout="this.style.backgroundColor='<?= $cor ?>';"
				onmouseover="this.style.backgroundColor='#ffffcc';">

				<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
				<td style="text-align: center;"><?= $atributo['lfxdscnatureza'] ?></td>
				<td style="text-align: center;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td>
			</tr>
			<?php endforeach; ?>
			
			 -->

			<tr bgcolor="#ffffff">
				<td />
				
				
				<td align="right"><b>Total</b></td>
				<!-- 
				<td align="center"><b><?= number_format($total_assitencia, 2, ",", ".") ?></b></td>
				 -->
			</tr>


		</table>
		</td>
	</tr>

	<!-- Despesas variáveis -->
	
	<!-- Investimento -->

	<?
	
	$sql_financeiro_natureza = "
		select
			substr( ndpcod, 1, 2 ) || '.' ||
			substr( ndpcod, 3, 2 ) || '.' ||
			substr( ndpcod, 5, 2 ) || '.' ||
			substr( ndpcod, 7, 2 )
				as codigo,
			ndpdsc as descricao
		from naturezadespesa
		where
			substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
			ndpstatus = 'A'
			" . $where_natureza . "
		order by ndpcod
	";
	
	$sql_despesa_investimento = "SELECT tipcod, tipdsc
								   FROM reuni.tipodesp						
								   WHERE tipcod = 4

	";
	$cabecalho_investimento  = $db->carregar($sql_despesa_investimento);
	
	?>
	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0"
			cellPadding="0" align="center">
			<tr>
				<td class="SubTituloEsquerda" style="cursor: pointer;" onclick="mostra_esconde_tr('conteudo_4')"><?=$cabecalho_investimento[0]['tipcod'] ?>
				- <?=$cabecalho_investimento[0]['tipdsc'] ?></td>
			</tr>

		</table>
		</td>
	</tr>
		
	<tr id="conteudo_4">
		<td>
		<div id="aba_financeiro" class="conteudoAba">

		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
            <thead>
                <tr>
                    <td align="center" class="title" style="width: 170px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cod. Despesa</strong></td>
                    <td align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
                    <td align="center" class="title" style="width: 170px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
                    <td align="center" class="title" style="width: 79px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                </tr>
            </thead>

			<?php			
			
			
			
			if ( $permissao_momento ) : ?>
			<tr bgcolor="#f7f7f7" align="center">
				<td>
				<?php
                $complemento = ' id="nova_natureza" onkeypress="item_financeiro_alterar_campo_focado(event, \'natureza\');" ';
				echo texto_popup('nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 11, 20, '##.##.##.##', '', $complemento, 'carregarDescricao();' );
                ?>
                </td>

				<td align="left">
				<?php
                $complemento = ' id="nova_descricao" onkeypress="item_financeiro_alterar_campo_focado(event, \'descricao\');" ';
				echo campo_texto( 'nova_descricao', 'N', 'N', '', 50, 50, '', '', 'left', '', 0, $complemento );
                ?>
				</td>

				<td align="right">
				<?php
                $complemento = ' id="novo_valor" onkeypress="item_financeiro_alterar_campo_focado( event, \'valor\' );" ';
				echo campo_texto( 'novo_valor', 'N', 'S', 'Valor', 25, 14, '##.###.###.###,##', '', 'right', '', 0, $complemento );
				?>
                </td>

				<td align="left">
                    <a href="javascript:item_financeiro_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" /></a>
				</td>
			</tr>
			<? else : ?>
			<tr>
                <td colspan="4">
                    <input type="hidden" value="" name="nova_natureza" id="nova_natureza" />
                    <input type="hidden" value="" name="nova_descricao" id="nova_descricao" />
                    <input type="hidden" value="" name="novo_valor" id="novo_valor" />
                </td>
			</tr>
			<? endif; ?>

			<tr>
				<td colspan="4">
				<div class="div_rolagem" id="div_tabela_financeiro" style="width: 100%; padding: 0; margin: 0; height: 100px; position-y: absolute;">
				<table id="tabela_financeiro" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
					<tr valign="bottom">
						<td width="169" style="height: 0; margin: 0; padding: 0;"></td>
						<td style="height: 0; margin: 0; padding: 0;"></td>
						<td width="169" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="30" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="30" style="height: 0; margin: 0; padding: 0;"></td>
					</tr>
				</table>
				</div>
				</td>
			</tr>

			<tr valign="bottom">
				<td colspan="4">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Disponível</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?= campo_texto( 'item_financeiro_valor_disponivel', 'N', 'N', 'Valor Disponível', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_disponivel" ' ); 

					?></td>
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Lançado</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?= campo_texto( 'item_financeiro_valor_lancado', 'N', 'N', 'Valor Lançado', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_lancado" ' ); 

					?></td>
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?= campo_texto( 'item_financeiro_valor_total_investimento', 'N', 'N', 'Valor Total', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_total_investimento" ' ); 

					?></td>
				<td style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
                </table>
                </td>
			</tr>
		</table>
		</div>
		</td>
	</tr>

	
	<!-- Outras Despesas -->
	
	<?
	$sql_despesa_outras = "SELECT tipcod, tipdsc
						   FROM reuni.tipodesp
						   WHERE tipcod = 5";
	
	$cabecalho_outras  = $db->carregar($sql_despesa_outras);	
	
	
		/*	$sql_financeiro_natureza_despesa = "
				select
					substr( ndpcod, 1, 2 ) || '.' ||
					substr( ndpcod, 3, 2 ) || '.' ||
					substr( ndpcod, 5, 2 ) || '.' ||
					substr( ndpcod, 7, 2 )
						as codigo,
					ndpdsc as descricao
				from naturezadespesa
				where
					substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
					ndpstatus = 'A'
					" . $where_natureza . "
				order by ndpcod
			";*/
			
	
	?>

	<tr>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0"
			cellPadding="0" align="center">
			<tr>
				<td class="SubTituloEsquerda" style="cursor: pointer;" onclick="mostra_esconde_tr('conteudo_5')"><?=$cabecalho_outras[0]['tipcod'] ?>
				- <?=$cabecalho_outras[0]['tipdsc'] ?></td>
			</tr>

		</table>
		</td>
	</tr>
		
	<tr id="conteudo_5">
		<td>
		<div id="aba_financeiro_despesa" class="conteudoAba">

		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
            <thead>
                <tr>
                    <td align="center" class="title" style="width: 170px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cod. Despesa</strong></td>
                    <td align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
                    <td align="center" class="title" style="width: 170px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
                    <td align="center" class="title" style="width: 79px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                </tr>
            </thead>
			<!-- 
			<?php			

			if ( $permissao_momento ) : ?>
			<tr bgcolor="#f7f7f7" align="center">
				<td>
				<?php
                //$complemento = ' id="nova_natureza_despesa" onkeypress="item_financeiro_alterar_campo_focado(event, \'natureza_despesa\');" ';
				//echo texto_popup('nova_natureza_despesa', $sql_financeiro_natureza_despesa, 'Natureza da Despesa', 11, 20, '##.##.##.##', '', $complemento, 'carregarDescricaoDespesa();' );
                ?>
                </td>

				<td align="left">
				<?php
               // $complemento = ' id="nova_descricao_despesa" onkeypress="item_financeiro_alterar_campo_focado(event, \'descricao_despesa\');" ';
				//echo campo_texto( 'nova_descricao_despesa', 'N', 'N', '', 70, 50, '', '', 'left', '', 0, $complemento );
                ?>
				</td>

				<td align="right">
				<?php
                //$complemento = ' id="novo_valor_despesa" onkeypress="item_financeiro_alterar_campo_focado( event, \'valor_despesa\' );" ';
				//echo campo_texto( 'novo_valor_despesa', 'N', 'S', 'Valor', 25, 14, '##.###.###.###,##', '', 'right', '', 0, $complemento );
				?>
                </td>
				
				<td align="left">
                    <a href="javascript:item_financeiro_adicionar_do_formulario_despesa();" title="Incluir" style="margin: 0; padding: 0 6px;"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" /></a>
				</td>
				
			</tr>
			<? else : ?>
			<tr>
                <td colspan="4">
                    <input type="hidden" value="" name="nova_natureza_despesa" id="nova_natureza_despesa" />
                    <input type="hidden" value="" name="nova_descricao_despesa" id="nova_descricao_despesa" />
                    <input type="hidden" value="" name="novo_valor_despesa" id="novo_valor_despesa" />
                </td>
			</tr>
			<? endif; ?>
			 
			<tr>
				<td colspan="4">
				<div class="div_rolagem" id="div_tabela_financeiro_despesa" style="width: 100%; padding: 0; margin: 0; height: 100px; position-y: absolute;">
				<table id="tabela_financeiro_despesa" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
					<tr valign="bottom">
						<td width="169" style="height: 0; margin: 0; padding: 0;"></td>
						<td style="height: 0; margin: 0; padding: 0;"></td>
						<td width="169" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="30" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="30" style="height: 0; margin: 0; padding: 0;"></td>
					</tr>
				</table>
				</div>
				</td>
			</tr>
			-->
			<tr valign="bottom">
				<td colspan="4">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Disponível</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?//= campo_texto( 'item_financeiro_valor_disponivel_despesa', 'N', 'N', 'Valor Disponível', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_disponivel_despesa" ' ); 

					?></td>
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Lançado</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?//= campo_texto( 'item_financeiro_valor_lancado_despesa', 'N', 'N', 'Valor Lançado', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_lancado_despesa" ' ); 

					?></td>
				<td align="right"
					style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
				<td align="left"
					style="color: #0066cc; border-top: 3px solid #dfdfdf;"
					valign="bottom"><?//= campo_texto( 'item_financeiro_valor_total_despesa', 'N', 'N', 'Valor Total', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_total_despesa" ' ); 

					?></td>
				<td style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
                </table>
                </td>
			</tr>
		</table>
		</div>
		</td>
	</tr>
	
	<tr>
	<td align="center"><br/>
		<input type="button" name="salvar" value="Salvar" onclick="javascript:validar_cadastro();"/>
	</td>
	</tr>
	
	

</table>

</form>

<script type="text/javascript"><!--

// JAVASCRIPT FINANCEIRO
	
		
		//criando e alimentado array com as descrições das naturezas
		var descricoes            = new Array();
		var descricoes_quantidade = 0;
		<?php
		
			$sql_financeiro_natureza = "
				select
					substr( ndpcod, 1, 2 ) || '.' ||
					substr( ndpcod, 3, 2 ) || '.' ||
					substr( ndpcod, 5, 2 ) || '.' ||
					substr( ndpcod, 7, 2 )
						as codigo,
					ndpdsc as descricao
				from naturezadespesa
				where
					substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
					ndpstatus = 'A'				
			";
		
			//$sql_descricoes = "select ndpdsc, ndpcod from public.naturezadespesa";
			$dados_temp = $db->carregar( $sql_financeiro_natureza );
			if ( $dados_temp )
			{
				$i = 0;
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>
						descricoes[<?php echo $i++; ?>] = new Array('<?= $dados_temp_valor['codigo'] ?>',
											  					    '<?= $dados_temp_valor['descricao'] ?>');
						descricoes_quantidade++;
					<?
				}
			}
		?>

		//carrega a descrição 
		function carregarDescricao()
		{
			for (var i = 0; i < descricoes.length; i++) {
				if (document.formulario.nova_natureza.value == descricoes[i][0]) {
					document.formulario.nova_descricao.value = descricoes[i][1];
					break;
				}
			}
		}
		
		//criando e alimentado array com as descrições das naturezas de despesa
		var descricoes_despesa = new Array();
		var descricoes_quantidade_despesa  = 0;
		<?php
			$sql_financeiro_natureza = "
				select
					substr( ndpcod, 1, 2 ) || '.' ||
					substr( ndpcod, 3, 2 ) || '.' ||
					substr( ndpcod, 5, 2 )|| '.' ||
					substr( ndpcod, 7, 2 )
						as codigo,
					ndpdsc as descricao
				from naturezadespesa
				where
					substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
					ndpstatus = 'A'				
			";
			//$sql_descricoes_despesa  = "select ndpdsc, ndpcod from public.naturezadespesa";
			$dados_temp_despesa  = $db->carregar( $sql_financeiro_natureza  );
			if ( $dados_temp_despesa  )
			{
				$i = 0;
				foreach ( $dados_temp_despesa  as $dados_temp_valor_despesa )
				{
					?>
						descricoes_despesa [<?php echo $i++; ?>] = new Array('<?= $dados_temp_valor_despesa ['codigo'] ?>',
											  					    '<?= $dados_temp_valor_despesa ['descricao'] ?>');
						descricoes_quantidade_despesa ++;
					<?
				}
			}
		?>
		
		
		//carrega a descrição de despesa
		function carregarDescricaoDespesa()
		{
			for (var i = 0; i < descricoes_despesa.length; i++) {
				if (document.formulario.nova_natureza_despesa.value == descricoes_despesa[i][0]) {
					document.formulario.nova_descricao_despesa.value = descricoes_despesa[i][1];
					break;
				}
			}
		}
		
		/**
		 * Contém os itens financeiros adicionados. Utilizado para
		 * evitar repetições de natureza-iduso-fonte-idoc na tabela
		 * de item financeiro.
		 * 
		 * @var boolean[]
		 */
		var item_financeiros_adicionados = new Array();
		var item_financeiros_adicionados_despesa = new Array();
		
		/**
		 * Soma de todos os itens da tabela financeiro.
		 * 
		 * @var float
		 */
		var item_financeiro_valor_total_investimento = 0;
		var item_financeiro_valor_lancado = 0;
		var item_financeiro_valor_disponivel = 0;
		
		var item_financeiro_valor_total_despesa = 0;
		//var item_financeiro_valor_lancado_despesa = 0;
		var item_financeiro_valor_disponivel_despesa = 0;
		
		/**
		 * Indica se usuário pode manipular itens financeiros onde
		 * as natureza possuem gnd = 1.
		 * 
		 * @var boolean[]
		 */
		var item_financeiro_habilita_gnd_1 = true;
		var item_financeiro_habilita_gnd_1_despesa = true;
		
		/**
		 * Sufixo do identificador de cada registro na tabela de
		 * financeiro. Variável incremental utilizada para evitar
		 * identificadores repetidos para cada tag 'tr' da tabela
		 * de financeiro.
		 * 
		 * @var float
		 */
		var item_financeiro_id_atual = 0;
		var item_financeiro_id_lancado = 0;
		var item_financeiro_id_disponivel = 0;
		
		var item_financeiro_id_atual_despesa = 0;
		var item_financeiro_id_lancado_despesa = 0;
		var item_financeiro_id_disponivel_despesa = 0;
		/**
		 * Indica se existia itens financeiros ao iniciar a tela.
		 * 
		 * @var float
		 */
		var possui_item_financeiro_inicial = <?= count( $itens_financeiro ) ? 'true' : 'false' ?>;
		var possui_item_financeiro_inicial_despesa = <?= count( $itens_financeiro_despesa ) ? 'true' : 'false' ?>;
		
		// VALORES DOS CAMPOS ITEM FINANCEIRO VÁLIDOS
		var financeiro_naturezas_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_natureza );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?> financeiro_naturezas_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>		
		
		
		// VALORES DOS CAMPOS DESCRIÇÃO	
		var financeiro_descricao = new Array();
		<?
//			$dados_temp = $db->carregar( $sql_financeiro_descricao );
//			if ( $dados_temp )
//			{
//				foreach ( $dados_temp as $dados_temp_valor )
//				{
//					?> financeiro_descricao['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
//				}
//			}
		?>
		
		function mostra_esconde_tr( item )
		{
			var tr = document.getElementById(item);
			if ( !tr )
			{
				return;
			}
			var fechado = tr.style.display == 'none';
			if ( fechado )
			{
				if ( document.all ) // ms ie
				{
					tr.style.display = 'block';
				}
				else // resto do mundo
				{
					tr.style.display = 'table-row';
				}
			}
			else
			{
				tr.style.display = 'none';
			}
		}
		
		function item_financeiro_hilight( id_tr, loop )
		{
			var tr = document.getElementById( id_tr );
			if ( !tr )
			{
				return;
			}
			if ( loop < 1 )
			{
				item_financeiro_organizar_cor_linha();
				return;
			}
			var cor = '';
			switch ( loop-- % 9 )
			{
				case 0:	case 8:
					//cor = '#d0d0d0';
					cor = '#ffffff';
					break;
				case 1:	case 7:
					//cor = '#d8d8d8';
					cor = '#ffffdd';
					break;
				case 2:	case 6:
					//cor = '#e0e0e0';
					cor = '#ffffaa';
					break;
				case 3:	case 5:
					//cor = '#e8e8e8';
					cor = '#ffff80';
					break;
				case 4:
					//cor = '#f0f0f0';
					cor = '#ffff40';
					break;
			}
			tr.style.backgroundColor = cor;
			var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
			setTimeout( funcao_hilight, 120 );
		}

        /**
         * Transforma uma string com vírgulas e pontos em um número real.
         * 
         * @param string
         * @return float
         */
        function desformata_decimal( valor )
        {
            if ( valor == null )
            {
                return 0.0;
            }
            valor += '';
            valor = valor.replace( /\./g, '' );
            valor = valor.replace( ',', '.' );
            if ( valor[valor.length] == '.' )
            {
                valor += '0';
            }
            if ( valor.length == 0 )
            {
                return 0.0;
            }
            return parseFloat( valor ) - 0;
        }

		/**
		 * Adiciona um item à tabela de itens de despesa. Também é adiciona
		 * um campo no formulário para as linhas.
		 * 
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return boolean
		 */
		function item_financeiro_adicionar(natureza, descricao, valor)
		{
			valor_int = desformata_decimal( valor );
			
			var valor_round = Math.round( valor * 100 ) / 100;
			
			valor_int = valor_round;
			
			if ( valor_int <= 0 )
			{
				alert( 'Valor deve ser maior que zero.' );
				document.getElementById( 'novo_valor' ).focus();
				return false;
			}

			var id_novo_item = natureza + descricao;

			// verifica se item já existe
			if ( item_financeiros_adicionados[id_novo_item] )
			{
				alert( 'Já existe item financeiro para esta natureza e descricao. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_adicionados[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;
			}

			// define dados gerais do item a ser adicionado
			item_financeiro_id_lancado++;

			var tabela = document.getElementById('tabela_financeiro');
			var id     = 'item_financeiro_' + item_financeiro_id_lancado;
			item_financeiros_adicionados[id_novo_item] = id;

			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro, no fim desta.

			var nova_linha             = tabela.insertRow(tabela.rows.length);
			    nova_linha.id          = id;
			    nova_linha.onmouseover = new Function('this.style.backgroundColor="#ffffcc";');
			// FIM cria nova linha para a tabela de financeiro

			// cria as célular da nova linha
			var celula_natureza  = nova_linha.insertCell(0);
			var celula_descricao = nova_linha.insertCell(1);
			var celula_valor     = nova_linha.insertCell(2);
			    celula_valor.style.textAlign = 'right';
			    celula_valor.style.color = '#0066cc';

			var celula_acao_alterar = nova_linha.insertCell(3);
			    celula_acao_alterar.style.textAlign = 'center';

			var celula_acao_remover = nova_linha.insertCell(4);
			    celula_acao_remover.style.textAlign = 'center';

            // FIM cria as célular da nova linha

			// cria campos ocultos com os dados do novo item

			var input_hidden_natureza  = item_financeiro_criar_input( natureza, 'natureza' );
			var input_hidden_descricao = item_financeiro_criar_input( descricao, 'descricao' );
			var input_hidden_valor     = item_financeiro_criar_input( valor, 'valor' );
				input_hidden_valor.id = id + '_valor';

			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de ação
            <?php echo "permissao_momento = " , ($permissao_momento ? 'true' : 'false') , ";\n"; ?>

			if ( !permissao_momento || ( item_financeiro_habilita_gnd_1 == false && natureza.substr( 1, 1 ) == '1' ) )
			{
				link_acao_alterar = document.createElement( 'font' );
				link_acao_alterar.innerHTML = ' - ';
				link_acao_remover = document.createElement( 'font' );
				link_acao_remover.innerHTML = ' - ';
			}
			else
			{
				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_alterar( \'' + id + '\', \'' + natureza + '\', \'' + descricao + '\', \'' + valor + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_financeiro_remover( \'' + id + '\', true );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
			}

			link_acao_alterar.id = id + '_ancora';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_natureza.appendChild( document.createTextNode( natureza ) );
			celula_natureza.appendChild( input_hidden_natureza );
			celula_descricao.appendChild( document.createTextNode( descricao ) );
			celula_descricao.appendChild( input_hidden_descricao );			
			celula_valor.appendChild( document.createTextNode( valor ) );
			celula_valor.appendChild( input_hidden_valor );
			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}
			// FIM insere os dados ( textos e inputs ) nas células
			// altera soma dos valores de acordo com o novo item
			item_financeiro_valor_lancado += desformata_decimal( valor );
			item_financeiro_atualizar_valor_total();
			// FIM altera soma dos valores de acordo com o novo item

			// atualiza as cores alternadas das linhas
			item_financeiro_organizar_cor_linha();
			return true;
		}
		
		
		/**
		 * O mesmo para Despesas
		 */
		function item_financeiro_adicionar_despesa(natureza, descricao, valor)
		{
			valor_int = desformata_decimal( valor );
			
			var valor_round = Math.round( valor * 100 ) / 100;
			
			valor_int = valor_round;
			
			if ( valor_int <= 0 )
			{
				alert( 'Valor deve ser maior que zero.' );
				document.getElementById( 'novo_valor_despesa' ).focus();
				return false;
			}

			var id_novo_item = natureza + descricao;

			// verifica se item já existe
			if ( item_financeiros_adicionados_despesa[id_novo_item] )
			{
				alert( 'Já existe item financeiro para esta natureza e descricao. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_adicionados_despesa[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados_despesa[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;
			}

			// define dados gerais do item a ser adicionado
			item_financeiro_id_lancado_despesa++;

			var tabela = document.getElementById('tabela_financeiro_despesa');
			var id     = 'item_financeiro_despesa' + item_financeiro_id_lancado_despesa;
			item_financeiros_adicionados_despesa[id_novo_item] = id;

			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro, no fim desta.

			var nova_linha             = tabela.insertRow(tabela.rows.length);
			    nova_linha.id          = id;
			    nova_linha.onmouseover = new Function('this.style.backgroundColor="#ffffcc";');
			// FIM cria nova linha para a tabela de financeiro

			// cria as célular da nova linha
			var celula_natureza  = nova_linha.insertCell(0);
			var celula_descricao = nova_linha.insertCell(1);
			var celula_valor     = nova_linha.insertCell(2);
			    celula_valor.style.textAlign = 'right';
			    celula_valor.style.color = '#0066cc';

			var celula_acao_alterar = nova_linha.insertCell(3);
			    celula_acao_alterar.style.textAlign = 'center';

			var celula_acao_remover = nova_linha.insertCell(4);
			    celula_acao_remover.style.textAlign = 'center';

            // FIM cria as célular da nova linha

			// cria campos ocultos com os dados do novo item

			var input_hidden_natureza  = item_financeiro_criar_input( natureza, 'natureza_despesa' );
			var input_hidden_descricao = item_financeiro_criar_input( descricao, 'descricao_despesa' );
			var input_hidden_valor     = item_financeiro_criar_input( valor, 'valor_despesa' );
				input_hidden_valor.id = id + '_valor_despesa';

			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de ação
            <?php echo "permissao_momento = " , ($permissao_momento ? 'true' : 'false') , ";\n"; ?>

			if ( !permissao_momento || ( item_financeiro_habilita_gnd_1 == false && natureza.substr( 1, 1 ) == '1' ) )
			{
				link_acao_alterar = document.createElement( 'font' );
				link_acao_alterar.innerHTML = ' - ';
				link_acao_remover = document.createElement( 'font' );
				link_acao_remover.innerHTML = ' - ';
			}
			else
			{
				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_alterar_despesa( \'' + id + '\', \'' + natureza + '\', \'' + descricao + '\', \'' + valor + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_financeiro_remover_despesa( \'' + id + '\', true );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';
			}

			link_acao_alterar.id = id + '_ancora_despesa';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_natureza.appendChild( document.createTextNode( natureza ) );
			celula_natureza.appendChild( input_hidden_natureza );
			celula_descricao.appendChild( document.createTextNode( descricao ) );
			celula_descricao.appendChild( input_hidden_descricao );			
			celula_valor.appendChild( document.createTextNode( valor ) );
			celula_valor.appendChild( input_hidden_valor );
			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}
			// FIM insere os dados ( textos e inputs ) nas células
			// altera soma dos valores de acordo com o novo item
			item_financeiro_valor_lancado_despesa += desformata_decimal( valor );
			item_financeiro_atualizar_valor_total_despesa();
			// FIM altera soma dos valores de acordo com o novo item

			// atualiza as cores alternadas das linhas
			item_financeiro_organizar_cor_linha_despesa();
			return true;
		}
		
		/**
		 * Insere item financeiro na tabela de financeiro com
		 * os dados atuais do formulário. As verificações de
		 * preenchimento dos dados são realizada nesta função.
		 * 
		 * @return void
		 */
		function item_financeiro_adicionar_do_formulario()
		{
			// captura os campos do financeiro
			var natureza  = document.getElementById('nova_natureza');
			var descricao = document.getElementById('nova_descricao');
			var valor     = document.getElementById('novo_valor');

			// realiza validação dos dados
			if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
			if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
			if ( !financeiro_naturezas_validas[natureza.value] )
			{
				alert( 'Natureza de Despesa inválida.' );
				natureza.focus();
				natureza.select();
				return;
			}

			// insere os dados na tabela de financeiro
			if (!item_financeiro_adicionar(natureza.value, descricao.value, valor.value))
			{
				return;
			}

			// limpa campos para nova inserção
			document.getElementById('nova_natureza').value  = '';
			document.getElementById('nova_descricao').value  = '';
			document.getElementById('novo_valor').value  = '';

			//item_financeiro_atualizar_fisico();
			//item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById('nova_natureza').focus();
		}
		
		
		/**
		 * O mesmo para Despesa
		 * 
		 * @return void
		 */
		function item_financeiro_adicionar_do_formulario_despesa()
		{
			// captura os campos do financeiro
			var natureza  = document.getElementById('nova_natureza_despesa');
			var descricao = document.getElementById('nova_descricao_despesa');
			var valor     = document.getElementById('novo_valor_despesa');

			// realiza validação dos dados
			if ( !validaBranco( natureza, 'Natureza da Despesa' ) ) return;
			if ( !validaBranco( valor, 'Valor da Proposta' ) ) return;
			if ( !financeiro_naturezas_validas[natureza.value] )
			{
				alert( 'Natureza de Despesa inválida.' );
				natureza.focus();
				natureza.select();
				return;
			}
			
			// insere os dados na tabela de financeiro
			if (!item_financeiro_adicionar_despesa(natureza.value, descricao.value, valor.value))
			{
				return;
			}

			// limpa campos para nova inserção
			document.getElementById('nova_natureza_despesa').value  = '';
			document.getElementById('nova_descricao_despesa').value  = '';
			document.getElementById('novo_valor_despesa').value  = '';

			//item_financeiro_atualizar_fisico();
			//item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById('nova_natureza_despesa').focus();
		}

		/**
		 * Prepara formulário para edição de um item da tabela financeiro.
		 * 
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return void
		 */
		function item_financeiro_alterar( id_tr, natureza, descricao, valor )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_natureza = document.getElementById( 'nova_natureza' );			
			var campo_descricao = document.getElementById( 'nova_descricao' );
			var campo_valor = document.getElementById( 'novo_valor' );
			campo_natureza.value = natureza;			
			campo_descricao.value = descricao;
			campo_valor.value = valor;
			item_financeiro_remover( id_tr, false );
			// retira item da lista dos já inseridos
			var id_novo_item = natureza + descricao;
			item_financeiros_adicionados[id_novo_item] = false;
		}
		
		/**
		 * Mesmo q acima para Depesa
		 */
		function item_financeiro_alterar_despesa( id_tr, natureza, descricao, valor )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_natureza = document.getElementById( 'nova_natureza_despesa' );			
			var campo_descricao = document.getElementById( 'nova_descricao_despesa' );
			var campo_valor = document.getElementById( 'novo_valor_despesa' );
			campo_natureza.value = natureza;			
			campo_descricao.value = descricao;
			campo_valor.value = valor;
			item_financeiro_remover_despesa( id_tr, false );
			// retira item da lista dos já inseridos
			var id_novo_item = natureza + descricao;
			item_financeiros_adicionados_despesa[id_novo_item] = false;
		}
		
		/**
		 * Detecta qual o campo focado atualmente e altera para o
		 * próxima caso a tecla apertada seja 'enter'.
		 * 
		 * @param void
		 * @return void
		 */
		function item_financeiro_alterar_campo_focado(event, campo)
		{
			if (event.keyCode != 13)
			{
				return;
			}

			switch (campo)
			{
				case 'natureza':
					var campo = document.getElementById('nova_descricao');
					break;
				case 'descricao':
					var campo = document.getElementById('novo_valor');
					break;
				case 'valor':
					item_financeiro_adicionar_do_formulario();
					return;
				case 'natureza_despesa':
					var campo = document.getElementById('nova_descricao_despesa');
					break;
				case 'descricao_despesa':
					var campo = document.getElementById('novo_valor_despesa');
					break;
				case 'valor_despesa':
					item_financeiro_adicionar_do_formulario_despesa();
					return;					
				default:
					return;
			}

			campo.focus();
			campo.select();
		}
		
		
		
		
		//configurando a quantidade disponivel inicial do disponivel	

		var disponivel =  desformata_decimal(document.getElementById('item_financeiro_valor_total_investimento').value) - desformata_decimal(document.getElementById('item_financeiro_valor_lancado').value);
		
		disponivel = disponivel + '';
		disponivel = MascaraMonetario(disponivel);
		document.getElementById('item_financeiro_valor_disponivel').value  = disponivel
		
		/**
		 * Atualiza o dado da tela de acordo com a variável
		 * item_financeiro_valor_total.
		 * 
		 * @param void
		 * @return void
		 */
		function item_financeiro_atualizar_valor_total()
		{
			var valor_total_string = item_financeiro_valor_lancado + '';
			valor_total_string = MascaraMonetario( valor_total_string );
			//valor_total_string = valor_total_string.substr( 0, valor_total_string.length - 3 );
			//document.getElementById( 'item_financeiro_valor_total_investimento' ).innerHTML = valor_total_string;
			document.getElementById( 'item_financeiro_valor_lancado' ).value = valor_total_string;
			
			
			var valorLancado = item_financeiro_valor_lancado;
			var valorTotal = desformata_decimal( document.getElementById( 'item_financeiro_valor_total_investimento' ).value );
			var valorDisponivel =  valorTotal  - ( Math.round( valorLancado * 100 ) / 100 );
			
			var valorFinal = valorDisponivel + '';
			valorFinal = MascaraMonetario( valorFinal );
			//valorFinal = valorFinal.substr( 0, valorFinal.length - 3 );
			 
			document.getElementById( 'item_financeiro_valor_disponivel' ).value = valorFinal;
			/*
			
			var valor_disponivel_string = desformata_decimal(document.getElementById('item_financeiro_valor_total_investimento').value) - desformata_decimal(document.getElementById( 'item_financeiro_valor_lancado' ).value)
			document.getElementById('item_financeiro_valor_disponivel').value = valor_disponivel_string;
			*/
		}
		
		/**
		 * Mesmo q acima para Depesa
		 */
		function item_financeiro_atualizar_valor_total_despesa()
		{
			var valor_total_string = item_financeiro_valor_lancado_despesa + '';
			valor_total_string = MascaraMonetario( valor_total_string );
			//valor_total_string = valor_total_string.substr( 0, valor_total_string.length - 3 );
			document.getElementById( 'item_financeiro_valor_lancado_despesa' ).value = valor_total_string;
			
			var valorLancado = item_financeiro_valor_lancado_despesa;
			var valorTotal = desformata_decimal( document.getElementById( 'item_financeiro_valor_total_despesa' ).value );
			var valorDisponivel =  valorTotal  - ( Math.round( valorLancado * 100 ) / 100 );
			
			var valorFinal = valorDisponivel + '';
			valorFinal = MascaraMonetario( valorFinal );
			//valorFinal = valorFinal.substr( 0, valorFinal.length - 3 );
			 
			document.getElementById( 'item_financeiro_valor_disponivel_despesa' ).value = valorFinal;
			
			/*
			
			var valor_total_string = item_financeiro_valor_lancado_despesa + '';
			valor_total_string = MascaraMonetario( valor_total_string );
			valor_total_string = valor_total_string.substr( 0, valor_total_string.length - 3 );
			//document.getElementById( 'item_financeiro_valor_total_despesa' ).innerHTML = valor_total_string;
			document.getElementById( 'item_financeiro_valor_lancado_despesa' ).value = valor_total_string;
			
			var  valor_disponivel_string_despesa = desformata_decimal(document.getElementById('item_financeiro_valor_total_despesa').value) - desformata_decimal(document.getElementById( 'item_financeiro_valor_lancado_despesa' ).value);
			var valor_total_string_saida = valor_disponivel_string_despesa + '';
			valor_total_string_saida = MascaraMonetario( valor_total_string );
			valor_total_string_saida = valor_total_string_saida.substr( 0, valor_total_string_saida.length - 3 );
			document.getElementById('item_financeiro_valor_disponivel_despesa').value = valor_total_string_saida;
			*/
		}	
		
		/**
		 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
		 * 
		 * @return void
		 */
		function item_financeiro_organizar_cor_linha()
		{
			var tabela = document.getElementById( 'tabela_financeiro' );
			var quantidade_tr = tabela.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}
		
		/**
		 * O mesmo q acima para Despesa
		 */
		function item_financeiro_organizar_cor_linha_despesa()
		{
			var tabela = document.getElementById( 'tabela_financeiro_despesa' );
			var quantidade_tr = tabela.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}
		
		/**
		 * Cria o conteúdo de uma célula da tabela de itens de despesa
		 * 
		 * @param string
		 * @param string
		 * @return object
		 */
		function item_financeiro_criar_input( valor, nome_campo )
		{
			var input = document.createElement( 'input' );
			input.type = 'hidden';
			input.name = nome_campo + '[]';
			input.value = valor;
			return input;
		}

		/**
		 * Remove um item da tabela de financeiro.
		 * 
		 * @param string
		 * @param boolean
		 * @return void
		 */
		function item_financeiro_remover( id_tr, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor' );
			if ( tr && input )
			{
				item_financeiro_valor_lancado -= desformata_decimal( input.value );
				item_financeiro_atualizar_valor_total();
				tr.parentNode.removeChild( tr );
				item_financeiro_organizar_cor_linha();
			}
			//item_financeiro_atualizar_fisico();
		}		
		
		/**
		 * Mesmo q acima pra Despesa
		 */
		function item_financeiro_remover_despesa( id_tr, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor_despesa' );
			if ( tr && input )
			{
				item_financeiro_valor_lancado_despesa -= desformata_decimal( input.value );
				item_financeiro_atualizar_valor_total_despesa();
				tr.parentNode.removeChild( tr );
				item_financeiro_organizar_cor_linha();
			}
			//item_financeiro_atualizar_fisico_despesa();
		}
		
		/**
	 * Verifica se formulário este correto e o submete.
	 * 
	 * @return void
	 */
	function validar_cadastro()
	{
		var valor_disponivel= desformata_decimal( document.getElementById( 'item_financeiro_valor_disponivel' ).value.replace( /\./g, '' ) );
		//var valor_total = desformata_decimal( document.getElementById( 'item_financeiro_valor_total_investimento' ).value.replace( /\./g, '' ) );
		
		//var valor_disponivel_despesa = desformata_decimal( document.getElementById( 'item_financeiro_valor_disponivel_despesa' ).value.replace( /\./g, '' ) );
		//var valor_total_despesa = desformata_decimal( document.getElementById( 'item_financeiro_valor_total_despesa' ).value.replace( /\./g, '' ) );
		
		
		if ( valor_disponivel < 0 )
		{
			alert( 'Dados inválidos. Saldo insuficiente para os valores lançados em Invetimento.' );
			return;
		}
		
		/*if ( valor_disponivel_despesa < 0 )
		{
			alert( 'Dados inválidos. Saldo insuficiente para os valores lançados em Outras Despesas.' );
			return;
		}*/
		
		// submete
		var formulario = document.formulario;
				
		document.formulario.act.value = 'gravar'; // altera status do formulário		
		document.formulario.submit();
	}

		// insere itens do financeiro existentes anteriores à exibição do formulário
		document.getElementById( 'item_financeiro_valor_lancado' ).value = '0';
		<? foreach ( $itens_financeiro as $item ) : ?>
			item_financeiro_adicionar( '<?= $item['natureza'] ?>', '<?= $item['descricao'] ?>', '<?= $item['valor'] ?>' );
		<? endforeach; ?>
		// preenche campos do formulario de item finacneiro com valores padrões
		//item_financeiro_preencher_valores_default();
		
		// insere itens do financeiro de despesa existentes anteriores à exibição do formulário
		//document.getElementById( 'item_financeiro_valor_lancado_despesa' ).value = '0';
		//<? foreach ( $itens_financeiro_despesa as $item_despesa ) : ?>
		//	item_financeiro_adicionar_despesa( '<?= $item_despesa['natureza'] ?>', '<?= $item_despesa['descricao'] ?>', '<?= $item_despesa['valor'] ?>' );
		//<? endforeach; ?>
		// preenche campos do formulario de item finacneiro com valores padrões
		//item_financeiro_preencher_valores_default();
		
		
		document.getElementById( 'nova_natureza' ).focus();
		
		
		
	// FIM JAVASCRIPT FINANCEIRO

--></script>
