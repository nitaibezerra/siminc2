<?php
$insid 					  = isset($_GET['insid']) ? $_GET['insid'] : $_SESSION['snf']['insid'];
$_SESSION['snf']['insid'] = $insid;

if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) ){
	if ( empty( $insid ) ){	
		//Chamada de programa
		include  APPRAIZ."includes/cabecalho.inc";
		?>
		<br />
		<?php 
		$db->cria_aba($abacod_tela,$url,'');
		monta_titulo('Dados da Intituição de Ensino', '');
	}
?> 
	<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

	<script>
	function selecionaInstituicao(insid){
		location.href = '?modulo=principal/cadastroPolo&acao=A&insid=' + insid;
	}
	</script>
	<?php 
	$sql = "SELECT
				DISTINCT
				ie.insid AS codigo,
				ie.insnome AS descricao
			FROM
				snf.dirigentemaximo dir
			JOIN
				snf.instituicaoensino ie ON ie.insid = dir.insid AND
											ie.insstatus = 'A'
			WHERE
				dir.dirstatus = 'A'
			ORDER BY
				descricao";
	
	$htmlComboEstado = '
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" class="SubtituloTabela bold" >Selecione uma Instituição</td>
			</tr>
			<tr class="center SubtituloDireita" >
				<td colspan="2" align="center">' 
				.
				$db->monta_combo("insid", $sql, "S", "Selecione uma Instituição...", "selecionaInstituicao", "", "", '', "N", '', true)
				. '
				</td>
			</tr>
		</table>';
				
	if ( empty($insid) ){
		echo $htmlComboEstado;
		die("<script>jQuery('#aguarde').hide();</script>");
	}
}


if( !isset($_SESSION['snf']['insid']) ){
	$_SESSION['snf']['insid'] = pegaInsid();
}

if(!$_SESSION['snf']['insid']){
	echo "<script>
			alert('Não existe nehuma instituição vinculada ao seu perfil!');
			document.location.href = 'snf.php?modulo=inicio&acao=C';
		  </script>";	
	exit();
}

if($_REQUEST['requisicao'] == 'carregarMunicipios'){
	ob_clean();
	
	$sql = "select 
				muncod as codigo,
				mundescricao as descricao
			from 
				territorios.municipio
			where
				estuf = '{$_REQUEST['estuf']}'
			order by
				mundescricao";
	
	$db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '', 'S','muncod1');
	die;
}

if($_REQUEST['requisicao'] == 'verificarCep'){
	ob_clean();
	
	$sql = "SELECT 
				muncod,
				logradouro,
				bairro,
				estado
			FROM 
				cep.v_endereco2
			WHERE 
				cep='".$_REQUEST['endcep']."' 
			ORDER BY 
				cidade ASC";
	
	$rs = $db->pegaLinha($sql);
	echo simec_json_encode($rs);
	die;
}

if($_REQUEST['requisicao'] == 'carregarPolo'){
	
	$sql = "select
				pl.polid,
				pl.insid, 
				pl.poldescricao, 
				pl.polsigla, 
				ed.muncod, 
				ed.estuf, 
				ed.endcep, 
				ed.endlog, 
				ed.endcom, 
				ed.endbai, 
				ed.endnum, 
				ed.endstatus, 
				ed.endlatitude, 
				ed.endlongitude,
				mun.mundescricao  
			from 
				snf.polo pl
			left join 
				snf.enderecopolo ed on ed.endid = pl.endid
			left join
				territorios.municipio mun ON mun.muncod = ed.muncod
			where 
				pl.polid = {$_REQUEST['polid']}";
	
	$rs = $db->pegaLinha($sql);
	extract($rs);
	//echo simec_json_encode($rs);
	//die;
}

if($_REQUEST['requisicao'] == 'deletarPolo'){
	
	if($_REQUEST['polid']){
		
		$sql = "delete from snf.enderecopolo where endid in (select endid from snf.polo where polid = {$_REQUEST['polid']});
				delete from snf.polo where polid = {$_REQUEST['polid']}";
		
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/cadastroPolo');		
	}	
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	$_REQUEST['insid'] 	= $_REQUEST['insid'] ? $_REQUEST['insid'] : 'null';
	$_REQUEST['endcep'] = $_REQUEST['endcep'] ? str_replace(array('.','-'), '', $_REQUEST['endcep']) : '';
	
	if($_REQUEST['polid']){
		
		$polid = $_REQUEST['polid'];
		
		$sql = "update snf.polo set
					poldescricao = '{$_REQUEST['poldescricao']}', 
					polsigla = '{$_REQUEST['polsigla']}'
				where 
					polid = {$polid}";
		
		$db->executar($sql);
		
	}else{
		
		$sql = "insert into snf.polo
					(
						insid, 
						poldescricao, 
						polsigla
					)
				values
					(
						{$_REQUEST['insid']}, 
						'{$_REQUEST['poldescricao']}', 
						'{$_REQUEST['polsigla']}'
					)
				returning polid;";
		
		$polid = $db->pegaUm($sql);
		
	}
	
	if($polid){
		
		$sql = "select endid from snf.polo where polid = {$polid}";
		$endid = $db->pegaUm($sql);
			
		if($endid){
			$sql = "update snf.enderecopolo set
						muncod 		 = '{$_REQUEST['muncod']}', 
						estuf		 = '{$_REQUEST['estuf']}', 
						endcep		 = '{$_REQUEST['endcep']}', 
						endlog		 = '{$_REQUEST['endlog']}', 
						endcom		 = '{$_REQUEST['endcom']}', 
						endbai		 = '{$_REQUEST['endbai']}', 
						endnum		 = '{$_REQUEST['endnum']}',
						endlongitude = '".implode(".",$_REQUEST['longitude'])."',
						endlatitude = '".implode(".",$_REQUEST['latitude'])."'
					where
						endid = {$endid}";
			$db->executar($sql);
		}else{
			$sql = "insert into snf.enderecopolo
						(
							muncod, 
							estuf, 
							endcep, 
							endlog, 
							endcom, 
							endbai, 
							endnum, 
							endstatus, 
							endlatitude, 
							endlongitude
						)
					values
						(
							'{$_REQUEST['muncod']}',
							'{$_REQUEST['estuf']}',
							'{$_REQUEST['endcep']}',
							'{$_REQUEST['endlog']}',
							'{$_REQUEST['endcom']}',
							'{$_REQUEST['endbai']}',
							'{$_REQUEST['endnum']}',
							'A',
							'".implode(".",$_REQUEST['latitude'])."',
							'".implode(".",$_REQUEST['longitude'])."'
						) returning endid";
		$endid = $db->pegaUm($sql);
		}
		
		$sql = "update snf.polo set
					endid = $endid
				where 
					polid = {$polid}";
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	if($_REQUEST['curid']){
		echo "<script>
				window.opener.selecionaUnidade('1');
				window.close();
			  </script>";
		exit;
	}else{
		$db->sucesso('principal/cadastroPolo');	
	}
}

//Chamada de programa
if($_REQUEST['popupoferta']) {
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
} else {
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br/>";
}

$boAtivo = 'S';

monta_titulo('Dados da Intituição de Ensino', '');
cabecalhoInstituicao( $insid );
?>
<br/>
<?php 
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Unidade de Oferta', $linha2);
?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
$(function(){

	$('[name=insid]').val($('#insid_temp').val());
	
	$('[name=endcep]').blur(function(){
		
		var endcep = replaceAll(replaceAll(this.value, '-', ''), '.', '');

		$.ajax({
			url			: 'snf.php?modulo=principal/cadastroPolo&acao=A',
			type		: 'post',
			data		: 'requisicao=verificarCep&endcep='+endcep,
			dataType	: 'json',
			success		: function(e){
			
				if(e.muncod){
					$('[name=endlog]').val(e.logradouro);
					$('[name=endbai]').val(e.bairro);
					$('[name=estuf]').val(e.estado).attr({'disabled':true});
					$.ajax({
						url		: 'snf.php?modulo=principal/cadastroPolo&acao=A',
						type	: 'post',
						data	: 'requisicao=carregarMunicipios&estuf='+e.estado,
						success	: function(res){
							$('#td_combo_municipio').html(res);
							$('[name=muncod]').val(e.muncod).attr({'disabled':true});
						}
					});						
				}else{
					$('[name=estuf], [name=muncod], [name=endlog], [name=endbai]').attr({'disabled':false,'readonly':false});
				}
				
			}
		});
	});

	jQuery("[name=poldescricao]").addClass("required");
	jQuery("[name=endlog]").addClass("required");
	jQuery("[name=endnum]").addClass("required");
	jQuery("[name=endbai]").addClass("required");
	jQuery("[name=estuf]").addClass("required");
	jQuery("[name=muncod]").addClass("required");
	
	jQuery("#formulario_id").validate();

	$('#btn_salvar').click(function(){
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		if(erro == 0){
			$('[name=requisicao]').val('salvaDados');		
			<?php if($_GET['curid']): ?>
				$('[name=curid]').val(<?php echo $_GET['curid'] ?>);
			<?php endif; ?>
			$('input, select, textarea').attr({'disabled':false,'readonly':false});
			$('#formulario_id').submit();
		}
		
	});
	
});

function abreMapaEntidade(tipoendereco)
{	
	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var entid = window.document.getElementById("entid").value;
	var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&entid='+entid, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}

function editarPolo(polid)
{
	document.location.href = 'snf.php?modulo=principal/cadastroPolo&acao=A&requisicao=carregarPolo&polid='+polid;
	/*
	$.ajax({
		url		 : 'snf.php?modulo=principal/cadastroPolo&acao=A',
		type	 : 'post',
		data	 : 'requisicao=carregarPolo&polid='+polid,
		dataType : 'json',
		success  : function(e){
				
				$('[name=polid]').val(e.polid);
				$('[name=poldescricao]').val(e.poldescricao);
				$('[name=polsigla]').val(e.polsigla);
				
				if(e.muncod){
					
					$('[name=endcep]').val(e.endcep);
					$('[name=endlog]').val(e.endlog);
					$('[name=endcom]').val(e.endcom);
					$('[name=endbai]').val(e.endbai);
					$('[name=endnum]').val(e.endnum);
					$('[name=estuf]').val(e.estuf);
					$('[name=endlatitude]').val(e.endlatitude);
					$('[name=endlongitude]').val(e.endlongitude);
					$('[name=mundescricao1]').val(e.mundescricao);
					
					$.ajax({
						url		: 'snf.php?modulo=principal/cadastroPolo&acao=A',
						type	: 'post',
						data	: 'requisicao=carregarMunicipios&estuf='+e.estuf,
						success	: function(res){
							$('#td_combo_municipio').html(res);
							$('[name=muncod]').val(e.muncod);
						}
					});						
				}
			}
		});
		*/
}

function excluirPolo(polid)
{
	if(confirm('Deseja deletar o registro?')){
		document.location.href = 'snf.php?modulo=principal/cadastroPolo&acao=A&requisicao=deletarPolo&polid='+polid;
	}
}

function carregarMunicipios(estuf){

	$.ajax({
		url		: 'snf.php?modulo=principal/cadastroPolo&acao=A',
		type	: 'post',
		data	: 'requisicao=carregarMunicipios&estuf='+estuf,
		success	: function(res){
			$('#td_combo_municipio').html(res);
			$('[name=muncod]').val('<?=$muncod ?>');
		}
	});		
}
</script>
<?php 
echo $htmlComboEstado;
?>
<form name="formulario" id="formulario_id" method="post" action="">
	<input type="hidden" name="polid" id="polid" value="<?php echo $polid ?>" />
	<input type="hidden" name="insid" id="insid" value="<?php echo $insid ?>" />
	<input type="hidden" name="curid" id="curid" value="<?php echo $_GET['curid'] ?>" />
	<input type="hidden" name="requisicao" id="requisicao" value="" />
	<input type="hidden" name="mundescricao1" id="mundescricao1" value="<?php echo $mundescricao ?>" />
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>			
			<td colspan="2" class="SubtituloTabela bold">Dados</td>
		</tr>
		<tr>
			<td width="220" class="subtituloDireita">Nome</td>
			<td>
				<?php echo campo_texto('poldescricao', 'S', $boAtivo, '', 50, 50, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Sigla</td>
			<td>
				<?php echo campo_texto('polsigla', 'N', $boAtivo, '', 50, 50, '', ''); ?>
			</td>
		</tr>
		<tr>			
			<td colspan="2" class="SubtituloTabela bold">Endereço</td>
		</tr>
		<tr>
			<td class="subtituloDireita">CEP</td>
			<td>
				<?php
				$endcep = $endcep ? formata_cep($endcep) : '';
				echo campo_texto('endcep', 'N', $boAtivo, '', 10, 40, '##.###-###', '',"","","","id='endcep1'"); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Logradouro</td>
			<td>
				<?php
				echo campo_texto('endlog', 'S', $boAtivo, '', 70, 255, '', '',"","","","id='endlog1'"); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número</td>
			<td>
				<?php
				echo campo_texto('endnum', 'S', $boAtivo, '', 10, 20, '', '',"","","","id='endnum1'"); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Complemento</td>
			<td>
				<?php
				echo campo_texto('endcom', 'N', $boAtivo, '', 70, 255, '', '',"","","","id='endcom1'"); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Bairro</td>
			<td>
				<?php
				echo campo_texto('endbai', 'S', $boAtivo, '', 50, 255, '', '',"","","","id='endbai1'"); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">UF</td>
			<td>
				<?php
				$sql = "select
							estuf as codigo,
							estdescricao as descricao
						from 
							territorios.estado
						order by
							estdescricao";
				$db->monta_combo('estuf', $sql, $boAtivo, 'Selecione...', 'carregarMunicipios', '', '', '', 'S','estuf1');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td id="td_combo_municipio">
				<?php if($estuf && $muncod): ?>
					<script>
					
						$(carregarMunicipios('<?=$estuf ?>'));
					</script>
				<?php endif; ?>
				<?php			
				$db->monta_combo('muncod', array(), $boAtivo, 'Selecione...', '', '', '', '', 'S','muncod1');			 
				?>
			</td>
		</tr>
		  
		<tr>
			<td class="SubTituloDireita">Latitude :</td>
			<td>
				<?php 
					str_replace(" ","",$endlatitude);
					str_replace(" ","",$endlongitude);
					if($endlatitude){
						$latitude = explode(".",$endlatitude);
					}
					if($endlongitude){
						$longitude = explode(".",$endlongitude);
					}
				?>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3"
				value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> <span
				id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span>
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
			º <input name="longitude[]" id="minlongitude1" size="3" maxlength="2"
				value="<? echo $longitude[1]; ?>" type="hidden"> <span
				id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>
			' <input name="longitude[]" id="seglongitude1" size="3" maxlength="2"
				value="<? echo $longitude[2]; ?>" type="hidden"> <span
				id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span>
			" <input name="longitude[]" id="pololongitude1"
				value="<? echo $longitude[3]; ?>" type="hidden"> <span
				id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
			<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td><a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar
			No Mapa</a> <input style="display: none;" name="endereco[1][endzoom]"
				id="endzoom1" value="" type="text"></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btn_salvar" />				
				<input type="button" value="Limpar" id="btn_limpar" onclick="window.location.href='snf.php?modulo=principal/cadastroPolo&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<?php 

$sql = "select 
			'<img style=\"cursor:pointer\" src=\"../imagens/alterar.gif\" onclick=\"editarPolo(' || pl.polid || ')\" />
			<img style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" onclick=\"excluirPolo(' || pl.polid || ')\" />' as acao,
			poldescricao,
			polsigla,
			mn.mundescricao,
			mn.estuf,
			endlatitude,
			endlongitude			 
		from 
			snf.polo pl
		left join
			snf.enderecopolo ed on ed.endid = pl.endid
		left join
			territorios.municipio mn on mn.muncod = ed.muncod
		where
			pl.insid = {$_SESSION['snf']['insid']}
		order by 
			pl.poldescricao";

$cabecalho = array('Ação','Descrição','Sigla','Município','UF','Latitude','Longitude');
$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', '', '', '');

?>