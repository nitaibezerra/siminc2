<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = "DELETE FROM emenda.ptitemkit WHERE perid = {$_REQUEST['perid']}";
	$db->executar($sql);
	
	$sql = "DELETE FROM emenda.ptaespecificacaorendimento WHERE perid = {$_REQUEST['perid']}";
	$db->executar($sql);
	
	$db->commit();	
	$db->sucesso( 'principal/especificacaoRendimento' );
}

if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$espkit = $_POST['espkit'];
	$valorUnit = $_POST['pervalorunitario'];

	$valorUnit = str_replace('.','', $valorUnit);
	$valorUnit = str_replace(',','.', $valorUnit);
	
	$pervalorrendimento = str_replace('.','', $_POST['pervalorrendimento']);
	$pervalorrendimento = str_replace(',','.', $pervalorrendimento);
	
	$pervalorrendimento = trim($pervalorrendimento);
	$valorUnit 	= trim($valorUnit);

	if( $_POST['perid'] ){
		
		$iceid = $_POST['iceid'];
		$iceid = $iceid ? $iceid : null;
		
		$sql = "UPDATE emenda.ptaespecificacaorendimento SET 
					  ptiid = {$_POST['ptiid']},
					  iceid = ".$iceid.",
					  perquantidade = ".$_POST['perquantidade'].",
					  pervalorunitario = $valorUnit,
					  pervalorrendimento = ".($pervalorrendimento ? $pervalorrendimento : 'NULL').",
					  perdatainicio = '".formata_data_sql($_POST['perdatainicio'])."',
					  perdatafim = '".formata_data_sql($_POST['perdatafim'])."',
					  perstatus = 'A'				 
				WHERE 
				  	perid = {$_POST['perid']}";
		
		$db->executar($sql);
		$db->commit();
	}else{
		$sql = "INSERT INTO emenda.ptaespecificacaorendimento(ptiid, iceid, perquantidade, pervalorunitario, pervalorrendimento, perdatainicio, perdatafim, perstatus) 
				VALUES (
				  {$_POST['ptiid']},
				  ".$_POST['iceid'].",
				  ".$_POST['perquantidade'].",
				  ".$valorUnit.",
				  ".($pervalorrendimento ? $pervalorrendimento : 'NULL').",
				  '".formata_data_sql($_POST['perdatainicio'])."',
				  '".formata_data_sql($_POST['perdatafim'])."',
				  'A'
				)RETURNING perid";
					  
		$perid = $db->pegaUm( $sql );
	}
	
	if( $db->commit() ){
		$db->sucesso( 'principal/especificacaoRendimento' );
	}
}

if($_REQUEST['perid']){
	$sql = "SELECT 
			  ptie.perid,
			  ptie.ptiid,
			  ptie.iceid,
			  ptie.perquantidade,
			  ptie.pervalorunitario,
			  (ptie.perquantidade * ptie.pervalorunitario) as total,
			  ptie.pervalorrendimento,
			  to_char(ptie.perdatainicio, 'DD/MM/YYYY') as perdatainicio,
			  to_char(ptie.perdatafim, 'DD/MM/YYYY') as perdatafim,
			  e.espunidademedida,
			  e.espvalorunitariopadrao,
			  e.espkit,
			  ('<select style=\"width: 250px;\" disabled=\"disabled\"><option>'|| e.espnome ||'</option></select>') as espnome
			FROM 
			  emenda.ptaespecificacaorendimento ptie 
			INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
			INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
			inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
			WHERE ptie.perid = {$_REQUEST['perid']}";

	$arDados = $db->pegaLinha($sql);
	
	$arDados['total'] = number_format($arDados['total'], '2', '.', ',');
	$arDados['pervalorunitario'] = number_format($arDados['pervalorunitario'], '2', '.', ',');
	$arDados['pervalorrendimento'] = number_format($arDados['pervalorrendimento'], '2', '.', ',');
	
	extract($arDados);
}

if($_REQUEST['requisicaoAjax'] == 'insereEspecificacaoPendente'){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$valorUnit = str_replace('.','', $_REQUEST['pervalorunitario']);
	$valorUnit = str_replace(',','.', $valorUnit);
	
	$Proponente = str_replace('.','', $_REQUEST['pervalorrendimento']);
	$Proponente = str_replace(',','.', $Proponente);

	$sql = "INSERT INTO emenda.ptaespecificacaorendimento(ptiid, iceid, perquantidade, pervalorunitario, pervalorrendimento, perdatainicio, perdatafim, perstatus) 
			VALUES (
			  {$_REQUEST['ptiid']},
			  ".$_REQUEST['iceid'].",
			  ".$_REQUEST['perquantidade'].",".
			  ($valorUnit ? $valorUnit : '0').",".
			  ($Proponente ? $Proponente : '0') .",".
			  ($_REQUEST['perdatainicio'] ? "'".formata_data_sql($_REQUEST['perdatainicio'])."'" : 'now()').",".
			  ($_REQUEST['perdatafim'] ? "'".formata_data_sql($_REQUEST['perdatafim'])."'" : 'now()').",
			  'P'
			)RETURNING perid";

	$perid = $db->pegaUm($sql);
	$db->commit();
	echo $perid;
	exit();
}

if($_REQUEST['requisicaoAjax'] == 'montaComboUnidade'){
	header('content-type: text/html; charset=ISO-8859-1');
	$iceid = $_REQUEST['iceid'];
	
	if($iceid){
		$sql = "SELECT DISTINCT
				  e.espunidademedida,
				  e.espkit,
				  e.espvalorunitariopadrao
				FROM 
				  emenda.especificacao e 
				  INNER JOIN emenda.iniciativaespecificacao ie ON (e.espid = ie.espid)
				  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE /*e.espstatus = 'A'
				  AND ie.icestatus = 'A'
				  AND*/ ie.iceid = $iceid";
		
		$dados = $db->pegaLinha($sql);
		
		$html = '';
		if($dados){
			$html.= $dados['espunidademedida'];
			$html.= '<input type="hidden" value="'.$dados['espunidademedida'].'" id="espunidademedida" name="espunidademedida">
					<input type="hidden" value="'.$dados['espvalorunitariopadrao'].'" id="espvalorunitariopadrao" name="espvalorunitariopadrao">
					<input type="hidden" value="'.$dados['espkit'].'" id="espkit" name="espkit">';
		}else{
			$html.= '<input type="hidden" value="'.$dados['espunidademedida'].'" id="espunidademedida" name="espunidademedida">';
		}
	}
	echo $html;
	exit;
}

$obPTA = new PTA();

$obPTA->validaSessionPTA($_SESSION["emenda"]["ptrid"]);
$obPTA->validaSessionPTA($_SESSION["emenda"]["ptiid"]);

$ptrid = $_SESSION["emenda"]["ptrid"];
$ptiid = $_SESSION['emenda']['ptiid'];

#carrega os dados da inciativa cadastrado
$acao = $obPTA->carregaCabecalhoIniciativaEspecificacao( $ptiid );

$iniid = $acao['iniid'];
$ininome = $acao['ininome'];
$ptiid = ($ptiid ? $ptiid : $acao['ptiid'] );

include  APPRAIZ."includes/cabecalho.inc";

echo $obPTA->exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

$valorRendimento = $db->pegaUm( "select ptrrendimentoaplicacao from emenda.planotrabalho p where ptrid = $ptrid" );
$valorRendimentoUsado = $db->pegaUm( "select ptrrendimentoaplicacao from emenda.planotrabalho p where ptrid = $ptrid" );

monta_titulo( 'Detalhamento da Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
#monta cabeçalho do plano de trabalho
echo $obPTA->cabecalhoPlanoTrabalho( $ptrid );
echo "<br />";
echo montarAbasArray(carregaAbasPlanoTrabalho('especificacaoRendimento'), "/emenda/emenda.php?modulo=principal/especificacaoRendimento&acao=A");
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<style type="text/css">	
	div.dcontexto{
		 position:relative; 
		 padding:0;
		 color:#039;
		 text-decoration: none;
		 cursor: pointer; 
		 margin-right: -1px;
		 z-index:24;
	}
	div.dcontexto:hover{
		background:transparent;
		text-decoration: none;
		z-index:25; 
	}
	div.dcontexto span{
		display: none;
		text-decoration: none;
	}
	div.dcontexto:hover span{
		display: block;
		position:absolute;
		width: auto;
		font-size: 10px;
		top:2em;
		text-align:center;
		/*left:50px;*/
		padding:4px 0px;
		border:1px solid #000;
		background:#eee; 
		color:#000;
		text-decoration: none;
	}

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    _position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
.borda{
	border: 1px solid;
	border-color: red;
	color: red;
}
.borda1{
	border: 1px solid;
	border-color: red;
	color: black;
}
</style>

<form id="formulario" name="formulario" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="ptiid" id="ptiid" value="<?=$ptiid; ?>">
	<input type="hidden" name="perid" id="perid" value="<?=$perid; ?>">
	<input type="hidden" name="iniid" id="iniid" value="<?=$iniid;?>">
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
	
	<input type="hidden" name="valorRendimento" id="valorRendimento" value="<?=$valorRendimento; ?>">
	<input type="hidden" name="hidValorTotal" id="hidValorTotal" value="<?=$arDados['total']; ?>">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td align="center" bgcolor="#dedfde"><b>Iniciativa</b></td>
	</tr>
	<tr>
		<td align="center"><?=$ininome; ?></td>
	</tr>
	<tr>
		<td align="center" bgcolor="#dedfde"><b>Especificação do Rendimento de Aplicação</b></td>
	</tr>
</table>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
<thead>
	<tr>
		<td rowspan="2" align="Center" class="title" width="22%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Especificação da Iniciativa</strong></td>
		<td colspan="2" align="Center" class="title" width="25%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Indicador Físico</strong></td>
		<td colspan="2" align="Center" class="title" width="15%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custo</strong></td>
		<td rowspan="2" align="Center" class="title" width="10%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Inicial</strong></td>
		<td rowspan="2" align="Center" class="title" width="10%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Final</strong></td>
		<td rowspan="2" align="Center" class="title" width="15%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
	</tr>
	<tr>
		<td align="Center" class="title" width="20%"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade de Medida</strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Quantidade</strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Unitário</strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Total</strong></td>
	</tr>
</thead>
<tbody>
	<tr bgcolor="#FCFDDB">
		<td><?
			if( $_REQUEST['perid'] ){
				echo $arDados['espnome'];
			} else {
				$sql = "SELECT
							ie.iceid as codigo,
							e.espnome as descricao
						FROM
							emenda.iniciativa i
						    inner join emenda.iniciativaespecificacao ie on ie.iniid = i.iniid
						    inner join emenda.especificacao e on e.espid = ie.espid
						    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
						    inner join emenda.ptiniciativa pti on pti.iniid = i.iniid
						WHERE
							pti.ptiid = $ptiid
						    AND ie.icestatus = 'A'
						    AND e.espstatus = 'A'
						    AND i.inistatus = 'A'
						ORDER BY e.espnome";
				
				$db->monta_combo("iceid", $sql, 'S','-- Selecione --', 'montaComboUnidade', '', '', 300,'S','iceid', '', '', 'Especificação da Iniciativa');
			}
		?></td>
		<td valign="middle"><div id="unidade"><? 
		if($arDados['espkit'] == 't'){
			echo $arDados['espunidademedida']."<img id='addKit' border='0' style='cursor: pointer' onclick='AddKit();' title='Adicionar Kit' alt='Ir' src='/imagens/gif_inclui.gif'/>";	
		} else {
			echo $arDados['espunidademedida'];
		} ?><input type="hidden" name="iceid" id="iceid" value="<?=$iceid;?>"></div></td>
		<td style="text-align: center;">
			<input id="perquantidade" class="obrigatorio normal" type="text" title="Quantidade" onblur="MouseBlur(this);calculaTotal('');" 
						onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
						onkeyup="this.value=mascaraglobal('[#]',this.value);" value="<?=($arDados['perquantidade'] ? $arDados['perquantidade'] : '0'); ?>" maxlength="5" size="4" name="perquantidade"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
		</td>
		<td style="text-align: center;"><?=campo_texto( 'pervalorunitario', 'S', 'S', 'Valor Unitário', 10, 18, '[###.]###,##', '','','','','id="pervalorunitario"','','',"calculaTotal(this.value); this.value=mascaraglobal('[###.]###,##',this.value)")?></td>
		<td style="text-align: center;"><?=campo_texto( 'pervalorrendimento', 'S', 'N', 'Valor Rendimento', 10, 13, '[###.]###,##', '','','','','id="pervalorrendimento"','','',"this.value=mascaraglobal('[###.]###,##',this.value); verificaValorTotal(this.value);")?></td>
		<td style="text-align: center;"><div id="dataini"><?=campo_data2('perdatainicio', 'S','S','Data Inicial','','','') ?></div></td>
		<td style="text-align: center;"><?=campo_data2('perdatafim', 'S','S','Data Final','','','') ?></td>
		<td style="text-align: center;">
			<input type="button" value="Salvar" name="btnIncluir" id="btnIncluir" onclick="incluirEspecificacao();">		
			<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Cancelar();">
		</td>
	</tr>
	<tr>
		<td align="center" colspan="10">
			<?=listaEspecificacaoCadastradas( $ptiid, $ptrid ); ?>
		</td>
	</tr>
</tbody>
</table>
<div id="erro"></div>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function Cancelar(){
	window.location.href = window.location;
}

function excluir(perid){
	window.location.href = 'emenda.php?modulo=principal/especificacaoRendimento&acao=A&requisicao=excluir&perid='+perid;
}

function incluirEspecificacao(){
	var erro = false;
	$('.obrigatorio').each(function(){
		if(	$(this).val() == '' || $(this).val() == '0' ){
			alert('O campo '+$(this).attr('title')+' é de preenchimento obrigatório');
			var nome = $(this).attr('name');
			$('[name="'+nome+'"]').focus();
			erro = true;			
			return false;
		}
	});
	
	if( erro ){
		//return false;
	}
	
	if($('#espkit').val() == 't'){
		
		if($('#pervalorunitario').val() == ""){
			alert('A especificação selecionada é do tipo KIT e, para preenchê-la,\n é necessário informar todos os itens que compõem esse kit. Para informar esses itens, \n selecione a opção + no campo Unidade de Medida!');
			AddKit();
			return false;
		}
		
	}

	var valorUnit = $('#pervalorunitario').val().replace(/\./gi,"");
	valorUnit = valorUnit.replace(/\,/gi,".");

	if(parseInt( $('#perquantidade').val() ) <= 0){
		alert('A quantidade não pode ser menor ou igual a 0.');
		$('#perquantidade').focus();
		return false;
	}else if( parseInt(valorUnit) <= '0' ){
		alert('O valor unitário não pode ser menor ou igual a 0.');
		$('#pervalorunitario').focus();
		return false;
	}
	
	if(!verificaValorTotal( $('#pervalorrendimento').val() )){
		$('#pervalorrendimento').focus();
		return false;
	}
	
	if(!validaData( document.getElementById('perdatainicio') ) ) {
		alert('Data início está no formato incorreto.');
		$('#perdatainicio').focus();
		return false;
	}else if(!validaData(document.getElementById('perdatafim') ) ) {
		alert('Data fim está no formato incorreto.');
		$('#perdatafim').focus();
		return false;
	}else if( !validaDataMaior( document.getElementById('perdatainicio'), document.getElementById('perdatafim') ) ){
		alert("A data inicial não pode ser maior que data final.");
			$('#perdatainicio').focus();
		return false;
	}
	
	$('#requisicao').val('salvar');	
	$('#formulario').submit();
}

function montaComboUnidade(iceid){

	$.ajax({
		type: "POST",
		url: 'emenda.php?modulo=principal/especificacaoRendimento&acao=A&requisicaoAjax=montaComboUnidade&iceid='+iceid,
		async: false,
		success: function(retornoajax) {
			$('#unidade').html(retornoajax);
			
        	$('#pervalorunitario').val('');
        	$('#perquantidade').val('');
        	$('#pervalorrendimento').val('');
        	$('#total').html('');
        	
			if( $('#espkit').val() == "t" ){
				$('#pervalorunitario').attr('readonly', true);
				$('#pervalorrendimento').attr('readonly', true);
				$('#perquantidade').val('1');
				$('#perquantidade').attr('readonly', true);
				alert('Deverão ser informados todos os itens que compõem esta especificação.\nPara informá-los, selecione a opção + no campo Unidade de Medida.');
				$('#unidade').html(retornoajax + " <img id='addKit' border='0' style='cursor: pointer' onclick='AddKit();' title='Adicionar Kit' alt='Ir' src='/imagens/gif_inclui.gif'/>");
				AddKit();
			}else{
				$('#pervalorunitario').attr('readonly', false);
				$('#perquantidade').attr('readonly', false);
				$('#pervalorrendimento').attr('readonly', false);
			}
			
			if( $('#espvalorunitariopadrao').val() != "" ){
				$('#pervalorunitario').attr('readonly', false);
				$('#perquantidade').val('1');
				$('#pervalorunitario').val( mascaraglobal('###.###.###.###,##', $('#espvalorunitariopadrao').val() ));
				calculaTotal('');
			}
		}
	});
}

function AddKit(){
	if($('#perid').val() == ""){
		insereEspecificacaoPendente();
	}
	window.open('emenda.php?modulo=principal/popupKitRendimento&acao=A&perid='+$('#perid').val()+'&iceid='+$('#iceid').val()+'&ptrid='+$('#ptrid').val(), 'popkit','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, fullscreen=yes');
}

function AlteraKit(perid, iceid, visualiza){
	window.open('emenda.php?modulo=principal/popupKitRendimento&acao=A&perid='+perid+'&iceid='+iceid+'&visualiza='+visualiza+'&ptrid='+$('#ptrid').val(),'popkit','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, fullscreen=yes');
}

function insereEspecificacaoPendente(){

	$.ajax({
		type: "POST",
		url: 'emenda.php?modulo=principal/especificacaoRendimento&acao=A&requisicaoAjax=insereEspecificacaoPendente&'+$('#formulario').serialize(),
		async: false,
		success: function(retornoajax) {
			//$('#erro').html(retornoajax);
			$('#perid').val(retornoajax);
		}
	});
}

function calculaTotal(valor){
	
	var result = "";
	if( (valor) || ($('#pervalorunitario').val() != "") ){
		valor = $('#pervalorunitario').val();
		
		if(parseInt($('#perquantidade').val()) <= 0){
			alert('A quantidade não pode se menor ou igual a 0!');
			$('#perquantidade').focus();
			return false;
		}else{				
			valor = valor.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");
			result = parseInt($('#perquantidade').val()) * valor;

			$('#hidValorTotal').val(result);
			$('#total').html('R$ ' + mascaraglobal('###.###.###.###,##', result.toFixed(2)));

			$('#pervalorrendimento').val( mascaraglobal('###.###.###.###,##', result.toFixed(2)) );
			
			if($('#iceid').val() != ""){
				verificaValorTotal($('#pervalorrendimento').val());
			}
		}
	}
}

function verificaValorTotal(valor){
	
	var valorRendimento = $('#valorRendimento').val();
	var valorRendimentoUsado = $('#valorRendimentoUsado').val();
	
	var valor = valor.replace(/\./gi,"");
	valor = valor.replace(/\,/gi,".");
	
	var totalUsado = parseFloat(valor) + parseFloat(valorRendimentoUsado);
	
	var result = 0;
	//valorRendimento
	if(parseFloat(totalUsado) > parseFloat(valorRendimento) ){
		alert('O valor informado de rendimento não pode ser maior que Valor Total: R$ '+mascaraglobal('###.###.###.###,##', valorRendimento)+'!');
		$('#pervalorrendimento').focus();
		return false;
	}else{
		/*result = valorTotal - valor;
		$('#pervalorrendimento').val( mascaraglobal('###.###.###.###,##', result.toFixed(2)) );*/
		return true;
	}
}

function alterar(perid, celula){
	window.location.href = 'emenda.php?modulo=principal/especificacaoRendimento&acao=A&perid='+perid;
}
</script>
<?
function listaEspecificacaoCadastradas( $ptiid, $ptrid ){
	global $db;
	
	$estado = pegarEstadoAtual( $ptrid );
	
	if( verificaPermissaoPerfil('pta', 'boolean', $estado, true) ) {
		if( $estado == EM_REFORMULACAO_PROCESSO ){
			$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptie.perid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
				            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptie.perid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
		} else {
			$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptie.perid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">  '  || 
			            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptie.perid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
		}
	} else {
		$acao = "( '<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" onclick=\"\" \" border=\"0\" alt=\"Ir\" title=\"Alterar\">  '  || 
			            '<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" onclick=\"\" border=\"0\" alt=\"Ir\" title=\"Excluir\"></center>' )";
	}
	
	$sql = "SELECT 
			  	  ".$acao." as acao,
			      e.espnome,
				  e.espkit,
				  e.espunidademedida,
				  ptie.ptiid,
				  ptie.perid,
				  ptie.iceid,
				  ptie.perquantidade,
				  ptie.pervalorunitario,
				  ptie.pervalorrendimento,
				  to_char(ptie.perdatainicio, 'DD/MM/YYYY') as dataini,
				  to_char(ptie.perdatafim, 'DD/MM/YYYY') as datafim
				FROM 
				  emenda.ptaespecificacaorendimento ptie INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
				  INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
				  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE 
				    --    ie.icestatus = 'A' AND 
					--    e.espstatus = 'A' AND 
				    ptie.perstatus = 'A' AND 
				    ptie.ptiid = $ptiid
				ORDER BY
					ptie.perid";

	$arDados = $db->carregar($sql);
			
	$totalizadorValorTotal		= 	0;
	$totalizadorValorProponente	=	0;
	
	$sql = "SELECT 
				  to_char(min(perdatainicio), 'DD/MM/YYYY') as min_dataini, 
  				  to_char(max(perdatafim), 'DD/MM/YYYY') as max_datafim 
				FROM 
					emenda.ptaespecificacaorendimento
				WHERE 
					ptiid = $ptiid 
					and perstatus = 'A'";
		
	$arData = $db->pegaLinha( $sql );
			
	$totalizadorDataInicio	=	$arData["min_dataini"];
	$totalizadorDataFim		=	$arData["max_datafim"];
	
	$html = '';
	if($arDados){
		//$html.= $this->cabecalhoEspecificacaoIniciativa();
		foreach ($arDados as $key => $value) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			
			if($value['espkit'] == 't'){
				$unidade = '<a onclick="AlteraKit('.$value['perid'].','.$value['iceid'].','."'true'".');" style="cursor: pointer;">'.$value['espunidademedida'].'</a>';
			}else{
				$unidade = $value['espunidademedida'];
			}
		
		$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		$html.= '	<td>'.$value['espnome'].'</td>';
		$html.= '	<td>'.$unidade.'</td>';
		$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">'.$value['perquantidade'].'</td>';
		$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['pervalorunitario'],2,',','.').'</td>';
		$html.= '	<td style="text-align: center; color: rgb(0, 102, 204);">R$ '.number_format($value['pervalorrendimento'],2,',','.').'</td>';
		$html.= '	<td style="text-align: center;">'.$value['dataini'].'</td>';
		$html.= '	<td style="text-align: center;">'.$value['datafim'].'</td>';
		$html.= '	<td style="text-align: center;">'.$value['acao'].'</td>';
		$html.= '</tr>';
		
		// Soma os valores para o totalizador
		$totalizadorValorTotal			+= 	$value['pervalorrendimento'];
		}
		$html.= '<tr>';
		$html.= '<td colspan="4" bgcolor="#cccccc" style="text-align: right;"><b>Totalizador:</b></td>';
		$html.= '<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorValorTotal,2,',','.').'</b><input type="hidden" name="valorRendimentoUsado" id="valorRendimentoUsado" value="'.$totalizadorValorTotal.'"></td>';
		$html.= '<td style="text-align: center;"><b>'.$totalizadorDataInicio.'</b></td>';
		$html.= '<td style="text-align: center;"><b>'.$totalizadorDataFim.'</b></td>';
		$html.= '</tr>';
		$html.= '</table>';
	
		$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html.= '<tr bgcolor="#ffffff">';
		$html.= '	<td><b>Total de Registros: '.count($arDados).'</b></td>';
		$html.= '	<td></td>';
		$html.= '</tr>';
		$html.= '</table>';
	}else{
		$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html.= '<tr><td align="center" style="color:#cc0000;"></td></tr>';
		$html.= '</table>';
	}
	
	return $html;
}
?>