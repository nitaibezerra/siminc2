<?php

if( $_POST['requisicao'] == 'salvarSubacao' ){
	$arrInuid = array();
	$arrInuidEmpenhado = array();
	$arrInuidSemSubacao = array();
	$sqlUpdate = "";
	
	if( $_POST['tipo'] == 1 ){ //Estadual
		if( is_array( $_POST['estados'] ) ){
			foreach( $_POST['estados'] as $estuf ){
				$arrInuid[] = $db->pegaLinha("SELECT inuid, estuf as local FROM par.instrumentounidade WHERE estuf = '".$estuf."'");
			}
		}
	} else { //Municipal
		if( is_array( $_POST['municipios'] ) ){
			foreach( $_POST['municipios'] as $muncod ){
				$arrInuid[] = $db->pegaLinha("SELECT iu.inuid, m.mundescricao || '/' || m.estuf as local FROM par.instrumentounidade iu INNER JOIN territorios.municipio m ON m.muncod = iu.muncod WHERE iu.muncod = '".$muncod."'");
			}
		}
	}

	foreach( $arrInuid as $inuid ){

		$sql = "SELECT
					s.sbaid
				FROM
					par.subacao s
				INNER JOIN par.acao a ON a.aciid = s.aciid
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.inuid = {$inuid['inuid']}
				WHERE
					s.sbastatus = 'A' AND s.ppsid = {$_POST['ppsid']}";
		
		$sbaid = $db->pegaUm($sql);

		if( $sbaid ){
			$sql = "SELECT sbaid FROM par.empenhosubacao WHERE eobano = date_part('YEAR',  current_date) and eobstatus = 'A' AND sbaid = ".$sbaid;
			$sbaidEmpenhado = $db->pegaUm($sql);
			if( $sbaidEmpenhado ){
				$arrInuidPulo[$inuid['inuid']]['inuid'] = $inuid['inuid'];
				$arrInuidPulo[$inuid['inuid']]['local'] = $inuid['local'];
				$arrInuidPulo[$inuid['inuid']]['motivo'] = 'Já foi empenhado.';
			} else {
				$sqlUpdate .= "UPDATE par.subacaodetalhe SET sbdplanointerno = '{$_POST['planointerno']}', sbdptres = '{$_POST['ptres']}' WHERE sbaid = {$sbaid} AND sbdano = date_part('YEAR',  current_date); ";
			}
		} else {
			$arrInuidPulo[$inuid['inuid']]['inuid'] = $inuid['inuid'];
			$arrInuidPulo[$inuid['inuid']]['local'] = $inuid['local'];
			$arrInuidPulo[$inuid['inuid']]['motivo'] = 'Não possui subação.';
		}
	}

	if( $sqlUpdate ){
		$db->executar( $sqlUpdate );
	}
	
	if(count($arrInuidPulo) > 0){
		enviaEmailInuidPiPtres( $arrInuidPulo );
	}
	
	if( $db->commit() ){
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.close();
				</script>";
	} else {
		echo "<script>
					alert('Erro no processo!');
					window.location.href = window.reload;
				</script>";
	}	
}

if( $_POST['requisicao'] == 'carregarPlanoInterno' ){
	
	$prgid = $db->pegaUm( "select prgid from par.propostasubacao where ppsid = {$_REQUEST['ppsid']}" );
	
	$sql = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
				FROM
					par.planointerno
				WHERE  prgid = ".$prgid;
	echo $db->monta_combo( "planointerno", $sql, 'S', 'Selecione...', 'filtraPTRES', '', '','','S', 'planointerno', false, $planointerno, 'Plano Interno' );
	exit();
}

if( $_POST['requisicao'] == 'carregarPtres' ){
	
	$sql = "select DISTINCT
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$_POST['plicod']}'";
			
	echo $db->monta_combo( "ptres", $sql, 'S', 'Selecione...', '', '', '','','S', 'ptres', false, $ptres, 'PTRES' );	
	exit();
}
if( $_REQUEST['carregarDimensao'] ){
	
	$_SESSION['par']['cadSubacao']['itrid'] = $_REQUEST['itrid'];
	
	$sql = "SELECT 
				dimid as codigo, 
				substr(dimcod || '.' || dimdsc, 0, 200) as descricao 
			FROM 
				par.dimensao 
			WHERE 
				dimstatus = 'A' AND
				itrid = ".$_REQUEST['itrid']."
			ORDER BY
				dimcod";
	
	return $db->monta_combo( "dimid", $sql, 'S', 'Selecione a Dimensão', 'carregaArea(this.value);', '', '', 500 );
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
	
	return $db->monta_combo( "areid", $sql, 'S', 'Selecione a Área', 'carregaIndicador(this.value);', '', '', 500 );
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
	
	return $db->monta_combo( "indid", $sql, 'S', 'Selecione o Indicador', 'carregaSubacao(this.value);', '', '', 500 );
}

if( $_REQUEST['carregarSubacao'] ){
	$sql = "select 
				pps.ppsid as codigo,
				substr(pps.ppsordem || '.' || pps.ppsdsc, 0, 200) as descricao
			from 
				par.propostasubacao pps
			where 
				pps.indid = ".$_REQUEST['indid']."
			ORDER BY
				pps.ppsordem";
	
	return $db->monta_combo( "ppsid", $sql, 'S', 'Selecione a Subação', 'mostraGrupoEntidade( this.value );', '', '', 500 );
}

if( $_REQUEST['carregarDadosSubacao'] ){
	$sql = "select
				ppsdsc,
				frmid, 
				ppsestrategiaimplementacao, 
				prgid, 
				undid, 
				ppscronograma, 
				ppsmonitora, 
				foaid,
				ptsid
			from 
				par.propostasubacao 
			where 
				ppsid = ".$_REQUEST['ppsid'];
	
	$dados = $db->pegaLinha( $sql );
	
	echo $dados['ppsdsc'].'|'.$dados['frmid'].'|'.$dados['ppsestrategiaimplementacao'].'|'.$dados['prgid'].'|'.$dados['undid'].'|'.$dados['ppscronograma'].'|'.$dados['ppsmonitora'].'|'.$dados['foaid'].'|'.$dados['ptsid'];	
	exit();
}

if( $_REQUEST['carregarTipoEstadoMunicipio'] ){
	
	if($_REQUEST['itrid'] == 2){
		
		/*$sqlComboIDEB = "select
							tpmid as codigo,
							tpmdsc as descricao
						from territorios.tipomunicipio
						where
							gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
							tpmstatus = 'A'";*/
		$sqlGrupoMunicipios = "select * from (
						(select 
							tpmid as codigo,
							case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
						from  
							territorios.tipomunicipio
						where 
							tpmid in (1, 16, 17, 140, 141, 150, 151, 152, 154)
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
		
		$dado = '<table>
					<tr>
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
				        		<option value="">Duplo clique para selecionar da lista</option>
					        </select>
						</td>
					</tr>
			</table>';
				
			echo $dado;
	} else {
		echo '<td bgcolor="#e9e9e9" align="right">Estados</td>
				<td>
				<select 
					multiple="multiple" 
					size="5" 
					name="estados[]" 
		        	id="estados"  
		        	ondblclick="abrepopupEstado();"  
		        	class="CampoEstilo" 
		        	style="width:400px;" >
	        		<option value="">Duplo clique para selecionar da lista</option>
		        </select>
			</td>';
	}
	exit();
}

$sql = "SELECT
			d.dimcod, 
			d.dimdsc, 
			a.arecod, 
			a.aredsc, 
			i.indcod, 
			i.inddsc 
		FROM
			par.dimensao d
		INNER JOIN par.area a ON a.dimid = d.dimid
		INNER JOIN par.indicador i ON i.areid = a.areid 
		WHERE
			i.indid = ".$_REQUEST['indid'];
//$dados = $db->pegaLinha($sql);

$sql = "select acidsc from par.acao WHERE acistatus = 'A' AND aciid = ".$_REQUEST['aciid'];
//$acao = $db->pegaUm($sql);

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Atulização de Plano Interno e PTRES - PAR', '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="">
	<input type="hidden" name="indid" value="<?php echo $_REQUEST['indid'] ?>" />
	<input type="hidden" name="aciid" value="<?php echo $_REQUEST['aciid'] ?>" />
	<input type="hidden" name="ppsid" id="ppsid" value="" />
	<input type="hidden" name="itridHidden" id="itridHidden" value="" />
	<input type="hidden" name="requisicao" value="salvarSubacao" />
	<input type="hidden" name="salvaformulario" id="salvaformulario" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<colgroup>
		<col width="25%" />
		<col width="75%" />
	</colgroup>
	<tr id="trtipo">
		<td class="SubTituloDireita">Tipo de Entidade:</td>
		<td><input type="radio" name="tipo" onclick="javascript:carregaDimensao(this.value);" value="1">Estadual<input type="radio" name="tipo" onclick="javascript:carregaDimensao(this.value);" value="2">Municipal</td>
	</tr>
	<tr id="trdimensao" style="display: none;">
			<td class="SubTituloDireita">Dimensão:</td>
			<td id="dimensao"></td>
	</tr>
	<tr id="trarea" style="display: none;">
		<td class="SubTituloDireita">Área:</td>
		<td id="area"></td>
	</tr>
	<tr id="trindicador" style="display: none;">
		<td class="SubTituloDireita">Indicador:</td>
		<td id="indicador"></td>
	</tr>
	<tr id="trsubacao" style="display: none;">
		<td class="SubTituloDireita">Subação:</td>
		<td id="subacao"></td>
	</tr>
	<tr id="grupoentidade" style="display: none;">
		<td class="SubTituloDireita">Grupo de Entidade:</td>
		<td align="left">
			<table>
				<tr id="tipoEstadoMunicipio"></tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<table id="tab2" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="display: none;">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Preenchimento do PI e PTRES</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="15%"><b>Plano Interno:</b></td>
				<td>
					<span id="planointernoSPAN">
					<?php
					$sql = "SELECT 	DISTINCT
									plinumplanointerno as codigo,
									plinumplanointerno as descricao
							FROM
								par.planointerno
							WHERE 1 = 2";
					$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', '', '', '', '', 'S' );
					?>
					</span>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>PTRES:</b></td>
				<td>
					<span id="ptresSPAN">
					<?php
					$sql = "select 
								pliptres as codigo,
								pliptres as descricao
							from par.planointerno
							where 1 = 2";
					$db->monta_combo( "ptres", $sql, 'S', 'Selecione', '', '', '', '', 'S' );
					?>
					</span>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td class="SubTituloEsquerda" colspan="4">
			<input type="button" value="Salvar" name="btn_salvar" onclick="salvarSubacao()" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function carregaDimensao(itrid){
	if( itrid ){
		divCarregando();
  		jQuery( '#itridHidden' ).val( itrid );
		jQuery.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarDimensao=1&itrid='+itrid,
			  success	: function(res) {
			  		jQuery( '#area' ).html( "" );
			  		jQuery( '#indicador' ).html( "" );
			  		jQuery( '#subacao' ).html( "" );
			  		jQuery( '#dimensao' ).html( res );
			  		jQuery( '#grupoentidade' ).hide();
			  		jQuery( '#trdimensao' ).show();
			  		jQuery( '#trarea' ).show();
			  		jQuery( '#trindicador' ).show();
			  		jQuery( '#trsubacao' ).show();
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o itrid!');
	}
}

function carregaArea(dimid){
	if( dimid ){
		divCarregando();
		jQuery.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarArea=1&dimid='+dimid,
			  success	: function(res) {
			  		jQuery( '#indicador' ).html( "" );
			  		jQuery( '#subacao' ).html( "" );
			  		jQuery( '#area' ).html( res );
			  		jQuery( '#grupoentidade' ).hide();
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
		jQuery.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarIndicador=1&areid='+areid,
			  success	: function(res) {
			  		jQuery( '#subacao' ).html( "" );
			  		jQuery( '#indicador' ).html( res );
			  		jQuery( '#grupoentidade' ).hide();
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o areid!');
	}
}

function carregaSubacao(indid){
	if( indid ){
		divCarregando();
		jQuery.ajax({
			  type		: 'post',
			  url		: window.location,
			  data		: 'carregarSubacao=1&indid='+indid,
			  success	: function(res) {
			  		jQuery( '#subacao' ).html( res );
			  }
		});
		divCarregado();
	} else {
		alert('Erro: Falta o indid!');
	}
}

function filtraPTRES(plicod) {
	divCarregando();
	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=carregarPtres&plicod="+plicod,
		success: function(msg){
			document.getElementById("ptresSPAN").innerHTML = msg;
		}
	});
	divCarregado();
}

function mostraGrupoEntidade( ppsid ){
	
	itrid = jQuery( '#itridHidden' ).val();
	jQuery( '#ppsid' ).val( ppsid );
	
	jQuery.ajax({
		  type		: 'post',
		  url		: window.location,
		  data		: 'carregarTipoEstadoMunicipio=1&itrid='+itrid,
		  success	: function(res) {
				jQuery( '#grupoentidade' ).show();
				jQuery( '#tab2' ).show();
		  		jQuery( '#tipoEstadoMunicipio' ).html( res );
		  }
	});
	
	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=carregarPlanoInterno&ppsid="+ppsid,
		async: false,
		success: function(msg){
			document.getElementById('planointernoSPAN').innerHTML=msg;
	}
	});
	
	//filtraPTRES( document.getElementById('planointerno').value );
}

function salvarSubacao(){
	var erro = 0;
	var formulario = document.formulario;
	
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	
	if(erro == 0){
		if( $('[name="tipo"]::checked').val() == 1){ // Estadual
			selectAllOptions( formulario.estados );
			if($('#estados').val() == ''){
				alert('O campo Grupo de Entidade é obrigatório informar o(s) Estado(s).');
				return false;
			}
		} else if( $('[name="tipo"]::checked').val() == 2){ // Municipal
			selectAllOptions( formulario.municipios );
			if($('#municipios').val() == ''){
				alert('O campo Grupo de Entidade é obrigatório informar o(s) Município(s).');
				return false;
			}
		}
		formulario.submit();
	}
}

function abrepopupMunicipio(){
	window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_bandalarga.php','Municipios','width=400,height=400,scrollbars=1');
}

function abrepopupEstado(){
	window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/par.php?modulo=principal/popupEstados&acao=A','Estados','width=400,height=400,scrollbars=1');
}
</script>