<?php
header('content-type: text/html; charset=ISO-8859-1');

include_once "_funcoes_novasturmas.php";

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] );

if( empty($_SESSION['proinfantil']['muncod']) ){
	echo '<script type="text/javascript"> 
    		alert("Falta dados de Sessão.");
    		window.location.href="proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A";
    	  </script>';
    die;
}

if($_REQUEST['req']) {
	$_REQUEST['req']($_REQUEST);
	exit;
}

function cancelaTermoNovasTurmas($dados){
	global $db;
	$dados['resposta'] = 'C';
	extract($dados);
	
	$sql_adpid = "SELECT adpid FROM	proinfantil.adesaoprogramanovasturmas 
				  where muncod = '{$_SESSION['proinfantil']['muncod']}' AND adpano = ".$_SESSION['exercicio'];	
	$adpid = $db->pegaUm($sql_adpid);
	
	$sql = "UPDATE proinfantil.adesaoprogramanovasturmas SET 
				adpdataresposta = now(), 
				adpresposta = '{$resposta}', 
				usucpf ='{$_SESSION['usucpf']}'
			WHERE adpid = {$adpid}";
	$db->executar($sql);
	
	$db->commit();
	$db->sucesso('novasturmas/termoNovasTurmas&acao=A');
}

function respondeTermoNovasTurmas($dados){
	global $db;
	extract($dados);
	
	if(!$adpid){
		$sql = "INSERT INTO proinfantil.adesaoprogramanovasturmas(adpano, muncod, adpdataresposta, adpresposta, usucpf)
				VALUES (
					".$_SESSION['exercicio'].", 
					'".$_SESSION['proinfantil']['muncod']."', 
					'".date("Y-m-d G:i:s")."', 
					'".$resposta."', 
					'".$_SESSION['usucpf']."') returning adpid ";
		$adpid = $db->pegaUm($sql);
	}else{
		$sql = "UPDATE proinfantil.adesaoprogramanovasturmas SET 
					adpdataresposta=now(), 
				   	adpresposta='".$resposta."', 
				    usucpf='".$_SESSION['usucpf']."'
				WHERE adpid = ".$adpid;
		$db->executar($sql);
	}
	$db->commit();
	
	echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso.");
    		window.location.href="proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio&acao=A";
    	  </script>';
    die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$perfis = pegaPerfil($_SESSION['usucpf']);

$sql = "SELECT adpresposta, adpid FROM proinfantil.adesaoprogramanovasturmas WHERE muncod = '".$_REQUEST['muncod']."' and adpano = ".$_SESSION['exercicio'];
$arrAdesao = $db->pegaLinha($sql);

$arMnuid = array();
if( $arrAdesao['adpresposta'] != 'S' ){
	$arMnuid = array('13170', '13171');
}

// Monta abas
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$urlBlq = substr($_SESSION['favurl'], 0, strpos($_SESSION['favurl'], '&'));
$arrAcesso = $_SESSION['proinfantil']['acesso'][$urlBlq];
$arrAcesso = ($arrAcesso ? $arrAcesso : array());
$arBloqueio = array();
foreach ($arrAcesso as $key => $acesso) {
	if($acesso == 'N'){
		array_push($arBloqueio, $key);
	}
}
$arBloqueio = '"'.implode('", "',$arBloqueio).'"';

monta_titulo('Termo de Adesão', '');
cabecalhoTurma();

$tprcod = $_SESSION['proinfantil']['muncod'] == '' ? 1 : 2;

if($teste == '' || $teste == 'N' ) {
	if( $teste == 'N' ){
		$entidade = $tprcod == 1 ? "Estado" : "Município";
	    echo "<script>
	    		alert('Este {$entidade} não aderiu a este programa.');
	   		</script>";
	}
	
	$sql = "select muncod, mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['proinfantil']['muncod']}'";
	$arMunicipio = $db->pegaLinha($sql);
	$municipio = $arMunicipio['mundescricao'].' - '.$arMunicipio['estuf']
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready( function() {

	var arBloqueio = new Array(<?=$arBloqueio ?>);
	console.log(arBloqueio);
	for(var i=0; i<arBloqueio.length; i++){
		switch (arBloqueio[i]) {
			case ('select'):
				$('#form').find('select').attr('disabled', 'disabled');
				break;
			default:
				$('#form').find('input[type='+arBloqueio[i]+']').attr('disabled', 'disabled');
				break;
		}
	}
});

function validaForm( resposta ){

	var form = document.getElementById('form');
	document.getElementById('resposta').value  = resposta;
	document.getElementById('req').value = 'respondeTermoNovasTurmas';
	form.submit();
}

function cancela(){

	var form = document.getElementById('form');

	document.getElementById('req').value = 'cancelaTermoNovasTurmas';

	form.submit();
}

function continuar(){

	var form = document.getElementById('form');
	document.getElementById('continua').value = 'S';
	form.submit();
}

</script>
<center>
<form id="form" nme="form" method="POST">
<input type="hidden" id="resposta" name="resposta" value=""/>
<input type="hidden" id="continua" name="continua" value=""/>
<input type="hidden" id="req" name="req" value=""/>
<input type="hidden" id="adpid" name="adpid" value="<?=$arrAdesao['adpid'] ?>"/>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td width="90%" align="center">
			<br />
			<div style="overflow: auto; height:400px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="center" >
				<div style="width: 95%; magin-top: 10px; ">
					<br />
			        <style type="text/css">
			           @page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
			            .divT_1{ margin-left: 2.5598in; text-align: center; }
			            .P_1{ font-family: Arial; font-size: 10pt; margin-left: 0.1in; margin-right: 0.1in; text-align: justify ! important; clear: both; }
			            .P_2{ font-family: Arial; font-size: 8pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
			            .P_3{ font-family: Arial; font-size: 10pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
			            .P_4{ font-family: Arial; font-size: 10pt; line-width:50px;float: left; text-align: left; width="100px"}
			            .Data{ font-family: Arial; font-size: 10pt; margin-left: 1in; margin-right: 0in; text-align: right; }
			            .T{ font-family: Arial; font-size: 10pt; font-weight: bold; text-align: center; }
			            .T_1{ font-family: Arial; font-size: 10pt; font-weight: bold; margin-left: 0.2in; text-align: center; }
			            .T_11{ font-family: Arial; font-size: 10pt; font-weight: bold; margin-left: 0.2in; text-align: center; }
			            .N{ font-weight: bold; }
			            .I{ font-style: italic; }
			            .S{ text-decoration: underline; }
			        </style>
			        <link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
			        
			        <div>
						<p class="T_1">DECLARAÇÃO PARA RECEBIMENTO DE RECURSOS DE CUSTEIO PARA EDUCAÇÃO INFANTIL.</p>
					
						<p class="P_1">Declaro, como representante do Poder Executivo do município de (<?=$municipio; ?>), que as informações prestadas no Simec – Módulo E. I. Manutenção são fidedignas, responsabilizo-me pela exatidão delas e afirmo que:
						<br />
						<br />
						1 – essas informações referem-se exclusivamente as novas matrículas em novas turmas que estão sendo atendidas em unidade(s) de educação infantil públicas ou conveniadas; e
						</p>
						<p class="P_1">
						<br />
						2 – a(s) nova(s) turma(s) criadas está(ão) em pleno funcionamento; e
						</p>
						<p class="P_1"> 
						<br />
						3 – as novas matrículas em novas turmas informadas não estão computadas para efeito de recebimento dos recursos do Fundeb; e
						</p>
						<p class="P_1">
						<br />
						4 – a(s) nova(s) matrícula(s) em nova(s) turma(s) não está(ão) cadastrada(s) no Educacenso ou, ainda que cadastrada(s), não teve(tiveram) essas matrícula(s) computadas para efeitos de recebimento de recursos do Fundeb; e
						</p>
						<p class="P_1">
						<br />
						5 – todas as matrículas serão cadastradas no próximo Educacenso/Inep.
						</p>
					</div>
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center">
			<div style="float: center; width: 70%; ">
				<?php if( $_SESSION['exercicio'] == date('Y') ){ ?>
                    <?php if($arrAdesao['adpresposta'] != 'S' ): ?>
					   <input type="button" name="aceito" value="Aceito" onclick="validaForm('S')"/>
                    <?php endif; ?>
                    <?php /*if($arrAdesao['adpresposta'] == 'S' ): ?>
                       <input type="button" name="cancelar" value="Cancelar Adesão" onclick="cancela()"/>
                    <?php endif;*/ ?>
                    <?php if($arrAdesao['adpresposta'] != 'S'): ?>
					   <input type="button" name="naceito" value="Não Aceito" onclick="validaForm('N')"/>
                    <?php endif;
					} ?>
			</div>
		</td>
	</tr>
</table>
</center>
</form>
<?php
}
?>