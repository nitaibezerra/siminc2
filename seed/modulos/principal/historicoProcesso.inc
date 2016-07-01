<?php
$obSeed = new Seed();
$obProcesso = new Processos();


include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

$prcid = $_SESSION['seed']['prcid'];
$obSeed->validaSession( $prcid );

monta_titulo('SEED', 'Histórico');

$obSeed->montaCabecalhoProcesso( $prcid );

$historico = $obProcesso->carregaHistoricoProcesso( $prcid );

?>

<form action="" method="post" name="formulario">
			<table class="tabela" cellspacing="0" cellpadding="3" align="center">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
							<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
							<div><?php echo $documento['docdsc']; ?></div>
						</td>
					</tr>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 20px;"><b>Seq.</b></td>
							<td style="width: 200px;"><b>Onde Estava</b></td>
							<td style="width: 200px;"><b>O que aconteceu</b></td>
							<td style="width: 90px;"><b>Quem fez</b></td>
							<td style="width: 120px;"><b>Quando fez</b></td>
							<td style="width: 17px;">&nbsp;</td>
						</tr>
					<?php endif; ?>
				</thead>
				<?php $i = 1; ?>
				<?php foreach ( $historico as $item ) : ?>
					<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
					<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td align="right"><?=$i?>.</td>
						<td style="color:#008000;">
							<?php echo $item['sitdsc']; ?>
						</td>
						<td valign="middle" style="color:#133368">
							<?php echo $item['hstdsc']; ?>
						</td>
						<td style="font-size: 6pt;">
							<?php echo $item['usunome']; ?>
						</td>
						<td style="color:#133368">
							<?php echo $item['data']; ?>
						</td>
						<td style="color:#133368; text-align: center;">
							<?php if( $item['cmddsc'] ) : ?>
								<img
									align="middle"
									style="cursor: pointer;"
									src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
									onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
								/>
							<?php endif; ?>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
			</table>
		</form>