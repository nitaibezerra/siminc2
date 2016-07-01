<?php
function cte_montaTitulo( $titulo, $subtitulo = '&nbsp;' )
{
	global $db;
	monta_titulo( $titulo, $subtitulo );
	$estuf = cte_pegarEstuf( $_SESSION['inuid'] );
	$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
	if ( $estuf )
	{
		$descricao = cte_pegarEstdescricao( $estuf );
	}
	else if ( $muncod )
	{
		$descricao = cte_pegarMundescricao( $muncod );
	}
	else
	{
		return;
	}
	
	$percentagem = cte_pegarPercentagem( $_SESSION['inuid'] );
	$estado_documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-bottom: 0 !important;">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td style="padding: 0 15px 0 15px; background-color:#fafafa; color:#404040;">
					<div style="float: left; position: relative;">
						<h3 title="Unidade da Federação">
							<a href="?modulo=principal/estrutura_avaliacao&acao=A"><?= $descricao ?></a>
						</h3>
					</div>
					<?php if( $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO ): ?>
					<div style="float: right; text-align: right; position: relative; top: 15px; margin-bottom: 30px;">
						<?= $percentagem ?>%
						<div style="margin-left: 0; padding: 1px; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;" title="<?= $percentagem ?>%">
						<div style="font-size:4px;width: <?= $percentagem ?>%; height: 6px; max-height: 6px; background-color:#339933;">
					</div>
					<?php endif; ?>
				</td>
			</tr>
		</tbody>
	</table>
	<?php
}

function cte_desenhaRelatorio( array $itens, $exibeSol, $exibeAte, $exibeFis, $exibeFin, $profundidade = 0 )
{
	if ( count( $itens ) == 0 )
	{
		return;
	}
	
	// verifica quais campos de valores devem aparecer
	$exibeFis = (boolean) $exibeFis;
	$exibeFin = (boolean) $exibeFin;
	
	$exibeSol = (boolean) $exibeSol;
	$exibeAte = (boolean) $exibeAte;
	
	$rowspan = 0;
	$rowspan += $exibeSol ? 1 : 0;
	$rowspan += $exibeAte ? 1 : 0;
	
	$padding = $profundidade * 25;
		
	foreach ( $itens as $agrupador => $item )
	{
		$boIgnora = true;
		
		if( $exibeSol ){
			if( $exibeFin )
				if( array_sum( $item['fin_sol'] ) ) $boIgnora = false;
			if( $exibeFis )
				if( array_sum( $item['fis_sol'] ) ) $boIgnora = false;				
		}
		if( $exibeAte ){
			if( $exibeFin )
				if( array_sum( $item['fin_ate'] ) ) $boIgnora = false;
			if( $exibeFis )
				if( array_sum( $item['fis_ate'] ) ) $boIgnora = false;
		}
		if( !$exibeFis && !$exibeFin ){
			$boIgnora = false;
		}
		
		if( $boIgnora ) continue;
		
		?>
		<tr>
			<td style="padding-left: <?= $padding ?>px; background-color: #efefef;" colspan="9">
				<?php if ( $profundidade > 0 ) : ?>
					<img src="/imagens/seta_filho.gif" align="absmiddle"/>
				<?php endif; ?>
				<b><?= $agrupador ?></b>
			</td>
		</tr>
		<?php if ( $exibeFis ) : ?>
			<tr>
				<td style="padding-left: <?= $padding ?>px; color: #606060;" rowspan="<?= $rowspan ?>">
					Físico
				</td>
				<?php if ( $exibeSol ) : ?>
					<? cte_desenhaValores( 'Original', $item['fis_sol'], 0 ) ?>
				<?php elseif ( $exibeAte ) : ?>
					<? cte_desenhaValores( 'Atual', $item['fis_ate'], 0 ) ?>
				<?php endif; ?>
			</tr>
			<?php if ( $exibeSol && $exibeAte ) : ?>
				<tr>
					<? cte_desenhaValores( 'Atual', $item['fis_ate'], 0 ) ?>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
		<?php if ( $exibeFin ) : ?>
			<tr>
				<td style="padding-left: <?= $padding ?>px; color: #606060;" rowspan="<?= $rowspan ?>">
					Financeiro
				</td>
				<?php if ( $exibeSol ) : ?>
					<? cte_desenhaValores( 'Original', $item['fin_sol'], 2, true ) ?>
				<?php elseif ( $exibeAte ) : ?>
					<? cte_desenhaValores( 'Atual', $item['fin_ate'], 2, true ) ?>
				<?php endif; ?>
			</tr>
			<?php if ( $exibeSol && $exibeAte ) : ?>
				<tr>
					<? cte_desenhaValores( 'Atual', $item['fin_ate'], 2, true ) ?>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
		<?php
		cte_desenhaRelatorio( $item['sub'], $exibeSol, $exibeAte, $exibeFis, $exibeFin, $profundidade + 1 );
	}
}

function cte_desenhaValores( $label, $valores, $casas,  $boFinanceiro = false )
{
	$casas = (integer) $casas; ?>
	
	<td align="center" style="color: #606060;">
		<?= $label ?>
	</td>
	<?php for( $i=0; $i<6; $i++ ){ ?>
		<td align="right" style="color: #508050;">
			<?php if( $boFinanceiro ){
			$valores[$i] = $valores[$i];
			}
			?>		
			<?php echo number_format( $valores[$i], $casas, ",", "." ); ?>
		</td>
	<?php } ?>
	<td align="right" style="color: #105010;">
		<?= number_format( $valores[0] + $valores[1] + $valores[2] + $valores[3] + $valores[4] + $valores[5], $casas, ",", "." ) ?>
	</td>
	<?php
}
?>