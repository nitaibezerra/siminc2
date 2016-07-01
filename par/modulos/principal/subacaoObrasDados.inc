<?php
if ($_SESSION ['par'] ['itrid'] == 2 || $_SESSION ['par'] ['esfera'] == 'M') {
	$mun = $_SESSION ['par'] ['muncod'];
} else {
	$mun = '';
}

$oSubacaoControle = new SubacaoControle ();
$oPreObraControle = new PreObraControle ();

if ($_SESSION ['par'] ['muncod']) {
	$arEscolasQuadraSelecionadas = $oPreObraControle->verificaEscolasQuadraSelecionadas ( $_SESSION ['par'] ['muncod'] );
	$boTipo_A = $oPreObraControle->verificaGrupoMunicipioTipoObra_A ( $_SESSION ['par'] ['muncod'] );
}
$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array ();

// $arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra($preid);
$sqlEscolasQuadra = $oPreObraControle->recuperarSqlEscolasQuadra ( $preid );
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array ();

$estadoAtualSubacao = wf_pegarEstadoAtual ( $subacao ['docid'] );


cabecalho();


unset ( $docid );
unset ( $esdid );
if ($preid) {
	$obSubacaoControle = new SubacaoControle ();
	$obPreObra = new PreObra ();
	$arDados = $obSubacaoControle->recuperarPreObra ( $preid );
	$docid = prePegarDocid ( $preid );
	$esdid = prePegarEstadoAtual ( $docid );

}

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidadesn.js"></script>

<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" /> <input
		type="hidden" name="ano" value="<?php echo $ano?>" /> <input
		type="hidden" name="preid" value="<?php echo $preid?>" /> <input
		type="hidden" name="requisicao" value="salvarParObras" /> <input
		type="hidden" name="entid" id="entid" value="" /> <input type="hidden"
		name="frmid_libera" id="frmid_libera" value="<?=$subacao['frmid']; ?>" />
	<input readonly="readonly" type="hidden" name="mundescricao"
		id="mundescricao1" value="<?php echo $municipio; ?>" /> <input
		type="hidden" name="muncod" id="muncod1"
		value="<?php echo !empty($arDados['muncod']) ? $arDados['muncod'] : $_SESSION['par']['muncod']; ?>" />

	<table class="tabela" style="margin-bottom: 15px" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulodireita">Nome do terreno:</td>
			<td>
				<?php
				$predescricao = $arDados ['predescricao'];
				echo campo_texto ( "predescricao", 'S', $boAtivo, '', 40, '', '', '', '', '', '', 'id="predescricao"', '', $predescricao );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo da Obra:</td>
			<td>
			<?php
			$sql = '';
			if( $estadoAtualSubacao ['esdid'] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao ['esdid'] == WF_SUBACAO_DILIGENCIA || $esdid == WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL ) {
				$sql = "SELECT DISTINCT
						    pto.ptoid as codigo,
						    pto.ptodescricao as descricao
						FROM
						    par.propostasubacaoobra pso
						INNER JOIN obras.pretipoobra pto ON pto.ptoid = pso.ptoid
						WHERE
							pto.ptoidpaiemenda IS NOT NULL
							AND pto.ptostatus = 'A'
							AND pto.ptoid NOT IN (71, 72)
						ORDER BY
						    descricao";
			}
			
			if( in_array($esdid, Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_EM_DILIGENCIA)) ){
				$sql = "SELECT DISTINCT
							pto.ptoid as codigo,
							pto.ptodescricao as descricao
						FROM
							par.propostasubacaoobra pso
						INNER JOIN obras.pretipoobra pto ON pto.ptoid = pso.ptoid
						WHERE
							ppsid = (SELECT ppsid FROM par.subacao WHERE sbastatus = 'A' AND sbaid = $sbaid)
							AND ptostatus = 'A'
							AND pto.ptoid NOT IN (71, 72)
						ORDER BY
							descricao";
			}
			
			$arrSugestaoPtoid = Array( WF_PAR_OBRA_EM_REFORMULACAO );
			if( in_array( $esdid, $arrSugestaoPtoid ) ){
				$sql = "SELECT DISTINCT * FROM (
							SELECT
								pto.ptoid as codigo,
								pto.ptodescricao as descricao
							FROM
								obras.pretipoobra pto
							INNER JOIN obras.pretipoobradiligencia tod ON tod.ptoid = pto.ptoid
							WHERE
								preid = $preid
								AND todstatus = 'A'
							UNION ALL
							SELECT
								pto.ptoid as codigo,
								pto.ptodescricao as descricao
							FROM
								obras.pretipoobra pto
							INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
							WHERE
								preid = $preid ) as foo";
					
				$arrSugestao = $db->carregar( $sql );
					
				if( $arrSugestao[0]['codigo'] != '' ){
					$arTipo = $arrSugestao;
				}
			}
			
			if( in_array($esdid, Array(WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR) ) ){
				$sql = "SELECT
							ptoid as codigo,
							ptodescricao as descricao
						FROM
							obras.pretipoobra
						WHERE
							ptoid in (73,74) AND
							ptostatus = 'A'";
			}
			
			if( $sql != '' ){
				$ptoid = $arDados['ptoid'];
				$db->monta_combo ( "ptoid", $sql, $boAtivo, 'Selecione...', 'exibeTipoFundacao', '', '', '', 'N', 'ptoid', false, $ptoid, 'Tipo da Obra' );
			}else{
				echo $arDados['ptodescricao'];
			}
			?> 	
			<input type="hidden" name="hdn_ptoid" id="hdn_ptoid" value="<?php echo $arDados['ptoid'] ?>" /></td>
		</tr>
		<tr id="td_tipo_fundacao" style="display:<?php echo ($ptoid == OBRA_TIPO_B || $ptoid == OBRA_TIPO_B_220v) ? "" : "none" ?>" >
			<td class="subtitulodireita">Tipo De Fundação:</td>
			<td><input type="radio" name="pretipofundacao"
				<?php echo $arDados['pretipofundacao'] != "E" ? " checked='checked' onclick=\"alertasapata()\"" : ""?>
				value="E" /> Estaca <input type="radio" name="pretipofundacao"
				<?php echo $arDados['pretipofundacao'] != "S" ? " checked='checked' onclick=\"alertasapata()\"" : ""?>
				value="S" /> Sapata</td>
		</tr>
		<tr id="td_escolas" style="display:<?php echo $arDados['ptoexisteescola'] == 't' ? "" : "none" ?>" >
			<td class="subtitulodireita">Escolas:</td>
			<td id="escolas_<?php echo $ano; ?>">
				<?php
				if (trim ( $arDados ['entcodent'] ) != '') {
					$sql = "SELECT 
								entcodent as codigo,
								'(' || entcodent || ') - ' || entnome as descricao 
							FROM 
								entidade.entidade 
							WHERE 
								entcodent = '" . trim ( $arDados ['entcodent'] ) . "'";
					$entcodent = $db->carregar ( $sql );
				}
				
				if ($_SESSION ['par'] ['itrid'] == 1) {
					$tpcid = 1;
					$sqlEscolasQuadra = "";
					/*
					 * $sqlEscolasQuadra = "SELECT ent.entcodent as codigo, ent.entcodent || ' - ' || ent.entnome as descricao FROM entidade.entidade ent INNER JOIN entidade.endereco ed ON ent.entid = ed.entid WHERE ed.estuf = '{$_SESSION['par']['estuf']}' AND ent.tpcid = {$tpcid} AND ent.entid IN (select entid from par.escolas where escid IN ( select escid from par.subacaoescolas where sbaid = {$_REQUEST['sbaid']} AND sesano = {$_REQUEST['ano']} AND sesstatus = 'A')) AND ent.entcodent NOT IN (SELECT COALESCE( entcodent, '' ) FROM obras.preobra p2 INNER JOIN par.subacaoobra so ON so.preid = p2.preid WHERE p2.estuf = '{$_SESSION['par']['estuf']}' AND p2.ptoid = 17 AND so.sbaid = {$_REQUEST['sbaid']} AND p2.prestatus <> 'I')";
					 */
					
					$sqlEscolasQuadra = "SELECT
											mat.fk_cod_entidade as codigo,
											e.entnome as descricao
										FROM
											".SCHEMAEDUCACENSO.".tab_entidade ent 
										INNER JOIN
											".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
										INNER JOIN
											".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ent.pk_cod_entidade
										INNER JOIN
											entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
										INNER JOIN 
							            	territorios.municipio m on m.muncod = ent.fk_cod_municipio::character varying
										LEFT JOIN 
											par.escolas esc on esc.entid = e.entid
										LEFT JOIN 
											par.subacaoescolas ses on ses.escid = esc.escid  and ses.sesstatus = 'A' and ses.sesano = {$ano} and ses.sbaid = {$_REQUEST['sbaid']}
										WHERE
											mat.id_status = 1
											AND (e.entescolanova = false or e.entescolanova is null)
											AND e.entstatus = 'A'
											AND e.tpcid = {$tpcid}
											AND m.estuf = '{$_SESSION['par']['estuf']}'
											AND mat.fk_cod_entidade::varchar NOT IN (SELECT 
															COALESCE( entcodent, '' )
														  FROM 
															obras.preobra p2
														  INNER JOIN par.subacaoobra so    ON so.preid = p2.preid
														  WHERE 
															p2.estuf = '{$_SESSION['par']['estuf']}'
															AND p2.ptoid = 17
															AND so.sbaid = {$_REQUEST['sbaid']}
															AND p2.prestatus <> 'I')
										GROUP BY
											ses.escid, e.entid, e.entnome, mat.fk_cod_entidade, ted.num_salas_existentes
										ORDER BY
											e.entnome";
					
					echo $entcodent [0] ['descricao'];
					?><br>
			<a
				href="javascript:inserirEscolas('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')">Editar
					/ Inserir Escolas</a><?php
				} elseif ($_SESSION ['par'] ['itrid'] == 2) {
					$tpcid = 3;
					$sqlEscolasQuadra = "";
					/*
					 * $sqlEscolasQuadra = "SELECT ent.entcodent as codigo, ent.entcodent || ' - ' || ent.entnome as descricao FROM entidade.entidade ent INNER JOIN entidade.endereco ed ON ent.entid = ed.entid INNER JOIN territorios.municipio m ON ed.muncod	 = m.muncod WHERE ed.muncod = '{$_SESSION['par']['muncod']}' AND ent.tpcid = {$tpcid} AND ent.entid IN (select entid from par.escolas where escid IN ( select escid from par.subacaoescolas where sbaid = {$_REQUEST['sbaid']} AND sesano = {$_REQUEST['ano']} AND sesstatus = 'A' )) AND ent.entcodent NOT IN (SELECT COALESCE( entcodent, '' ) FROM obras.preobra p2 INNER JOIN par.subacaoobra so ON so.preid = p2.preid WHERE p2.muncod = '{$_SESSION['par']['muncod']}' AND p2.ptoid = 17 AND so.sbaid = {$_REQUEST['sbaid']} AND p2.prestatus <> 'I')";
					 */
					$sqlEscolasQuadra = "SELECT
											mat.fk_cod_entidade as codigo,
											e.entnome as descricao
										FROM
											".SCHEMAEDUCACENSO.".tab_entidade ent 
										INNER JOIN
											".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade= ent.pk_cod_entidade
										INNER JOIN
											".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ent.pk_cod_entidade
										INNER JOIN
											entidade.entidade e on e.entcodent = ent.pk_cod_entidade::character varying
										LEFT JOIN 
											par.escolas esc on esc.entid = e.entid
										LEFT JOIN 
											par.subacaoescolas ses on ses.escid = esc.escid  and ses.sesstatus = 'A' and ses.sesano = {$ano} and ses.sbaid = {$_REQUEST['sbaid']}
										WHERE
											mat.id_status = 1
											AND (e.entescolanova = false or e.entescolanova is null)
											AND e.entstatus = 'A'
											AND e.tpcid = {$tpcid}
											AND fk_cod_municipio = {$_SESSION['par']['muncod']}
											AND mat.fk_cod_entidade::varchar NOT IN (SELECT 
															COALESCE( entcodent, '' )
														  FROM 
															obras.preobra p2
														  INNER JOIN par.subacaoobra so    ON so.preid = p2.preid
														  WHERE 
															p2.muncod = '{$_SESSION['par']['muncod']}'
															AND p2.ptoid = 17
															AND so.sbaid = {$_REQUEST['sbaid']}
															AND p2.prestatus <> 'I')
										GROUP BY
											ses.escid, e.entid, e.entnome, mat.fk_cod_entidade, ted.num_salas_existentes
										ORDER BY
											e.entnome";
					
					echo $entcodent [0] ['descricao'];
					?><br>
			<a
				href="javascript:inserirEscolas('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')">Editar
					/ Inserir Escolas</a><?php
					/*
					 * combo_popup('entcodent_', $sqlEscolasQuadra, 'Selecione...', "400x400", 1, array(), "", (($entcodent)?"N":"S"), false, false, 1, 400, null, null, '', '', $entcodent, true, false, '', '', Array('descricao'), Array('dsc') );
					 */
				}
				// $db->monta_combo( "entcodent", $arEscolasQuadra, $boAtivo, 'Selecione...', '', '', '', '', 'S', 'entcodent',false,$entcodent,'Escolas');
				?>
			    <input type="hidden" id="entcodent" name="entcodent"
				value="<?=$arDados['entcodent'] ?>" />
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Unidade de Medida:</td>
			<td><?php
			// $db->monta_combo( "unidadeMedida", $arUnidadeMedida, 'S', 'Selecione...', '', '', '', '', 'S', 'unidadeMedida',false,null,'Unidade de Medida');
			echo "Unidade Escolar";
			?></td>
		</tr>
		<tr>
			<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 25%; white-space: nowrap"><label>CEP:</label></td>
			<td>
			<?php
			$arDados ['precep'] = substr ( $arDados ['precep'], 0, 2 ) . "." . substr ( $arDados ['precep'], 2, 3 ) . "-" . substr ( $arDados ['precep'], 5, 3 );
			if ($readonly == '' || $editavel == '') {
				?>
				<input type="text" name="endcep1" title="CEP" <?php echo$readonly;?>
				onkeyup="this.value=mascaraglobal('##.###-###', this.value);"
				onblur="this.value=mascaraglobal('##.###-###', this.value)"
				class="CampoEstilo" id="endcep1"
				value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10"
				<?php echo $stAtivo ?> />				
			<?php }else{ ?>
				<input type="text" name="read" title="CEP"
				<?php echo$readonly; echo $editavel; ?> class="CampoEstilo"
				id="read" value="<?php echo $arDados['precep']; ?>" size="13"
				maxlength="10" />
			<?php }
			if( $subacao['frmid'] != 15 ){
			?>
				<img src="../imagens/obrig.gif" />
			<?php }?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>
			<td><input type="text" title="Logradouro"
				<?php echo$readonly; echo $editavel;?> name="endlog"
				class="CampoEstilo" id="endlog1"
				value="<?php echo $arDados['prelogradouro']; ?>" size="48"
				<?php echo $stAtivo ?> />
			<?php 		
			if( $subacao['frmid'] != 15 ){
			?>
				<img src="../imagens/obrig.gif" />
			<?php }?>
			</td>
		</tr>

		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 150px; white-space: nowrap"><label>Número:</label></td>
			<td><input type="text" name="endnum" title="Número"
				<?php echo$readonly; echo $editavel;?> class="CampoEstilo"
				id="endnum1" value="<?php echo $arDados['prenumero']; ?>" size="6"
				maxlength="4" onkeypress="return somenteNumeros(event);"
				<?php echo $stAtivo ?> /> 
				<?php 		
				if( $subacao['frmid'] != 15 ){
				?>
					<img src="../imagens/obrig.gif" />
				<?php }?>
			</td>
		</tr>

		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
			<td><input type="text" name="endcom"
				<?php echo$readonly; echo $editavel;?> class="CampoEstilo"
				id="endcom1" value="<?php echo $arDados['precomplemento']; ?>"
				size="48" maxlength="100" <?php echo $stAtivo ?> /></td>
			</td>
		</tr>

		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 150px; white-space: nowrap"><label>Ponto de Referência:</label></td>
			<td><input type="text" name="endreferencia"
				<?php echo$readonly; echo $editavel;?> class="CampoEstilo"
				id="endreferencia1" value="<?php echo $arDados['prereferencia']; ?>"
				size="48" maxlength="100" <?php echo $stAtivo ?> />
				<?php 		
				if( $subacao['frmid'] != 15 ){
				?>
					<img src="../imagens/obrig.gif" />
				<?php }?>
			</td>
			</td>
		</tr>

		<tr>
			<td align="right" class="SubTituloDireita"
				style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
			<td><input type="text" title="Bairro"
				<?php echo$readonly; echo $editavel;?> name="endbai"
				class="CampoEstilo" id="endbai1"
				value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
				<?php 		
				if( $subacao['frmid'] != 15 ){
				?>
					<img src="../imagens/obrig.gif" />
				<?php }?>
			</td>
		</tr>
		<tr id="tr_estado">
			<td class="subtitulodireita">Estado:</td>
			<td><?
			$estuf = $arDados ['estuf'];
			
			if ($estuf) {
				$where = " where e.estuf = '{$estuf}' ";
			} elseif ($_SESSION ['par'] ['estuf']) {
				$where = " where e.estuf = '{$_SESSION['par']['estuf']}' ";
			}
			
			$sql = "select
						 e.estuf as codigo, e.estdescricao as descricao 
						from
						 territorios.estado e
						 $where
						order by
						 e.estdescricao asc";
			$db->monta_combo ( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1', false, $estuf, 'Estado' );
			?></td>
		</tr>
		<tr id="tr_municipio">
			<td class="subtitulodireita">Município: <br />
			</td>
			<td id="municipio"><?
			if ($arDados ['estuf']) {
				$sql = "select
							 muncod as codigo, 
							 mundescricao as descricao 
							from
							 territorios.municipio
							where
							 estuf = '" . $arDados ['estuf'] . "' 
							order by
							 mundescricao asc";
				$muncod_ = $arDados ['muncod'];
				$db->monta_combo ( "muncod_", $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S', 'muncod_', false, $muncod_, 'Município' );
			} else {
				$db->monta_combo ( "muncod_", array (), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_', false, null, 'Município' );
			}
			?></td>
		</tr>
		<script> document.getElementById('endcep1').value = mascaraglobal('##.###-###', document.getElementById('endcep1').value);</script>
		<?php
		$latitude = explode ( '.', $arDados ['prelatitude'] );
		$latitude[0] = ( $latitude[1] || $latitude[1] )  && !$latitude[0] ? '00' : $latitude[0];
		$longitude = explode ( '.', $arDados ['prelongitude'] );
		$longitude[0] = ( $longitude[1] || $longitude[1] )  && !$longitude[0] ? '00' : $longitude[0];
		?>
		<tr>
			<td class="SubTituloDireita">Latitude :</td>
			<td><input name="latitude[]" id="graulatitude1" maxlength="2"
				size="3" value="<? echo $latitude[0]; ?>" class="normal"
				type="hidden"> <span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span>
				º <input name="latitude[]" id="minlatitude1" size="3" maxlength="2"
				value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> <span
				id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>
				' <input name="latitude[]" id="seglatitude1" size="3" maxlength="2"
				value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> <span
				id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span>
				" <input name="latitude[]" id="pololatitude1"
				value="<? echo $latitude[3]; ?>" type="hidden"> <span
				id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td>
			<td><input name="longitude[]" id="graulongitude1" maxlength="2"
				size="3" value="<? echo $longitude[0]; ?>" type="hidden"> <span
				id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>
				º <input name="longitude[]" id="minlongitude1" size="3"
				maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> <span
				id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>
				' <input name="longitude[]" id="seglongitude1" size="3"
				maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> <span
				id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span>
				" <input name="longitude[]" id="pololongitude1"
				value="<? echo $longitude[3]; ?>" type="hidden"> <span
				id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
				<input type="hidden" name="endzoom" id="endzoom"
				value="<? echo $obCoendereCoentrega->endzoom; ?>" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td><a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar
					No Mapa</a> <input style="display: none;"
				name="endereco[1][endzoom]" id="endzoom1" value="" type="text"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td class="SubTituloEsquerda">
			
			<?php 
			if($habil == 'S' && $readonly == '' && $editavel != "readonly='readonly'"){ ?>
				<input type="button" value="Salvar" name="btn_salvar"
				onclick="salvarDadosObra()" />
			<?php } ?>
				<input type="button" value="Fechar" name="btn_fechar"
				onclick="window.close()" />
			</td>
		</tr>
	</table>
</form>
<script>

jQuery(document).ready(function(){
		
	var ptoid = $('ptoid').value;
	
	var muncod_usuario = '<?php echo $mun; ?>';
	
	jQuery("#predescricao").addClass("obrigatorio");
	jQuery("#ptoid").addClass("obrigatorio");
	jQuery("#preobservacao").addClass("obrigatorio");
	jQuery("#endcep1").addClass("obrigatorio");
	jQuery("#endlog1").addClass("obrigatorio");
	jQuery("#endnum1").addClass("obrigatorio");
	jQuery("#endreferencia1").addClass("obrigatorio");
	jQuery("#endbai1").addClass("obrigatorio");
	jQuery("#muncod_").addClass("obrigatorio");
	jQuery("#predescricao").addClass("obrigatorio");
	
	jQuery.ajax({
			   type		: "POST",
			   url		: window.location,
			   data		: "requisicaoAjax=listaObras",
			   async    : false,
			   success	: function(res){
					jQuery('#obras_<?php echo $_GET['ano'] ?>',window.opener.document).html(res);
				}
			 });
	
	jQuery('#endcep1').blur(function(){

		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaCepMunicipio'}, 
				  {name : 'ajax'	  , value : '1'},
				  {name : 'db'		  , value : true},
				  {name : 'cep'		  , value : jQuery(this).val()}
				 );

		var boBuscaEndCep = true;
		jQuery.ajax({
			   type		: "POST",
			   url		: 'ajax.php',
			   data		: data,
			   async    : false,
			   success	: function(res){
						var muncod = res;
						if(muncod != ''&&muncod_usuario != ''){
							if(muncod != muncod_usuario){
								alert('Favor informar um cep do município cadastrado.');
								jQuery('#endlog1').val('');
								jQuery('#endbai1').val('');
								jQuery('#mundescricao1').val('');
								jQuery('#estuf1').val('');
								jQuery('#muncod1').val('');
								boBuscaEndCep = false;
								return false;
							}
						}
					}
			 });
		if(boBuscaEndCep){
			getEnderecoPeloCEP(jQuery(this).val(),'1');
		}
		filtraTipo(jQuery('#estuf1').val());
    })
	
});

function inserirEscolas(ano,sbaid){
	url = "par.php?modulo=principal/subacaoObrasEscolas&acao=A&sbaid=" + sbaid + "&ano=" + ano;
	janela(url,700,550,"Escolas da Subação");
}

function filtraTipo(estuf) {
	select = document.getElementsByName('muncod_')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	var data = new Array();
		data.push({name : 'requisicao', value : 'montaComboMunicipioPorUf'}, 
				  {name : 'db', value : true},
				  {name : 'estuf', value : estuf}
				 );
	 
	jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(res){
						jQuery('#municipio').html(res);
						jQuery('#municipio').css('visibility','visible');
					  }
		 });
	  
	// Espera 100 milisegundos para dar tempo da função AJAX ser executada.
	window.setTimeout('alteraComboMuncod()', 1000);
	
}

function alteraComboMuncod(){
	var muncod = jQuery('#muncod1').val()
	if(muncod){
		var comboMunicipio = document.getElementById('muncod_');
		for (var i = 0; i < comboMunicipio.length; i++) {
			var indiceCombo = comboMunicipio.options[i].index;
			var textoCombo = comboMunicipio.options[i].text;
			var valorCombo = comboMunicipio.options[i].value;
			
			if(valorCombo == muncod){
				comboMunicipio.options[i].selected = true;
			}
		}
	}
}

if(jQuery('#muncod1').val()){
	alteraComboMuncod();
}

function alertasapata(){
	var preid = jQuery('#preid').val();
	if( preid ){
		alert('Ao alterar o tipo de Obra Fundação, os dados referentes ao tipo antecessor serão perdidos!')
	}
}


function exibeTipoFundacao(value){

	jQuery('.enviar').attr('disabled', false);
	/*
	 * Somente deixar selecionar tipo de obra = Tipo A, se o município da obra estiver estive no grupo 1 do PAC
	 */
	if(value == "<? echo OBRA_TIPO_A?>"){
		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaGrupoMunicipioTipoObra_A'}, 
				  {name : 'db', value : true}
				 );

		var validacao_obrtipoA = true;
		
		jQuery.ajax({
			   type		: "POST",
			   url		: "ajax.php",
			   data		: data,
			   async    : false,
			   success	: function(res){			   				
			   				if(res=="false") {
			   					alert("O município selecionado não pode conter obras do tipo A");
			   					validacao_obrtipoA = false;
			 					jQuery('#ptoid').val('');
			   				}
						  }
			 });
		 
		if(!validacao_obrtipoA) return false;

		
	}
	
	if(value == "<? echo OBRA_TIPO_B?>" || value == "<?php echo OBRA_TIPO_B_220v ?>"){
		jQuery('#td_tipo_fundacao').show();
	}else{
		jQuery('#td_tipo_fundacao').hide();
	}

	var data = new Array();
	data.push({name : 'requisicao', value : 'verificaTipoEscola'}, 
			  {name : 'db', value : true},
			  {name : 'ptoid', value : value}
			 );

	var validacao_obrtipoA = true;

	if(value!=''){
		jQuery.ajax({
			   type		: "POST",
			   url		: "ajax.php",
			   data		: data,
			   async    : false,
			   success	: function(res){
							if(res){
								jQuery('#td_escolas').show();
								<?php if(!count($arEscolasQuadra)): ?>
									jQuery('.enviar').attr('disabled', true);
								<?php endif; ?>
							}else{
								jQuery('#td_escolas').hide();
							}
						  }
			 });
	}
	
	if(jQuery('#hdn_ptoid').val()){
		var preid = jQuery('#preid').val();
		if( preid != '' ){
			alert('Ao alterar o tipo de Obra, os dados referentes ao tipo antecessor serão perdidos!');
		}
	}
}

function salvarDadosObra()
{
	var erro = 0;
	var msg = '';
	
	var arrCamposNaoObrigatorios = new Array();
	arrCamposNaoObrigatorios[0] = "entcodent_disable";
	
	if(jQuery('#frmid_libera').val() == 15){
		if( jQuery('#predescricao').val() == '' ){
			alert('O campo "Nome do terreno" é de preenchimento obrigatório!');
			jQuery('#predescricao').focus();
			return false;
		}
		if( jQuery('#ptoid').val() == '' ){
			alert('O campo "Tipo da Obra" é de preenchimento obrigatório!');
			jQuery('#ptoid').focus();
			return false;
		}
		if( jQuery('#estuf1').val() == '' ){
			alert('O campo "Estado" é de preenchimento obrigatório!');
			jQuery('#estuf1').focus();
			return false;
		}
		if( jQuery('#muncod_').val() == '' ){
			alert('O campo "Município" é de preenchimento obrigatório!');
			jQuery('#muncod_').focus();
			return false;
		}
	} else {	
		jQuery("[class~=obrigatorio]").each(function() { 
			if( (!this.value || this.value == "Selecione..." || this.value == "") && !inArray(this.name,arrCamposNaoObrigatorios) ){			
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		var tamanhoCep = replaceAll( replaceAll( jQuery("#endcep1").val(), ".", "" ), "-", "") 
	
		if( tamanhoCep.length < 8 ){
			alert('O CEP está incompleto');
			jQuery("#endcep1").focus();
			return false;
		}
		selectAllOptions( document.getElementById( 'entcodent_' ) );

		var boPodeGravarLatitude =  true;
		var boPodeGravarLongitude =  true;
		jQuery('input[name=latitude[]]').each(function(i){
			if(jQuery(this).val() == ""){
				boPodeGravarLatitude = false;
			}
		});
		
		jQuery('input[name=longitude[]]').each(function(i){
			if(jQuery(this).val() == ""){
				boPodeGravarLongitude = false;
			}
		});
	
		if(!boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude e a Longitude.';
		} else if(!boPodeGravarLatitude && boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude.';
		} else if(boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Longitude.';
		}
	
		if(msg!=''){
			alert(msg);
			return false;
		}
	}
	
	if(erro == 0){
		jQuery("#formulario").submit();
	}
}

function inArray(needle, haystack) {
    var length = haystack.length;
    for(var i = 0; i < length; i++) {
        if(haystack[i] == needle) return true;
    }
    return false;
}


</script>
