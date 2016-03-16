<?php
header('content-type: text/html; charset=ISO-8859-1');
// Caso não tenha o inuid na sessão irá retornar erro e fechar o window
if( ! $_SESSION['par']['inuid'] )
{
	echo "
			<script language=\"javascript\" type=\"text/javascript\"> 
				alert('Faltam dados na sessão');
				window.close();
			</script>";
	die();
}

if( $_POST['requisicao'] == 'salvar' ){
	salvarSubacao($_POST);
	exit();
}

function salvarSubacao( $dados ){
	global $db;
	
	if( !$dados['aciid'] ){
		//Adiciono a ação
		$sql = "SELECT 
					pa.ppaid, pa.ppadsc, p.ptoid
				FROM par.indicador i
					inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
					inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
					inner join par.propostaacao pa ON pa.crtid = c.crtid
				WHERE 
					i.indid = {$dados['indid']} 
					and p.inuid = {$_SESSION['par']['inuid']}";
		$dadosPropostaAcao = $db->pegaLinha( $sql );
		
		$oAcao 			= new Acao();		
		$oAcao->ptoid		= $dadosPropostaAcao['ptoid']; 
		$oAcao->acidsc		= $dadosPropostaAcao['ppadsc'];
	    $oAcao->acistatus	= 'A';
	    $oAcao->acidata		= 'NOW()';
	    $oAcao->usucpf		= $_SESSION['usucpf'];
		$aciid = $oAcao->salvar();
		$oAcao->commit();
	} else {
		$aciid = $dados['aciid'];
	}
	
	$sql = "select MAX( sbaordem ) from par.subacao where sbastatus = 'A' AND aciid = ".$aciid;		
	$qnt = $db->pegaUm( $sql ) + 1;
	
	$arCamposNulo = array();	
	$oSubacao = new Subacao();
	
	$desc = 'Em Atualização';
	$esdid = WF_SUBACAO_ATUALIZACAO;
	
	$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
			VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
	$docid = $db->pegaUm($sql);
		
	if( empty($dados['foaid'])  ) $arCamposNulo = array( 'foaid' ); else $oSubacao->foaid = $dados['foaid'];
	$oSubacao->sbadsc = $dados['sbadsc'];
	$oSubacao->sbaordem = $qnt;
	$oSubacao->frmid = $dados['frmid'];
	$oSubacao->ptsid = $dados['ptsid'];
	$oSubacao->indid = $dados['indid'];
	$oSubacao->aciid = $aciid;
	if( $dados['undid'] ){
		$oSubacao->undid = $dados['undid'];
	}
	$oSubacao->prgid = 2;
	$oSubacao->sbacronograma = $dados['sbacronograma'];
	$oSubacao->docid = $docid;
	$oSubacao->sbaestrategiaimplementacao = $dados['sbaestrategiaimplementacao'];
	$oSubacao->salvar(true, true, $arCamposNulo);
	$oSubacao->commit();
	
	echo "<script>
		alert('Operação realizada com sucesso!');
		window.close();
 	</script>";
	exit;
}

if( $_REQUEST['carregarArea'] ){
	$sql = "SELECT
				areid as codigo,
				substr(arecod || '.' || aredsc, 0, 200) as descricao
			FROM 
				par.area 
			WHERE 
				arestatus = 'A' AND
				dimid = ".$_REQUEST['dimid']."
			ORDER BY
				arecod";
	
	return $db->monta_combo( "areid", $sql, 'S', 'Selecione a Área', 'carregaIndicador(this.value);', '', '', 650 );
}

if( $_REQUEST['carregarIndicador'] ){
	$sql = "SELECT
				indid as codigo,
				substr(indcod || '.' || inddsc, 0, 200) as descricao
			FROM 
				par.indicador 
			WHERE 
				indstatus = 'A' AND
				areid = ".$_REQUEST['areid']."
			ORDER BY
				indcod";
	
	return $db->monta_combo( "indid", $sql, 'S', 'Selecione o Indicador', 'carregarAcao(this.value);', '', '', 650 );
}

if( $_REQUEST['carregarAcao'] ){
	
	$sql = "select
						a.aciid, a.acidsc
					from
						par.indicador i
					inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
					inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
					inner join par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
					where 
						i.indid = {$_REQUEST['indid']} AND p.inuid = {$_SESSION['par']['inuid']}";
	$arrAcao = $db->pegaLinha($sql);
	$html = '<input type="hidden" name="aciid" id="aciid" value="'.$arrAcao['aciid'].'" />'.$arrAcao['acidsc'];
	
	echo $html;
	exit();
}

if($_POST['filtraForm'] == 1){
	$displayTipoSubAcao = 'S';
	$frmid = $_REQUEST['frmid'];
	if(!$frmid || $frmid == '2' || $frmid == '4' ) $displayTipoSubAcao = 'N';
		
		if( $frmid == '12' || $frmid == '4' ){
			$where = "AND ptsid IN (46,42)";			
		}
	
		$arrTipoSubacao = array();
		if($frmid){
			
			$sql = "SELECT
						pts.ptsid as codigo,
						tps.tpsdescricao as descricao
					FROM
						par.propostatiposubacao pts
					INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
					WHERE
						frmid = $frmid
					{$where}
					AND
						ptsstatus = 'A'
					ORDER BY
						descricao";
					
			$arrTipoSubacao = $db->carregar($sql);
		}
		if($arrTipoSubacao){
			$diplay = "";
			$displayTipoSubAcao = "S";
		}else{
			$diplay = "none";
			$displayTipoSubAcao = "N";
		}
		
	if($arrTipoSubacao){
		echo '<tr id="tr_combo_tiposubaacao" style="display: <?php echo $diplay;?>;">
			<td class="SubTituloDireita">Tipo da subação:</td>
			<td id="td_combo_tiposubaacao">
					'.$db->monta_combo('ptsid', $arrTipoSubacao, 'S', 'Selecione...', '','', '','',$displayTipoSubAcao,'','',$ptsid).'
			</td>
		</tr>';
	}
	die();
}

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="">
<input type="hidden" name="requisicao" id="requisicao" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" style="color: blue; font-size: 22px">
			<a href="#">
				<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
	</tr>
	<tr id="trdimensao" style="display: '';">
		<td class="SubTituloDireita">Dimensão:</td>
		<td>
		<?
		$sql = "SELECT 
					dimid as codigo, 
					substr(dimcod || '.' || dimdsc, 0, 200) as descricao 
				FROM 
					par.dimensao 
				WHERE 
					dimstatus = 'A' AND
					itrid = ".$_SESSION['par']['itrid']."
				ORDER BY
					dimcod";
	
		echo $db->monta_combo( "dimid", $sql, 'S', 'Selecione a Dimensão', 'carregaArea', '', '', 650 );
		?>
		</td>
	</tr>
	<tr id="trarea" style="display: none;">
		<td class="SubTituloDireita">Área:</td>
		<td id="area"></td>
	</tr>
	<tr id="trindicador" style="display: none;">
		<td class="SubTituloDireita">Indicador:</td>
		<td id="indicador"></td>
	</tr>
	<tr id="tracao" style="display: none;">
		<td class="SubTituloDireita">Ação:</td>
		<td id="acao"></td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" style="display: none" id="tb_dados" cellSpacing="1" cellPadding="3" align="center">
	<tr style="background-color: #cccccc;">
		<td align="left" colspan="2"><strong>Dados da Subação</strong></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Forma de execução:</td>
		<td>
			<?php $frmid = $oPropostaSubacao->frmid ? $oPropostaSubacao->frmid : $_GET['frmid'] ?>
			<?php
			$arrPerfil = pegaPerfilGeral();
			if($_SESSION['par']['itrid'] == 1){ //estado
				$perf = FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO;
			}elseif( $_SESSION['par']['itrid'] == 2 ){ //municipio
				$perf = FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO;
			}
			if( in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) ){
				$perf = FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO.", ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO;
			}
			
			$sql = "SELECT 
						frmid as codigo, 
						frmdsc as descricao 
					FROM par.formaexecucao
					WHERE
						frmid in (".FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA.", ".$perf.")
					ORDER BY frmdsc";

			$db->monta_combo('frmid', $sql, 'S', 'Selecione...', 'filtraTipoSubacao','','','','S');
			?>
		</td>
	</tr>
	<?
		$diplay = "none";
	?>
	<tr id="tr_combo_tiposubaacao" style="display: <?php echo $diplay;?>;">
		<td class="SubTituloDireita">Tipo da subação:</td>
		<td id="td_combo_tiposubaacao">
		</td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita" >Descrição da Subação:</td>
		<td><?php echo campo_textarea('sbadsc',"S","S","",90,5,"","") ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estratégia de Implementação:</td>
		<td><?php echo campo_textarea('sbaestrategiaimplementacao',"S","S","",90,5,"","") ?></td>
	</tr>
	<tr id="tr_combo_undid">
		<td class="SubTituloDireita">Unidade de Medida:</td>
		<td>
			<?php $sql = "select undid as codigo, unddsc as descricao from par.unidademedida where undstatus = 'A' order by descricao"; ?>
			<?php $db->monta_combo("undid",$sql,"S","Selecione...","","","","","S") ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cronograma:</td>
		<td>
			<?php $arrCronograma = array( 0 => array("codigo" => 1, "descricao" => "Global"), 1 => array("codigo" => 2, "descricao" => "Escola")  ) ?>
			<?php $db->monta_combo("sbacronograma",$arrCronograma,"S","Selecione...","","","","","S") ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td class="SubTituloEsquerda">
			<input type="button" value="Salvar" name="btn_salvar" id="btn_salvar" style="display: none" onclick="salvarSubacao()" />
			<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function carregaArea(dimid){
	if( dimid ){
		divCarregando();
		$.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarArea=1&dimid='+dimid,
			  success	: function(res) {
			  		$( '#indicador' ).html( "" );
			  		$( '#subacao' ).html( "" );
			  		$( '#area' ).html( res );
			  		$( '#trarea' ).show();
			  		$( '#grupoentidade' ).hide();
			  		$( '#trexcel' ).hide();
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o dimid!');
	}
}
			
function carregaIndicador(areid){
	if( areid ){
		divCarregando();
		$.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarIndicador=1&areid='+areid,
			  success	: function(res) {
			  		$( '#subacao' ).html( "" );
			  		$( '#indicador' ).html( res );
			  		$( '#trindicador' ).show();
			  		$( '#grupoentidade' ).hide();
			  		$( '#trexcel' ).hide();
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o areid!');
	}
}

function carregarAcao(indid){
	if( indid ){
		divCarregando();
		$.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarAcao=1&indid='+indid,
			  success	: function(res) {
			  		$( '#acao' ).html( res );
			  		$( '#tracao' ).show();
			  		$( '#btn_salvar' ).show();
			  		$( '#tb_dados' ).show();
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o indid!');
	}
}

function salvarSubacao(){
	var formulario = document.formulario;
	
	if($('[name="dimid"]').val() == ''){
		alert('O campo Dimensão é de preenchimento obrigatório!');
		$('[name="dimid"]').focus();
		return false;
	}
	if($('[name="areid"]').val() == ''){
		alert('O campo Área é de preenchimento obrigatório!');
		$('[name="areid"]').focus();
		return false;
	}
	if($('[name="indid"]').val() == ''){
		alert('O campo Indicador é de preenchimento obrigatório!');
		$('[name="indid"]').focus();
		return false;
	}
	if($('[name="frmid"]').val() == ''){
		alert('O campo Forma de execução é de preenchimento obrigatório!');
		$('[name="frmid"]').focus();
		return false;
	}	
	if($('[name="ptsid"]').val() == ''){
		alert('O campo Tipo da subação é de preenchimento obrigatório!');
		$('[name="ptsid"]').focus();
		return false;
	}
	if($('[name="sbadsc"]').val() == ''){
		alert('O campo Descrição da Subação é de preenchimento obrigatório!');
		$('[name="sbadsc"]').focus();
		return false;
	}
	if($('[name="sbaestrategiaimplementacao"]').val() == ''){
		alert('O campo Estratégia de Implementação é de preenchimento obrigatório!');
		$('[name="sbaestrategiaimplementacao"]').focus();
		return false;
	}
	if($('[name="undid"]').val() == '' && document.getElementById('tr_combo_undid').style.display != 'none'){
		alert('O campo Unidade de Medida é de preenchimento obrigatório!');
		$('[name="undid"]').focus();
		return false;
	}
	if($('[name="sbacronograma"]').val() == ''){
		alert('O campo Cronograma é de preenchimento obrigatório!');
		$('[name="sbacronograma"]').focus();
		return false;
	}
	
	$('#requisicao').val('salvar');	
	$("#formulario").submit();	
}

function filtraTipoSubacao(frmid){
	if(frmid){
	
		if( frmid == 6 ){
			$('#tr_combo_undid').hide();
		} else {
			$('#tr_combo_undid').show();
		}
		$.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'filtraForm=1&frmid='+frmid+'&indid='+$('[name="indid"]').val(),
			  success	: function(res) {
			  		if(res){
						$('#tr_combo_tiposubaacao').show();
						$('[name=ptsid]').attr("class",'CampoEstilo obrigatorio');
						$('#td_combo_tiposubaacao').html(res);
					}else{
						$('[name=ptsid]').attr("class",'CampoEstilo');
						$('#tr_combo_tiposubaacao').hide();
					}
			  }
		});
	}
}

</script>