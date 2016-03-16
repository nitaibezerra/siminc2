<?php

function agrupar( $lista, $campo )
{
	$a = array();
	if ( !empty( $lista ) ) {
		foreach( $lista as $item ){
			if ( !isset( $a[$item[$campo]] ) ) {
				$a[$item[$campo]] = array();
			}
			array_push( $a[$item[$campo]], $item );
		}
	}
	return $a;
}

function restringir( $campo, $tabela ){
	$condicao = "";
	if ( !empty( $_REQUEST[$campo] ) ) {
		$condicao = " and {$tabela}.{$campo} = {$_REQUEST[$campo]} ";
	}
	return $condicao;
}

$restricao_dimensao = restringir( "dimid", "d" );
$restricao_area = restringir( "ardid", "ad" );
$restricao_indicador = restringir( "indid", "i" );

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

$sql_dimensao = sprintf( "
	select distinct d.dimid, d.dimcod, d.dimdsc, d.itrid
	from cte.dimensao d
	inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A'
	inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A'
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where d.dimstatus = 'A'
	order by d.dimcod"
);
$dimensoes_combo = $db->carregar( $sql_dimensao );
$dimensoes_combo = $dimensoes_combo ? $dimensoes_combo : array();
$sql_dimensao = sprintf( "
	select distinct d.dimid, d.dimcod, d.dimdsc, d.itrid
	from cte.dimensao d
	inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A' %s
	inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' %s
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where d.dimstatus = 'A' %s
	order by d.dimcod",
	$restricao_area,
	$restricao_indicador,
	$restricao_dimensao
);
$dimensoes = $db->carregar( $sql_dimensao );
$dimensoes = $dimensoes ? $dimensoes : array();

$sql_area_combo = sprintf( "
	select distinct ad.ardid, ad.ardcod, ad.arddsc, ad.dimid
	from cte.areadimensao ad
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A'
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where ad.ardstatus = 'A'
	order by ad.ardcod"
);
$areas_combo = agrupar( $db->carregar( $sql_area_combo ), "dimid" );
$sql_area = sprintf( "
	select distinct ad.ardid, ad.ardcod, ad.arddsc, ad.dimid
	from cte.areadimensao ad
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' %s
	inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A' %s
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where ad.ardstatus = 'A' %s
	order by ad.ardcod",
	$restricao_dimensao,
	$restricao_indicador,
	$restricao_area
);
$areas = agrupar( $db->carregar( $sql_area ), "dimid" );

$sql_indicador_combo = sprintf( "
	select distinct i.indid, i.indcod, i.inddsc, i.ardid
	from cte.indicador i
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where i.indstatus = 'A'
	order by i.indcod"
);
$indicadores_combo = agrupar( $db->carregar( $sql_indicador_combo ), "ardid" );
$sql_indicador = sprintf( "
	select distinct i.indid, i.indcod, i.inddsc, i.ardid
	from cte.indicador i
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A' %s
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' %s
	inner join cte.criterio c on c.indid = i.indid
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	where i.indstatus = 'A' %s
	order by i.indcod",
	$restricao_area,
	$restricao_dimensao,
	$restricao_indicador
);
$indicadores = agrupar( $db->carregar( $sql_indicador ), "ardid" );

$sql_criterio_combo = sprintf( "
	select distinct c.crtid, c.ctrord, c.crtdsc, c.indid, c.ctrpontuacao
	from cte.criterio c
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	order by c.ctrord"
);
$criterios_combo = agrupar( $db->carregar( $sql_criterio_combo ), "indid" );
$sql_criterio = sprintf( "
	select distinct c.crtid, c.ctrord, c.crtdsc, c.indid, c.ctrpontuacao
	from cte.criterio c
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A' %s
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A' %s
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' %s
	inner join cte.proposicaoacao pa on pa.crtid = c.crtid
	order by c.ctrord",
	$restricao_indicador,
	$restricao_area,
	$restricao_dimensao
);
$criterios = agrupar( $db->carregar( $sql_criterio ), "indid" );

$sql_acoes = sprintf( "
	select pa.ppaid, pa.ppadsc, pa.crtid
	from cte.proposicaoacao pa
	inner join cte.criterio c on c.crtid = pa.crtid
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A' %s
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A' %s
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' %s",
	$restricao_indicador,
	$restricao_area,
	$restricao_dimensao
);
$acoes = agrupar( $db->carregar( $sql_acoes ), "crtid" );

$sql_subacoes = sprintf( "
	select ps.*, um.*, p.*, fe.*
	from cte.proposicaosubacao ps
	inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
	inner join cte.criterio c on c.crtid = pa.crtid
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A' %s
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A' %s
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A' %s
	left join cte.unidademedida um on um.undid = ps.undid
	left join cte.programa p on p.prgid = ps.prgid
	left join cte.formaexecucao fe on fe.frmid = ps.frmid
	order by ps.ppsordem",
	$restricao_indicador,
	$restricao_area,
	$restricao_dimensao
);
$subacoes = agrupar( $db->carregar( $sql_subacoes ), "ppaid" );

?>
<script type="text/javascript">
<!--
	
	function limpar( identificador ){
		var campo = document.getElementById( identificador );
		if ( campo ) {
			campo.selectedIndex = 0;
		}
		for ( var i = 0; i < campo.options.length; i++ ) {
			campo.options[i].disabled = false;
		}
	}
	
	function selecionarDimensao( campo ){
		limpar( 'ardid' );
		limpar( 'indid' );
		var dimid = campo.options[campo.selectedIndex].value;
		var areas = document.getElementById( 'ardid' );
		for ( var i = 0; i < areas.options.length; i++ ) {
			areas.options[i].disabled = ( areas.options[i].getAttribute( 'dimid' ) == dimid ) == false;
		}
	}
	
	function selecionarArea( campo ){
		limpar( 'indid' );
		var dimid = campo.options[campo.selectedIndex].value;
		var areas = document.getElementById( 'indid' );
		for ( var i = 0; i < areas.options.length; i++ ) {
			areas.options[i].disabled = ( areas.options[i].getAttribute( 'ardid' ) == dimid ) == false;
		}
	}
	
	function selecionarIndicador( campo ){
	}
	
//-->
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<form action="cte.php?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>" method="post" name="formulario">
					<div style="text-align: right; margin: 0 0 15px 0;" class="notprint">
						<select id="dimid" name="dimid" style="width: 200px;" onchange="selecionarDimensao( this );">
							<option/>
							<?php foreach( $dimensoes_combo as $dimensao ): ?>
								<option value="<?= $dimensao["dimid"] ?>"><?= $dimensao["dimdsc"] ?></option>
							<?php endforeach; ?>
						</select>
						<select id="ardid" name="ardid" style="width: 200px;" onchange="selecionarArea( this );">
							<option/>
							<?php foreach( $areas_combo as $dimid => $lista ): ?>
								<?php foreach( $lista as $area ): ?>
									<option dimid="<?= $dimid ?>" value="<?= $area["ardid"] ?>" style="display: block;"><?= $area["arddsc"] ?></option>
								<?php endforeach; ?>
							<?php endforeach; ?>
						</select>
						<select id="indid" name="indid" style="width: 200px;" onchange="selecionarIndicador( this );">
							<option/>
							<?php foreach( $indicadores_combo as $ardid => $lista ): ?>
								<?php foreach( $lista as $indicador ): ?>
									<option ardid="<?= $ardid ?>" value="<?= $indicador["indid"] ?>"><?= $indicador["inddsc"] ?></option>
								<?php endforeach; ?>
							<?php endforeach; ?>
						</select>
						<input type="submit" name="" value="Filtrar"/>
					</div>
				</form>
				<?php foreach( $dimensoes as $dimensao ): ?>
					<table style="width: 100%;">
						<tr>
							<td style="width: 100%; padding: 5px; font-size: 12px; background-color: #dadada; border: 1px solid #bbbbbb;">
								<?= $dimensao["dimcod"] ?>. <?= $dimensao["dimdsc"] ?>
							</td>
						</tr>
					</table>
					<?php foreach( $areas[$dimensao["dimid"]] as $area ): ?>
						<table style="width: 100%;">
							<tr>
								<td style="width: 100%; padding: 5px; font-size: 12px; background-color: #eeeeee; border: 1px solid #bbbbbb;">
									<?= $area["ardcod"] ?>. <?= $area["arddsc"] ?>
								</td>
							</tr>
						</table>
						<?php foreach( $indicadores[$area["ardid"]] as $indicador ): ?>
							<table style="width: 100%;">
								<tr>
									<td style="width: 100%; padding: 5px; font-size: 12px; background-color: #eeeeee; border: 1px solid #bbbbbb;">
										<?= $indicador["indcod"] ?>. <?= $indicador["inddsc"] ?>
									</td>
								</tr>
							</table>
							<?php foreach( $criterios[$indicador["indid"]] as $criterio ): ?>
								<table style="width: 100%;">
									<tr>
										<td style="width: 100%; padding: 5px; font-size: 12px; background-color: #eeeeee; border: 1px solid #bbbbbb;">
											<?= $criterio["crtdsc"] ?>
											<p style="padding: 0 0 0 20px; font-weight: bold;">
												<img src="../imagens/seta_pai.png"/> <?= $acoes[$criterio["crtid"]][0]["ppadsc"] ?>
											</p>
										</td>
									</tr>
								</table>
								<?php foreach( $subacoes[$acoes[$criterio["crtid"]][0]["ppaid"]] as $subacao ): ?>
									<table style="width: 100%; margin: 5px 0 5px 0;">
										<colgroup>
											<col style="width: 15%; font-size: 10px; background-color: #eeeeee;"/>
											<col style="width: 85%; font-size: 10px; background-color: #eeeeee;"/>
										</colgroup>
										<thead>
											<tr>
												<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Subação</td>
												<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left; font-weight: bold;">
													<?= $subacao["ppsdsc"] ?>
												</td>
											</tr>
											<?php if( !empty( $subacao["ppsobjetivo"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Objetivo</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["ppsobjetivo"] ?>& 
													</td>
												</tr>
											<?php endif; ?>
											<?php if( !empty( $subacao["unddsc"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Unidade Medida</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["unddsc"] ?>
													</td>
												</tr>
											<?php endif; ?>
											<?php if( !empty( $subacao["prgdsc"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Programa</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["prgdsc"] ?>
													</td>
												</tr>
											<?php endif; ?>
											<?php if( !empty( $subacao["ppstexto"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Texto para Geração do Termo de Cooperação</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["ppstexto"] ?>
													</td>
												</tr>
											<?php endif; ?>
											<?php if( !empty( $subacao["ppsmetodologia"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Estratégia de Implementação</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["ppsmetodologia"] ?>
													</td>
												</tr>
											<?php endif; ?>
											<?php if( !empty( $subacao["frmdsc"] ) ): ?>
												<tr>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: right;">Forma de Execução</td>
													<td style="padding: 5px;  border: 1px solid #bbbbbb; text-align: left;">
														<?= $subacao["frmdsc"] ?>
													</td>
												</tr>
											<?php endif; ?>
										</thead>
									</table>
								<?php endforeach; ?>
							<?php endforeach; ?>
						<?php endforeach; ?>
					<?php endforeach; ?>
				<?php endforeach; ?>
			</td>
		</tr>
	</tbody>
</table>
