<?php


include APPRAIZ . "includes/cabecalho.inc";

// carrega unidades permitidas
$sqlUnidadesPermitidas =
"
	select u.unicod
	from monitora.usuarioresponsabilidade u
		inner join perfil p on u.pflcod = p.pflcod
	where
		u.usucpf = '" . $_SESSION['usucpf'] . "' and
		u.prsano = '" . $_SESSION['exercicio'] . "' and
		u.rpustatus = 'A' and
		( p.pflcod = 112 or p.pflcod = 113 )
	group by u.unicod
";
$cods = $db->carregar( $sqlUnidadesPermitidas );
$cods = $cods ? $cods : array();
$unicods = array();
foreach ( $cods as $cod )
{
	array_push( $unicods, $cod['unicod'] );
}
//$unicods = $db->usuarioUnidadesPermitidas( "monitora" );
//$unicods = array( '26210' );

// carrega referencias
$sql = "
	select
		refcod,
		refdsc
	from monitora.referencia
	where
		refdata_limite_parecer_aca is not null and
		refsngrupo = 't' and
		refano_ref = '" . $_SESSION['exercicio'] . "' and
		refsnmonitoramento = 't'
	order by refmes_ref
";
$meses = $db->carregar( $sql );
$referencias = array();
if($meses[0]) {
foreach ( $meses as $mes )
{
	array_push( $referencias, $mes['refcod'] );
}
}

// monta titulo da página
echo '<br>';

$db->cria_aba( $abacod_tela,$url,'');
$subtitulo_modulo = '<b>Legenda:</b> 	<img src="../imagens/valida0.gif" border="0" width="15" height="15"/> Não Validado | 
						<img src="../imagens/valida1.gif" border="0" width="15" height="15"/> Validado com Concordância | 
						<img src="../imagens/valida3.gif" border="0" width="15" height="15"/> Validado com Discordância | 
						<img src="../imagens/valida2.gif" border="0" width="15" height="15"/> Pendente <br>Clique no Ícone correspondente à Ação e Trimestre Desejado para Validar as Ações abaixo.';
monta_titulo( $titulo_modulo, $subtitulo_modulo );

?>
<?php if( $unicods ): ?>
	<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
		<tbody>
			<?php foreach( $unicods as $unicod ): ?>
				<?php
					
					if($referencias) {
						
						$sql = sprintf(
							"select
								a.prgid,
								a.acaid,
								a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod as codigo,
						 		a.acadsc,
								a.sacdsc,
								a.unicod,
								u.unidsc,
								case
									when cor.corcod is null then 0
									else cor.corcod end as parecer,
								case
									when cor.corsignificadopar is null then 'Não Validado'
									else cor.corsignificadopar end as situacao,
								ref.refcod,
								ref.refmes_ref as mes,
								ref.refano_ref as ano
							from monitora.acao a
								left join monitora.referencia ref on
									ref.refdata_limite_parecer_aca is not null and
									ref.refsngrupo = true and
									ref.refstatus = 'A' and
									ref.refcod in ( %s ) 
								left join monitora.avaliacaoparecer pa on
									a.acaid = pa.acaid and
									pa.tpaid = 2 and
									pa.refcod = ref.refcod and
									pa.avpstatus = 'A'
								left join cor on
									pa.avpsituacao = cor.corcod
								inner join public.unidade u on
									u.unicod = a.unicod and u.unitpocod = 'U' and u.unistatus = 'A'
							where
								a.prgano = '%s' and
								a.acastatus = 'A' and
								acasnrap = 'f' and
								a.unicod = '%s' and
								ref.refdata_limite_parecer_aca is not null and
								ref.refsnmonitoramento = 't'
							order by
								a.prgcod,
								a.acacod,
								a.unicod,
								a.loccod,
								a.acaid,
								ref.refmes_ref
						",
							implode( ',', $referencias ),
							$_SESSION['exercicio'],
							$unicod
						);
						$registros = $db->carregar( $sql );
					
					}
					$acoes = array();
					if ( $registros )
					{
						foreach( $registros as $registro )
						{
							if ( !array_key_exists( $registro['acaid'], $acoes ) )
							{
								$acoes[$registro['acaid']] = array(
									'prgid'   => $registro['prgid'],
									'acaid'   => $registro['acaid'],
									'codigo'  => $registro['codigo'],
									'acadsc'  => $registro['acadsc'],
									'sacdsc'  => $registro['sacdsc'],
									'unicod'  => $registro['unicod'],
									'unidsc'  => $registro['unidsc'],
									'situacoes' => array()
								);
							}
							$situacao = array(
								'avaliacao' => $registro['avaliacao'] ? $registro['avaliacao'] : 0,
								'parecer'   => $registro['parecer'] ? $registro['parecer'] : 0,
								'situacao'  => $registro['situacao'] ? $registro['situacao'] : 0,
								'refcod'    => $registro['refcod'],
								'mes'       => $registro['mes'],
								'ano'       => $registro['ano']
							);
							if ( $situacao['situacao'] == 1 )
							{
								$situacao['cor_parecer'] = $situacao['cor_avaliacao'];
							}
							$acoes[$registro['acaid']]['situacoes'][] = $situacao;
						}
						$acoes = array_values( $acoes );
					}
					
				?>
				<tr style="background-color: #e7e7e7;">
					<td colspan="2" style="font-size: 10pt; padding-left: 5px;">
						<?php echo $acoes[0]['unicod']; ?> -
						<?php echo $acoes[0]['unidsc']; ?>
					</td>
					<td colspan="<?php echo 1 + count( $acoes[0]['situacoes'] ) ?>">
						Responsabilidade:
						<span style="color: #902020;">
							<?php
								$sqlPerfil =
								"
									select pfldsc
									from monitora.usuarioresponsabilidade u
										inner join perfil p on u.pflcod = p.pflcod
									where
										usucpf = '" . $_SESSION['usucpf'] . "' and
										rpustatus = 'A' and
										u.unicod = '" . $acoes[0]['unicod'] . "'
								";
								$perfil = $db->pegaUm( $sqlPerfil );
								echo $perfil ? $perfil : "sem perfil nesta unidade";
							?>
						</span>
					</td>
				</tr>
				<tr style="background-color: #e7e7e7;">
					<td align="center">Código</td>
					<td align="center">Descrição</td>
					<td align="center">Localizador</td>
					<?php $mesdsc = array(); ?>
					<?php foreach( $meses as $mes ): ?>
						<td align="center"><?= $mes['refdsc'] ?></td>
						<?php $mesdsc[$mes['refcod']] = $mes['refdsc']; ?>
					<?php endforeach; ?>
				</tr>
				<?php foreach( $acoes as $indice => $acao ): ?>
				<tr style="background-color: <?= $indice % 2 ? '#ffffff' : '#f5f5f5' ?>">
					<td style="text-align:center; padding-left: 15px;">
						<a href="?modulo=principal/acao/cadacao&acao=C&acaid=<?= $acao['acaid'] ?>&prgid=<?= $acao['prgid'] ?>">
							<?= $acao['codigo'] ?>
						</a>
					</td>
					<td><?= $acao['acadsc'] ?></td>
					<td><?= $acao['sacdsc'] ?></td>
					<?php foreach( $acao['situacoes'] as $situacao ): ?>
						<td style="text-align:center; width:50px;">
							<a href="monitora.php?modulo=principal/unidade/validar&acao=A&refcod=<?= $situacao['refcod'] ?>&unicod=<?= $acao['unicod'] ?>">
								<img
									src="../imagens/valida<?= $situacao['parecer'] ?>.gif"
									title="<?= $mesdsc[$situacao['refcod']] . " - " . $situacao['situacao'] ?>"
									align="absmiddle"
									border="0"
									width="15"
									height="15"
								/>
							</a>
						</td>
					<?php endforeach; ?>
				</tr>
			<?php endforeach; ?>
			<?php endforeach; ?>
		</tbody>
	</table>
<?php else: ?>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="10" class="listagem">
	<tbody>
		<td style="text-align: center; background-color: #ffffff">
			<p>Não há unidades sob sua responsabilidade.</p>
		</td>
	</tbody>
</table>
<?php endif; ?>