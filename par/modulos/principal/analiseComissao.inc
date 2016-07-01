<?php 

if($_REQUEST['requisicao'] == 'criaSessaoPlano'){
	
	$_SESSION['par']['itrid']	= trim($_REQUEST['muncod']) ? 2 : 1;	
	$_SESSION['par']['inuid']	= $_REQUEST['inuid'];
	$_SESSION['par']['estuf']	= $_REQUEST['estuf'];
	$_SESSION['par']['muncod']	= $_REQUEST['muncod'];
	
	die('true');
}

if($_REQUEST['requisicao'] == 'cancelarAprovacao'){
	$sql = "delete from par.subacaoparecerdacomissao where sbdid = {$_REQUEST['sbdid']} returning pdcid;";	
	$pdcid = $db->pegaUm($sql);
	$db->executar($sql);
	$db->commit();
	
	$sql = "Select spcid From par.subacaoparecerdacomissao Where pdcid = $pdcid;";
	$spcid = $db->pegaUm($sql);
	
	if($spcid){
		die;
	}else{
		$sql = "Delete From par.parecerdacomissao Where pdcid = $pdcid;";
		$db->executar($sql);
		$db->commit();
	}
}

if($_REQUEST['requisicao'] == 'aprovarSubacoes'){
	
	if($_REQUEST['muncod'] != '' && $_REQUEST['muncod'] != 'undefined'){	 
		$mun_codigo = "'$_REQUEST[muncod]'";
	}else{
		$mun_codigo = "null";
	}
	
	if($_REQUEST['estuf'] != '' && $_REQUEST['estuf'] != 'undefined'){ 
		$est_uf = "'$_REQUEST[estuf]'";
	}else{
		$est_uf = "null";
	}
	
	$parecer	= $_REQUEST['pdcparecer'];
	$cpf_usu	= $_SESSION['usucpf'];	
	
	if($_REQUEST['pdcparecer']){	
		$sql = "
			insert into par.parecerdacomissao(
				muncod, 
				estuf, 
				pdcparecer, 
				pdcdata, 
				usucpf
			)values(
				$mun_codigo,
				$est_uf,
				'{$_REQUEST['pdcparecer']}',
				'now()',
				'$cpf_usu'
			)returning pdcid
		";
		$pdcid = $db->pegaUm($sql);	
	}	
	if($_REQUEST['sbdid'] && $pdcid > 0){
		$sql = '';
		foreach($_REQUEST['sbdid'] as $sbdid){
			$sql .= "
				insert into par.subacaoparecerdacomissao(
						sbdid,
						pdcid
				)values(
						{$sbdid}, 
						{$pdcid}
				) returning spcid;
			";
		}
	}	
	$parecer = $db->pegaUm($sql);
	$db->commit();
	
	if($parecer['spcid'] > 0){
		die("ok");
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";

monta_titulo('Análise da Comissão', 'Pesquisa');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$('#btn_pesquisar').click(function(){

		/*if($('[name=sbdano]').val() == ''){
			alert('Selecione um ano primeiro!');
			$('[name=sbdano]').focus();
			return false;
		}*/

		if($('[name=estuf]').val() == ''){
			alert('Selecione uma Unidade Federativa!');
			$('[name=estuf]').focus();
			return false;
		}

		selectAllOptions( document.getElementById( 'grupo' ) );
        selectAllOptions( document.getElementById( 'ideb' ) );
		
		$('[name=formulario]').submit();
	});

	$('.btn_aprovar').click(function(){

		var parecer = $('[name=parecer]').val();
		parecer = trim(parecer);

		var muncod  = $('[name=muncod]').val();
		var estuf	= $('[name=estuf]').val();
		
		if(parecer == ''){
			alert('O campo parecer é obrigatório!');
			$('[name=parecer]').focus();
			return false;
		}

		var total = $('[name=sbdid[]]:checked:enabled').length;
		
		if(total < 1){
			alert('Selecione pelo menos uma subação!');
			return false;
		}

		var sbdid = $('[name=sbdid[]]:checked:enabled').serialize();

		$.ajax({
			url		: 'par.php?modulo=principal/analiseComissao&acao=A',
			type	: 'post',
			data	: 'requisicao=aprovarSubacoes&pdcparecer='+parecer+'&'+sbdid+'&muncod='+muncod+'&estuf='+estuf,
			success	: function(e){
				if(e == "ok"){
					alert('Plano aprovado com sucesso!');
					$('[name=formulario]').submit();
				}
			}		
		});		
	});	 	
});

function abrirSubacao(estuf, muncod, inuid, sbaid)
{
	$.ajax({
		url		: 'par.php?modulo=principal/analiseComissao&acao=A',
		type	: 'post',
		data	: 'requisicao=criaSessaoPlano&estuf='+estuf+'&muncod='+muncod+'&inuid='+inuid,
		success	: function(e){
			if(e=='true'){
				var popUp = window.open('par.php?modulo=principal/subacao&acao=A&sbaid='+sbaid, 'popSubacao', 'height=600,width=800,scrollbars=yes,top=50,left=200');
				popUp.focus();
			}else{
				alert('Não foi possível abrir o plano!');
			}			
		}
	});	
}

function abrirArvore(estuf, muncod, inuid)
{		
	$.ajax({
		url		: 'par.php?modulo=principal/analiseComissao&acao=A',
		type	: 'post',
		data	: 'requisicao=criaSessaoPlano&estuf='+estuf+'&muncod='+muncod+'&inuid='+inuid,
		success	: function(e){

			if(e=='true'){
				var popUp = window.open('par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore', 'popSubacao', 'height=600,width=800,scrollbars=yes,top=50,left=200');
				popUp.focus();
			}else{
				alert('Não foi possível abrir o plano!');
			}			
		}
	});	
}

function cancelarAprovacao(sbdid)
{
	if(confirm('Deseja cancelar a aprovação?')){
		$.ajax({
			url		: 'par.php?modulo=principal/analiseComissao&acao=A',
			type	: 'post',
			data	: 'requisicao=cancelarAprovacao&sbdid='+sbdid,
			success	: function(e){
				$('#sbdid_'+sbdid).attr({'disabled':false,'checked':false});
				$('#img_'+sbdid).remove();
				$('[name=formulario]').submit();	
			}
		});
	}	
}

function abrirDetalhesParecer(pdcid, estuf, muncod){
	$.ajax({
		url		: 'par.php?modulo=principal/analiseComissao&acao=A',
		type	: 'post',
		data	: 'requisicao=abrirDetalhesParecer&pdcid='+pdcid,
		success	: function(e){
			var popUp = window.open('par.php?modulo=principal/detalheParecerAprovado&acao=A&pdcid='+pdcid+'&estuf='+estuf+'&muncod='+muncod, 'popDetalheParecer', 'height=450,width=820,scrollbars=yes,top=50,left=200');
			popUp.focus();
		}
	});	
}

</script>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td width="220" class="subtituloDireita">Ano</td>
			<td>
				<?php 
					$sql = "
						Select 	distinct sbdano as codigo,
								sbdano as descricao
						From par.subacaodetalhe 
						order by sbdano
					";
					$sbdano = $_POST['sbdano'];
					$db->monta_combo('sbdano', $sql, 'S', 'Todos os anos', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td>
				<?php
					$municipio = simec_htmlentities( $_POST['municipio'] );
					echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Estado</td>
			<td>
				<?php					
					$sql = "select 
								e.estuf as codigo, 
								e.estdescricao as descricao 
							from 
								territorios.estado e 
							order by 
								e.estdescricao asc";
					
					$estuf = $_POST['estuf'];
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', '', '','','','','estuf');
					echo '&nbsp; <img src="../imagens/obrig.gif" title="Indica campo obrigatório.">';
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Grupo de município</td>
			<td>
				<?php
				// Filtros do relatório
				$stSql = "SELECT
							tpmid AS codigo,
							tpmdsc AS descricao
						  FROM
						  	territorios.tipomunicipio mtq
						  WHERE
						  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)";
				
				if( $_REQUEST['grupo'][0] != '' ){
					$grupos = implode( $_REQUEST['grupo'], "," );
					$sql = "SELECT
							tpmid AS codigo,
							tpmdsc AS descricao
						  FROM
						  	territorios.tipomunicipio mtq
						  WHERE
						  	tpmid IN ({$grupos})";
					$grupo = $db->carregar( $sql );
				}

				combo_popup( "grupo", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Classificação IDEB</td>
			<td>
				<br />
				<style>
					#ideb_, #ideb { width: 300px; }
				</style>
				<table border="0" cellpadding="3" cellspacing="0">
					<tr>
						<td width="342">
							Classificação IDEB
						</td>
						<td>
							Filtrar por:
						</td>
					</tr>
				</table>
				<?php

				$agrupador = new Agrupador( "formulario" );
				
				$sql = sprintf( "SELECT tpmid, tpmdsc FROM territorios.tipomunicipio WHERE gtmid = 2 AND tpmstatus = 'A'" );
				$tipos = $db->carregar( $sql );
				
				$origem = array();
				$destino = array();
				if ( $tipos ) {
					foreach ( $tipos as $tipo ) {
						if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
							$destino[] = array(
								'codigo'    => $tipo['tpmid'],
								'descricao' => $tipo['tpmdsc']
							);
						} else {
							$origem[] = array(
								'codigo'    => $tipo['tpmid'],
								'descricao' => $tipo['tpmdsc']
							);
						}
					}
				}
				
				$agrupador->setOrigem( "ideb_", null, $origem );
				$agrupador->setDestino( "ideb", null, $destino );
				$agrupador->exibir();

				?>
			</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">Tipo de subação</td>
			<td>
			<?php 
				$sql = "SELECT 
					    	pts.ptsid as codigo,
							tps.tpsdescricao as descricao
						FROM
							par.propostatiposubacao pts
						
						INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
						WHERE 
							pts.ptsstatus = 'A'";
				
				$db->monta_combo('ptsid', $sql, 'S', 'Selecione...', '', '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Programa</td>
			<td>
			<?php 
				$sql = "select 
							prgid as codigo,
							prgdsc as descricao
						from 
							par.programa
						where 
							prgstatus = 'A'";
				
				$db->monta_combo('prgid', $sql, 'S', 'Selecione...', '', '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">				
				<input type="button" value="Pesquisar" id="btn_pesquisar" />				
				<input type="button" value="Limpar" id="btn_limpar" />
			</td>
		</tr>
	</table>
</form>
<?php if($_POST): ?>
	<br/>
	<?php 
	monta_titulo('Tela de Aprovação de Programa', 'Lista de Programa (Subações)');
		
	$arWhere = array();
	$arInner = array();
	
	# Ano da subação
	if($_REQUEST['sbdano']){
		$arWhere[] = " sd.sbdano = '{$_REQUEST['sbdano']}' ";
	}else{
		$arWhere[] = " sd.sbdano in (2011, 2012, 2013, 2014)";
	}
	
	# Estado
	if($_REQUEST['estuf']){
		$arWhere[] = " e.estuf = '{$_REQUEST['estuf']}' ";
	}
	
	# Municipio
	if($_REQUEST['municipio']){
		$arWhere[] = " m.mundescricao ilike '%{$_REQUEST['municipio']}%' ";
	}
	
	# Tipo de subação
	if($_REQUEST['ptsid']){
		$arWhere[] = " s.ptsid = {$_REQUEST['ptsid']} ";
	}
	
	# Programa
	if($_REQUEST['prgid']){
		$arWhere[] = " s.prgid = {$_REQUEST['prgid']} ";
	}
	
	# Grupo de município
	if( $_REQUEST['grupo'][0] ){
		$arInner[] = "INNER JOIN 
							territorios.muntipomunicipio mtm on mtm.muncod = m.muncod 
								AND mtm.estuf = m.estuf 
								AND mtm.tpmid IN (". implode( ",", $_REQUEST['grupo'] ) .") ";
	}
	
	# Classificação Ideb
	if( $_REQUEST['ideb'][0] ){
		$arInner[] = "INNER JOIN 
							territorios.muntipomunicipio mti2 on mti2.muncod = m.muncod 
								AND mti2.estuf = m.estuf 
								AND mti2.tpmid in ('" . implode( "','", $_REQUEST['ideb'] ) . "') ";
	}
	
	$arCampo	= array();
	$cabecalho  = array('Ação');
	
	if(!$_REQUEST['estuf']){
		$arCampo[] = 'm.mundescricao,'; 
		array_push($cabecalho, 'Município');
	}else{
		$arCampo[] = 'e.estuf,'; 
		array_push($cabecalho, 'Estado');
	}
	
	array_push($cabecalho, 'Subação');
	array_push($cabecalho, 'Valor');
	array_push($cabecalho, 'Protocolo de aprovação');
	array_push($cabecalho, 'Ano');
	
	$btnAcao = '
		&nbsp;
		<input type="checkbox" name="sbdid[]" id="sbdid_\' || sd.sbdid || \'" value="\' || sd.sbdid || \'"  \' || case when spcid is not null then \'disabled="disabled" checked="checked"\' else \'\' end || \' />
		&nbsp;
		<img src="../imagens/alterar.gif" onclick="abrirSubacao(\'\'\' || coalesce(e.estuf,\'\') || \'\'\',\'\'\' || coalesce(m.muncod,\'\') || \'\'\',\' || coalesce(iu.inuid,0) || \',\' || s.sbaid || \')" style="cursor:pointer;"/>
		&nbsp;
		<img src="../imagens/consultar.gif" onclick="abrirArvore(\'\'\' || coalesce(e.estuf,\'\') || \'\'\',\'\'\' || coalesce(m.muncod,\'\') || \'\'\',\' || coalesce(iu.inuid,0) || \')" style="cursor:pointer;" />
		&nbsp;
		\'|| case when spcid is not null then \'<img src="../imagens/codigo.gif" id="img_\' || sd.sbdid || \'" onclick="cancelarAprovacao(\' || sd.sbdid || \')" style="cursor:pointer;" alt="Cancelar aprovação" title="Cancelar aprovação"/>\' else \'\' end || \'
		&nbsp;
	';
	
	$sql = "
		SELECT 	DISTINCT 
				'{$btnAcao}' as acao,				
				".(!empty($arCampo) ? implode(' ', $arCampo) : '')."
				s.sbadsc,		
				to_char(	
					CASE WHEN sbacronograma = 1 --global
	                   THEN(
	                      SELECT	CASE WHEN sic.icovalidatecnico IS NULL -- antigos
	                      			   THEN 
	                      			      sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric
	                        			ELSE -- novos
	                                	  CASE WHEN sic.icovalidatecnico = 'S' 
	                                	     THEN -- validado (caso não o item não é contado)
	                                            sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric
	                                	  END
	                        		END   
			                        as vlrsubacao
                           FROM par.subacaoitenscomposicao sic
                           WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                           GROUP BY sic.sbaid, sic.icovalidatecnico
			           --escolas
			       	   )ELSE(
			        	   SELECT	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
	                                   THEN -- escolas sem itens
	                                      CASE WHEN se.sesvalidatecnico IS NULL -- antigos
	                                         THEN
	                                           sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric
	                                         ELSE -- novos
	                                           sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric
	                                      END
	                                   ELSE -- escolas com itens
	                                      CASE WHEN se.sesvalidatecnico IS NULL -- antigos
	                                         THEN
	                                            sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric
	                                         ELSE -- novos
	                                            sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric
	                                      END
	                                   END as vlrsubacao
                           FROM par.subacaoitenscomposicao sic
                           INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
                           LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid
                           WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                           GROUP BY sic.sbaid, se.sesvalidatecnico
                    	)
                    END,'999G999G999G999D99') AS valorsubacao,
	                spc.pdcid, sd.sbdano
		FROM par.dimensao d
		INNER JOIN par.area a ON a.dimid = d.dimid
		INNER JOIN par.indicador i ON i.areid = a.areid
		INNER JOIN par.criterio c ON c.indid = i.indid
		INNER JOIN par.pontuacao p ON p.crtid = c.crtid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid		
		INNER JOIN par.acao ac ON ac.ptoid = p.ptoid
		INNER JOIN par.subacao s ON s.aciid = ac.aciid and s.sbastatus = 'A'
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
		INNER JOIN workflow.documento doc ON doc.docid = s.docid AND doc.esdid = ".WF_SUBACAO_ANALISE_COMISSAO."
		
		LEFT JOIN par.programa  pr ON s.prgid = pr.prgid
		LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod and iu.mun_estuf = m.estuf
		LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
		LEFT JOIN par.subacaoparecerdacomissao spc ON spc.sbdid = sd.sbdid						
		".( !empty($arInner) ? implode(' ',$arInner) : '' )."
		".(!empty($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')
		."order by sd.sbdano
	";
	//dbg($sql, 1);
	$db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', 'S', '', array('120'));
	?>
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Parecer</td>
			<td>
			<?php
				//$db->monta_combo('prgid', $sql, 'S', 'Selecione...', '', '', '');
				echo campo_textarea('parecer', 'S', 'S', '', 100, 8, 4000);
			?>
			</td>
		</tr>
		<tr>
			<td width="220" class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">				
				<input type="button" value="Aprovar" class="btn_aprovar"/>		
			</td>
		</tr>
	</table>

<?php endif;

	if($_POST){
		
		# Filtro de Estado
		if($_REQUEST['estuf']){
			$where .=  "where pc.estuf = '{$_REQUEST['estuf']}'";
		}
	
		# Municipio
		if($_REQUEST['municipio']){
			$where .= " m.mundescricao ilike '%{$_REQUEST['municipio']}%' ";
		}
		
		monta_titulo('Programas Aprovados', 'Lista de Programa Aprovados. (Subações)');
	
		$acao = "'&nbsp; <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer\"; onclick=\"abrirDetalhesParecer('||pdcid||','||chr(39)||pc.estuf||chr(39)||','||coalesce(pc.muncod,chr(39)||'null'||chr(39))||')\"/>'";
		
		$cabecalho = array("Ação", "N de protocolo", "Parecer", "Entidade","Data da aprovação","Nome");
		echo '<br>';
		$sql = "
			SELECT 	$acao as acao,
					pdcid as id,
					substring(pdcparecer, 1, 160)||'...' as pdcparecer,
					CASE WHEN pc.estuf IS NOT NULL THEN pc.estuf ELSE m.mundescricao END  as entidade ,  
					to_char(pdcdata, 'DD/MM/YYYY') as dataprovacao, 
					usunome as nome 
			FROM par.parecerdacomissao pc
			LEFT JOIN territorios.municipio m ON m.muncod = pc.muncod 
			INNER JOIN seguranca.usuario u ON u.usucpf = pc.usucpf
			$where
		";
//die($sql);
		$db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', 'S', '');
	}
?>