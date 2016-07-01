<?php
include_once APPRAIZ."pde/www/_funcoes_cockpit.php";
if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	die;	
}
include APPRAIZ . 'includes/cabecalho.inc';  
echo "<br>";
monta_titulo(str_replace("...","",$_SESSION['mnudsc']),'&nbsp;');

$habilitado = "S";

?>
<style>.bold{font-weight:bold}</style>
<style>.link{cursor:pointer}</style>
<style>.number{color:blue;text-align:right}</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form id="pesquisa" name="pesquisa" method="post" action="" >
	<input type="hidden" name="requisicaoAjax" value="filtrarDemandaMaisMedicos" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<?php 
		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf";
		mostrarComboPopup( 'UF:', 'uf',  $sql, $stSqlCarregados, 'Selecione a(s) UF(s)');
		$sql = "select uniid as codigo, unisigla || ' - ' || uninome as descricao from maismedicos.universidade where unistatus = 'A' order by unisigla,uninome";
		mostrarComboPopup( 'Insituição Supervisora:', 'uniid',  $sql, $stSqlCarregados, 'Selecione a(s) Instituição(ões) Supervisora(s)');
		$sql = "select distinct tpmid as codigo, tpmdsc as descricao from territoriosgeo.tipomunicipio where tpmstatus = 'A' and gtmid = 1 order by tpmdsc";
		mostrarComboPopup( 'Regiões da Saúde:', 'tpmid',  $sql, $stSqlCarregados, 'Selecione a(s) Região(ões) da Saúde');
		$sql = "select muncod as codigo, mundescricao as descricao from territoriosgeo.municipio order by mundescricao";
		mostrarComboPopup( 'Municípios:', 'muncod',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)');
		?>
		<tr>
			<td width="25%" class="subtituloDireita" >Proporção</td>
			<td>
				<?php $proporcao1 = $_POST['proporcao1'] ? $_POST['proporcao1'] : "1"; echo campo_texto('proporcao1', 'N', "N", '', 5, 10, '[#]', '', 'left', '', 0, 'id="proporcao1"', ''); ?> /
				<?php $proporcao2 = $_POST['proporcao2'] ? $_POST['proporcao2'] : "10"; echo campo_texto('proporcao2', 'S', $habilitado, '', 5, 10, '[#]', '', 'left', '', 0, 'id="proporcao2"', ''); ?>
				(Supervisores / Médicos) 
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Déficit</td>
			<td>
				 <?php echo campo_texto('deficil_inicio', 'N', "S", '', 5, 10, '[#]', '', 'left', '', 0, 'id="deficil_inicio" ', ''); ?>
				 a
				 <?php echo campo_texto('deficil_fim', 'N', "S", '', 5, 10, '[#]', '', 'left', '', 0, 'id="deficil_fim" ', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Excedente</td>
			<td>
				 <?php echo campo_texto('excedente_inicio', 'N', "S", '', 5, 10, '[#]', '', 'left', '', 0, 'id="excedente_inicio" ', ''); ?>
				 a
				 <?php echo campo_texto('excedente_fim', 'N', "S", '', 5, 10, '[#]', '', 'left', '', 0, 'id="excedente_fim" ', ''); ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" bgcolor="#DCDCDC" align="center">
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="filtrarDemandaMaisMedicos()"  /> 
				<input type="button" name="btn_todos" value="Ver Todos" onclick="window.location=window.location"  /> 
			</td>
		</tr>
	</table>
</form>
<br/>
<div id="div_resultado_pesquisa"><?php filtrarDemandaMaisMedicos(); ?></div>
<script>
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function filtrarDemandaMaisMedicos()
{
	$("[name='btn_pesquisar']").val("Carregando...").attr("disabled",true);
	selectAllOptions( document.getElementById( 'uf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );
	selectAllOptions( document.getElementById( 'tpmid' ) );
	selectAllOptions( document.getElementById( 'uniid' ) );
	
	var dados = $("#pesquisa").serialize();
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: dados,
	   success: function(msg){
	   		$('#div_resultado_pesquisa').html( msg );
	   		$("[name='btn_pesquisar']").val("Pesquisar").attr("disabled",false);
	   }
	 });
}

function abreMunicipios(tmpid)
{
	var src = $("#img_reg_"+tmpid).attr("src");
	if(src.search("mais.gif") > 0){
		$("#img_reg_"+tmpid).attr("src","../imagens/menos.gif");
		$(".tr_mun_"+tmpid).show();
	}else{
		$("#img_reg_"+tmpid).attr("src","../imagens/mais.gif");
		$(".tr_mun_"+tmpid).hide();
	}
}
</script>