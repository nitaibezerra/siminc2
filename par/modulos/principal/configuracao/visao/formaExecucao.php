<?php 
$ppsid = $ppsid ? $ppsid : $_POST['ppsid'];

if($ppsid){
	$oPropostaSubacao = new PropostaSubacao();
	$oPropostaSubacao->carregarPorId($ppsid);
}

if($ppsid){
	$aba = "par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A&acaoGuia=editar&ppsid={$ppsid}";
}else{
	$aba = "par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A&acaoGuia=incluir&indid={$_GET['indid']}";	
}
?>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){
	 jQuery('#btn_salvar').click(function() {
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		var chk = jQuery('[name="pontuacao[]"]:checked').length;
		if(erro == 00 && chk <= 0){
			erro = 1;
			alert('É necessário escolher pelo menos uma Pontuação!');
			return false;
		}
		<?php $formas = array(FORMA_EXECUCAO_ASSITENCIA_TECNICA, FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO)?>
		<?php if(in_array($frmid, $formas)): ?>
			if(erro == 0 && !jQuery('[name="ppsmonitora"]:checked').val()){
				erro = 1;
				alert('Favor informar se é passível de monitoramento técnico!');
				return false;
			}
		<?php endif; ?>
		if(erro == 0){
			selectAllOptions( document.getElementById( 'municipios' ) );
			selectAllOptions( document.getElementById( 'estados' ) );
			selectAllOptions( document.getElementById( 'ideb' ) );
			selectAllOptions( document.getElementById( 'grupo' ) );
			selectAllOptions( document.getElementById( 'iniid' ) );
			jQuery("#formulario").submit();
		}
	});
	
	<?php
	if($ppsid && ( ($oPropostaSubacao->frmid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $oPropostaSubacao->ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS) || ( $oPropostaSubacao->frmid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $oPropostaSubacao->ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS) ) ) {
		/*
		$sql = "SELECT count(pc.picid) FROM par.propostasubacao p 
									INNER JOIN par.propostaitemtiposubacao pi ON p.ptsid = pi.ptsid 
									INNER JOIN par.propostaitemcomposicao pc ON pc.picid = pi.picid and pc.picstatus = 'A'  
									LEFT JOIN par.propostasubacaoitem ps ON ps.picid = pc.picid
									WHERE p.ppsid='".$ppsid."' AND ps.ppsid IS NULL";
		$itemComposicao = $db->pegaUm($sql);
		if($itemComposicao > 0){
			echo "alert('Favor cadastrar todos os Itens de Composição!')";
		}
		*/
		$sql = "SELECT DISTINCT
				   count(pic.picid)
			FROM par.propostaitemcomposicao pic
			LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A'
			LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
			LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid AND ptpstatus = 'A'
			LEFT JOIN par.pregaouf p   ON p.ptpid   = ptp.ptpid
			WHERE psi.ppsid = '".$ppsid."'
			AND pic.picstatus = 'A'";
		
		$itemComposicao = $db->pegaUm($sql);
		if($itemComposicao == 0){
			echo "alert('Favor cadastrar todos os Itens de Composição!')";
		}
		
	}
	
	?>

});

function carregaAbasSubacao(ptsid)
{
 jQuery.ajax({
			  type		: 'post',
			  url		: 'ajax.php',
			  data		: 'requisicao=carregaAbasSubacao&indid=<?php echo $_GET['indid'] ?>&ppsid=<?php echo $ppsid ?>&frmid=<?php echo $frmid ?>&ptsid=' + ptsid,
			  success	: function(res) {
			  	if(res){
					jQuery('#abasSubacao').html(res);
			  	}
			  }
		});
}

function incluirMarcador(marcador)
{
	insertAtCursor( document.getElementById( 'ppstexto' ), marcador );
}

function insertAtCursor(myField, myValue) {
	// IE support
	if (document.selection) {
		myField.focus();
		sel = document.selection.createRange();
		sel.text = myValue;
	}
	// MOZILLA/NETSCAPE support
	else if (myField.selectionStart || myField.selectionStart == '0' ) {
		var startPos = myField.selectionStart;
		var endPos = myField.selectionEnd;
		myField.value = myField.value.substring(0, startPos) + myValue + myField.value.substring(endPos, myField.value.length);
	} else {
		myField.value += myValue;
	}
}

</script>
<table width="100%"border="0" cellspacing="1" cellpadding="0">
	<tr>
		<td  id="abasSubacao" colspan="2" align="left">
			<br />			
			<?php $frmid = $frmid ? $frmid : $_GET['frmid'] ?>
			<?php print carregaAbasPropostaSubacao($aba, $frmid, $ppsid, $_GET['indid']); ?>
		</td>
	</tr>
	<tr bgcolor="#e9e9e9" align="center">
		<td colspan="2" style="padding:3px;"><b>Preenchimento dos dados da subação</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição da subação:</td>
		<td>
			<?php $ppsdsc = $oPropostaSubacao->ppsdsc ?>
			<?php echo campo_textarea('ppsdsc', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $ppsdsc) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pontuação:</td>
		<td>
			<table width="100%">
			<?php
			$oConfiguracaoControle = new ConfiguracaoControle();
			$oCriterioPropostaSubacao = new CriterioPropostaSubacao();
			$indid = $_GET['indid'] ? $_GET['indid'] : $_POST['indid'];
			if($ppsid){
				$oPropostaSubacao->carregarPorId($ppsid);			
				$indid = $oPropostaSubacao->indid;
				$arCriterios = $oCriterioPropostaSubacao->carregarPorPpsid($ppsid);
			}
			
			if($indid){
				$arDados = $oConfiguracaoControle->recuperaDadosFormGuiaSubacao($indid);
			}
			
			$arCriterios = $arCriterios ? $arCriterios : array();

			foreach($arCriterios as $criterio){
				$criterios[] = $criterio['crtid'];
			}
			$criterios = $criterios ? $criterios : array();
					
			$x=1;			
			?>
			<?php foreach($arDados as $dado): ?>
				<tr>
					<td valign="top" align="left" width="50px">
						<?php if(in_array($dado['idcriterio'], $criterios)): ?>
							<input type="checkbox" name="pontuacao[]" align="absmiddle" value="<?php echo $dado['idcriterio'] ?>" checked>
						<?php else: ?>
							<input type="checkbox" name="pontuacao[]" align="absmiddle" value="<?php echo $dado['idcriterio'] ?>">
						<?php  endif;?>
						<?php echo "(".$dado['pontuacao'].")"; ?>
					</td>
					<td>
						<?php echo $dado['descricaocriterio'] ?><br />
					</td>
				</tr>
				<?php $x++; ?>								
			<?php endforeach; ?>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"> Anos: </td>
		<td>
		<?php 
			$oAnosSubacao = new PropostaSubacaoAnos();
			$arrAnos= $oAnosSubacao->recuperaAnos();	
		
			if($ppsid){
				$ArrAnosSub= $oAnosSubacao->recuperarAnosPorSubacao($ppsid);			
			}
			 foreach($arrAnos as $listaAno): 
			 	$check = '';	
			 	if(is_array($ArrAnosSub)){
				 	foreach($ArrAnosSub as $todosAnos){
					 	if($listaAno['praid'] == $todosAnos['praid'] ){
					 		$check = "checked";
					 	}
				 	}
			 	}
		?>
			<?php echo $listaAno['praanos']; ?> : <input type="checkbox" name="anos[]" align="absmiddle" value="<?php echo $listaAno['praid']; ?>" <?php echo $check; ?> >
			<br>
		<?php endforeach; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"> Vai para a Pontuação: </td>
		<td>
			<?php 
			$ppscarga = $oPropostaSubacao->ppscarga;
			if( $ppscarga == '' || $ppscarga == 't' ){
				$chkSim = "checked";
			} else {
				$chkNao = "checked";
			}
			?>
			<input type="radio" name="ppscarga" align="absmiddle" value="t" <?=$chkSim ?>> Sim
			<input type="radio" name="ppscarga" align="absmiddle" value="f" <?=$chkNao ?>> Não
		</td>
	</tr>
	<?php if($frmid == FORMA_EXECUCAO_TRANSFERENCIA_VOLUNTARIA): ?>
	<tr>
		<td class="SubTituloDireita">Tipo de obra:</td>
		<td>
			<?php
			
			$arObras = array(
						array('codigo' => '1', 'descricao' => 'Obra1'),
						array('codigo' => '2', 'descricao' => 'Obra2'),
						array('codigo' => '3', 'descricao' => 'Obra3'),
						array('codigo' => '4', 'descricao' => 'Obra4')
					);			
 
			$db->monta_combo('prgid', $arObras, 'S', 'Selecione...', '','');
			?>
		</td>
	</tr>
	<?php endif; ?>
	<?php if($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA): ?>
	<tr>
		<td class="SubTituloDireita">Objetivo:</td>
		<td>
			<?php $ppsobjetivo = $oPropostaSubacao->ppsobjetivo ?>
			<?php echo campo_texto('ppsobjetivo', 'S', 'S', '', '60', '', '', '', '', '', '', '', '', $ppsobjetivo) ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Texto para Geração do Termo de Cooperação:</td>
		<td>
			<textarea id="ppstexto" name="ppstexto" class="CampoEstilo obrigatorio" style="width: 80%; height: 150px;"><?=$oPropostaSubacao->ppstexto ?></textarea> <?=obrigatorio()?>
			<br/>
			<input type="button" class="botao" name="" value="Objetivo" onclick="incluirMarcador('#OBJETIVO#');"/>
			<input type="button" class="botao" name="" value="Quantidade" onclick="incluirMarcador('#QUANTIDADE#');"/>
			<input type="button" class="botao" name="" value="Unidade Medida" onclick="incluirMarcador('#UNIDADE_MEDIDA#');"/>
			<input type="button" class="botao" name="" value="Programa" onclick="incluirMarcador('#PROGRAMA#');"/>
		</td>
	</tr>
	<?php endif; ?>
	<tr>
		<td class="SubTituloDireita" style="width:250px;">Estratégia de implementação:</td>
		<td>
			<?php $ppsestrategiaimplementacao = $oPropostaSubacao->ppsestrategiaimplementacao ?>
			<?php echo campo_textarea('ppsestrategiaimplementacao', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $ppsestrategiaimplementacao) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td>
			<?php $prgid = $oPropostaSubacao->prgid ?>
			<?php
			$sql = "SELECT 
						prgid AS codigo, 
						prgdsc AS descricao
					FROM par.programa
					WHERE prgstatus = 'A'
					ORDER BY prgdsc";
			$db->monta_combo('prgid', $sql, 'S', 'Selecione...', '','', '','200px','S','','',$prgid);
			?>
		</td>
	</tr>
	<!-- Esta condição verifica se a Forma de execução é diferente de "Assistencia Financeira do MEC", se esta for a forma de execução a Unidade de Medida não é mostrada. -->
	<?php if ($frmid != 6) {?>
	<tr>
		<td class="SubTituloDireita">Unidade de medida:</td>
		<td>
			<?php $undid = $oPropostaSubacao->undid ?>
			<?php
			$sql = "SELECT 
						undid as codigo , 
						unddsc as descricao
					FROM par.unidademedida
					WHERE
						undstatus = 'A'
					ORDER BY unddsc";
			$db->monta_combo('undid', $sql, 'S', 'Selecione...', '','', '','','S','','',$undid);
			?>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td class="SubTituloDireita">Cronograma:</td>
		<td>
			<input type="radio" id="global" name="cronograma" value="1" <?=($oPropostaSubacao->ppscronograma == 1) ? 'checked' : '' ?>> Global
			<input type="radio" id="porescola" name="cronograma" value="2" <?=($oPropostaSubacao->ppscronograma == 2) ? 'checked' : '' ?>>Por escola 
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Subação apenas para um grupo de municípios:</td>
		<td align="left">
		<?php if($oPropostaSubacao && $ppsid){
					if( $_SESSION['par']['itrid'] == 1 ){
						$arrEst = $oPropostaSubacao->recuperarEstadosPorSubacao($ppsid);
					} else {
						$arrMun = $oPropostaSubacao->recuperarMunicipiosPorSubacao($ppsid);
					}
			  } 
		?>
			<table>
				<tr>
				<?php if($_SESSION['par']['itrid'] == 2){ ?>
					<td bgcolor="#e9e9e9" align="right">Municípios</td>
					<td>
						<select 
							multiple="multiple" 
							size="5" 
							name="municipios[]" 
				        	id="municipios"  
				        	ondblclick="abrepopupMunicipio();"  
				        	class="CampoEstilo" 
				        	style="width:400px;" >
				        	<?php if($arrMun): ?>
				        		<?php foreach($arrMun as $mun): ?>
				        			<option value="<?php echo $mun['muncod'] ?>"><?php echo $mun['mundescricao']." - ".$mun['estuf'] ?></option>
				        		<?php endforeach; ?>
				        	<?php else: ?>
				        		<option value="">Duplo clique para selecionar da lista</option>
				        	<?php endif; ?>
				        </select>
					</td>
				<?php } else { ?>
					<td bgcolor="#e9e9e9" align="right">Estados</td>
					<td>
						<select 
							multiple="multiple" 
							size="5" 
							name="estados[]" 
				        	id="estados"  
				        	ondblclick="abrepopupEstado();"  
				        	class="CampoEstilo" 
				        	style="width:400px;" >
				        	<?php if($arrEst): ?>
				        		<?php foreach($arrEst as $est): ?>
				        			<option value="<?php echo $est['estuf'] ?>"><?php echo $est['estdescricao'] ?></option>
				        		<?php endforeach; ?>
				        	<?php else: ?>
				        		<option value="">Duplo clique para selecionar da lista</option>
				        	<?php endif; ?>
				        </select>
					</td>
				<?php } ?>
				</tr>
				<tr>
					<td bgcolor="#e9e9e9" align="right">Classificação IDEB</td>
					<td>
						<?php
						if($ppsid){
							$sql = "select
									tpmid as codigo,
									tpmdsc as descricao
								from territorios.tipomunicipio
								where
									tpmid in ( select tpmid from par.propostasubacaoideb where ppsid = '$ppsid' ) and
									gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) AND
									tpmstatus = 'A'";
							$ideb_dados = $db->carregar($sql);
						}
						$sqlComboIDEB = "
							select
								tpmid as codigo,
								tpmdsc as descricao
							from territorios.tipomunicipio
							where
								gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
								tpmstatus = 'A'
						";
						combo_popup( "ideb", $sqlComboIDEB, "Classificação IDEB", "215x400", 0, "", "", "S", false, false, 5, 400 ,'','','','',$ideb_dados);
						?>
					</td>
				</tr>
				<tr>
					<td bgcolor="#e9e9e9" align="right">Grupo de Municípios</td>
					<td>
						<?php
						if($ppsid){
							$sql = "select * from (
										(select 
											tpmid as codigo,
											case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
										from  
											territorios.tipomunicipio
										where 
											tpmid in (1, 16, 17, 140, 141, 150, 151, 152, 154, 170)
										)
										union
										(select 
											gtmid as codigo,
											gtmdsc as descricao 
										from  
											territorios.grupotipomunicipio
										where 
											gtmid = 5)
										order by descricao) as tbl where codigo in ( select tpmid from par.propostasubacaoideb where ppsid = '$ppsid' )";

							$grupo = $db->carregar($sql);
						}
						
						$sqlGrupoMunicipios = "
							select * from (
							(select 
								tpmid as codigo,
								case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
							from  
								territorios.tipomunicipio
							where 
								tpmid in (1, 16, 17, 140, 141, 150, 151, 152, 154, 170)
							)								
							union
							(select 
								gtmid as codigo,
								gtmdsc as descricao 
							from  
								territorios.grupotipomunicipio
							where 
								gtmid = 5)
							order by descricao) as tbl";
						combo_popup( "grupo", $sqlGrupoMunicipios, "Grupo de Município", "215x400", 0, "", "", "S", false, false, 5, 400 ,'','','','',$grupo);
						?>
					</td>
				</tr>							
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Peso da subação:</td>
		<td>
			<?php $ppspeso = $oPropostaSubacao->ppspeso ?>
			<?php echo campo_texto('ppspeso', 'S', 'S', '', '15', '', '', '', '', '', '', '', '', $ppspeso) ?>
		</td>
	</tr>									
	<tr>
		<td class="SubTituloDireita">Forma de atendimento:</td>
		<td>
			<?php $foaid = $oPropostaSubacao->foaid ?>
			<?php
			$sql = "SELECT 
						foaid as codigo , 
						foadescricao as descricao
					FROM par.formaatendimento
					WHERE foastatus = 'A'
					ORDER BY foadescricao";
 
			$db->monta_combo('foaid', $sql, 'S', 'Selecione...', '','', '','','N','','',$foaid);
			?>
		</td>
	</tr>
	<?php $formas = array(FORMA_EXECUCAO_ASSITENCIA_TECNICA, FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO)?>
	<?php if(in_array($frmid, $formas)): ?>				
	<tr>
		<td class="SubTituloDireita">Passível de monitoramento técnico:</td>
		<td>
			<input type="radio" <?php echo $oPropostaSubacao->ppsmonitora == "t" ? "checked='checked'" : "" ?> name="ppsmonitora" value="t" >Sim
			<input type="radio" <?php echo $oPropostaSubacao->ppsmonitora == "f" ? "checked='checked'" : "" ?> name="ppsmonitora" value="f" >Não
		</td>
	</tr>
	<?php endif; ?>
	<tr>
		<td class="SubTituloDireita">Ordem:</td>
		<td>
			<?php $ppsordem = $oPropostaSubacao->ppsordem ?>
			<?php $ppsordem = $ppsordem ? number_format($ppsordem,0,'','.') : "" ?>
			<?php echo campo_texto('ppsordem', 'S', 'S', '', '15', '', '[.###]', '', '', '', '', '', '', $ppsordem) ?>
		</td>
	</tr>
	<?php $formasEmenda = array(ASSISTENCIA_FINANCEIRA_EMENDA, ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS)?>
	<?php if(in_array($frmid, $formasEmenda)){ ?>	
	<tr>
		<td class="SubTituloDireita" style="width:250px;">Iniciativas Emendas:</td>
		<td>
			<?php
			$iniid = array();
			
			if( $ppsid ){
				$sql = "SELECT distinct
							ini.iniid as codigo,
							(select resdsc from emenda.iniciativaresponsavel ir
								inner join emenda.responsavel re on re.resid = ir.resid
                            	where ir.iniid = ini.iniid limit 1)||' - '||ini.ininome as descricao
						FROM 
							par.propostasubacaoiniciativaemenda pse
						    inner join emenda.iniciativa ini on ini.iniid = pse.iniid
						WHERE
							pse.ppsid = $ppsid";
				$iniid = $db->carregar($sql);
			}
			
			$sql = "SELECT
						i.iniid as codigo,
						re.resdsc||' - '||i.ininome as descricao
					FROM
						emenda.iniciativa i
						inner join emenda.iniciativaresponsavel ir on ir.iniid = i.iniid
						inner join emenda.responsavel re on re.resid = ir.resid
					WHERE
						/*ir.resid = 3
						and*/ ir.irestatus = 'A' 
						and i.inistatus = 'A'
					ORDER BY
						i.ininome";
						
			combo_popup('iniid', $sql, '', '400x600', 0, array(), '', 'S', false, false, 05, 400, '', '', '', '', $iniid );
			?>
		</td>
	</tr>
	<?php
	} if($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA): ?>
	<tr>
		<td class="SubTituloDireita">Cobertura universal MEC:</td>
		<td>
			<?php $ppscobertura = $oPropostaSubacao->ppscobertura ?>
			<input <?php echo $ppscobertura == "t" ? "checked='checked'" : "" ?> type="radio" name="ppscobertura" value="t" >Sim
			<input <?php echo $ppscobertura == "f" ? "checked='checked'" : "" ?> type="radio" name="ppscobertura" value="f" >Não
		</td>
	</tr>
	<?php endif; ?>
	<?php if($frmid == FORMA_EXECUCAO_TRANSFERENCIA_VOLUNTARIA): ?>
	<tr>
		<td class="SubTituloDireita">Natureza de despesa:</td>
		<td>
			<?php $ppsnaturezadespesa = $oPropostaSubacao->ppsnaturezadespesa ?>
			<?php echo campo_texto('ppsnaturezadespesa', 'S', 'S', '', '15', '', '', '', '', '', '', '', '', $ppsnaturezadespesa) ?>
		</td>
	</tr>
	<?php endif; ?>
	<tr bgcolor="#e9e9e9">
		<td colspan="2" align="center">			
			<input type="button" id="btn_salvar" value="Salvar" />
		</td>
	</tr>
</table>