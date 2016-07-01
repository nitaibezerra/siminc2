<?php
if($_POST['classe'] && $_POST['requisicaoAjax'])
{
	header('content-type: text/html; charset=ISO-8859-1');
	require_once APPRAIZ."rehuf/classes/{$_POST['classe']}.class.inc";
	$n = new $_POST['classe']();
	$n->$_POST['requisicaoAjax']();
	die;
}

if($_POST['classe'] && $_POST['requisicao'])
{
	require_once APPRAIZ."rehuf/classes/{$_POST['classe']}.class.inc";
	$n = new $_POST['classe']();
	$n->$_POST['requisicao']();
	die;
}

if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}

/* Tratamento de segurança */
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
/* FIM - Tratamento de segurança */

if($_REQUEST['exec_function']){
	if (function_exists($_REQUEST['exec_function'])){
		$_REQUEST['exec_function']($_REQUEST);
	}	
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

// monta menu padrão contendo informações sobre as entidades
$menu = array(
				0 => array("id" => 1, "descricao" => "Lista de hospitais",   		   "link" => "/rehuf/rehuf.php?modulo=inicio&acao=C"),
			 	1 => array("id" => 2, "descricao" => "Dados do hospital",    		   "link" => "/rehuf/rehuf.php?modulo=principal/dadoshospital&acao=A"),
			 	2 => array("id" => 3, "descricao" => "Pregões - Itens Homologados",    "link" => "/rehuf/rehuf.php?modulo=pregao/listaPregaoHomologado&acao=A"),
			 	3 => array("id" => 4, "descricao" => "Lista de Itens Homologados",     "link" => "/rehuf/rehuf.php?modulo=pregao/itemPregaoHomologado&acao=A&preid={$_GET['preid']}"),
		  	);
echo montarAbasArray($menu,$_SERVER['REQUEST_URI']);

$titulo_modulo = "Lista de Itens Homologados";
monta_titulo( $titulo_modulo, '&nbsp;' );
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<?php
$sql = "select 
			*
		from
			rehuf.itemhomologado
		where
			preid = {$_GET['preid']}
		order by
			itecatmat,itedescricao";
$arrItens = $db->carregar($sql); 
$arrItens = $arrItens ? $arrItens : array(); 

require APPRAIZ . "www/includes/webservice/pj.php";


$sql = "select 
			tpaid as codigo,
			tpadsc as descricao
		from
			rehuf.tipoaquisicao
		where
			tpastatus = 'A'
		order by
			tpadsc";
$arrTipoAquisicao = $db->carregar($sql);


require_once APPRAIZ."rehuf/classes/HospitalItemHomologado.class.inc";
$hospitalItem = new HospitalItemHomologado();
$arrItensHospital = $hospitalItem->recuperarItens();

?>
<style>
	.center{text-align:center}
	.bold{font-weight:bold}
</style>
<script>
function carregaDadosCNPJ(obj)
{
	
}

function fake(valor)
{
	return true;
}

function exibeCampoPregao(obj)
{
	
}

function salvarItens()
{
	
}

function salvarItensAjax()
{
	
}

jQuery(function() {
	<?php if($_SESSION['rehuf']['hospitalitemhomologado']['alert']): ?>
		alert('<?php echo $_SESSION['rehuf']['hospitalitemhomologado']['alert'] ?>');
		<?php unset($_SESSION['rehuf']['hospitalitemhomologado']['alert']) ?>
	<?php endif; ?>
	//window.setInterval('salvarItensAjax()', 36000000); //10 minutos
});

</script>
<form name="formulario_itens" id="formulario_itens"  method="post" action="" >
	<input type="hidden" name="preid"  id="preid" value="<?php echo $_GET['preid'] ?>" />
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" >
		<tr bgcolor="#c5c5c5" >
			<td class="center bold" >CATMAT</td>
			<td class="center bold" >Descrição dos Itens Homologados</td>
			<td class="center bold" >Apresentação</td>
			<td class="center bold" >Valor Unitário Homologado</td>
			<td class="center bold" >Preço de Aquisição pelo HU</td>
			<td class="center bold" >CNPJ do Fornecedor</td>
			<td class="center bold" >Nome do Fornecedor</td>
			<td class="center bold" >Tipo de Aquisição</td>
			<td class="center bold" >Marca</td>
			<td class="center bold" >Vigência / Data da Última Aquisição</td>
			<td class="center bold" >Observações</td>
		</tr>
		<?php if($arrItens): ?>
			<?php $n=0; foreach($arrItens as $i): ?>
				<?php $cor = $n%2 ? "#ffffff" : ""?>
				<tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
					<td class="center" >
						<input type="hidden" name="iteid[<?php echo $i['iteid'] ?>]" value="<?php echo $i['iteid'] ?>" id="iteid_<?php echo $i['iteid'] ?>" />
						<input type="hidden" name="hitnomefornecedor[<?php echo $i['iteid'] ?>]" value="<?php echo $arrItensHospital[$i['iteid']]['hitnomefornecedor']?>" id="hitnomefornecedor_<?php echo $i['iteid'] ?>" />
						<?php echo $i['itecatmat'] ?>
					</td>
					<td class="center" >
						<div title="<?php echo $i['itedescricao'] ?>">
						<?php 
							if(strlen($i['itedescricao']) > 100){
								echo substr($i['itedescricao'],0,100)."...";
							}else{
								echo $i['itedescricao'];
							}
						?>
						</div>
					</td>
					<td class="center" ><?php echo $i['iteapresentacao'] ?></td>
					<td class="center" ><?php echo number_format($i['itevalorhomologado'],2,',','.') ?></td>
					<td class="center" ><?php echo $arrItensHospital[$i['iteid']]['hitvalorunitario'] ? number_format($arrItensHospital[$i['iteid']]['hitvalorunitario'],2,',','.') : null ?></td>
					<td class="center" >
						<?php echo ((!is_null($arrItensHospital[$i['iteid']]['hitcnpjfornecedor']))?$arrItensHospital[$i['iteid']]['hitcnpjfornecedor']:"") ?>
					</td>
					<td class="center" id="hitnome_formnecedor_<?php echo $i['iteid'] ?>" style="width:150px" ><?php echo $arrItensHospital[$i['iteid']]['hitnomefornecedor']?></td>
					<td class="center" >
						<?php $db->monta_combo("tpaid[{$i['iteid']}]",$arrTipoAquisicao,"N","Selecione...",'exibeCampoPregao(this);fake','','','','N', 'tpaid_'.$i['iteid'],"",$arrItensHospital[$i['iteid']]['tpaid']); ?>
						<div id="div_pregao_<?php echo $i['iteid'] ?>" style="display:<?php echo $arrItensHospital[$i['iteid']]['tpaid'] == 1 ? "" : "none"?>;margin-top:5px" >
							<b>Pregão:</b> <?php echo $arrItensHospital[$i['iteid']]['hitpregao'] ?>
						</div>
					</td>
					<td class="center" ><?php echo $arrItensHospital[$i['iteid']]['hitmarca'] ?></td>
					<td class="center" style="width:140px" ><?php echo formata_data($arrItensHospital[$i['iteid']]['hitdatavigencia']) ?></td>
					<td class="center" >
						<?php echo $arrItensHospital[$i['iteid']]['hitobs']?>
					</td>
				</tr>
			<?php $n++;endforeach;?>
			<tr align="center"  >
				<td colspan="10">
					<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)" />
				</td>
			</tr>
		<?php else: ?>
			<tr align="center"  >
				<td colspan="10">Não existem itens cadastrados para este pregão.</td>
			</tr>
			<tr align="center"  >
				<td colspan="10">
					<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)" />
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>