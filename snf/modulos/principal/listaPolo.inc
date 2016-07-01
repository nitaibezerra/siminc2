<?php
if($_REQUEST['requisicao'] == 'carregarMunicipios'){
	if($_REQUEST['estuf']){
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
	}else{
		echo "Selecione a UF.";
	}	
	die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";
$boAtivo = 'S';
monta_titulo('Unidade de Oferta', '');
if($_POST){
	extract($_POST);
}
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
function carregaMunicipios(estuf)
{
	$.ajax({
		url		: 'snf.php?modulo=principal/listaPolo&acao=A',
		type	: 'post',
		data	: 'requisicao=carregarMunicipios&estuf='+estuf,
		success	: function(res){
			$('#td_combo_municipio').html(res);
		}
	});	
}
function pesquisarPolo()
{
	$('#formulario_id').submit();
}
</script>
<form name="formulario" id="formulario_id" method="post" action="">
	<input type="hidden" name="requisicao" id="requisicao" value="" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td width="220" class="subtituloDireita">Nome</td>
			<td>
				<?php echo campo_texto('poldescricao', 'S', $boAtivo, '', 50, 50, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Sigla</td>
			<td>
				<?php echo campo_texto('polsigla', 'N', $boAtivo, '', 20, 50, '', ''); ?>
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
				$db->monta_combo('estuf', $sql, $boAtivo, 'Selecione...', 'carregaMunicipios', '', '', '', 'S','estuf1');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td id="td_combo_municipio">
				<?php
				if($estuf){	
					$sql = "select
								muncod as codigo,
								mundescricao as descricao
							from
								territorios.municipio
							where
								estuf = '$estuf'";
					$db->monta_combo('muncod', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S','muncod1');
				}else{
					echo "Selecione a UF.";	 
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Buscar" id="btn_salvar" onclick="pesquisarPolo()" />				
				<input type="button" value="Ver Todas" id="btn_todas" onclick="window.location.href='snf.php?modulo=principal/listaPolo&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<?php 
if($_POST['poldescricao']){
	$arrWhere[] = "(removeacento(insnome) ilike(removeacento('%{$_POST['poldescricao']}%')) or removeacento(poldescricao) ilike(removeacento('%{$_POST['poldescricao']}%')))";
}
if($_POST['polsigla']){
	$arrWhere[] = "removeacento(polsigla) ilike(removeacento('%{$_POST['polsigla']}%'))";
}
if($_POST['muncod']){
	$arrWhere[] = "mn.muncod = '{$_POST['muncod']}'";
}
if($_POST['estuf']){
	$arrWhere[] = "mn.estuf = '{$_POST['estuf']}'";
}
$sql = "select 
			inssigla || ' - ' || insnome as instituicao,
			inscnpj,
			polsigla || ' - ' || poldescricao as polo,
			mn.mundescricao,
			mn.estuf,
			endlatitude,
			endlongitude			 
		from 
			snf.polo pl
		inner join
			snf.instituicaoensino ins ON ins.insid = pl.insid
		left join
			snf.enderecopolo ed on ed.endid = pl.endid
		left join
			territorios.municipio mn on mn.muncod = ed.muncod
		where
			1=1
			".($arrWhere ? " and ".implode(" and ", $arrWhere) : "")."
		order by 
			pl.poldescricao";

$cabecalho = array('Instituição','CNPJ','Polo','Município','UF','Latitude','Longitude');
$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', '', '', '');

?>